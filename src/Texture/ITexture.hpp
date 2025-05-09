// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ITexture interface
*/

#ifndef SRC_TEXTURE_ITEXTURE_HPP_
    #define SRC_TEXTURE_ITEXTURE_HPP_
    #include <memory>
    #include <string>
    #include "Math/Vector2D/Vector2D.hpp"
    #include "Math/Vector3D/Vector3D.hpp"

namespace RayTracer {

/**
 * Interface for all textures
 */
class ITexture {
 public:
    virtual ~ITexture() = default;

    /**
     * Get color at UV coordinates
     * @param uv UV coordinates
     * @return Color at the given UV coordinates
     */
    virtual Math::Vector3D getColorAt(const Math::Vector2D& uv) const = 0;

    /**
     * Clone the texture
     * @return A new texture instance with the same properties
     */
    virtual std::shared_ptr<ITexture> clone() const = 0;

    /**
     * Get the texture type
     * @return The type of texture (for serialization)
     */
    virtual std::string getType() const = 0;
};

}  // namespace RayTracer

#endif  // SRC_TEXTURE_ITEXTURE_HPP_
