#include <iostream>
#include <memory>
#include <libconfig.h++>
#include "ConfigParser.hpp"
#include "Light/DirectionalLight/DirectionalLight.hpp"
#include "Light/AmbientLight/AmbientLight.hpp"
#include "Light/PointLight/PointLight.hpp"
#include "Material/Material.hpp"

namespace RayTracer {

// Helper methods for ConfigParser base class
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
    int r = setting.exists("r") ? static_cast<int>(setting["r"]) : 255;
    int g = setting.exists("g") ? static_cast<int>(setting["g"]) : 255;
    int b = setting.exists("b") ? static_cast<int>(setting["b"]) : 255;
    return Math::Vector3D(Math::Coords{r / 255.0, g / 255.0, b / 255.0});
}

// CameraParser implementation
void CameraParser::parse(const libconfig::Setting& setting, SceneBuilder& builder) {
    Math::Point3D position;
    if (setting.exists("position")) {
        position = parsePoint3D(setting["position"]);
    } else {
        position = Math::Point3D(Math::Coords{0, 0, 5});
    }

    // Calculate lookAt point
    Math::Point3D lookAt;
    if (setting.exists("rotation")) {
        Math::Vector3D forward(Math::Coords{0, 0, -1});
        lookAt = position + forward;
    } else {
        lookAt = Math::Point3D(Math::Coords{0, 0, 0});
    }

    builder.setCamera(position, lookAt);
}

// LightsParser implementation
void LightsParser::parse(const libconfig::Setting& setting, SceneBuilder& builder) {
    // Parse ambient light
    double ambientIntensity = getValueOrDefault<double>(setting, "ambient", 0.4);
    builder.setAmbientLight(Math::Vector3D(Math::Coords{
        ambientIntensity, ambientIntensity, ambientIntensity
    }));

    // Parse point lights
    if (setting.exists("point")) {
        parsePointLights(setting["point"], builder);
    }

    // Parse directional lights
    if (setting.exists("directional")) {
        parseDirectionalLights(setting["directional"], builder);
    }
}

void LightsParser::parsePointLights(const libconfig::Setting& lights, SceneBuilder& builder) {
    int count = lights.getLength();
    for (int i = 0; i < count; ++i) {
        const libconfig::Setting& light = lights[i];
        Math::Point3D position = parsePoint3D(light);

        // Create and add the point light
        auto pointLight = std::make_shared<PointLight>(
            position,
            Math::Vector3D(Math::Coords{1.0, 1.0, 1.0}), // Default white light
            1.0,   // Constant attenuation
            0.09,  // Linear attenuation
            0.032  // Quadratic attenuation
        );

        builder.addLight(pointLight);
    }
}

void LightsParser::parseDirectionalLights(const libconfig::Setting& lights, SceneBuilder& builder) {
    int count = lights.getLength();
    for (int i = 0; i < count; ++i) {
        const libconfig::Setting& light = lights[i];
        Math::Vector3D direction = parseVector3D(light).normalize();

        // Create and add the directional light
        auto dirLight = std::make_shared<DirectionalLight>(
            direction,
            Math::Vector3D(Math::Coords{1.0, 1.0, 1.0}) // Default white light
        );

        builder.addLight(dirLight);
    }
}

// PrimitivesParser implementation
void PrimitivesParser::parse(const libconfig::Setting& setting, SceneBuilder& builder) {
    // Parse spheres
    if (setting.exists("spheres")) {
        parseSpheres(setting["spheres"], builder);
    }

    // Parse planes
    if (setting.exists("planes")) {
        parsePlanes(setting["planes"], builder);
    }

    // Parse cylinders if they exist
    if (setting.exists("cylinders")) {
        parseCylinders(setting["cylinders"], builder);
    }

    // Parse cones if they exist
    if (setting.exists("cones")) {
        parseCones(setting["cones"], builder);
    }
}

void PrimitivesParser::parseSpheres(const libconfig::Setting& spheres, SceneBuilder& builder) {
    int count = spheres.getLength();
    for (int i = 0; i < count; ++i) {
        const libconfig::Setting& sphere = spheres[i];
        double x = sphere["x"];
        double y = sphere["y"];
        double z = sphere["z"];
        double radius = sphere["r"];

        // Create material with the specified color
        auto material = std::make_shared<Material>();
        if (sphere.exists("color")) {
            material->color = parseColor(sphere["color"]);
        } else {
            material->color = Math::Vector3D(Math::Coords{1.0, 1.0, 1.0}); // Default white
        }

        // Add sphere to the scene
        builder.addSphere(
            Math::Point3D(Math::Coords{x, y, z}),
            radius,
            material
        );
    }
}

