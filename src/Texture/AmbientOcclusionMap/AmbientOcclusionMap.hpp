#ifndef SRC_TEXTURE_AMBIENTOCCLUSIONMAP_AMBIENTOCCLUSIONMAP_HPP_
#define SRC_TEXTURE_AMBIENTOCCLUSIONMAP_AMBIENTOCCLUSIONMAP_HPP_
#include <memory>
#include <string>
#include <vector>
#include "../../Math/Vector3D/Vector3D.hpp"
#include "../../Math/Vector2D/Vector2D.hpp"

namespace RayTracer {

/**
 * Class defining an ambient occlusion map that controls the attenuation of ambient light
 */
class AmbientOcclusionMap {
 private:
    std::vector<unsigned char> mapData;
    int width;
    int height;
    int channels;
    double strength;  // Strength of occlusion effect (0.0 - 1.0)
    bool bilinearFilter;

    /**
     * Load map data from file
     * @param filepath Path to the ambient occlusion map image file
     */
    void loadMapData(const std::string& filepath);

    /**
     * Create a default ambient occlusion map if loading fails
     */
    void createDefaultMap();

    /**
     * Store image data from the loaded buffer
     * @param data Pointer to the image data
     */
    void storeMapData(unsigned char* data);

    /**
     * Apply post-processing effects to the occlusion value
     * @param occlusion Raw occlusion value (0.0 - 1.0)
     * @return Processed occlusion value
     */
    double applyOcclusionEffect(double occlusion) const;

    /**
     * Sample using nearest neighbor filtering
     * @param u U coordinate (0.0 - 1.0)
     * @param v V coordinate (0.0 - 1.0)
     * @return Occlusion value at the given coordinates
     */
    double nearestNeighbor(double u, double v) const;

    /**
     * Sample using bilinear filtering
     * @param u U coordinate (0.0 - 1.0)
     * @param v V coordinate (0.0 - 1.0)
     * @return Occlusion value at the given coordinates with bilinear interpolation
     */
    double bilinearSample(double u, double v) const;

    /**
     * Normalize texture coordinate to [0, 1] range
     * @param coord Input coordinate
     * @return Normalized coordinate
     */
    double normalizeCoordinate(double coord) const;

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
     * @param filepath Path to the ambient occlusion map image file
     * @param strength Strength of occlusion effect (0.0 - 1.0)
     * @param useBilinearFilter Whether to use bilinear filtering
     */
    explicit AmbientOcclusionMap(const std::string& filepath,
                                 double strength = 1.0,
                                 bool useBilinearFilter = true);

    /**
     * Virtual destructor
     */
    virtual ~AmbientOcclusionMap() = default;

    /**
     * Get occlusion value at the given UV coordinates
     * @param uv UV coordinates
     * @return Occlusion value (0.0 - 1.0, where 0 = fully occluded)
     */
    virtual double getOcclusionAt(const Math::Vector2D& uv) const;

    /**
     * Clone this ambient occlusion map
     * @return A new pointer to an ambient occlusion map with the same properties
     */
    virtual std::shared_ptr<AmbientOcclusionMap> clone() const;

    /**
     * Get the type of ambient occlusion map
     * @return Type of ambient occlusion map
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

#endif  // SRC_TEXTURE_AMBIENTOCCLUSIONMAP_AMBIENTOCCLUSIONMAP_HPP_
