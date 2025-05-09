// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Material
*/

#ifndef SRC_MATERIAL_MATERIAL_HPP_
#define SRC_MATERIAL_MATERIAL_HPP_
#include <algorithm>
#include <memory>
#include "defs.hpp"
#include "Math/Vector3D/Vector3D.hpp"
#include "Math/Vector2D/Vector2D.hpp"
#include "Texture/ITexture.hpp"
#include "Texture/NormalMap/NormalMap.hpp"
#include "Texture/DisplacementMap/DisplacementMap.hpp"
#include "Texture/SpecularMap/SpecularMap.hpp"
#include "Texture/AmbientOcclusionMap/AmbientOcclusionMap.hpp"

namespace RayTracer {
class Material {
 public:
    Math::Vector3D color;
    double reflectivity;
    double transparency;
    double refractionIndex;
    double shininess;

    std::shared_ptr<ITexture> texture;
    std::shared_ptr<NormalMap> normalMap;
    std::shared_ptr<DisplacementMap> displacementMap;
    std::shared_ptr<SpecularMap> specularMap;
    std::shared_ptr<AmbientOcclusionMap> aoMap;

    Material();
    explicit Material(const Math::Vector3D &color, double reflectivity = 0.0,
        double transparency = 0.0, double refractionIndex = 1.0, double shininess = 10.0);

    Material(const Math::Vector3D &color,
        std::shared_ptr<ITexture> texture,
        std::shared_ptr<NormalMap> normalMap = nullptr,
        std::shared_ptr<DisplacementMap> displacementMap = nullptr,
        std::shared_ptr<SpecularMap> specularMap = nullptr,
        std::shared_ptr<AmbientOcclusionMap> aoMap = nullptr,
        double reflectivity = 0.0,
        double transparency = 0.0,
        double refractionIndex = 1.0,
        double shininess = 10.0);
    virtual ~Material() = default;
    Math::Vector3D getColorAt(const Math::Vector2D& uv) const;
    Math::Vector3D getNormalAt(const Math::Vector2D& uv, const Math::Vector3D& originalNormal) const;
    double getDisplacementAt(const Math::Vector2D& uv) const;
    double getSpecularAt(const Math::Vector2D& uv) const;
    double getAmbientOcclusionAt(const Math::Vector2D& uv) const;
    Math::Vector3D transformNormalFromTangentSpace(
        const Math::Vector3D& normalFromMap,
        const Math::Vector3D& surfaceNormal) const;
    Math::Point3D displacePoint(const Math::Point3D& point,
                               const Math::Vector3D& normal,
                               const Math::Vector2D& uv) const;
    std::shared_ptr<Material> clone() const;
    void setTexture(std::shared_ptr<ITexture> newTexture);
    void setNormalMap(std::shared_ptr<NormalMap> newNormalMap);
    void setDisplacementMap(std::shared_ptr<DisplacementMap> newDisplacementMap);
    void setSpecularMap(std::shared_ptr<SpecularMap> newSpecularMap);
    void setAmbientOcclusionMap(std::shared_ptr<AmbientOcclusionMap> newAOMap);

    bool hasTexture() const;
    bool hasNormalMap() const;
    bool hasDisplacementMap() const;
    bool hasSpecularMap() const;
    bool hasAmbientOcclusionMap() const;
};
}  // namespace RayTracer

#endif  // SRC_MATERIAL_MATERIAL_HPP_