void PrimitivesParser::parsePlanes(const libconfig::Setting& planes, SceneBuilder& builder) {
    int count = planes.getLength();
    for (int i = 0; i < count; ++i) {
        const libconfig::Setting& plane = planes[i];
        std::string axis = plane["axis"];
        double position = plane["position"];

        // Default normal based on axis
        Math::Vector3D normal(Math::Coords{0, 1, 0}); // Default to Y-axis
        Math::Point3D planePos(Math::Coords{0, position, 0});

        if (axis == "X") {
            normal = Math::Vector3D(Math::Coords{1, 0, 0});
            planePos = Math::Point3D(Math::Coords{position, 0, 0});
        } else if (axis == "Y") {
            normal = Math::Vector3D(Math::Coords{0, 1, 0});
            planePos = Math::Point3D(Math::Coords{0, position, 0});
        } else if (axis == "Z") {
            normal = Math::Vector3D(Math::Coords{0, 0, 1});
            planePos = Math::Point3D(Math::Coords{0, 0, position});
        }

        // Create material with the specified color
        auto material = std::make_shared<Material>();
        if (plane.exists("color")) {
            material->color = parseColor(plane["color"]);
        } else {
            material->color = Math::Vector3D(Math::Coords{1.0, 1.0, 1.0}); // Default white
        }

        // Add plane to the scene
        builder.addPlane(planePos, normal, material);
    }
}

void PrimitivesParser::parseCylinders(const libconfig::Setting& cylinders, SceneBuilder& builder) {
    int count = cylinders.getLength();
    for (int i = 0; i < count; ++i) {
        const libconfig::Setting& cylinder = cylinders[i];
        Math::Point3D center = parsePoint3D(cylinder);

        // Get cylinder properties
        double radius = cylinder.exists("radius") ?
            static_cast<double>(cylinder["radius"]) : 1.0;
        double height = cylinder.exists("height") ?
            static_cast<double>(cylinder["height"]) : 2.0;

        // Parse axis or use default
        Math::Vector3D axis;
        if (cylinder.exists("axis")) {
            axis = parseVector3D(cylinder["axis"]);
        } else {
            axis = Math::Vector3D(Math::Coords{0, 1, 0}); // Default Y-axis
        }

        // Create material
        auto material = std::make_shared<Material>();
        if (cylinder.exists("color")) {
            material->color = parseColor(cylinder["color"]);
        } else {
            material->color = Math::Vector3D(Math::Coords{1.0, 1.0, 1.0}); // Default white
        }

        // Add cylinder to the scene
        builder.addCylinder(center, axis, radius, height, material);
    }
}

void PrimitivesParser::parseCones(const libconfig::Setting& cones, SceneBuilder& builder) {
    int count = cones.getLength();
    for (int i = 0; i < count; ++i) {
        const libconfig::Setting& cone = cones[i];
        Math::Point3D apex = parsePoint3D(cone);

        // Get cone properties
        double radius = cone.exists("radius") ?
            static_cast<double>(cone["radius"]) : 1.0;
        double height = cone.exists("height") ?
            static_cast<double>(cone["height"]) : 2.0;

        // Parse axis or use default
        Math::Vector3D axis;
        if (cone.exists("axis")) {
            axis = parseVector3D(cone["axis"]);
        } else {
            axis = Math::Vector3D(Math::Coords{0, 1, 0}); // Default Y-axis
        }

        // Create material
        auto material = std::make_shared<Material>();
        if (cone.exists("color")) {
            material->color = parseColor(cone["color"]);
        } else {
            material->color = Math::Vector3D(Math::Coords{1.0, 1.0, 1.0}); // Default white
        }

        // Add cone to the scene
        builder.addCone(apex, axis, radius, height, material);
    }
}

// SceneConfigParser implementation
SceneConfigParser::SceneConfigParser() {
    // Initialize parser components
    sectionParsers["camera"] = std::make_unique<CameraParser>();
    sectionParsers["lights"] = std::make_unique<LightsParser>();
    sectionParsers["primitives"] = std::make_unique<PrimitivesParser>();
}

std::unique_ptr<Scene> SceneConfigParser::parseFile(const std::string& filename) {
    builder.reset();

    try {
        libconfig::Config cfg;
        cfg.readFile(filename.c_str());

        // Process each section with its specialized parser
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

    // Return an empty scene in case of errors
    return builder.build();
}

}  // namespace RayTracer