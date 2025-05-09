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

/**
 * @brief Sets the camera for the scene
 * @param cam The camera to set
 */
void Scene::setCamera(const Camera &cam) { camera = cam; }

/**
 * @brief Gets the current camera of the scene
 * @return The current camera
 */
Camera Scene::getCamera() const { return camera; }

/**
 * @brief Sets the ambient light for the scene
 * @param light The ambient light to set
 */
void Scene::setAmbientLight(const AmbientLight &light) { ambientLight = light; }

/**
 * @brief Adds a primitive to the scene
 * @param primitive The primitive to add
 */
void Scene::addPrimitive(const std::shared_ptr<IPrimitive> &primitive) {
    primitives.push_back(primitive);
}

/**
 * @brief Adds a light to the scene
 * @param light The light to add
 */
void Scene::addLight(const std::shared_ptr<ILight> &light) {
    lights.push_back(light);
}

/**
 * @brief Adds a shader to the scene
 * @param shader The shader to add
 */
void Scene::addShader(const std::shared_ptr<IShader> &shader) {
    shaders.push_back(shader);
}

/**
 * @brief Adds a post-processing effect to the scene
 * @param postProcess The post-processing effect to add
 */
void Scene::addPostProcess(const std::shared_ptr<IPostProcess> &postProcess) {
    postProcessEffects.push_back(postProcess);
}

/**
 * @brief Traces a ray through the scene and finds the closest hit
 * @param ray The ray to trace
 * @return Information about the closest hit, if any
 */
std::optional<HitInfo> Scene::trace(const Ray &ray) const {
    double closest = std::numeric_limits<double>::infinity();
    std::optional<HitInfo> closestHit;
    for (const auto &primitive : primitives) {
        auto hit = primitive->hit(ray, 0.001, closest);
        if (hit && hit->primitive) {
            auto material = hit->primitive->getMaterial();
            if (material->hasDisplacementMap()) {
                applyDisplacementMapping(hit, material);
            }
            closest = hit->distance;
            closestHit = hit;
        }
    }
    return closestHit;
}

/**
 * @brief Applies displacement mapping to a hit point
 * @param hit The hit information to modify
 * @param material The material containing the displacement map
 */
void Scene::applyDisplacementMapping(std::optional<HitInfo> &hit,
                                   const std::shared_ptr<Material> &material) const {
    Math::Vector3D originalNormal = hit->normal;
    hit->hitPoint = material->displacePoint(hit->hitPoint, hit->normal, hit->uv);
    const double epsilon = 0.005;
    Math::Vector3D tangentU = createTangentVector(originalNormal);
    Math::Vector3D tangentV = originalNormal.cross(tangentU).normalize();
    Math::Vector2D uvEpsilonU(hit->uv.U + epsilon, hit->uv.V);
    Math::Vector2D uvEpsilonV(hit->uv.U, hit->uv.V + epsilon);
    Math::Point3D pu = hit->hitPoint + tangentU * epsilon;
    Math::Point3D pv = hit->hitPoint + tangentV * epsilon;
    pu = material->displacePoint(pu, originalNormal, uvEpsilonU);
    pv = material->displacePoint(pv, originalNormal, uvEpsilonV);
    Math::Vector3D newU = pu - hit->hitPoint;
    Math::Vector3D newV = pv - hit->hitPoint;
    hit->normal = newV.cross(newU).normalize();
    if (hit->normal.dot(originalNormal) < 0) {
        hit->normal = hit->normal * -1.0;
    }
}

/**
 * @brief Creates a tangent vector perpendicular to the normal
 * @param normal The normal to create a tangent for
 * @return A normalized tangent vector
 */
Math::Vector3D Scene::createTangentVector(const Math::Vector3D &normal) const {
    Math::Vector3D tangent;
    if (std::abs(normal.X) < 0.9) {
        tangent = Math::Vector3D(Math::Coords{1.0, 0.0, 0.0}).cross(normal).normalize();
    } else {
        tangent = Math::Vector3D(Math::Coords{0.0, 1.0, 0.0}).cross(normal).normalize();
    }
    return tangent;
}

/**
 * @brief Checks if a point is in shadow from a light
 * @param hitPoint The point to check
 * @param lightDir The direction to the light
 * @param light The light source
 * @return True if the point is in shadow, false otherwise
 */
