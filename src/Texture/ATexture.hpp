// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ATexture - classe abstraite de base pour les textures
*/

#ifndef SRC_TEXTURE_ATEXTURE_HPP_
    #define SRC_TEXTURE_ATEXTURE_HPP_
    #include <memory>
    #include <string>
    #include "ITexture.hpp"

namespace RayTracer {

/**
 * Abstract base class for textures
 */
class ATexture : public ITexture {
 protected:
    std::string type;

 public:
    /**
     * Constructor
     * @param textureType The type of texture
     */
    explicit ATexture(const std::string& textureType);

    /**
     * Destructor
     */
    ~ATexture() override = default;

    /**
     * Get texture type
     * @return The type of texture
     */
    std::string getType() const override;
};

}  // namespace RayTracer

#endif  // SRC_TEXTURE_ATEXTURE_HPP_
