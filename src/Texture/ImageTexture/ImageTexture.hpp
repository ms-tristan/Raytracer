// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ImageTexture - texture based on an image
*/

#ifndef SRC_TEXTURE_IMAGETEXTURE_IMAGETEXTURE_HPP_
    #define SRC_TEXTURE_IMAGETEXTURE_IMAGETEXTURE_HPP_
    #include <memory>
    #include <string>
    #include <vector>
    #include "../ATexture.hpp"

namespace RayTracer {

/**
 * Texture class based on image file
 */
class ImageTexture : public ATexture {
 private:
    std::vector<unsigned char> imageData;
    int width;
    int height;
    int channels;
    bool bilinearFilter;

    /**
     * Nearest neighbor sampling method
     * @param u U coordinate (0.0 - 1.0)
     * @param v V coordinate (0.0 - 1.0)
     * @return Color at the given UV coordinates
     */
    Math::Vector3D nearestNeighbor(double u, double v) const;

    /**
     * Bilinear sampling method
     * @param u U coordinate (0.0 - 1.0)
     * @param v V coordinate (0.0 - 1.0)
     * @return Color at the given UV coordinates with bilinear filtering
     */
    Math::Vector3D bilinearSample(double u, double v) const;

    /**
     * Load image data from file
     * @param filepath Path to the image file
     */
    void loadImageData(const std::string& filepath);

    /**
     * Create a default texture if image loading fails
     */
    void createDefaultTexture();

    /**
     * Store image data from the loaded buffer
     * @param data Pointer to the image data
     */
    void storeImageData(unsigned char* data);

    /**
     * Normalize texture coordinate to [0, 1] range
     * @param coord Input coordinate
     * @return Normalized coordinate
     */
    double normalizeCoordinate(double coord) const;

    /**
     * Interpolate channel value for bilinear filtering
     * @param channelOffset Channel offset (0 for R, 1 for G, 2 for B)
     * @param tx X interpolation factor
     * @param ty Y interpolation factor
     * @param idx00 Index of top-left pixel
     * @param idx01 Index of top-right pixel
     * @param idx10 Index of bottom-left pixel
     * @param idx11 Index of bottom-right pixel
     * @return Interpolated channel value
     */
    double interpolateChannel(int channelOffset, double tx, double ty,
                              size_t idx00, size_t idx01,
                              size_t idx10, size_t idx11) const;

 public:
    /**
     * Constructor
     * @param filepath Path to the image file
     * @param useBilinearFilter Whether to use bilinear filtering
     */
    explicit ImageTexture(const std::string& filepath, bool useBilinearFilter = true);

    /**
     * Destructor
     */
    ~ImageTexture() override = default;

    /**
     * Get color at UV coordinates
     * @param uv UV coordinates
     * @return Color at the given coordinates
     */
    Math::Vector3D getColorAt(const Math::Vector2D& uv) const override;

    /**
     * Clone the texture
     * @return A new texture instance with the same properties
     */
    std::shared_ptr<ITexture> clone() const override;

    /**
     * Get static type name for the texture
     * @return Type name
     */
    static std::string getTypeNameStatic() {
        return "image";
    }
};

}  // namespace RayTracer

#endif  // SRC_TEXTURE_IMAGETEXTURE_IMAGETEXTURE_HPP_
