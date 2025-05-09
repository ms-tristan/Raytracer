// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ChessboardTexture - texture procédurale en damier
*/

#ifndef SRC_TEXTURE_PROCEDURALTEXTURE_CHESSBOARDTEXTURE_HPP_
    #define SRC_TEXTURE_PROCEDURALTEXTURE_CHESSBOARDTEXTURE_HPP_
    #include <memory>
    #include <string>
    #include "ProceduralTexture.hpp"

namespace RayTracer {

/**
 * Procedural texture creating a chessboard pattern
 */
class ChessboardTexture : public ProceduralTexture {
 public:
    /**
     * Constructor
     * @param color1 First color of the chessboard (typically white)
     * @param color2 Second color of the chessboard (typically black)
     * @param scale Scale factor for the chessboard cells
     */
    ChessboardTexture(const Math::Vector3D& color1 = Math::Vector3D(Math::Coords{1.0, 1.0, 1.0}),
                      const Math::Vector3D& color2 = Math::Vector3D(Math::Coords{0.0, 0.0, 0.0}),
                      double scale = 1.0);

    /**
     * Destructor
     */
    ~ChessboardTexture() override = default;

    /**
     * Get color at given UV coordinates
     * @param uv The UV coordinates to sample
     * @return The color at the given coordinates
     */
    Math::Vector3D getColorAt(const Math::Vector2D& uv) const override;

    /**
     * Clone this texture
     * @return A new instance with the same properties
     */
    std::shared_ptr<ITexture> clone() const override;

    /**
     * Get the static type name for texture serialization
     * @return The type name string
     */
    static std::string getTypeNameStatic() {
        return "chessboard";
    }
};

}  // namespace RayTracer

#endif  // SRC_TEXTURE_PROCEDURALTEXTURE_CHESSBOARDTEXTURE_HPP_
