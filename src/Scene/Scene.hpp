// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Scene
*/

#ifndef SRC_SCENE_SCENE_HPP_
  #define SRC_SCENE_SCENE_HPP_
  #include <algorithm>
  #include <limits>
  #include <memory>
  #include <vector>
  #include <libconfig.h++>
  #include "WindowSize.hpp"
  #include "Camera/Camera.hpp"
  #include "defs.hpp"
  #include "Light/AmbientLight/AmbientLight.hpp"
  #include "Light/ILight.hpp"
  #include "Primitive/IPrimitive.hpp"
  #include "Ray/Ray.hpp"
  #include "Shader/IShader.hpp"
  #include "PostProcess/IPostProcess.hpp"

namespace RayTracer {
/**
 * @brief The Scene class represents a 3D scene with primitives, lights, shaders, and post-processing effects
 */
class Scene {
 private:
    std::vector<std::shared_ptr<IPrimitive>> primitives;
    std::vector<std::shared_ptr<ILight>> lights;
    std::vector<std::shared_ptr<IShader>> shaders;
    std::vector<std::shared_ptr<IPostProcess>> postProcessEffects;
    AmbientLight ambientLight;
    Camera camera;
    static constexpr int maxReflectionDepth = 5;
    int imageWidth = WIDTH;
    int imageHeight = HEIGHT;

    Math::Vector3D createTangentVector(const Math::Vector3D &normal) const;
    void applyDisplacementMapping(std::optional<HitInfo> &hit,
                                const std::shared_ptr<Material> &material) const;

    double calculateMaxShadowDistance(const Math::Point3D &hitPoint,
                                    const std::shared_ptr<ILight> &light) const;

    // Helper methods for material and lighting
    Math::Vector3D calculateBaseColor(const std::shared_ptr<Material> &material,
                                    const Math::Vector2D &uv) const;
    Math::Vector3D calculateSurfaceNormal(const std::shared_ptr<Material> &material,
                                        const std::optional<HitInfo> &hit) const;
    double calculateAmbientOcclusion(const std::shared_ptr<Material> &material,
                                   const Math::Vector2D &uv) const;
    Math::Vector3D getLightColor(const std::shared_ptr<ILight> &light,
                               const Math::Point3D &hitPoint) const;
    double calculateDiffuseFactor(const Math::Vector3D &normal,
                                const Math::Vector3D &lightDir,
                                bool shadowed,
                                double aoFactor) const;
    Math::Vector3D calculateSpecular(const Math::Point3D &hitPoint,
                                   const Math::Point3D &viewerPos,
                                   const Math::Vector3D &lightDir,
                                   const Math::Vector3D &normal,
                                   const Math::Vector3D &lightColor,
                                   const std::shared_ptr<Material> &material,
                                   const Math::Vector2D &uv) const;
    Math::Vector3D applyLighting(const Math::Vector3D &basePixelColor,
                               const std::optional<HitInfo> &hit,
                               const Ray &ray,
                               const Math::Vector3D &normal,
                               const Math::Vector3D &baseColor,
                               const std::shared_ptr<Material> &material,
                               double aoFactor) const;

    // Helper methods for reflection and refraction
    Math::Vector3D calculateReflection(const Ray &ray,
                                     const std::optional<HitInfo> &hit,
                                     const Math::Vector3D &normal,
                                     double reflectivity,
                                     double epsilon,
                                     int depth) const;
    Math::Vector3D calculateRefraction(const Ray &ray,
                                     const std::optional<HitInfo> &hit,
                                     const Math::Vector3D &normal,
                                     const std::shared_ptr<Material> &material,
                                     double transparency,
                                     double epsilon,
                                     int depth) const;
    Math::Vector3D calculateTotalInternalReflection(const Ray &ray,
                                                  const Math::Vector3D &normal) const;
    Math::Vector3D calculateSnellRefraction(const Ray &ray,
                                          const Math::Vector3D &normal,
                                          double cosTheta,
                                          double refractionRatio) const;
    Math::Vector3D applyReflectionAndRefraction(const Math::Vector3D &baseColor,
                                              const std::optional<HitInfo> &hit,
                                              const Ray &ray,
                                              const Math::Vector3D &normal,
                                              const std::shared_ptr<Material> &material,
                                              int depth) const;

