#include <iostream>
#include <memory>
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

void CameraParser::parse(const libconfig::Setting& setting, SceneBuilder& builder) {
    Math::Point3D position;
    if (setting.exists("position")) {
        position = parsePoint3D(setting["position"]);
    } else {
        position = Math::Point3D(Math::Coords{0, 0, 5});
    }

    Math::Point3D lookAt;
    if (setting.exists("rotation")) {
        Math::Vector3D forward(Math::Coords{0, 0, -1});
        lookAt = position + forward;
    } else {
        lookAt = Math::Point3D(Math::Coords{0, 0, 0});
    }

    builder.setCamera(position, lookAt);
}

void LightsParser::parse(const libconfig::Setting& setting, SceneBuilder& builder) {

    double ambientIntensity = getValueOrDefault<double>(setting, "ambient", 0.4);
    builder.setAmbientLight(Math::Vector3D(Math::Coords{
        ambientIntensity, ambientIntensity, ambientIntensity
    }));

    if (setting.exists("point")) {
        parsePointLights(setting["point"], builder);
    }

    if (setting.exists("directional")) {
        parseDirectionalLights(setting["directional"], builder);
    }
}

void LightsParser::parsePointLights(const libconfig::Setting& lights, SceneBuilder& builder) {
    int count = lights.getLength();
    for (int i = 0; i < count; ++i) {
        const libconfig::Setting& light = lights[i];

        Math::Point3D position;
        if (light.exists("position")) {
            position = parsePoint3D(light["position"]);
        } else {
            position = parsePoint3D(light);
        }

        Math::Vector3D color(Math::Coords{1.0, 1.0, 1.0});
        if (light.exists("color")) {
            color = parseColor(light["color"]);
        }

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

        builder.addLight(pointLight);
    }
}

void LightsParser::parseDirectionalLights(const libconfig::Setting& lights, SceneBuilder& builder) {
    int count = lights.getLength();
    for (int i = 0; i < count; ++i) {
        const libconfig::Setting& light = lights[i];

        Math::Vector3D direction;
        if (light.exists("direction")) {
            direction = parseVector3D(light["direction"]).normalize();
        } else {
            direction = parseVector3D(light).normalize();
        }

        Math::Vector3D color(Math::Coords{1.0, 1.0, 1.0});
        if (light.exists("color")) {
            color = parseColor(light["color"]);
        }

        auto dirLight = std::make_shared<DirectionalLight>(direction, color);
        builder.addLight(dirLight);
    }
}

void PrimitivesParser::parse(const libconfig::Setting& setting, SceneBuilder& builder) {
    auto pluginManager = PrimitivePluginManager::getInstance();
    if (!pluginManager->loadAllPlugins("plugins/primitives")) {
        std::cerr << "Failed to load plugins." << std::endl;
        return;
    }
    auto loadedPluginNames = pluginManager->getLoadedPluginNames();

    std::cout << "Loaded plugin names: " << std::endl;
    for (const auto& typeName : loadedPluginNames) {
        std::cout << "Parsing plugin primitive: " << typeName << std::endl;
        if (setting.exists(typeName)) {
            std::cout << "Found setting for plugin primitive: " << typeName << " in parameter " << setting.getName() << std::endl;
            parsePluginPrimitives(typeName, setting[typeName], builder);
        }
    }
}

void ShadersParser::parse(const libconfig::Setting& setting, SceneBuilder& builder) {
    auto pluginManager = ShaderPluginManager::getInstance();
    if (!pluginManager->loadAllPlugins("plugins/shaders")) {
        std::cerr << "Failed to load shader plugins." << std::endl;
    } else {
        auto loadedPluginNames = pluginManager->getLoadedPluginNames();
        std::cout << "Loaded shader plugins: " << loadedPluginNames.size() << std::endl;

        loadShaderPlugins();
    }

    int count = setting.getLength();
    for (int i = 0; i < count; ++i) {
        const libconfig::Setting& shader = setting[i];
        parsePluginShader(shader, builder);
    }
}

SceneConfigParser::SceneConfigParser() {
    sectionParsers["camera"] = std::make_unique<CameraParser>();
    sectionParsers["lights"] = std::make_unique<LightsParser>();
    sectionParsers["primitives"] = std::make_unique<PrimitivesParser>();
    sectionParsers["shaders"] = std::make_unique<ShadersParser>();
}

std::unique_ptr<Scene> SceneConfigParser::parseFile(const std::string& filename) {
    builder.reset();

    try {
        libconfig::Config cfg;
        cfg.readFile(filename.c_str());


        for (const auto& [section, parser] : sectionParsers) {
            if (cfg.exists(section)) {
                parser->parse(cfg.lookup(section), builder);
            }
        }

        return builder.build();

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

    return builder.build();
}


void ShadersParser::parsePluginShader(const libconfig::Setting& shader, SceneBuilder& builder) {
    static ShaderFactory shaderFactory;

    try {
        auto shaderInstance = shaderFactory.createShaderFromSetting(shader);
        if (shaderInstance) {
            builder.addShader(shaderInstance);
        }
    } catch (const libconfig::SettingException& ex) {
        std::cerr << "Error parsing shader: " << ex.what() << std::endl;
    } catch (const std::exception& ex) {
        std::cerr << "Error creating shader: " << ex.what() << std::endl;
    }
}


void PrimitivesParser::parsePluginPrimitives(const std::string& typeName,
                                    const libconfig::Setting& primitives,
                                    SceneBuilder& builder) {
    auto pluginManager = PrimitivePluginManager::getInstance();
    auto plugin = pluginManager->getPlugin(typeName);

    if (!plugin) {
        std::cerr << "Plugin not found for type: " << typeName << std::endl;
        return;
    }

    std::vector<std::string> requiredParams = plugin->getRequiredParameters();

    int count = primitives.getLength();
    std::cout << "Found " << count << " primitives of type: " << typeName << std::endl;

    for (int i = 0; i < count; ++i) {
        const libconfig::Setting& primitive = primitives[i];

        std::map<std::string, double> params = extractParametersFromSetting(primitive, requiredParams);

        auto material = extractMaterialFromSetting(primitive);

        std::cout << "Creating primitive of type: " << typeName << std::endl;

        try {
            auto primitiveObj = pluginManager->createPrimitive(typeName, params, material);
            if (primitiveObj) {
                builder.addPrimitive(primitiveObj);
                std::cout << "Successfully created primitive of type: " << typeName << std::endl;
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
            if (param == "ax" || param == "nx") params[param] = 0.0;
            else if (param == "ay" || param == "ny") params[param] = 1.0;
            else if (param == "az" || param == "nz") params[param] = 0.0;
            else if (param == "radius") params[param] = 1.0;
            else if (param == "height") params[param] = 2.0;
            else if (param == "width") params[param] = 1.0;
            else if (param == "depth") params[param] = 1.0;
            else {
                std::cerr << "Warning: Missing required parameter '" << param << "'" << std::endl;
                params[param] = 0.0; // Default fallback
            }
        }
    }

    return params;
}

std::shared_ptr<Material> PrimitivesParser::extractMaterialFromSetting(const libconfig::Setting& setting) {
    auto material = std::make_shared<Material>();

    if (setting.exists("material") && setting["material"].exists("color")) {
        material->color = parseColor(setting["material"]["color"]);
    } else if (setting.exists("color")) {
        material->color = parseColor(setting["color"]);
    } else {
        material->color = Math::Vector3D(Math::Coords{1.0, 1.0, 1.0});
    }

    return material;
}

}  // namespace RayTracer
