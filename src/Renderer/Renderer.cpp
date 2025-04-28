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

namespace RayTracer {

Renderer::Renderer(IDisplayManager& displayManager)
    : _displayManager(displayManager) {
}

Renderer::~Renderer() {
}

void Renderer::drawScene(const Scene& scene, const Camera& camera) {
    auto windowSize = _displayManager.getWindowSize();
    int imageWidth = windowSize.x;
    int imageHeight = windowSize.y;

    std::vector<color_t> pixelBuffer(imageWidth * imageHeight);

    const int tileSize = 32;

    int numTilesX = (imageWidth + tileSize - 1) / tileSize;
    int numTilesY = (imageHeight + tileSize - 1) / tileSize;
    int totalTiles = numTilesX * numTilesY;

    std::atomic<int> nextTileIndex(0);

    auto renderTiles = [&]() {
        while (true) {
            int tileIndex = nextTileIndex.fetch_add(1);
            if (tileIndex >= totalTiles) {
                break;
            }

            int tileY = tileIndex / numTilesX;
            int tileX = tileIndex % numTilesX;

            int startX = tileX * tileSize;
            int startY = tileY * tileSize;
            int endX = std::min(startX + tileSize, imageWidth);
            int endY = std::min(startY + tileSize, imageHeight);

            for (int y = startY; y < endY; ++y) {
                for (int x = startX; x < endX; ++x) {
                    double u = static_cast<double>(x) / (imageWidth - 1);
                    double v = static_cast<double>((imageHeight - 1) - y) / (imageHeight - 1);

                    Ray ray = camera.ray(u, v);
                    Math::Vector3D pixelColor = scene.computeColor(ray);

                    uint8_t r = static_cast<uint8_t>(255.999 * std::sqrt(std::max(0.0, std::min(1.0, pixelColor.X))));
                    uint8_t g = static_cast<uint8_t>(255.999 * std::sqrt(std::max(0.0, std::min(1.0, pixelColor.Y))));
                    uint8_t b = static_cast<uint8_t>(255.999 * std::sqrt(std::max(0.0, std::min(1.0, pixelColor.Z))));

                    pixelBuffer[y * imageWidth + x] = {r, g, b, 255};
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

    _displayManager.beginFrame();
    _displayManager.drawImage(pixelBuffer, imageWidth, imageHeight);
    _displayManager.endFrame();
}

}  // namespace RayTracer
