/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** Renderer
*/

#include "Renderer.hpp"
#include <thread>
#include <vector>
#include <mutex>
#include <cmath>
#include <algorithm>
#include <atomic>
#include <memory>
// Suppression de l'include direct de SupersamplingPostProcess

namespace RayTracer {

Renderer::Renderer(std::shared_ptr<IDisplayManager> displayManager)
    : _displayManager(displayManager) {
}

Renderer::~Renderer() {
}

void Renderer::drawScene(const Scene& scene, const Camera& camera, const bool lowRender) {
    auto windowSize = _displayManager->getWindowSize();
    int imageWidth = windowSize.x;
    int imageHeight = windowSize.y;

    const_cast<Scene&>(scene).setImageDimensions(imageWidth, imageHeight);

    std::vector<Math::Vector3D> rawColorBuffer(imageWidth * imageHeight);
    std::vector<color_t> pixelBuffer(imageWidth * imageHeight);

    const int tileSize = 32;

    int numTilesX = (imageWidth + tileSize - 1) / tileSize;
    int numTilesY = (imageHeight + tileSize - 1) / tileSize;
    int totalTiles = numTilesX * numTilesY;

    std::atomic<int> nextTileIndex(0);

    int samplesPerPixel = 1;
    for (const auto& postProcess : scene.getPostProcessEffects()) {
        if (postProcess->getTypeName() == "supersampling") {
            samplesPerPixel = static_cast<int>(postProcess->getParameter("samples"));
            break;
        }
    }
    auto renderTiles = [&]() {
        while (true) {
            int tileIndex = nextTileIndex.fetch_add(1);
            if (tileIndex >= totalTiles)
                break;

            int tileY = tileIndex / numTilesX;
            int tileX = tileIndex % numTilesX;

            int startX = tileX * tileSize;
            int startY = tileY * tileSize;
            int endX = std::min(startX + tileSize, imageWidth);
            int endY = std::min(startY + tileSize, imageHeight);

            for (int y = startY; y < endY; ++y) {
                if (!(y % 5 == 0) && lowRender)
                    continue;

                for (int x = startX; x < endX; ++x) {
                    if (!(x % 5 == 0) && lowRender)
                        continue;

                    double u = static_cast<double>(x) / (imageWidth - 1);
                    double v = static_cast<double>((imageHeight - 1) - y) / (imageHeight - 1);

                    Math::Vector3D pixelColor;
                    if (lowRender) {
                        pixelColor = scene.computeColor(camera.ray(u, v), true);
                    } else if (samplesPerPixel > 1) {
                        pixelColor = camera.supersampleRay(u, v, scene, samplesPerPixel);
                    } else {
                        pixelColor = scene.computeColor(camera.ray(u, v), false);
                    }

                    rawColorBuffer[y * imageWidth + x] = pixelColor;
                }
            }
        }
    };

    unsigned int numThreads = std::thread::hardware_concurrency();
    numThreads = std::max(2u, std::min(numThreads, 16u));

    std::vector<std::thread> threads;
    threads.reserve(numThreads);
    for (unsigned int i = 0; i < numThreads; ++i) {
        threads.emplace_back(renderTiles);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    std::vector<Math::Vector3D> processedColorBuffer =
        scene.applyPostProcessingToFrameBuffer(rawColorBuffer, imageWidth, imageHeight);

    for (int y = 0; y < imageHeight; ++y) {
        for (int x = 0; x < imageWidth; ++x) {
            int index = y * imageWidth + x;
            const Math::Vector3D& color = processedColorBuffer[index];

            uint8_t r = static_cast<uint8_t>(255.999 * std::sqrt(std::max(0.0, std::min(1.0, color.X))));
            uint8_t g = static_cast<uint8_t>(255.999 * std::sqrt(std::max(0.0, std::min(1.0, color.Y))));
            uint8_t b = static_cast<uint8_t>(255.999 * std::sqrt(std::max(0.0, std::min(1.0, color.Z))));

            pixelBuffer[index] = {r, g, b, 255};
        }
    }

    _displayManager->beginFrame();
    _displayManager->drawImage(pixelBuffer, imageWidth, imageHeight);
    _displayManager->endFrame();
}

}  // namespace RayTracer
