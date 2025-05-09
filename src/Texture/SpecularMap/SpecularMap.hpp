/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ConfigParser
*/

#ifndef SRC_TEXTURE_SPECULARMAP_SPECULARMAP_HPP_
#define SRC_TEXTURE_SPECULARMAP_SPECULARMAP_HPP_
#include <memory>
#include <string>
#include <vector>
#include "../../Math/Vector3D/Vector3D.hpp"
#include "../../Math/Vector2D/Vector2D.hpp"

namespace RayTracer {

/**
 * Specular map that controls the intensity of specular reflections
 */
class SpecularMap {
 private:
    std::vector<unsigned char> mapData;
    int width;
    int height;
    int channels;
    bool bilinearFilter;
    std::string filepath;

    /**
     * Sample the specular map using nearest neighbor filtering
     * @param u U coordinate (0.0 - 1.0)
     * @param v V coordinate (0.0 - 1.0)
     * @return Specular value at the given coordinates
     */
    double nearestNeighbor(double u, double v) const;

    /**
     * Sample the specular map using bilinear filtering
     * @param u U coordinate (0.0 - 1.0)
     * @param v V coordinate (0.0 - 1.0)
     * @return Specular value at the given coordinates with bilinear interpolation
     */
    double bilinearSample(double u, double v) const;

    /**
     * Normalize texture coordinate to [0, 1] range
     * @param coord Input coordinate
     * @return Normalized coordinate
     */
    double normalizeCoordinate(double coord) const;

    /**
     * Load map data from file
     * @param filepath Path to the specular map image file
     */
    void loadMapData(const std::string& filepath);

    /**
     * Create a default specular map if loading fails
     */
    void createDefaultMap();

    /**
     * Store image data from the loaded buffer
     * @param data Pointer to the image data
     */
    void storeMapData(unsigned char* data);

    /**
     * Interpolate values for bilinear filtering
     * @param tx X interpolation factor
     * @param ty Y interpolation factor
     * @param v00 Top-left value
     * @param v01 Top-right value
     * @param v10 Bottom-left value
     * @param v11 Bottom-right value
     * @return Interpolated value
     */
    double interpolateValues(double tx, double ty, double v00, double v01, double v10, double v11) const;

 public:
    /**
     * Constructor
     * @param filepath Path to the specular map image file
     * @param useBilinearFilter Whether to use bilinear filtering
     */
    explicit SpecularMap(const std::string& filepath, bool useBilinearFilter = true);

    /**
     * Virtual destructor
     */
    virtual ~SpecularMap() = default;

    /**
     * Get specular value at the given UV coordinates
     * @param uv UV coordinates
     * @return Specular value (0.0 - 1.0)
     */
    virtual double getSpecularAt(const Math::Vector2D& uv) const;

    /**
     * Clone this specular map
     * @return A new pointer to a specular map with the same properties
     */
    virtual std::shared_ptr<SpecularMap> clone() const;

    /**
     * Get the type of specular map
     * @return Type of specular map
     */
    virtual std::string getType() const;
};

}  // namespace RayTracer

#endif  // SRC_TEXTURE_SPECULARMAP_SPECULARMAP_HPP_