 public:
    /**
     * @brief Default constructor
     */
    Scene() = default;

    /**
     * @brief Sets the camera for the scene
     * @param cam The camera to set
     */
    void setCamera(const Camera &cam);

    /**
     * @brief Gets the current camera of the scene
     * @return The current camera
     */
    Camera getCamera() const;

    /**
     * @brief Sets the ambient light for the scene
     * @param light The ambient light to set
     */
    void setAmbientLight(const AmbientLight &light);

    /**
     * @brief Adds a primitive to the scene
     * @param primitive The primitive to add
     */
    void addPrimitive(const std::shared_ptr<IPrimitive> &primitive);

    /**
     * @brief Adds a light to the scene
     * @param light The light to add
     */
    void addLight(const std::shared_ptr<ILight> &light);

    /**
     * @brief Adds a shader to the scene
     * @param shader The shader to add
     */
    void addShader(const std::shared_ptr<IShader> &shader);

    /**
     * @brief Adds a post-processing effect to the scene
     * @param postProcess The post-processing effect to add
     */
    void addPostProcess(const std::shared_ptr<IPostProcess> &postProcess);

    /**
     * @brief Traces a ray through the scene and finds the closest hit
     * @param ray The ray to trace
     * @return Information about the closest hit, if any
     */
    std::optional<HitInfo> trace(const Ray &ray) const;

    /**
     * @brief Checks if a point is in shadow from a light
     * @param hitPoint The point to check
     * @param lightDir The direction to the light
     * @param light The light source
     * @return True if the point is in shadow, false otherwise
     */
    bool isInShadow(const Math::Point3D &hitPoint,
      const Math::Vector3D &lightDir,
      const std::shared_ptr<ILight> &light) const;

    /**
     * @brief Computes the color for a ray
     * @param ray The ray to compute color for
     * @param depth The current recursion depth
     * @return The computed color
     */
    Math::Vector3D computeColor(const Ray &ray, int depth = 0) const;

    /**
     * @brief Applies post-processing effects to a frame buffer
     * @param frameBuffer The original frame buffer
     * @param width The width of the frame buffer
     * @param height The height of the frame buffer
     * @return The processed frame buffer
     */
    std::vector<Math::Vector3D> applyPostProcessingToFrameBuffer(
        const std::vector<Math::Vector3D>& frameBuffer, int width, int height) const;

    /**
     * @brief Writes a color to the standard output
     * @param color The color to write
     */
    void writeColor(const Math::Vector3D &color) const;

    /**
     * @brief Saves the scene configuration to a libconfig setting
     * @param setting The setting to save to
     */
    void getLibConfigParams(std::shared_ptr<libconfig::Setting> setting) const;

    /**
     * @brief Gets the primitives in the scene
     * @return A vector of primitives
     */
    const std::vector<std::shared_ptr<IPrimitive>>& getPrimitives() const { return primitives; }

    /**
     * @brief Gets the lights in the scene
     * @return A vector of lights
     */
    const std::vector<std::shared_ptr<ILight>>& getLights() const { return lights; }

    /**
     * @brief Gets the shaders in the scene
     * @return A vector of shaders
     */
    const std::vector<std::shared_ptr<IShader>>& getShaders() const { return shaders; }

    /**
     * @brief Gets the post-processing effects in the scene
     * @return A vector of post-processing effects
     */
    const std::vector<std::shared_ptr<IPostProcess>>& getPostProcessEffects() const { return postProcessEffects; }

    /**
     * @brief Gets the ambient light in the scene
     * @return The ambient light
     */
    const AmbientLight& getAmbientLight() const { return ambientLight; }

    /**
     * @brief Gets the image width
     * @return The image width
     */
    int getImageWidth() const { return imageWidth; }

    /**
     * @brief Gets the image height
     * @return The image height
     */
    int getImageHeight() const { return imageHeight; }

    /**
     * @brief Sets the image dimensions
     * @param width The image width
     * @param height The image height
     */
    void setImageDimensions(int width, int height) {
        imageWidth = width;
        imageHeight = height;
    }
};
}  // namespace RayTracer

#endif  // SRC_SCENE_SCENE_HPP_
