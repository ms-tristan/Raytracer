// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ProceduralTexture - documentation and optimization
*/

#ifndef SRC_TEXTURE_PROCEDURALTEXTURE_PROCEDURALTEXTURE_HPP_
    #define SRC_TEXTURE_PROCEDURALTEXTURE_PROCEDURALTEXTURE_HPP_
    #include <memory>
    #include <string>
    #include "../ATexture.hpp"
    #include "Math/Vector3D/Vector3D.hpp"

namespace RayTracer {

/**
 * Abstract base class for all procedural textures
 */
class ProceduralTexture : public ATexture {
 protected:
    /**
     * Primary color used in the texture
     */
    Math::Vector3D color1;

    /**
     * Secondary color used in the texture
     */
    Math::Vector3D color2;

    /**
     * Scaling factor for the texture pattern
     */
    double scale;

 public:
    /**
     * Constructor for procedural textures
     * @param type The texture type identifier
     * @param primaryColor The primary color used in the pattern
     * @param secondaryColor The secondary color used in the pattern
     * @param textureScale The scale factor for the pattern
     */
    ProceduralTexture(const std::string& type,
                      const Math::Vector3D& primaryColor,
                      const Math::Vector3D& secondaryColor,
                      double textureScale = 1.0);

    /**
     * Virtual destructor
     */
    ~ProceduralTexture() override = default;

    /**
     * Get the primary color of the texture
     * @return The primary color
     */
    const Math::Vector3D& getPrimaryColor() const;

    /**
     * Get the secondary color of the texture
     * @return The secondary color
     */
    const Math::Vector3D& getSecondaryColor() const;

    /**
     * Get the scale factor of the texture
     * @return The scale factor
     */
    double getScale() const;

    /**
     * Set the primary color of the texture
     * @param color The new primary color
     */
    void setPrimaryColor(const Math::Vector3D& color);

    /**
     * Set the secondary color of the texture
     * @param color The new secondary color
     */
    void setSecondaryColor(const Math::Vector3D& color);

    /**
     * Set the scale factor of the texture
     * @param newScale The new scale factor
     */
    void setScale(double newScale);
};

}  // namespace RayTracer

#endif  // SRC_TEXTURE_PROCEDURALTEXTURE_PROCEDURALTEXTURE_HPP_
