/*
** EPITECH PROJECT, 2025
** RayTracer
** File description:
** IDisplayManager
*/

#ifndef SOURCES_SHARED_CLASSES_INTERFACES_IDISPLAYMANAGER_HPP_
#define SOURCES_SHARED_CLASSES_INTERFACES_IDISPLAYMANAGER_HPP_
#include <string>
#include <vector>
#include <memory>

namespace RayTracer {

  struct Color {
    unsigned char r, g, b, a;
  };
  using color_t = Color;

  struct Vector2f {
    float x, y;
  };
  using vector2f_t = Vector2f;

  struct Vector2u {
    unsigned int x, y;
  };
  using vector2u_t = Vector2u;

  struct Recti {
    int x, y, width, height;
  };
  using recti_t = Recti;

  struct Vector2i {
    int x, y;
  };
  using vector2i_t = Vector2i;

class IDisplayManager {
 public:
  virtual ~IDisplayManager() = default;

  virtual bool initialize(unsigned int width, unsigned int height,
                        const std::string& title, bool fullscreen = false) = 0;

  virtual bool isWindowOpen() const = 0;

  virtual void closeWindow() = 0;

  virtual bool processEvents() = 0;

  virtual void beginFrame() = 0;

  virtual void endFrame() = 0;

  virtual void setBackgroundColor(const color_t& color) = 0;

  virtual bool loadTexture(const std::string& id, const std::string& FP) = 0;

  virtual void unloadTexture(const std::string& id) = 0;

  virtual bool loadFont(const std::string& id, const std::string& FP) = 0;

  virtual void unloadFont(const std::string& id) = 0;

  virtual void drawSprite(const std::string& textureId,
                          const vector2f_t& position,
                          const vector2f_t& scale = {1.0f, 1.0f},
                          float rotation = 0.0f,
                          const color_t& color = {255, 255, 255, 255},
                          const vector2f_t& origin = {0.0f, 0.0f}) = 0;

  virtual void drawTextureRegion(const std::string& textureId,
                                const vector2f_t& position,
                                const recti_t& sourceRect,
                                const vector2f_t& scale = {1.0f, 1.0f},
                                float rotation = 0.0f,
                                const color_t& color = {255, 255, 255, 255},
                                const vector2f_t& origin = {0.0f, 0.0f}) = 0;

  virtual void drawText(const std::string& text,
                        const std::string& fontId,
                        const vector2f_t& position,
                        unsigned int fontSize = 20,
                        const color_t& color = {255, 255, 255, 255},
                        int alignment = 0) = 0;

  virtual void drawRectangle(const recti_t& rect,
                            const color_t& color,
                            const color_t& outlineColor = {0, 0, 0, 0},
                            float outlineThickness = 0.0f) = 0;

  virtual void drawCircle(const vector2f_t& center,
                          float radius,
                          const color_t& color,
                          const color_t& outlineColor = {0, 0, 0, 0},
                          float outlineThickness = 0.0f) = 0;

  virtual void drawImage(const std::vector<color_t>& pixelData, 
                        unsigned int width, 
                        unsigned int height,
                        const vector2f_t& position = {0.0f, 0.0f},
                        const vector2f_t& scale = {1.0f, 1.0f}) = 0;

  virtual void drawLine(const vector2f_t& start,
                        const vector2f_t& end,
                        const color_t& color = {255, 255, 255, 255},
                        float thickness = 1.0f) = 0;

  virtual void drawLines(const std::vector<vector2f_t>& points,
                        const color_t& color = {255, 255, 255, 255},
                        float thickness = 1.0f,
                        bool closed = false) = 0;

  virtual void setWindowTitle(const std::string& title) = 0;

  virtual void setFullscreen(bool fullscreen) = 0;

  virtual vector2u_t getWindowSize() const = 0;

  virtual void setVSync(bool enabled) = 0;

  virtual void setFrameRateLimit(unsigned int fps) = 0;

  virtual void setView(const vector2f_t& center, float zoom = 1.0f) = 0;
  virtual void resetView() = 0;
  virtual vector2f_t getViewCenter() const = 0;
  virtual float getViewZoom() const = 0;
  virtual vector2f_t windowToWorld(const vector2f_t& windowPos) const = 0;
  virtual vector2f_t worldToWindow(const vector2f_t& worldPos) const = 0;
};
}  // namespace RayTracer

#endif  // SOURCES_SHARED_CLASSES_INTERFACES_IDISPLAYMANAGER_HPP_
