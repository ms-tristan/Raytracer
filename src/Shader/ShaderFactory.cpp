// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ShaderFactory implementation
*/
#include "ShaderFactory.hpp"
#include "BasicShader.hpp"
#include "Shader/PhongShader/PhongShader.hpp"
#include "Shader/ToonShader/ToonShader.hpp"
#include "Shader/FresnelShader/FresnelShader.hpp"
#include "ShaderDecorator/FresnelDecorator.hpp"
#include <iostream>

namespace RayTracer {

ShaderFactory::ShaderFactory() {
    registerBuiltInShaders();
}

std::shared_ptr<IShader> ShaderFactory::createShader(const std::string& type, const libconfig::Setting& setting) {
    auto it = shaderCreators.find(type);
    if (it != shaderCreators.end()) {
        return it->second(setting);
    }

    std::cerr << "Unknown shader type: " << type << ". Using basic shader instead." << std::endl;
    return std::make_shared<BasicShader>();
}

std::shared_ptr<IShader> ShaderFactory::createShaderFromSetting(const libconfig::Setting& setting) {
    try {
        if (!setting.exists("type")) {
            std::cerr << "Shader setting missing 'type' field. Using basic shader." << std::endl;
            return std::make_shared<BasicShader>();
        }

        std::string type = setting["type"];
        return createShader(type, setting);
    } catch (const libconfig::SettingException& e) {
        std::cerr << "Error creating shader: " << e.what() << ". Using basic shader." << std::endl;
        return std::make_shared<BasicShader>();
    }
}

void ShaderFactory::registerShader(const std::string& type, ShaderCreator creator) {
    shaderCreators[type] = creator;
}

void ShaderFactory::registerBuiltInShaders() {
    registerShader("basic", [](const libconfig::Setting&) {
        return std::make_shared<BasicShader>();
    });

    registerShader("phong", [](const libconfig::Setting& setting) {
        double specularStrength = 0.5;
        double shininess = 32.0;

        if (setting.exists("specularStrength")) {
            specularStrength = static_cast<double>(setting["specularStrength"]);
        }

        if (setting.exists("shininess")) {
            shininess = static_cast<double>(setting["shininess"]);
        }

        return std::make_shared<PhongShader>(specularStrength, shininess);
    });

    registerShader("toon", [](const libconfig::Setting& setting) {
        int levels = 4;
        double edgeThreshold = 0.2;

        if (setting.exists("levels")) {
            levels = static_cast<int>(setting["levels"]);
        }

        if (setting.exists("edgeThreshold")) {
            edgeThreshold = static_cast<double>(setting["edgeThreshold"]);
        }

        return std::make_shared<ToonShader>(levels, edgeThreshold);
    });

    registerShader("fresnel", [](const libconfig::Setting& setting) {
        double fresnelPower = 2.0;
        Math::Vector3D rimColor(Math::Coords{1.0, 1.0, 1.0});

        if (setting.exists("fresnelPower")) {
            fresnelPower = static_cast<double>(setting["fresnelPower"]);
        }

        if (setting.exists("rimColor")) {
            double r = 1.0, g = 1.0, b = 1.0;
            const libconfig::Setting& color = setting["rimColor"];

            if (color.exists("r")) r = static_cast<double>(color["r"]);
            if (color.exists("g")) g = static_cast<double>(color["g"]);
            if (color.exists("b")) b = static_cast<double>(color["b"]);

            rimColor = Math::Vector3D(Math::Coords{r, g, b});
        }

        return std::make_shared<FresnelShader>(fresnelPower, rimColor);
    });

    registerShader("fresnel-decorator", [this](const libconfig::Setting& setting) {
        double fresnelPower = 2.0;
        Math::Vector3D rimColor(Math::Coords{1.0, 1.0, 1.0});

        if (setting.exists("fresnelPower")) {
            fresnelPower = static_cast<double>(setting["fresnelPower"]);
        }

        if (setting.exists("rimColor")) {
            double r = 1.0, g = 1.0, b = 1.0;
            const libconfig::Setting& color = setting["rimColor"];

            if (color.exists("r")) r = static_cast<double>(color["r"]);
            if (color.exists("g")) g = static_cast<double>(color["g"]);
            if (color.exists("b")) b = static_cast<double>(color["b"]);

            rimColor = Math::Vector3D(Math::Coords{r, g, b});
        }

            std::shared_ptr<IShader> baseShader;
        if (setting.exists("baseShader")) {
            baseShader = createShaderFromSetting(setting["baseShader"]);
        } else {
            baseShader = std::make_shared<BasicShader>();
        }

        return std::make_shared<FresnelDecorator>(baseShader, fresnelPower, rimColor);
    });
}

}  // namespace RayTracer