bool Scene::isInShadow(const Math::Point3D &hitPoint,
const Math::Vector3D &lightDir, const std::shared_ptr<ILight> &light) const {
    double shadowBias = 0.001;
    Math::Point3D shadowOrigin = hitPoint + lightDir * shadowBias;
    Ray shadowRay(shadowOrigin, lightDir);
    double maxDistance = calculateMaxShadowDistance(hitPoint, light);

    for (const auto &primitive : primitives) {
        auto shadowHit = primitive->hit(shadowRay, 0.001, maxDistance);
        if (shadowHit) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Calculates the maximum distance for shadow rays
 * @param hitPoint The hit point
 * @param light The light source
 * @return The maximum shadow ray distance
 */
double Scene::calculateMaxShadowDistance(const Math::Point3D &hitPoint,
                                       const std::shared_ptr<ILight> &light) const {
    if (auto pointLight = dynamic_cast<PointLight*>(light.get())) {
        return (pointLight->position - hitPoint).length();
    }
    return std::numeric_limits<double>::infinity();
}

/**
 * @brief Computes the color for a ray
 * @param ray The ray to compute color for
 * @param depth The current recursion depth
 * @return The computed color
 */
Math::Vector3D Scene::computeColor(const Ray &ray, int depth) const {
    if (depth > maxReflectionDepth) {
        return Math::Vector3D(Math::Coords{0.0, 0.0, 0.0});
    }
    auto hit = trace(ray);
    if (!hit) {
        return Math::Vector3D(Math::Coords{0.0, 0.0, 0.0});
    }
    auto material = hit->primitive->getMaterial();
    Math::Vector3D baseColor = calculateBaseColor(material, hit->uv);
    Math::Vector3D surfaceNormal = calculateSurfaceNormal(material, hit);
    double aoFactor = calculateAmbientOcclusion(material, hit->uv);
    Math::Vector3D pixelColor = baseColor * ambientLight.color * aoFactor;
    pixelColor = applyLighting(pixelColor, hit, ray, surfaceNormal, baseColor, material, aoFactor);
    pixelColor = applyReflectionAndRefraction(pixelColor, hit, ray, surfaceNormal, material, depth);
    for (const auto& shader : shaders) {
        pixelColor = shader->apply(pixelColor, *hit, ray);
    }
    return pixelColor;
}

/**
 * @brief Calculates the base color of a material at a specific UV coordinate
 * @param material The material
 * @param uv The UV coordinate
 * @return The base color
 */
Math::Vector3D Scene::calculateBaseColor(const std::shared_ptr<Material> &material,
                                       const Math::Vector2D &uv) const {
    if (material->hasTexture()) {
        return material->getColorAt(uv);
    }
    return material->color;
}

/**
 * @brief Calculates the surface normal considering normal mapping
 * @param material The material
 * @param hit The hit information
 * @return The calculated surface normal
 */
Math::Vector3D Scene::calculateSurfaceNormal(const std::shared_ptr<Material> &material,
                                           const std::optional<HitInfo> &hit) const {
    Math::Vector3D normal = hit->normal;
    if (material->hasNormalMap()) {
        normal = material->getNormalAt(hit->uv, hit->normal);
        normal = normal.normalize();
    }
    return normal;
}

/**
 * @brief Calculates the ambient occlusion factor
 * @param material The material
 * @param uv The UV coordinate
 * @return The ambient occlusion factor
 */
double Scene::calculateAmbientOcclusion(const std::shared_ptr<Material> &material,
                                      const Math::Vector2D &uv) const {
    if (material->hasAmbientOcclusionMap()) {
        double aoValue = material->getAmbientOcclusionAt(uv);
        return 0.1 + (aoValue * 0.9);
    }
    return 1.0;
}

/**
 * @brief Applies lighting calculations to a color
 * @param basePixelColor The initial pixel color
 * @param hit The hit information
 * @param ray The viewing ray
 * @param normal The surface normal
 * @param baseColor The base material color
 * @param material The material
 * @param aoFactor The ambient occlusion factor
 * @return The color with lighting applied
 */
Math::Vector3D Scene::applyLighting(const Math::Vector3D &basePixelColor,
                                  const std::optional<HitInfo> &hit,
                                  const Ray &ray,
                                  const Math::Vector3D &normal,
                                  const Math::Vector3D &baseColor,
                                  const std::shared_ptr<Material> &material,
                                  double aoFactor) const {
    Math::Vector3D pixelColor = basePixelColor;
    for (const auto &light : lights) {
        Math::Vector3D lightDir = light->getLightDirection(hit->hitPoint);
        Math::Vector3D lightColor = getLightColor(light, hit->hitPoint);
        bool shadowed = isInShadow(hit->hitPoint, lightDir, light);
        double diffuseFactor = calculateDiffuseFactor(normal, lightDir, shadowed, aoFactor);
        Math::Vector3D diffuse = baseColor * lightColor * diffuseFactor;
        Math::Vector3D specular = calculateSpecular(hit->hitPoint, ray.origin, lightDir,
                                                  normal, lightColor, material, hit->uv);
        pixelColor += diffuse * 0.6 + specular * 0.4;
    }
    pixelColor.X = std::min(1.0, pixelColor.X);
    pixelColor.Y = std::min(1.0, pixelColor.Y);
    pixelColor.Z = std::min(1.0, pixelColor.Z);
    return pixelColor;
}

/**
 * @brief Gets the color of a light at a specific point
 * @param light The light source
 * @param hitPoint The point to calculate the light color at
 * @return The light color
 */
Math::Vector3D Scene::getLightColor(const std::shared_ptr<ILight> &light,
                                  const Math::Point3D &hitPoint) const {
    if (auto pointLight = dynamic_cast<PointLight*>(light.get())) {
        return pointLight->getLightColor(hitPoint);
    }
    return light->getLightColor();
}

/**
 * @brief Calculates the diffuse lighting factor
 * @param normal The surface normal
 * @param lightDir The direction to the light
 * @param shadowed Whether the point is in shadow
 * @param aoFactor The ambient occlusion factor
 * @return The diffuse factor
 */
double Scene::calculateDiffuseFactor(const Math::Vector3D &normal,
                                   const Math::Vector3D &lightDir,
                                   bool shadowed,
                                   double aoFactor) const {
    double factor = std::max(0.0, normal.dot(lightDir));
    if (shadowed) {
        factor *= 0.4;
    }
    double diffuseAO = 0.7 + 0.3 * aoFactor;
    return factor * diffuseAO;
}

/**
 * @brief Calculates the specular lighting component
 * @param hitPoint The hit point
 * @param viewerPos The position of the viewer
 * @param lightDir The direction to the light
 * @param normal The surface normal
 * @param lightColor The color of the light
 * @param material The material
 * @param uv The UV coordinate
 * @return The specular lighting component
 */
Math::Vector3D Scene::calculateSpecular(const Math::Point3D &hitPoint,
                                      const Math::Point3D &viewerPos,
                                      const Math::Vector3D &lightDir,
                                      const Math::Vector3D &normal,
                                      const Math::Vector3D &lightColor,
                                      const std::shared_ptr<Material> &material,
                                      const Math::Vector2D &uv) const {
    Math::Vector3D viewDir = (viewerPos - hitPoint).normalize();
    Math::Vector3D halfwayDir = (lightDir + viewDir).normalize();
    double specularFactor = std::pow(std::max(0.0, normal.dot(halfwayDir)), material->shininess);
    if (material->hasSpecularMap()) {
        double specMapValue = material->getSpecularAt(uv);
        specularFactor *= specMapValue * 1.5;
    }
    return lightColor * specularFactor;
}

/**
 * @brief Applies reflection and refraction to a color
 * @param baseColor The base color
 * @param hit The hit information
 * @param ray The viewing ray
 * @param normal The surface normal
 * @param material The material
 * @param depth The current recursion depth
 * @return The color with reflection and refraction applied
 */
Math::Vector3D Scene::applyReflectionAndRefraction(const Math::Vector3D &baseColor,
                                                 const std::optional<HitInfo> &hit,
                                                 const Ray &ray,
                                                 const Math::Vector3D &normal,
                                                 const std::shared_ptr<Material> &material,
                                                 int depth) const {
    double reflectivity = material->reflectivity;
    double transparency = material->transparency;
    double epsilon = 0.001;
    Math::Vector3D resultColor = baseColor * (1.0 - reflectivity - transparency);
    if (reflectivity > 0.0 && depth < maxReflectionDepth) {
        resultColor += calculateReflection(ray, hit, normal, reflectivity, epsilon, depth);
    }
    if (transparency > 0.0 && depth < maxReflectionDepth) {
        resultColor += calculateRefraction(ray, hit, normal, material, transparency, epsilon, depth);
    }
    return resultColor;
}

/**
 * @brief Calculates the reflection component
 * @param ray The viewing ray
 * @param hit The hit information
 * @param normal The surface normal
 * @param reflectivity The reflectivity factor
 * @param epsilon A small offset to avoid self-intersection
 * @param depth The current recursion depth
 * @return The reflection component
 */
Math::Vector3D Scene::calculateReflection(const Ray &ray,
                                        const std::optional<HitInfo> &hit,
                                        const Math::Vector3D &normal,
                                        double reflectivity,
                                        double epsilon,
                                        int depth) const {
    double dotProduct = ray.direction.dot(normal);
    Math::Vector3D reflectionDir = ray.direction - normal * (2.0 * dotProduct);
    reflectionDir = reflectionDir.normalize();
    Math::Point3D reflectionOrigin = hit->hitPoint + reflectionDir * epsilon;
    Ray reflectionRay(reflectionOrigin, reflectionDir);
    Math::Vector3D reflectedColor = computeColor(reflectionRay, depth + 1);
    return reflectedColor * reflectivity;
}

/**
 * @brief Calculates the refraction component
 * @param ray The viewing ray
 * @param hit The hit information
 * @param originalNormal The surface normal
 * @param material The material
 * @param transparency The transparency factor
 * @param epsilon A small offset to avoid self-intersection
 * @param depth The current recursion depth
 * @return The refraction component
 */
Math::Vector3D Scene::calculateRefraction(const Ray &ray,
                                        const std::optional<HitInfo> &hit,
                                        const Math::Vector3D &originalNormal,
                                        const std::shared_ptr<Material> &material,
                                        double transparency,
                                        double epsilon,
                                        int depth) const {
    Math::Vector3D normal = originalNormal;
    double refractionRatio = 1.0 / material->refractionIndex;
    double cosTheta = std::min(1.0, std::max(-1.0, ray.direction.dot(normal) * -1.0));
    if (cosTheta < 0) {
        normal = normal * -1.0;
        cosTheta *= -1.0;
        refractionRatio = material->refractionIndex;
    }
    double sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);
    Math::Vector3D refractionDir;
    if (refractionRatio * sinTheta > 1.0) {
        refractionDir = calculateTotalInternalReflection(ray, normal);
    } else {
        refractionDir = calculateSnellRefraction(ray, normal, cosTheta, refractionRatio);
    }
    Math::Point3D refractionOrigin = hit->hitPoint + refractionDir * epsilon;
    Ray refractionRay(refractionOrigin, refractionDir);
    Math::Vector3D refractedColor = computeColor(refractionRay, depth + 1);
    return refractedColor * transparency;
}

/**
 * @brief Calculates the direction for total internal reflection
 * @param ray The viewing ray
 * @param normal The surface normal
 * @return The reflection direction
 */
Math::Vector3D Scene::calculateTotalInternalReflection(const Ray &ray,
                                                     const Math::Vector3D &normal) const {
    double dotProduct = ray.direction.dot(normal);
    Math::Vector3D reflectionDir = ray.direction - normal * (2.0 * dotProduct);
    return reflectionDir.normalize();
}

/**
 * @brief Calculates the direction for Snell's law refraction
 * @param ray The viewing ray
 * @param normal The surface normal
 * @param cosTheta The cosine of the angle between the ray and normal
 * @param refractionRatio The ratio of refractive indices
 * @return The refraction direction
 */
Math::Vector3D Scene::calculateSnellRefraction(const Ray &ray,
                                             const Math::Vector3D &normal,
                                             double cosTheta,
                                             double refractionRatio) const {
    Math::Vector3D perpendicular = (ray.direction + normal * cosTheta) * refractionRatio;
    Math::Vector3D parallel = normal * -std::sqrt(1.0 - perpendicular.dot(perpendicular));
    return (perpendicular + parallel).normalize();
}

/**
 * @brief Applies post-processing effects to a frame buffer
 * @param frameBuffer The original frame buffer
 * @param width The width of the frame buffer
 * @param height The height of the frame buffer
 * @return The processed frame buffer
 */
std::vector<Math::Vector3D> Scene::applyPostProcessingToFrameBuffer(
    const std::vector<Math::Vector3D>& frameBuffer, int width, int height) const {

        std::vector<Math::Vector3D> processedBuffer = frameBuffer;

    for (const auto& effect : postProcessEffects) {
        processedBuffer = effect->processFrameBuffer(processedBuffer, width, height);
    }

    return processedBuffer;
}

void Scene::writeColor(const Math::Vector3D &color) const {
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
