/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ConfigParser
*/

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>
#include <libconfig.h++>
#include "ConfigParser.hpp"
#include "Light/DirectionalLight/DirectionalLight.hpp"
#include "Light/AmbientLight/AmbientLight.hpp"
#include "Light/PointLight/PointLight.hpp"
#include "Material/Material.hpp"
#include "Shader/ShaderFactory.hpp"
#include "Primitive/Plugin/PrimitivePluginManager.hpp"
#include "Primitive/Plugin/IPrimitivePlugin.hpp"
#include "Shader/Plugin/ShaderPluginManager.hpp"
#include "Shader/Plugin/ShaderPluginLoader.hpp"
#include "PostProcess/Plugin/PostProcessPluginManager.hpp"
#include "PostProcess/Plugin/IPostProcessPlugin.hpp"
#include "PostProcess/Plugin/PostProcessPluginLoader.hpp"
#include "PostProcess/PostProcessFactory.hpp"
#include "Texture/ImageTexture/ImageTexture.hpp"
#include "Texture/NormalMap/NormalMap.hpp"
#include "Texture/ProceduralTexture/ChessboardTexture.hpp"
#include "Texture/ProceduralTexture/PerlinNoiseTexture.hpp"

namespace RayTracer {

Math::Point3D ConfigParser::parsePoint3D(const libconfig::Setting& setting) {
    double x = setting.exists("x") ? static_cast<double>(setting["x"]) : 0.0;
    double y = setting.exists("y") ? static_cast<double>(setting["y"]) : 0.0;
    double z = setting.exists("z") ? static_cast<double>(setting["z"]) : 0.0;
    return Math::Point3D(Math::Coords{x, y, z});
}

Math::Vector3D ConfigParser::parseVector3D(const libconfig::Setting& setting) {
    double x = setting.exists("x") ? static_cast<double>(setting["x"]) : 0.0;
    double y = setting.exists("y") ? static_cast<double>(setting["y"]) : 0.0;
    double z = setting.exists("z") ? static_cast<double>(setting["z"]) : 0.0;
    return Math::Vector3D(Math::Coords{x, y, z});
}

Math::Vector3D ConfigParser::parseColor(const libconfig::Setting& setting) {
    double r, g, b;

    try {
        int ri = setting["r"];
        int gi = setting["g"];
        int bi = setting["b"];

        r = ri / 255.0;
        g = gi / 255.0;
        b = bi / 255.0;
    } catch (const libconfig::SettingTypeException& ex) {
        try {
            r = static_cast<double>(setting["r"]);
            g = static_cast<double>(setting["g"]);
            b = static_cast<double>(setting["b"]);
        } catch (const std::exception& e) {
            r = g = b = 1.0;
        }
    }
    r = std::max(0.0, std::min(1.0, r));
    g = std::max(0.0, std::min(1.0, g));
    b = std::max(0.0, std::min(1.0, b));

    return Math::Vector3D(Math::Coords{r, g, b});
}

void CameraParser::parse(const libconfig::Setting& setting, std::shared_ptr<SceneBuilder> builder) {
    Math::Point3D position;

    if (setting.exists("position"))
        position = parsePoint3D(setting["position"]);
    else
        position = Math::Point3D(Math::Coords{0, 0, 5});

    Math::Vector3D forward(Math::Coords{0.0, 0.0, -1.0});
    Math::Vector3D up(Math::Coords{0.0, 1.0, 0.0});
    Math::Vector3D right(Math::Coords{1.0, 0.0, 0.0});

    double distance = 1.0;
    double width = 2.0;
    double height = 2.0;

    Math::Point3D screenCenter = position + (forward * distance);
    Rectangle3D screen(
        screenCenter - (right * (width/2)) - (up * (height/2)),
        right * width,
        up * height);

    Camera camera(position, screen);

    if (setting.exists("fieldOfView")) {
        double fov = static_cast<double>(setting["fieldOfView"]);
        camera.setFOV(fov);
    }

    if (setting.exists("rotation")) {
        Math::Vector3D rotation = parseVector3D(setting["rotation"]);

        camera.setOriginalRotation(rotation);

        if (std::isfinite(rotation.Y) && rotation.Y != 0.0)
            camera.rotateY(rotation.Y);
        if (std::isfinite(rotation.Z) && rotation.Z != 0.0)
            camera.rotateZ(rotation.Z);
        if (std::isfinite(rotation.X) && rotation.X != 0.0)
            camera.rotateX(rotation.X);

        camera.rotatedX = 0.0;
        camera.rotatedY = 0.0;
        camera.rotatedZ = 0.0;
    } else if (setting.exists("lookAt")) {
        Math::Point3D lookAt = parsePoint3D(setting["lookAt"]);
        Math::Vector3D forward = (lookAt - position).normalize();
        Math::Vector3D up(Math::Coords{0.0, 1.0, 0.0});
        Math::Vector3D right = up.cross(forward).normalize();
        up = forward.cross(right).normalize();

        screenCenter = position + (forward * distance);
        camera.screen.origin = screenCenter - (right * (width/2)) - (up * (height/2));
        camera.screen.bottom_side = right * width;
        camera.screen.left_side = up * height;
    }

    if (setting.exists("resolution")) {
        int width = 800, height = 600;
        if (setting["resolution"].exists("width"))
            width = static_cast<int>(setting["resolution"]["width"]);
        if (setting["resolution"].exists("height"))
            height = static_cast<int>(setting["resolution"]["height"]);

        double aspectRatio = static_cast<double>(width) / height;

        Math::Point3D screenCenter = camera.screen.origin +
            camera.screen.bottom_side * 0.5 + camera.screen.left_side * 0.5;
        Math::Vector3D viewDir = (screenCenter - camera.origin).normalize();
        Math::Vector3D rightDir = camera.screen.bottom_side.normalize();
        Math::Vector3D upDir = camera.screen.left_side.normalize();

        double screenDistance = (screenCenter - camera.origin).length();
        double halfHeight = camera.screen.left_side.length() / 2.0;
        double halfWidth = halfHeight * aspectRatio;

        camera.screen.bottom_side = rightDir * (halfWidth * 2.0);
        camera.screen.left_side = upDir * (halfHeight * 2.0);
        camera.screen.origin = screenCenter -
            camera.screen.bottom_side * 0.5 - camera.screen.left_side * 0.5;
    }

    builder->setCamera(camera);
}

void LightsParser::parse(const libconfig::Setting& setting, std::shared_ptr<SceneBuilder> builder) {
    double ambientIntensity = getValueOrDefault<double>(setting, "ambient", 0.4);
    builder->setAmbientLight(Math::Vector3D(Math::Coords{
        ambientIntensity, ambientIntensity, ambientIntensity
    }));

    if (setting.exists("point"))
        parsePointLights(setting["point"], builder);

    if (setting.exists("directional"))
        parseDirectionalLights(setting["directional"], builder);
}

void LightsParser::parsePointLights(const libconfig::Setting& lights, std::shared_ptr<SceneBuilder> builder) {
    int count = lights.getLength();
    for (int i = 0; i < count; ++i) {
        const libconfig::Setting& light = lights[i];

        Math::Point3D position;
        if (light.exists("position"))
            position = parsePoint3D(light["position"]);
        else
            position = parsePoint3D(light);

        Math::Vector3D color(Math::Coords{1.0, 1.0, 1.0});
        if (light.exists("color"))
            color = parseColor(light["color"]);

        double constant = 1.0;
        double linear = 0.09;
        double quadratic = 0.032;

        if (light.exists("attenuation")) {
            const libconfig::Setting& att = light["attenuation"];
            constant = att.exists("constant") ? static_cast<double>(att["constant"]) : constant;
            linear = att.exists("linear") ? static_cast<double>(att["linear"]) : linear;
            quadratic = att.exists("quadratic") ? static_cast<double>(att["quadratic"]) : quadratic;
        }

        auto pointLight = std::make_shared<PointLight>(
            position,
            color,
            constant,
            linear,
            quadratic
        );

        builder->addLight(pointLight);
    }
}

void LightsParser::parseDirectionalLights(const libconfig::Setting& lights, std::shared_ptr<SceneBuilder> builder) {
    int count = lights.getLength();
    for (int i = 0; i < count; ++i) {
        const libconfig::Setting& light = lights[i];

        Math::Vector3D direction;
        if (light.exists("direction"))
            direction = parseVector3D(light["direction"]).normalize();
        else
            direction = parseVector3D(light).normalize();

        Math::Vector3D color(Math::Coords{1.0, 1.0, 1.0});
        if (light.exists("color"))
            color = parseColor(light["color"]);

        auto dirLight = std::make_shared<DirectionalLight>(direction, color);
        builder->addLight(dirLight);
    }
}

void PrimitivesParser::parse(const libconfig::Setting& setting, std::shared_ptr<SceneBuilder> builder) {
    auto pluginManager = PrimitivePluginManager::getInstance();
    if (!pluginManager->loadAllPlugins("plugins/primitives")) {
        std::cerr << "Failed to load plugins." << std::endl;
        return;
    }
    auto loadedPluginNames = pluginManager->getLoadedPluginNames();

    for (const auto& typeName : loadedPluginNames) {
        if (setting.exists(typeName))
            parsePluginPrimitives(typeName, setting[typeName.c_str()], builder);
    }
}

void ShadersParser::parse(const libconfig::Setting& setting, std::shared_ptr<SceneBuilder> builder) {
    auto pluginManager = ShaderPluginManager::getInstance();
    if (!pluginManager->loadAllPlugins("plugins/shaders")) {
        std::cerr << "Failed to load shader plugins." << std::endl;
    } else {
        auto loadedPluginNames = pluginManager->getLoadedPluginNames();
        loadShaderPlugins();
    }

    int count = setting.getLength();
    for (int i = 0; i < count; ++i) {
        const libconfig::Setting& shader = setting[i];
        parsePluginShader(shader, builder);
    }
}

void PostProcessParser::parse(const libconfig::Setting& setting, std::shared_ptr<SceneBuilder> builder) {
    auto pluginManager = PostProcessPluginManager::getInstance();
    if (!pluginManager->loadAllPlugins("plugins/postprocess")) {
        std::cerr << "Failed to load PostProcess plugins." << std::endl;
    } else {
        auto loadedPluginNames = pluginManager->getLoadedPluginNames();
        loadPostProcessPlugins();
    }

    int count = setting.getLength();
    for (int i = 0; i < count; ++i) {
        const libconfig::Setting& postProcess = setting[i];
        parsePluginPostProcess(postProcess, builder);
    }
}

void PostProcessParser::parsePluginPostProcess(const libconfig::Setting& postProcess, std::shared_ptr<SceneBuilder> builder) {
    static PostProcessFactory postProcessFactory;

    try {
        std::string typeName;
        if (postProcess.exists("type")) {
            typeName = static_cast<const char*>(postProcess["type"]);
        } else {
            std::cerr << "Missing 'type' field in PostProcess definition" << std::endl;
            return;
        }

        auto pluginManager = PostProcessPluginManager::getInstance();
        auto plugin = pluginManager->getPlugin(typeName);

        if (!plugin) {
            std::cerr << "PostProcess plugin not found for type: " << typeName << std::endl;
            return;
        }

        std::vector<std::string> requiredParams = plugin->getRequiredParameters();
        std::map<std::string, double> params = extractParametersFromSetting(postProcess, requiredParams);

        try {
            auto postProcessObj = pluginManager->createPostProcess(typeName, params);
            if (postProcessObj)
                builder->addPostProcess(postProcessObj);
            else
                std::cerr << "Failed to create PostProcess effect of type: " << typeName << std::endl;
        } catch (const std::exception& ex) {
            std::cerr << "Error creating PostProcess effect: " << ex.what() << std::endl;
        }
    } catch (const libconfig::SettingException& ex) {
        std::cerr << "Error parsing PostProcess effect: " << ex.what() << std::endl;
    } catch (const std::exception& ex) {
        std::cerr << "Error creating PostProcess effect: " << ex.what() << std::endl;
    }
}

std::map<std::string, double> PostProcessParser::extractParametersFromSetting(
    const libconfig::Setting& setting,
    const std::vector<std::string>& requiredParams) {

    std::map<std::string, double> params;

    for (int i = 0; i < setting.getLength(); ++i) {
        const std::string& name = setting[i].getName();

        if (name == "type")
            continue;

        if (setting[i].getType() == libconfig::Setting::TypeFloat ||
            setting[i].getType() == libconfig::Setting::TypeInt) {
            params[name] = static_cast<double>(setting[i]);
        }
    }

    for (const auto& param : requiredParams) {
        if (params.find(param) == params.end()) {
            std::cerr << "Warning: Missing required parameter '" << param << "' for PostProcess" << std::endl;
            params[param] = 0.0;
        }
    }

    return params;
}

SceneConfigParser::SceneConfigParser() {
    builder = std::make_shared<SceneBuilder>();
    sectionParsers["camera"] = std::make_unique<CameraParser>();
    sectionParsers["lights"] = std::make_unique<LightsParser>();
    sectionParsers["primitives"] = std::make_unique<PrimitivesParser>();
    sectionParsers["shaders"] = std::make_unique<ShadersParser>();
    sectionParsers["postprocess"] = std::make_unique<PostProcessParser>();
}

std::unique_ptr<Scene> SceneConfigParser::parseFile(const std::string& filename) {
    builder->reset();

    try {
        libconfig::Config cfg;
        cfg.readFile(filename.c_str());

        for (const auto& [section, parser] : sectionParsers) {
            if (cfg.exists(section))
                parser->parse(cfg.lookup(section), builder);        }

        return builder->build();
    } catch (const libconfig::FileIOException& ex) {
        std::cerr << "Error reading scene file: " << filename << std::endl;
    } catch (const libconfig::ParseException& ex) {
        std::cerr << "Parse error at " << ex.getFile() << ":" << ex.getLine()
                << " - " << ex.getError() << std::endl;
    } catch (const libconfig::SettingNotFoundException& ex) {
        std::cerr << "Setting not found: " << ex.getPath() << std::endl;
    } catch (const libconfig::SettingTypeException& ex) {
        std::cerr << "Setting type error: " << ex.getPath() << std::endl;
    } catch (const std::exception& ex) {
        std::cerr << "Error parsing scene file: " << ex.what() << std::endl;
    }

    return builder->build();
}


void ShadersParser::parsePluginShader(const libconfig::Setting& shader, std::shared_ptr<SceneBuilder> builder) {
    static ShaderFactory shaderFactory;

    try {
        auto shaderInstance = shaderFactory.createShaderFromSetting(shader);
        if (shaderInstance)
            builder->addShader(shaderInstance);
    } catch (const libconfig::SettingException& ex) {
        std::cerr << "Error parsing shader: " << ex.what() << std::endl;
    } catch (const std::exception& ex) {
        std::cerr << "Error creating shader: " << ex.what() << std::endl;
    }
}


void PrimitivesParser::parsePluginPrimitives(const std::string& typeName,
                                    const libconfig::Setting& primitives,
                                    std::shared_ptr<SceneBuilder> builder) {
    auto pluginManager = PrimitivePluginManager::getInstance();
    auto plugin = pluginManager->getPlugin(typeName);

    if (!plugin) {
        std::cerr << "Plugin not found for type: " << typeName << std::endl;
        return;
    }

    std::vector<std::string> requiredParams = plugin->getRequiredParameters();

    int count = primitives.getLength();

    for (int i = 0; i < count; ++i) {
        const libconfig::Setting& primitive = primitives[i];

        std::map<std::string, double> params = extractParametersFromSetting(primitive, requiredParams);

        auto material = extractMaterialFromSetting(primitive);

        try {
            auto primitiveObj = pluginManager->createPrimitive(typeName, params, material);
            if (primitiveObj) {
                builder->addPrimitive(primitiveObj);
            } else {
                std::cerr << "Failed to create primitive of type: " << typeName << std::endl;
            }
        } catch (const std::exception& ex) {
            std::cerr << "Error creating primitive: " << ex.what() << std::endl;
        }
    }
}

std::map<std::string, double> PrimitivesParser::extractParametersFromSetting(
    const libconfig::Setting& setting,
    const std::vector<std::string>& requiredParams) {
    std::map<std::string, double> params;

    for (int i = 0; i < setting.getLength(); ++i) {
        const std::string& name = setting[i].getName();
        if (setting[i].getType() == libconfig::Setting::TypeFloat ||
            setting[i].getType() == libconfig::Setting::TypeInt) {
            params[name] = static_cast<double>(setting[i]);
        } else if (setting[i].getType() == libconfig::Setting::TypeGroup) {
            if (name == "position" || name == "apex" || name == "center") {
                if (setting[i].exists("x")) params["x"] = static_cast<double>(setting[i]["x"]);
                if (setting[i].exists("y")) params["y"] = static_cast<double>(setting[i]["y"]);
                if (setting[i].exists("z")) params["z"] = static_cast<double>(setting[i]["z"]);
            } else if (name == "direction" || name == "axis" || name == "normal") {
                if (setting[i].exists("x")) params["ax"] = static_cast<double>(setting[i]["x"]);
                if (setting[i].exists("y")) params["ay"] = static_cast<double>(setting[i]["y"]);
                if (setting[i].exists("z")) params["az"] = static_cast<double>(setting[i]["z"]);
            }
        }
    }

    if (setting.exists("x") && !params.count("x"))
        params["x"] = static_cast<double>(setting["x"]);
    if (setting.exists("y") && !params.count("y"))
        params["y"] = static_cast<double>(setting["y"]);
    if (setting.exists("z") && !params.count("z"))
        params["z"] = static_cast<double>(setting["z"]);

    if (setting.exists("r") && !params.count("radius"))
        params["radius"] = static_cast<double>(setting["r"]);

    for (const auto& param : requiredParams) {
        if (params.find(param) == params.end()) {
            if (param == "ax" || param == "nx") {
                params[param] = 0.0;
            } else if (param == "ay" || param == "ny") {
                params[param] = 1.0;
            } else if (param == "az" || param == "nz") {
                params[param] = 0.0;
            } else if (param == "radius") {
                params[param] = 1.0;
            } else if (param == "height") {
                params[param] = 2.0;
            } else if (param == "width") {
                params[param] = 1.0;
            } else if (param == "depth") {
                params[param] = 1.0;
            } else {
                std::cerr << "Warning: Missing required parameter '" << param << "'" << std::endl;
                params[param] = 0.0;
            }
        }
    }
    return params;
}

std::shared_ptr<Material> PrimitivesParser::extractMaterialFromSetting(const libconfig::Setting& setting) {
    auto material = std::make_shared<Material>();

    if (setting.exists("material") && setting["material"].exists("color")) {
        material->color = parseColor(setting["material"]["color"]);

        if (setting["material"].exists("reflectivity")) {
            material->reflectivity = static_cast<double>(setting["material"]["reflectivity"]);
            material->reflectivity = std::max(0.0, std::min(1.0, material->reflectivity));
        }

        if (setting["material"].exists("transparency")) {
            material->transparency = static_cast<double>(setting["material"]["transparency"]);
            material->transparency = std::max(0.0, std::min(1.0, material->transparency));
        }

        if (setting["material"].exists("refractionIndex")) {
            material->refractionIndex = static_cast<double>(setting["material"]["refractionIndex"]);
            material->refractionIndex = std::max(1.0, material->refractionIndex);
        }

        // Ajout du support pour les textures
        if (setting["material"].exists("texture")) {
            const libconfig::Setting& textureSetting = setting["material"]["texture"];
            if (textureSetting.exists("path")) {
                std::string texturePath = static_cast<const char*>(textureSetting["path"]);
                try {
                    std::cout << "Loading texture from file: " << texturePath << std::endl;
                    auto texture = std::make_shared<ImageTexture>(texturePath);
                    material->setTexture(texture);
                    std::cout << "Successfully loaded texture from: " << texturePath << std::endl;
                } catch (const std::exception& e) {
                    std::cerr << "Error loading texture: " << texturePath << " - " << e.what() << std::endl;
                    // Si le chargement échoue, on essaie avec un chemin relatif au dossier d'exécution
                    try {
                        std::string alternativePath = "../../" + texturePath;
                        std::cout << "Trying alternative path: " << alternativePath << std::endl;
                        auto texture = std::make_shared<ImageTexture>(alternativePath);
                        material->setTexture(texture);
                        std::cout << "Successfully loaded texture from alternative path: " << alternativePath << std::endl;
                    } catch (const std::exception& e) {
                        std::cerr << "Error loading texture from alternative path: " << e.what() << std::endl;
                    }
                }
            } else if (textureSetting.exists("type")) {
                // Traitement des textures procédurales
                std::string textureType = static_cast<const char*>(textureSetting["type"]);
                
                if (textureType == "chessboard") {
                    // Création d'une texture en damier
                    double scale = textureSetting.exists("scale") ? 
                        static_cast<double>(textureSetting["scale"]) : 1.0;
                    
                    Math::Vector3D color1(Math::Coords{0.0, 0.0, 0.0});
                    Math::Vector3D color2(Math::Coords{1.0, 1.0, 1.0});
                    
                    if (textureSetting.exists("color1")) {
                        color1 = parseColor(textureSetting["color1"]);
                    }
                    
                    if (textureSetting.exists("color2")) {
                        color2 = parseColor(textureSetting["color2"]);
                    }
                    
                    auto texture = std::make_shared<ChessboardTexture>(color1, color2, scale);
                    material->setTexture(texture);
                    std::cout << "Created chessboard texture with scale: " << scale << std::endl;
                } else if (textureType == "perlin") {
                    // Création d'une texture de bruit de Perlin
                    double scale = textureSetting.exists("scale") ? 
                        static_cast<double>(textureSetting["scale"]) : 1.0;
                    
                    int octaves = textureSetting.exists("octaves") ? 
                        static_cast<int>(textureSetting["octaves"]) : 4;
                    
                    double persistence = textureSetting.exists("persistence") ? 
                        static_cast<double>(textureSetting["persistence"]) : 0.5;
                    
                    Math::Vector3D color1(Math::Coords{1.0, 1.0, 1.0});
                    Math::Vector3D color2(Math::Coords{0.0, 0.0, 0.0});
                    
                    auto texture = std::make_shared<PerlinNoiseTexture>(color1, color2, scale, persistence, octaves);
                    material->setTexture(texture);
                    std::cout << "Created perlin noise texture with scale: " << scale 
                              << ", octaves: " << octaves 
                              << ", persistence: " << persistence << std::endl;
                } else {
                    std::cerr << "Unknown texture type: " << textureType << std::endl;
                }
            }
        }

        // Ajout du support pour les normal maps
        if (setting["material"].exists("normalMap")) {
            const libconfig::Setting& normalMapSetting = setting["material"]["normalMap"];
            if (normalMapSetting.exists("path")) {
                std::string normalMapPath = static_cast<const char*>(normalMapSetting["path"]);
                
                // Récupération de la force de l'effet (strength)
                double strength = 1.0;  // Valeur par défaut
                if (normalMapSetting.exists("strength")) {
                    strength = static_cast<double>(normalMapSetting["strength"]);
                    strength = std::max(0.0, std::min(1.0, strength));  // Limiter entre 0 et 1
                }

                try {
                    std::cout << "Loading normal map from file: " << normalMapPath << " with strength: " << strength << std::endl;
                    auto normalMap = std::make_shared<NormalMap>(normalMapPath, strength);
                    material->setNormalMap(normalMap);
                    std::cout << "Successfully loaded normal map from: " << normalMapPath << std::endl;
                } catch (const std::exception& e) {
                    std::cerr << "Error loading normal map: " << normalMapPath << " - " << e.what() << std::endl;
                    // Si le chargement échoue, on essaie avec un chemin relatif au dossier d'exécution
                    try {
                        std::string alternativePath = "../../" + normalMapPath;
                        std::cout << "Trying alternative path for normal map: " << alternativePath << " with strength: " << strength << std::endl;
                        auto normalMap = std::make_shared<NormalMap>(alternativePath, strength);
                        material->setNormalMap(normalMap);
                        std::cout << "Successfully loaded normal map from alternative path: " << alternativePath << std::endl;
                    } catch (const std::exception& e) {
                        std::cerr << "Error loading normal map from alternative path: " << e.what() << std::endl;
                    }
                }
            }
        }
        
        // Ajout du support pour les displacement maps
        if (setting["material"].exists("displacementMap")) {
            const libconfig::Setting& dispMapSetting = setting["material"]["displacementMap"];
            if (dispMapSetting.exists("path")) {
                std::string dispMapPath = static_cast<const char*>(dispMapSetting["path"]);
                
                // Récupération de la force de l'effet (strength)
                double strength = 1.0;  // Valeur par défaut
                if (dispMapSetting.exists("strength")) {
                    strength = static_cast<double>(dispMapSetting["strength"]);
                    strength = std::max(0.0, std::min(1.0, strength));  // Limiter entre 0 et 1
                }

                try {
                    std::cout << "Loading displacement map from file: " << dispMapPath << " with strength: " << strength << std::endl;
                    auto displacementMap = std::make_shared<DisplacementMap>(dispMapPath, strength);
                    material->setDisplacementMap(displacementMap);
                    std::cout << "Successfully loaded displacement map from: " << dispMapPath << std::endl;
                } catch (const std::exception& e) {
                    std::cerr << "Error loading displacement map: " << dispMapPath << " - " << e.what() << std::endl;
                    // Si le chargement échoue, on essaie avec un chemin relatif au dossier d'exécution
                    try {
                        std::string alternativePath = "../../" + dispMapPath;
                        std::cout << "Trying alternative path for displacement map: " << alternativePath << std::endl;
                        auto displacementMap = std::make_shared<DisplacementMap>(alternativePath, strength);
                        material->setDisplacementMap(displacementMap);
                        std::cout << "Successfully loaded displacement map from alternative path: " << alternativePath << std::endl;
                    } catch (const std::exception& e) {
                        std::cerr << "Error loading displacement map from alternative path: " << e.what() << std::endl;
                    }
                }
            }
        }
        
        // Ajout du support pour les specular maps
        if (setting["material"].exists("specularMap")) {
            const libconfig::Setting& specMapSetting = setting["material"]["specularMap"];
            if (specMapSetting.exists("path")) {
                std::string specMapPath = static_cast<const char*>(specMapSetting["path"]);

                try {
                    std::cout << "Loading specular map from file: " << specMapPath << std::endl;
                    auto specularMap = std::make_shared<SpecularMap>(specMapPath);
                    material->setSpecularMap(specularMap);
                    std::cout << "Successfully loaded specular map from: " << specMapPath << std::endl;
                } catch (const std::exception& e) {
                    std::cerr << "Error loading specular map: " << specMapPath << " - " << e.what() << std::endl;
                    // Si le chargement échoue, on essaie avec un chemin relatif au dossier d'exécution
                    try {
                        std::string alternativePath = "../../" + specMapPath;
                        std::cout << "Trying alternative path for specular map: " << alternativePath << std::endl;
                        auto specularMap = std::make_shared<SpecularMap>(alternativePath);
                        material->setSpecularMap(specularMap);
                        std::cout << "Successfully loaded specular map from alternative path: " << alternativePath << std::endl;
                    } catch (const std::exception& e) {
                        std::cerr << "Error loading specular map from alternative path: " << e.what() << std::endl;
                    }
                }
            }
        }
        
        // Ajout du support pour les ambient occlusion maps
        if (setting["material"].exists("aoMap") || setting["material"].exists("ambientOcclusionMap")) {
            const libconfig::Setting& aoMapSetting = setting["material"].exists("aoMap") ? 
                setting["material"]["aoMap"] : setting["material"]["ambientOcclusionMap"];
            
            if (aoMapSetting.exists("path")) {
                std::string aoMapPath = static_cast<const char*>(aoMapSetting["path"]);
                
                // Récupération de la force de l'effet (strength)
                double strength = 1.0;  // Valeur par défaut
                if (aoMapSetting.exists("strength")) {
                    strength = static_cast<double>(aoMapSetting["strength"]);
                    strength = std::max(0.0, std::min(1.0, strength));  // Limiter entre 0 et 1
                }

                try {
                    std::cout << "Loading ambient occlusion map from file: " << aoMapPath << " with strength: " << strength << std::endl;
                    auto aoMap = std::make_shared<AmbientOcclusionMap>(aoMapPath, strength);
                    material->setAmbientOcclusionMap(aoMap);
                    std::cout << "Successfully loaded ambient occlusion map from: " << aoMapPath << std::endl;
                } catch (const std::exception& e) {
                    std::cerr << "Error loading ambient occlusion map: " << aoMapPath << " - " << e.what() << std::endl;
                    // Si le chargement échoue, on essaie avec un chemin relatif au dossier d'exécution
                    try {
                        std::string alternativePath = "../../" + aoMapPath;
                        std::cout << "Trying alternative path for ambient occlusion map: " << alternativePath << std::endl;
                        auto aoMap = std::make_shared<AmbientOcclusionMap>(alternativePath, strength);
                        material->setAmbientOcclusionMap(aoMap);
                        std::cout << "Successfully loaded ambient occlusion map from alternative path: " << alternativePath << std::endl;
                    } catch (const std::exception& e) {
                        std::cerr << "Error loading ambient occlusion map from alternative path: " << e.what() << std::endl;
                    }
                }
            }
        }
        
        // Récupération du coefficient de brillance (shininess) pour les reflets spéculaires
        if (setting["material"].exists("shininess")) {
            material->shininess = static_cast<double>(setting["material"]["shininess"]);
            // Limiter à une valeur raisonnable
            material->shininess = std::max(1.0, material->shininess);
        }

        // Ajout du support pour les normal maps
        if (setting["material"].exists("normalMap")) {
            const libconfig::Setting& normalMapSetting = setting["material"]["normalMap"];
            if (normalMapSetting.exists("path")) {
                std::string normalMapPath = static_cast<const char*>(normalMapSetting["path"]);
                
                // Récupération de la force de l'effet (strength)
                double strength = 1.0;  // Valeur par défaut
                if (normalMapSetting.exists("strength")) {
                    strength = static_cast<double>(normalMapSetting["strength"]);
                    strength = std::max(0.0, std::min(1.0, strength));  // Limiter entre 0 et 1
                }

                try {
                    std::cout << "Loading normal map from file: " << normalMapPath << " with strength: " << strength << std::endl;
                    auto normalMap = std::make_shared<NormalMap>(normalMapPath, strength);
                    material->setNormalMap(normalMap);
                    std::cout << "Successfully loaded normal map from: " << normalMapPath << std::endl;
                } catch (const std::exception& e) {
                    std::cerr << "Error loading normal map: " << normalMapPath << " - " << e.what() << std::endl;
                    // Si le chargement échoue, on essaie avec un chemin relatif au dossier d'exécution
                    try {
                        std::string alternativePath = "../../" + normalMapPath;
                        std::cout << "Trying alternative path for normal map: " << alternativePath << " with strength: " << strength << std::endl;
                        auto normalMap = std::make_shared<NormalMap>(alternativePath, strength);
                        material->setNormalMap(normalMap);
                        std::cout << "Successfully loaded normal map from alternative path: " << alternativePath << std::endl;
                    } catch (const std::exception& e) {
                        std::cerr << "Error loading normal map from alternative path: " << e.what() << std::endl;
                    }
                }
            }
        }

        // Ajout du support pour les textures procédurales
        if (setting["material"].exists("proceduralTexture")) {
            const libconfig::Setting& proceduralTextureSetting = setting["material"]["proceduralTexture"];
            if (proceduralTextureSetting.exists("type")) {
                std::string type = static_cast<const char*>(proceduralTextureSetting["type"]);
                try {
                    if (type == "chessboard") {
                        auto texture = std::make_shared<ChessboardTexture>();
                        material->setTexture(texture);
                    } else if (type == "perlinNoise") {
                        auto texture = std::make_shared<PerlinNoiseTexture>();
                        material->setTexture(texture);
                    } else {
                        std::cerr << "Unknown procedural texture type: " << type << std::endl;
                    }
                } catch (const std::exception& e) {
                    std::cerr << "Error creating procedural texture: " << e.what() << std::endl;
                }
            }
        }
    } else if (setting.exists("color")) {
        material->color = parseColor(setting["color"]);
    } else {
        material->color = Math::Vector3D(Math::Coords{1.0, 1.0, 1.0});
    }

    return material;
}

}  // namespace RayTracer
