// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Scene implementation
*/
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <limits>
#include <memory>
#include <typeinfo>
#include <map>
#include "Scene/Scene.hpp"
#include "Light/PointLight/PointLight.hpp"
#include "Light/DirectionalLight/DirectionalLight.hpp"
#include "Shader/BasicShader.hpp"

namespace RayTracer {

void Scene::setCamera(const Camera &cam) { camera = cam; }

Camera Scene::getCamera() const { return camera; }

void Scene::setAmbientLight(const AmbientLight &light) { ambientLight = light; }

void Scene::addPrimitive(const std::shared_ptr<IPrimitive> &primitive) {
    primitives.push_back(primitive);
}

void Scene::addLight(const std::shared_ptr<ILight> &light) {
    lights.push_back(light);
}

void Scene::addShader(const std::shared_ptr<IShader> &shader) {
    shaders.push_back(shader);
}

void Scene::addPostProcess(const std::shared_ptr<IPostProcess> &postProcess) {
    postProcessEffects.push_back(postProcess);
}

std::optional<HitInfo> Scene::trace(const Ray &ray) const {
    double closest = std::numeric_limits<double>::infinity();
    std::optional<HitInfo> closestHit;

    for (const auto &primitive : primitives) {
        auto hit = primitive->hit(ray, 0.001, closest);
        if (hit && hit->primitive) {
            closest = hit->distance;
            closestHit = hit;
        }
    }

    return closestHit;
}

bool Scene::isInShadow(const Math::Point3D &hitPoint,
const Math::Vector3D &lightDir, const std::shared_ptr<ILight> &light)
const {
    double shadowBias = 0.001;
    Math::Point3D shadowOrigin = hitPoint + lightDir * shadowBias;
    Ray shadowRay(shadowOrigin, lightDir);

    double maxDistance = std::numeric_limits<double>::infinity();
    if (auto pointLight = dynamic_cast<PointLight*>(light.get()))
        maxDistance = (pointLight->position - hitPoint).length();

    for (const auto &primitive : primitives) {
        auto shadowHit = primitive->hit(shadowRay, 0.001, maxDistance);
        if (shadowHit)
            return true;
    }
    return false;
}

Math::Vector3D Scene::computeColor(const Ray &ray) const {
    auto hit = trace(ray);
    if (!hit)
        return Math::Vector3D(Math::Coords{0.0, 0.0, 0.0});

    Math::Vector3D pixelColor = hit->primitive->getMaterial()->color
        * ambientLight.color;

    for (const auto &light : lights) {
        Math::Vector3D lightDir = light->getLightDirection(hit->hitPoint);
        Math::Vector3D lightColor;

        if (auto pointLight = dynamic_cast<PointLight*>(light.get()))
            lightColor = pointLight->getLightColor(hit->hitPoint);
        else
            lightColor = light->getLightColor();

        bool shadowed = isInShadow(hit->hitPoint, lightDir, light);

        double diffuseFactor = std::max(0.0, hit->normal.dot(lightDir));

        if (shadowed)
            diffuseFactor *= 0.4;

        Math::Vector3D diffuse = hit->primitive->getMaterial()->color *
            lightColor * diffuseFactor;
        pixelColor += diffuse;
    }

    pixelColor.X = std::min(1.0, pixelColor.X);
    pixelColor.Y = std::min(1.0, pixelColor.Y);
    pixelColor.Z = std::min(1.0, pixelColor.Z);

    for (const auto& shader : shaders) {
        pixelColor = shader->apply(pixelColor, *hit, ray);
    }

    return pixelColor;
}

std::vector<Math::Vector3D> Scene::applyPostProcessingToFrameBuffer(
    const std::vector<Math::Vector3D>& frameBuffer, int width, int height) const {

        std::vector<Math::Vector3D> processedBuffer = frameBuffer;

    for (const auto& effect : postProcessEffects) {
        processedBuffer = effect->processFrameBuffer(processedBuffer, width, height);
    }

    return processedBuffer;
}

void Scene::writeColor(const Math::Vector3D &color) {
    double r = std::sqrt(std::max(0.0, std::min(1.0, color.X)));
    double g = std::sqrt(std::max(0.0, std::min(1.0, color.Y)));
    double b = std::sqrt(std::max(0.0, std::min(1.0, color.Z)));

    int ir = static_cast<int>(255.999 * r);
    int ig = static_cast<int>(255.999 * g);
    int ib = static_cast<int>(255.999 * b);

    std::cout << ir << " " << ig << " " << ib << std::endl;
}

void Scene::getLibConfigParams(std::shared_ptr<libconfig::Setting> setting) const {
    libconfig::Setting& cameraSettings = setting->add("camera", libconfig::Setting::TypeGroup);
    std::shared_ptr<libconfig::Setting> cameraSettingsPtr(&cameraSettings, [](libconfig::Setting*){});
    camera.getLibConfigParams(cameraSettingsPtr);

    libconfig::Setting& primitives = setting->add("primitives", libconfig::Setting::TypeGroup);

    std::map<std::string, libconfig::Setting*> primitiveTypes;

    for (const auto& primitive : getPrimitives()) {
        if (primitiveTypes.find(primitive->getTypeName()) == primitiveTypes.end()) {
            primitiveTypes[primitive->getTypeName()] = &primitives.add(
                primitive->getTypeName(), libconfig::Setting::TypeList);
        }

        libconfig::Setting& primitiveSetting = *primitiveTypes[primitive->getTypeName()];
        libconfig::Setting& primitiveGroup = primitiveSetting.add(libconfig::Setting::TypeGroup);
        std::shared_ptr<libconfig::Setting> primitiveGroupPtr(&primitiveGroup, [](libconfig::Setting*){});
        primitive->getLibConfigParams(primitiveGroupPtr);
    }

    libconfig::Setting& lights = setting->add("lights", libconfig::Setting::TypeGroup);

    lights.add("ambient", libconfig::Setting::TypeFloat) = ambientLight.getLightColor().X;
    lights.add("diffuse", libconfig::Setting::TypeFloat) = 0.6;

    libconfig::Setting& pointLights = lights.add("point", libconfig::Setting::TypeList);
    libconfig::Setting& directionalLights = lights.add("directional", libconfig::Setting::TypeList);

    for (const auto& light : getLights()) {
        const std::type_info& type = typeid(*light);

        if (type == typeid(PointLight)) {
            libconfig::Setting& pointLight = pointLights.add(libconfig::Setting::TypeGroup);
            std::shared_ptr<libconfig::Setting> pointLightPtr(&pointLight, [](libconfig::Setting*){});
            light->getLibConfigParams(pointLightPtr);
        } else if (type == typeid(DirectionalLight)) {
            libconfig::Setting& directionalLight = directionalLights.add(libconfig::Setting::TypeGroup);
            std::shared_ptr<libconfig::Setting> directionalLightPtr(&directionalLight, [](libconfig::Setting*){});
            light->getLibConfigParams(directionalLightPtr);
        }
    }

    if (!shaders.empty()) {
        libconfig::Setting& shadersSettings = setting->add("shaders", libconfig::Setting::TypeList);

        for (const auto& shader : shaders) {
            libconfig::Setting& shaderSetting = shadersSettings.add(libconfig::Setting::TypeGroup);
            std::shared_ptr<libconfig::Setting> shaderSettingPtr(&shaderSetting, [](libconfig::Setting*){});
            shader->getLibConfigParams(shaderSettingPtr);
        }
    }

    if (!postProcessEffects.empty()) {
        libconfig::Setting& postProcessSettings = setting->add("postprocess", libconfig::Setting::TypeList);

        for (const auto& effect : postProcessEffects) {
            libconfig::Setting& effectSetting = postProcessSettings.add(libconfig::Setting::TypeGroup);
            std::shared_ptr<libconfig::Setting> effectSettingPtr(&effectSetting, [](libconfig::Setting*){});
            effect->getLibConfigParams(effectSettingPtr);
        }
    }
}

}  // namespace RayTracer
