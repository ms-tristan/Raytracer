// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** PerlinNoiseTexture - texture procédurale avec bruit de Perlin
*/

#ifndef SRC_TEXTURE_PROCEDURALTEXTURE_PERLINNOISEEXTURE_HPP_
    #define SRC_TEXTURE_PROCEDURALTEXTURE_PERLINNOISEEXTURE_HPP_
    #include <memory>
    #include <vector>
    #include <random>
    #include <string>
    #include "ProceduralTexture.hpp"

namespace RayTracer {

/**
 * A procedural texture using Perlin noise algorithm
 */
class PerlinNoiseTexture : public ProceduralTexture {
 private:
    static const int PERM_SIZE = 256;
    std::vector<int> perm;
    double persistence;
    int octaves;

    /**
     * Apply fade function used in Perlin algorithm
     * @param t Input value
     * @return Smoothed value
     */
    double fade(double t) const;

    /**
     * Linear interpolation between two values
     * @param t Interpolation factor (0.0 - 1.0)
     * @param a First value
     * @param b Second value
     * @return Interpolated value
     */
    double lerp(double t, double a, double b) const;

    /**
     * Calculate gradient from hash
     * @param hash Hash value
     * @param x X coordinate
     * @param y Y coordinate
     * @param z Z coordinate
     * @return Gradient value
     */
    double grad(int hash, double x, double y, double z) const;

    /**
     * Calculate Perlin noise value at given coordinates
     * @param x X coordinate
     * @param y Y coordinate
     * @param z Z coordinate (optional)
     * @return Noise value
     */
    double noise(double x, double y, double z = 0.0) const;

    /**
     * Calculate turbulence (multiple octaves of noise)
     * @param x X coordinate
     * @param y Y coordinate
     * @param z Z coordinate
     * @param depth Number of octaves
     * @return Turbulence value
     */
    double turbulence(double x, double y, double z, int depth) const;

    /**
     * Initialize permutation table
     * @param seed Random seed
     */
    void initPermutation(unsigned int seed);

 public:
    /**
     * Constructor
     * @param color1 Primary color
     * @param color2 Secondary color
     * @param scale Texture scale
     * @param persistence Persistence parameter for noise
     * @param octaves Number of octaves
     * @param seed Random seed
     */
    PerlinNoiseTexture(const Math::Vector3D& color1 = Math::Vector3D(Math::Coords{1.0, 1.0, 1.0}),
                       const Math::Vector3D& color2 = Math::Vector3D(Math::Coords{0.0, 0.0, 0.0}),
                       double scale = 1.0,
                       double persistence = 0.5,
                       int octaves = 6,
                       unsigned int seed = 42);

    /**
     * Destructor
     */
    ~PerlinNoiseTexture() override = default;

    /**
     * Get color at given UV coordinates
     * @param uv UV coordinates
     * @return Color at coordinates
     */
    Math::Vector3D getColorAt(const Math::Vector2D& uv) const override;

    /**
     * Clone the texture
     * @return New instance with same properties
     */
    std::shared_ptr<ITexture> clone() const override;

    /**
     * Get persistence parameter
     * @return Persistence value
     */
    double getPersistence() const;

    /**
     * Get number of octaves
     * @return Octaves count
     */
    int getOctaves() const;

    /**
     * Set persistence parameter
     * @param value New persistence value
     */
    void setPersistence(double value);

    /**
     * Set number of octaves
     * @param value New octaves count
     */
    void setOctaves(int value);

    /**
     * Get static type name for texture
     * @return Type name
     */
    static std::string getTypeNameStatic() {
        return "perlin";
    }
};

}  // namespace RayTracer

#endif  // SRC_TEXTURE_PROCEDURALTEXTURE_PERLINNOISEEXTURE_HPP_
