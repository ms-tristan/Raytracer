#ifndef SRC_TEXTURE_DISPLACEMENTMAP_DISPLACEMENTMAP_HPP_
#define SRC_TEXTURE_DISPLACEMENTMAP_DISPLACEMENTMAP_HPP_
#include <memory>
#include <string>
#include <vector>
#include "../../Math/Vector3D/Vector3D.hpp"
#include "../../Math/Vector2D/Vector2D.hpp"

namespace RayTracer {

/**
 * Class defining a displacement map that can modify surface positions to simulate details
 */
class DisplacementMap {
 private:
    std::vector<unsigned char> mapData;
    int width;
    int height;
    int channels;
    double strength;  // Strength of displacement effect (0.0 - 1.0)
    bool bilinearFilter;
    std::string filepath;

    /**
     * Load map data from file
     * @param filepath Path to the displacement map image file
     */
    void loadMapData(const std::string& filepath);

    /**
     * Create a default displacement map if loading fails
     */
    void createDefaultMap();

    /**
     * Store image data from the loaded buffer
     * @param data Pointer to the image data
     */
    void storeMapData(unsigned char* data);

    /**
     * Sample using nearest neighbor filtering
     * @param u U coordinate (0.0 - 1.0)
     * @param v V coordinate (0.0 - 1.0)
     * @return Displacement value at the given coordinates
     */
    double nearestNeighbor(double u, double v) const;

    /**
     * Sample using bilinear filtering
     * @param u U coordinate (0.0 - 1.0)
     * @param v V coordinate (0.0 - 1.0)
     * @return Displacement value at the given coordinates with bilinear interpolation
     */
    double bilinearSample(double u, double v) const;

    /**
     * Normalize texture coordinate to [0, 1] range
     * @param coord Input coordinate
     * @return Normalized coordinate
     */
    double normalizeCoordinate(double coord) const;

    /**
     * Apply post-processing effects to the displacement value
     * @param displacement Raw displacement value (0.0 - 1.0)
     * @return Processed displacement value
     */
    double applyEffects(double displacement) const;

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
     * @param filepath Path to the displacement map image file
     * @param strength Strength of displacement effect (0.0 - 1.0)
     * @param useBilinearFilter Whether to use bilinear filtering
     */
    explicit DisplacementMap(const std::string& filepath, double strength = 1.0, bool useBilinearFilter = true);

    /**
     * Virtual destructor
     */
    virtual ~DisplacementMap() = default;

    /**
     * Get displacement value at the given UV coordinates
     * @param uv UV coordinates
     * @return Displacement value (0.0 - 1.0)
     */
    virtual double getDisplacementAt(const Math::Vector2D& uv) const;

    /**
     * Clone this displacement map
     * @return A new pointer to a displacement map with the same properties
     */
    virtual std::shared_ptr<DisplacementMap> clone() const;

    /**
     * Get the type of displacement map
     * @return Type of displacement map
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

#endif  // SRC_TEXTURE_DISPLACEMENTMAP_DISPLACEMENTMAP_HPP_
