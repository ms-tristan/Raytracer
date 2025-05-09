// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Material
*/
#include "Material/Material.hpp"
#include <memory>

namespace RayTracer {
Material::Material() : reflectivity(0.0), transparency(0.0), refractionIndex(1.0), shininess(10.0) {
    const Math::Coords colors{1.0, 1.0, 1.0};
    color = Math::Vector3D(colors);
}

Material::Material(const Math::Vector3D &colors, double reflectivity,
    double transparency, double refractionIndex, double shininess)
    : color(colors), reflectivity(reflectivity),
      transparency(transparency), refractionIndex(refractionIndex),
      shininess(shininess) {}

Material::Material(const Math::Vector3D &colors,
    std::shared_ptr<ITexture> texture,
    std::shared_ptr<NormalMap> normalMap,
    std::shared_ptr<DisplacementMap> displacementMap,
    std::shared_ptr<SpecularMap> specularMap,
    std::shared_ptr<AmbientOcclusionMap> aoMap,
    double reflectivity,
    double transparency,
    double refractionIndex,
    double shininess)
    : color(colors), texture(texture), normalMap(normalMap),
      displacementMap(displacementMap), specularMap(specularMap), aoMap(aoMap),
      reflectivity(reflectivity), transparency(transparency),
      refractionIndex(refractionIndex), shininess(shininess) {}

Math::Vector3D Material::getColorAt(const Math::Vector2D& uv) const {
    if (texture) {
        return texture->getColorAt(uv);
    }
    return color;
}

Math::Vector3D Material::getNormalAt(const Math::Vector2D& uv, const Math::Vector3D& originalNormal) const {
    if (normalMap) {
        Math::Vector3D normalFromMap = normalMap->getNormalAt(uv);
        return transformNormalFromTangentSpace(normalFromMap, originalNormal);
    }
    return originalNormal;
}

double Material::getDisplacementAt(const Math::Vector2D& uv) const {
    if (displacementMap) {
        return displacementMap->getDisplacementAt(uv);
    }
    return 0.0;
}

double Material::getSpecularAt(const Math::Vector2D& uv) const {
    if (specularMap) {
        return specularMap->getSpecularAt(uv);
    }
    return 0.5;
}

double Material::getAmbientOcclusionAt(const Math::Vector2D& uv) const {
    if (aoMap) {
        return aoMap->getOcclusionAt(uv);
    }
    return 1.0;
}

Math::Vector3D Material::transformNormalFromTangentSpace(
    const Math::Vector3D& normalFromMap,
    const Math::Vector3D& surfaceNormal) const {
    // Créer un vecteur tangent orthogonal à la normale
    Math::Vector3D c1 = surfaceNormal.cross(Math::Vector3D(Math::Coords{0.0, 0.0, 1.0}));
    if (c1.length() < 0.0001) {
        c1 = surfaceNormal.cross(Math::Vector3D(Math::Coords{0.0, 1.0, 0.0}));
    }
    c1 = c1.normalize();
    Math::Vector3D c2 = surfaceNormal.cross(c1);
    double normalStrength = 1.0;
    Math::Vector3D adjustedNormal = normalFromMap;
    if (normalStrength != 1.0) {
        Math::Vector3D baseNormal(Math::Coords{0.0, 0.0, 1.0});
        adjustedNormal = baseNormal * (1.0 - normalStrength) + normalFromMap * normalStrength;
        adjustedNormal = adjustedNormal.normalize();
    }
    Math::Vector3D transformedNormal = c1 * adjustedNormal.X +
                                      c2 * adjustedNormal.Y +
                                      surfaceNormal * adjustedNormal.Z;
    return transformedNormal.normalize();
}

Math::Point3D Material::displacePoint(const Math::Point3D& point,
                                    const Math::Vector3D& normal,
                                    const Math::Vector2D& uv) const {
    if (displacementMap) {
        double displacementValue = displacementMap->getDisplacementAt(uv);
        const double baseScale = 2.0;

        Math::Vector3D displacement = normal * (displacementValue * baseScale);
        return Math::Point3D(Math::Coords{
            point.X + displacement.X,
            point.Y + displacement.Y,
            point.Z + displacement.Z
        });
    }
    return point;
}

std::shared_ptr<Material> Material::clone() const {
    auto newMaterial = std::make_shared<Material>(color, reflectivity,
        transparency, refractionIndex, shininess);

    if (texture)
        newMaterial->texture = texture->clone();
    if (normalMap)
        newMaterial->normalMap = normalMap->clone();
    if (displacementMap)
        newMaterial->displacementMap = displacementMap->clone();
    if (specularMap)
        newMaterial->specularMap = specularMap->clone();
    if (aoMap)
        newMaterial->aoMap = aoMap->clone();
    return newMaterial;
}

void Material::setTexture(std::shared_ptr<ITexture> newTexture) {
    texture = newTexture;
}

void Material::setNormalMap(std::shared_ptr<NormalMap> newNormalMap) {
    normalMap = newNormalMap;
}

void Material::setDisplacementMap(std::shared_ptr<DisplacementMap> newDisplacementMap) {
    displacementMap = newDisplacementMap;
}

void Material::setSpecularMap(std::shared_ptr<SpecularMap> newSpecularMap) {
    specularMap = newSpecularMap;
}

void Material::setAmbientOcclusionMap(std::shared_ptr<AmbientOcclusionMap> newAOMap) {
    aoMap = newAOMap;
}

bool Material::hasTexture() const {
    return texture != nullptr;
}

bool Material::hasNormalMap() const {
    return normalMap != nullptr;
}

bool Material::hasDisplacementMap() const {
    return displacementMap != nullptr;
}

bool Material::hasSpecularMap() const {
    return specularMap != nullptr;
}

bool Material::hasAmbientOcclusionMap() const {
    return aoMap != nullptr;
}

}  // namespace RayTracer
