#include <iostream>
#include <memory>
#include <libconfig.h++>
#include "ConfigParser.hpp"
#include "Light/DirectionalLight/DirectionalLight.hpp"
#include "Light/AmbientLight/AmbientLight.hpp"
#include "Light/PointLight/PointLight.hpp"
#include "Material/Material.hpp"
#include "Shader/ShaderFactory.hpp"

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

    if (setting.exists("spheres")) {
        parseSpheres(setting["spheres"], builder);
    }


    if (setting.exists("planes")) {
        parsePlanes(setting["planes"], builder);
    }


    if (setting.exists("cylinders")) {
        parseCylinders(setting["cylinders"], builder);
    }


    if (setting.exists("cones")) {
        parseCones(setting["cones"], builder);
    }
}

void PrimitivesParser::parseSpheres(const libconfig::Setting& spheres, SceneBuilder& builder) {
    int count = spheres.getLength();
    for (int i = 0; i < count; ++i) {
        const libconfig::Setting& sphere = spheres[i];


        Math::Point3D position;
        if (sphere.exists("position")) {
            position = parsePoint3D(sphere["position"]);
        } else {

            try {
                double x = sphere["x"];
                double y = sphere["y"];
                double z = sphere["z"];
                position = Math::Point3D(Math::Coords{x, y, z});
            } catch (const libconfig::SettingNotFoundException& ex) {
                std::cerr << "Sphere position settings not found!" << std::endl;
                position = Math::Point3D(Math::Coords{0, 0, 0});
            }
        }

        double radius = 1.0;
        if (sphere.exists("radius")) {
            radius = static_cast<double>(sphere["radius"]);
        } else if (sphere.exists("r")) {
            radius = static_cast<double>(sphere["r"]);
        }


        auto material = std::make_shared<Material>();
        if (sphere.exists("material") && sphere["material"].exists("color")) {
            material->color = parseColor(sphere["material"]["color"]);
        } else if (sphere.exists("color")) {
            material->color = parseColor(sphere["color"]);
        } else {
            material->color = Math::Vector3D(Math::Coords{1.0, 1.0, 1.0});
        }


        builder.addSphere(position, radius, material);
    }
}

void PrimitivesParser::parseCylinders(const libconfig::Setting& cylinders, SceneBuilder& builder) {
    int count = cylinders.getLength();
    for (int i = 0; i < count; ++i) {
        const libconfig::Setting& cylinder = cylinders[i];
        Math::Point3D center = parsePoint3D(cylinder);


        double radius = cylinder.exists("radius") ?
            static_cast<double>(cylinder["radius"]) : 1.0;
        double height = cylinder.exists("height") ?
            static_cast<double>(cylinder["height"]) : 2.0;


            Math::Vector3D direction;
        if (cylinder.exists("direction")) {
            direction = parseVector3D(cylinder["direction"]);
        } else {
            direction = Math::Vector3D(Math::Coords{0, 1, 0});
        }

        auto material = std::make_shared<Material>();
        if (cylinder.exists("material") && cylinder["material"].exists("color")) {
            material->color = parseColor(cylinder["material"]["color"]);
        } else if (cylinder.exists("color")) {
            material->color = parseColor(cylinder["color"]);
        } else {
            material->color = Math::Vector3D(Math::Coords{1.0, 1.0, 1.0});
        }

        builder.addCylinder(center, direction, radius, height, material);
    }
}

void PrimitivesParser::parseCones(const libconfig::Setting& cones, SceneBuilder& builder) {
    int count = cones.getLength();
    for (int i = 0; i < count; ++i) {
        const libconfig::Setting& cone = cones[i];
        Math::Point3D apex = parsePoint3D(cone);


        double radius = cone.exists("radius") ?
            static_cast<double>(cone["radius"]) : 1.0;
        double height = cone.exists("height") ?
            static_cast<double>(cone["height"]) : 2.0;


            Math::Vector3D direction;
        if (cone.exists("direction")) {
            direction = parseVector3D(cone["direction"]);
        } else {
            direction = Math::Vector3D(Math::Coords{0, 1, 0});
        }

        auto material = std::make_shared<Material>();
        if (cone.exists("material") && cone["material"].exists("color")) {
            material->color = parseColor(cone["material"]["color"]);
        } else if (cone.exists("color")) {
            material->color = parseColor(cone["color"]);
        } else {
            material->color = Math::Vector3D(Math::Coords{1.0, 1.0, 1.0});
        }

        builder.addCone(apex, direction, radius, height, material);
    }
}

void ShadersParser::parseShader(const libconfig::Setting& shader, SceneBuilder& builder) {
    ShaderFactory shaderFactory;

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

void PrimitivesParser::parsePlanes(const libconfig::Setting& planes, SceneBuilder& builder) {
    int count = planes.getLength();
    for (int i = 0; i < count; ++i) {
        const libconfig::Setting& plane = planes[i];

        Math::Point3D position;
        if (plane.exists("position")) {
            position = parsePoint3D(plane["position"]);
        } else {

            try {
                std::string direction = plane["direction"];
                double pos = plane["position"];

                if (direction == "X") {
                    position = Math::Point3D(Math::Coords{pos, 0, 0});
                } else if (direction == "Y") {
                    position = Math::Point3D(Math::Coords{0, pos, 0});
                } else if (direction == "Z") {
                    position = Math::Point3D(Math::Coords{0, 0, pos});
                }
            } catch (const libconfig::SettingNotFoundException& ex) {
                std::cerr << "Plane position settings not found!" << std::endl;
                position = Math::Point3D(Math::Coords{0, -1, 0});
            }
        }

        Math::Vector3D normal;
        if (plane.exists("normal")) {
            normal = parseVector3D(plane["normal"]).normalize();
        } else {

            normal = Math::Vector3D(Math::Coords{0, 1, 0});
        }

        auto material = std::make_shared<Material>();
        if (plane.exists("material") && plane["material"].exists("color")) {
            material->color = parseColor(plane["material"]["color"]);
        } else if (plane.exists("color")) {
            material->color = parseColor(plane["color"]);
        } else {
            material->color = Math::Vector3D(Math::Coords{1.0, 1.0, 1.0});
        }

        builder.addPlane(position, normal, material);
    }
}

void ShadersParser::parse(const libconfig::Setting& setting, SceneBuilder& builder) {
    int count = setting.getLength();

    ShaderFactory shaderFactory;

    for (int i = 0; i < count; ++i) {
        const libconfig::Setting& shader = setting[i];
        parseShader(shader, builder);
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

}  // namespace RayTracer
