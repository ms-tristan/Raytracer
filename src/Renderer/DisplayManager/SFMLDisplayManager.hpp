/*
** EPITECH PROJECT, 2025
** RayTracer
** File description:
** SFML implementation of IDisplayManager
*/

#ifndef SRC_RENDERER_DISPLAYMANAGER_SFMLDISPLAYMANAGER_HPP_
   #define SRC_RENDERER_DISPLAYMANAGER_SFMLDISPLAYMANAGER_HPP_

   #include <unordered_map>
   #include <string>
   #include <vector>
   #include <memory>
   #include <SFML/Graphics.hpp>
   #include <SFML/Audio.hpp>
   #include "./IDisplayManager.hpp"

namespace RayTracer {

class SFMLDisplayManager : public IDisplayManager {
 public:
    SFMLDisplayManager();
    ~SFMLDisplayManager() override;

    bool initialize(unsigned int width, unsigned int height,
                  const std::string& title, bool fullscreen = false) override;

    bool isWindowOpen() const override;
    void closeWindow() override;
    bool processEvents() override;
    void beginFrame() override;
    void endFrame() override;
    void setBackgroundColor(const color_t& color) override;

    bool loadTexture(const std::string& id, const std::string& fileP) override;
    void unloadTexture(const std::string& id) override;
    vector2u_t getTextureSize(const std::string& id) const;
    bool loadFont(const std::string& id, const std::string& filepath) override;
    void unloadFont(const std::string& id) override;

    void drawSprite(const std::string& textureId,
                    const vector2f_t& position,
                    const vector2f_t& scale = {1.0f, 1.0f},
                    float rotation = 0.0f,
                    const color_t& color = {255, 255, 255, 255},
                    const vector2f_t& origin = {0.0f, 0.0f}) override;

    void drawTextureRegion(const std::string& textureId,
                           const vector2f_t& position,
                           const recti_t& sourceRect,
                           const vector2f_t& scale = {1.0f, 1.0f},
                           float rotation = 0.0f,
                           const color_t& color = {255, 255, 255, 255},
                           const vector2f_t& origin = {0.0f, 0.0f}) override;

    void drawText(const std::string& text,
                  const std::string& fontId,
                  const vector2f_t& position,
                  unsigned int fontSize = 20,
                  const color_t& color = {255, 255, 255, 255},
                  int alignment = 0) override;

    void drawRectangle(const recti_t& rect,
                       const color_t& color,
                       const color_t& outlineColor = {0, 0, 0, 0},
                       float outlineThickness = 0.0f) override;

    void drawCircle(const vector2f_t& center,
                     float radius,
                     const color_t& color,
                     const color_t& outlineColor = {0, 0, 0, 0},
                     float outlineThickness = 0.0f) override;

    void drawImage(const std::vector<color_t>& pixelData,
                  unsigned int width,
                  unsigned int height,
                  const vector2f_t& position = {0.0f, 0.0f},
                  const vector2f_t& scale = {1.0f, 1.0f}) override;

    void drawLine(const vector2f_t& start,
                  const vector2f_t& end,
                  const color_t& color = {255, 255, 255, 255},
                  float thickness = 1.0f) override;

    void drawLines(const std::vector<vector2f_t>& points,
                   const color_t& color = {255, 255, 255, 255},
                   float thickness = 1.0f,
                   bool closed = false) override;

    void setWindowTitle(const std::string& title) override;
    void setFullscreen(bool fullscreen) override;
    vector2u_t getWindowSize() const override;
    void setVSync(bool enabled) override;
    void setFrameRateLimit(unsigned int fps) override;
    void setView(const vector2f_t& center, float zoom = 1.0f) override;
    void resetView() override;
    vector2f_t getViewCenter() const override;
    float getViewZoom() const override;
    vector2f_t windowToWorld(const vector2f_t& windowPos) const override;
    vector2f_t worldToWindow(const vector2f_t& worldPos) const override;

    std::shared_ptr<sf::RenderWindow> getWindow();

    void drawText(const sf::Text& text);

 private:
    sf::Color toSFColor(const color_t& color) const;
    sf::Vector2f toSFVector(const vector2f_t& vector) const;
    sf::FloatRect toSFRect(const recti_t& rect) const;
    sf::IntRect toSFIntRect(const recti_t& rect) const;

    std::shared_ptr<sf::RenderWindow> _window;
    sf::Color _backgroundColor;
    bool _isFullscreen;
    sf::VideoMode _windowedVideoMode;
    std::string _windowTitle;

    std::unordered_map<std::string, sf::Texture> _textures;
    std::unordered_map<std::string, sf::Font> _fonts;

    sf::Event _event;
    sf::View _defaultView;
    sf::View _currentView;
    float _currentZoom;
};

}  // namespace RayTracer

#endif  // SRC_RENDERER_DISPLAYMANAGER_SFMLDISPLAYMANAGER_HPP_
