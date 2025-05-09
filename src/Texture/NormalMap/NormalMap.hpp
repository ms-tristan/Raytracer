// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** NormalMap - classe pour les cartes de normales
*/

#ifndef SRC_TEXTURE_NORMALMAP_NORMALMAP_HPP_
#define SRC_TEXTURE_NORMALMAP_NORMALMAP_HPP_
#include <memory>
#include <string>
#include <vector>
#include "../../Math/Vector3D/Vector3D.hpp"
#include "../../Math/Vector2D/Vector2D.hpp"

namespace RayTracer {

/**
 * Class defining a normal map that can modify surface normals to simulate details
 */
class NormalMap {
 private:
    std::vector<unsigned char> mapData;
    int width;
    int height;
    int channels;
    double strength;  // Strength of normal mapping effect (0.0 - 1.0)
    bool bilinearFilter;
    std::string filepath;

    /**
     * Load map data from file
     * @param filepath Path to the normal map image file
     */
    void loadMapData(const std::string& filepath);

    /**
     * Create a default normal map if loading fails
     */
    void createDefaultNormalMap();

    /**
     * Store image data from the loaded buffer
     * @param data Pointer to the image data
     */
    void storeMapData(unsigned char* data);

    /**
     * Sample the normal map using nearest neighbor filtering
     * @param u U coordinate (0.0 - 1.0)
     * @param v V coordinate (0.0 - 1.0)
     * @return Normal vector in RGB format (0-1 range)
     */
    Math::Vector3D nearestNeighbor(double u, double v) const;

    /**
     * Sample the normal map using bilinear filtering
     * @param u U coordinate (0.0 - 1.0)
     * @param v V coordinate (0.0 - 1.0)
     * @return Normal vector in RGB format (0-1 range) with bilinear interpolation
     */
    Math::Vector3D bilinearSample(double u, double v) const;

    /**
     * Extract normal vector from image data at the specified index
     * @param idx Index in the image data array
     * @return Normal vector in RGB format (0-1 range)
     */
    Math::Vector3D extractNormalFromIndex(size_t idx) const;

    /**
     * Process a normal vector from image space to tangent space
     * @param normal Normal vector in RGB format (0-1 range)
     * @return Processed normal vector in tangent space (-1 to 1 range)
     */
    Math::Vector3D processNormal(Math::Vector3D normal) const;

    /**
     * Apply strength factor to the normal vector
     * @param normal Normal vector to be modified
     */
    void applyStrength(Math::Vector3D& normal) const;

    /**
     * Normalize texture coordinate to [0, 1] range
     * @param coord Input coordinate
     * @return Normalized coordinate
     */
    double normalizeCoordinate(double coord) const;

    /**
     * Interpolate normals for bilinear filtering
     * @param tx X interpolation factor
     * @param ty Y interpolation factor
     * @param c00 Top-left normal
     * @param c01 Top-right normal
     * @param c10 Bottom-left normal
     * @param c11 Bottom-right normal
     * @return Interpolated normal
     */
    Math::Vector3D interpolateNormals(double tx, double ty,
                                      const Math::Vector3D& c00,
                                      const Math::Vector3D& c01,
                                      const Math::Vector3D& c10,
                                      const Math::Vector3D& c11) const;

 public:
    /**
     * Constructor
     * @param filepath Path to the normal map image file
     * @param strength Strength of normal mapping effect (0.0 - 1.0)
     * @param useBilinearFilter Whether to use bilinear filtering
     */
    explicit NormalMap(const std::string& filepath, double strength = 1.0, bool useBilinearFilter = true);

    /**
     * Virtual destructor
     */
    virtual ~NormalMap() = default;

    /**
     * Get perturbed normal at the given UV coordinates
     * @param uv UV coordinates
     * @return Perturbed normal vector
     */
    virtual Math::Vector3D getNormalAt(const Math::Vector2D& uv) const;

    /**
     * Clone this normal map
     * @return A new pointer to a normal map with the same properties
     */
    virtual std::shared_ptr<NormalMap> clone() const;

    /**
     * Get the type of normal map
     * @return Type of normal map
     */
    virtual std::string getType() const;

    /**
     * Get the effect strength
     * @return Strength value (0.0 - 1.0)
     */
    double getStrength() const;

    /**
     * Set the effect strength
     * @param value New strength value (0.0 - 1.0)
     */
    void setStrength(double value);
};

}  // namespace RayTracer

#endif  // SRC_TEXTURE_NORMALMAP_NORMALMAP_HPP_
