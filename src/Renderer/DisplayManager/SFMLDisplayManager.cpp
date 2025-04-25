/*
** EPITECH PROJECT, 2025
** RayTracer
** File description:
** SFML implementation of IDisplayManager
*/

#include <iostream>
#include <complex>
#include <vector>
#include <string>
#include "SFMLDisplayManager.hpp"

namespace RayTracer {

SFMLDisplayManager::SFMLDisplayManager()
    : _backgroundColor(sf::Color(135, 206, 235))
    , _isFullscreen(false)
    , _windowedVideoMode(800, 600)
    , _windowTitle("")
    , _currentZoom(1.0f) {
}

SFMLDisplayManager::~SFMLDisplayManager() {
    if (_window.isOpen())
        _window.close();

    _textures.clear();
    _fonts.clear();
}

bool SFMLDisplayManager::initialize
    (unsigned int width, unsigned int height,
    const std::string& title, bool fullscreen) {
    _windowTitle = title;
    _windowedVideoMode = sf::VideoMode(width, height);
    _isFullscreen = fullscreen;

    if (fullscreen) {
        _window.create(sf::VideoMode::getFullscreenModes()[0],
            title, sf::Style::Fullscreen);
    } else {
        _window.create(_windowedVideoMode, title,
            sf::Style::Close | sf::Style::Titlebar);
    }
    _defaultView = _window.getDefaultView();
    _currentView = _defaultView;
    return _window.isOpen();
}

bool SFMLDisplayManager::isWindowOpen() const {
    return _window.isOpen();
}

void SFMLDisplayManager::closeWindow() {
    _window.close();
}

bool SFMLDisplayManager::processEvents() {
    while (_window.pollEvent(_event)) {
        if (_event.type == sf::Event::Closed) {
            _window.close();
            return false;
        }
    }
    return true;
}

void SFMLDisplayManager::beginFrame() {
    _window.clear(_backgroundColor);
}

void SFMLDisplayManager::endFrame() {
    _window.display();
}

void SFMLDisplayManager::setBackgroundColor(const color_t& color) {
    _backgroundColor = toSFColor(color);
}

bool SFMLDisplayManager::loadTexture
    (const std::string& id, const std::string& filepath) {
    if (_textures.find(id) != _textures.end()) {
        return true;
    }

    sf::Texture texture;
    if (!texture.loadFromFile(filepath)) {
        std::cerr << "Failed to load texture: " << filepath << std::endl;
        return false;
    }

    _textures[id] = texture;
    return true;
}

void SFMLDisplayManager::unloadTexture(const std::string& id) {
    _textures.erase(id);
}

vector2u_t SFMLDisplayManager::getTextureSize(const std::string& id) const {
    auto it = _textures.find(id);
    if (it == _textures.end()) {
        std::cerr << "Texture not found when getting size: " << id << std::endl;
        return vector2u_t{0, 0};
    }

    sf::Vector2u size = it->second.getSize();
    return vector2u_t{size.x, size.y};
}

bool SFMLDisplayManager::loadFont
    (const std::string& id, const std::string& filepath) {
    if (_fonts.find(id) != _fonts.end()) {
        return true;
    }

    sf::Font font;
    if (!font.loadFromFile(filepath)) {
        std::cerr << "Failed to load font: " << filepath << std::endl;

        const std::vector<std::string> fallbackFonts = {
            "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
            "/usr/share/fonts/TTF/DejaVuSans.ttf",
            "/usr/share/fonts/liberation/LiberationSans-Regular.ttf",
            "/Library/Fonts/Arial.ttf",  // macOS
            "C:\\Windows\\Fonts\\arial.ttf"  // Windows
        };

        bool fallbackLoaded = false;
        for (const auto& fallbackPath : fallbackFonts) {
            if (font.loadFromFile(fallbackPath)) {
                fallbackLoaded = true;
                break;
            }
        }

        if (!fallbackLoaded) {
            return false;
        }
    }

    _fonts[id] = font;
    return true;
}

void SFMLDisplayManager::unloadFont(const std::string& id) {
    _fonts.erase(id);
}

void SFMLDisplayManager::drawSprite(const std::string& textureId,
                                    const vector2f_t& position,
                                    const vector2f_t& scale,
                                    float rotation,
                                    const color_t& color,
                                    const vector2f_t& origin) {
    auto it = _textures.find(textureId);
    if (it == _textures.end()) {
        std::cerr << "Texture not found: " << textureId << std::endl;
        return;
    }

    sf::Sprite sprite(it->second);
    sprite.setPosition(toSFVector(position));
    sprite.setScale(toSFVector(scale));
    sprite.setRotation(rotation);
    sprite.setColor(toSFColor(color));
    sprite.setOrigin(toSFVector(origin));

    _window.draw(sprite);
}

void SFMLDisplayManager::drawTextureRegion(
                                            const std::string& textureId,
                                            const vector2f_t& position,
                                            const recti_t& sourceRect,
                                            const vector2f_t& scale,
                                            float rotation,
                                            const color_t& color,
                                            const vector2f_t& origin) {
    auto it = _textures.find(textureId);
    if (it == _textures.end()) {
        std::cerr << "Texture not found: " << textureId << std::endl;
        return;
    }

    sf::Sprite sprite(it->second);
    sprite.setTextureRect(toSFIntRect(sourceRect));
    sprite.setPosition(toSFVector(position));
    sprite.setScale(toSFVector(scale));
    sprite.setRotation(rotation);
    sprite.setColor(toSFColor(color));
    sprite.setOrigin(toSFVector(origin));

    _window.draw(sprite);
}

void SFMLDisplayManager::drawText(const std::string& text,
                                    const std::string& fontId,
                                    const vector2f_t& position,
                                    unsigned int fontSize,
                                    const color_t& color,
                                    int alignment) {
    auto it = _fonts.find(fontId);
    if (it == _fonts.end()) {
        std::cerr << "Font not found: " << fontId << std::endl;
        if (loadFont(fontId, "assets/fonts/arial.ttf")) {
            it = _fonts.find(fontId);
        } else {
            return;
        }
    }

    sf::Text sfText;
    sfText.setFont(it->second);
    sfText.setString(text);
    sfText.setCharacterSize(fontSize);
    sfText.setFillColor(toSFColor(color));

    sf::FloatRect textBounds = sfText.getLocalBounds();

    if (alignment == 1) {
        sfText.setOrigin(textBounds.width / 2.0f, textBounds.height / 2.0f);
    } else if (alignment == 2) {
        sfText.setOrigin(textBounds.width, 0);
    }

    sfText.setPosition(toSFVector(position));
    _window.draw(sfText);
}

void SFMLDisplayManager::drawText(const sf::Text& text) {
    _window.draw(text);
}

void SFMLDisplayManager::drawRectangle(const recti_t& rect,
                                        const color_t& color,
                                        const color_t& outlineColor,
                                        float outlineThickness) {
    sf::RectangleShape shape(sf::Vector2f(rect.width, rect.height));
    shape.setPosition(sf::Vector2f(rect.x, rect.y));
    shape.setFillColor(toSFColor(color));
    shape.setOutlineColor(toSFColor(outlineColor));
    shape.setOutlineThickness(outlineThickness);

    _window.draw(shape);
}

void SFMLDisplayManager::drawCircle(const vector2f_t& center,
                                    float radius,
                                    const color_t& color,
                                    const color_t& outlineColor,
                                    float outlineThickness) {
    sf::CircleShape shape(radius);
    shape.setPosition(sf::Vector2f(center.x - radius, center.y - radius));
    shape.setFillColor(toSFColor(color));
    shape.setOutlineColor(toSFColor(outlineColor));
    shape.setOutlineThickness(outlineThickness);

    _window.draw(shape);
}

void SFMLDisplayManager::drawLine(const vector2f_t& start,
                                    const vector2f_t& end,
                                    const color_t& color,
                                    float thickness) {
    float length = std::sqrt
        (std::pow(end.x - start.x, 2) + std::pow(end.y - start.y, 2));
    float angle = std::atan2
        (end.y - start.y, end.x - start.x) * 180 / 3.14159265f;

    sf::RectangleShape line(sf::Vector2f(length, thickness));
    line.setPosition(toSFVector(start));
    line.setRotation(angle);
    line.setFillColor(toSFColor(color));
    line.setOrigin(0, thickness / 2.0f);

    _window.draw(line);
}

void SFMLDisplayManager::drawLines(
                                    const std::vector<vector2f_t>& points,
                                    const color_t& color,
                                    float thickness,
                                    bool closed) {
    if (points.size() < 2) {
        return;
    }

    for (size_t i = 0; i < points.size() - 1; ++i) {
        drawLine(points[i], points[i + 1], color, thickness);
    }

    if (closed && points.size() > 2) {
        drawLine(points[points.size() - 1], points[0], color, thickness);
    }
}

void SFMLDisplayManager::setWindowTitle(const std::string& title) {
    _windowTitle = title;
    _window.setTitle(title);
}

void SFMLDisplayManager::setFullscreen(bool fullscreen) {
    if (_isFullscreen == fullscreen) {
        return;
    }

    _isFullscreen = fullscreen;
    _window.close();

    if (_isFullscreen) {
        _window.create(sf::VideoMode::getFullscreenModes()[0],
            _windowTitle, sf::Style::Fullscreen);
    } else {
        _window.create(_windowedVideoMode,
            _windowTitle, sf::Style::Close | sf::Style::Titlebar);
    }
}

vector2u_t SFMLDisplayManager::getWindowSize() const {
    sf::Vector2u size = _window.getSize();
    return vector2u_t{size.x, size.y};
}

void SFMLDisplayManager::setVSync(bool enabled) {
    _window.setVerticalSyncEnabled(enabled);
}

void SFMLDisplayManager::setFrameRateLimit(unsigned int fps) {
    _window.setFramerateLimit(fps);
}

sf::Color SFMLDisplayManager::toSFColor
    (const color_t& color) const {
    return sf::Color(color.r, color.g, color.b, color.a);
}

sf::Vector2f SFMLDisplayManager::toSFVector
    (const vector2f_t& vector) const {
    return sf::Vector2f(vector.x, vector.y);
}

sf::FloatRect SFMLDisplayManager::toSFRect
    (const recti_t& rect) const {
    return sf::FloatRect(rect.x, rect.y, rect.width, rect.height);
}

sf::IntRect SFMLDisplayManager::toSFIntRect
    (const recti_t& rect) const {
    return sf::IntRect(rect.x, rect.y, rect.width, rect.height);
}

sf::RenderWindow& SFMLDisplayManager::getWindow() {
    return _window;
}

void SFMLDisplayManager::setView
(const vector2f_t& center, float zoom) {
    _currentView.setCenter(sf::Vector2f(center.x, center.y));
    _currentZoom = zoom;
    _currentView.setSize(_defaultView.getSize().x / zoom,
        _defaultView.getSize().y / zoom);
    _window.setView(_currentView);
}

void SFMLDisplayManager::resetView() {
    _window.setView(_defaultView);
    _currentView = _defaultView;
    _currentZoom = 1.0f;
}

vector2f_t SFMLDisplayManager::getViewCenter() const {
    sf::Vector2f center = _currentView.getCenter();
    return {center.x, center.y};
}

float SFMLDisplayManager::getViewZoom() const {
    return _currentZoom;
}

vector2f_t SFMLDisplayManager::windowToWorld(
    const vector2f_t& windowPos) const {
    sf::Vector2i pixelPos(static_cast<int>(windowPos.x),
        static_cast<int>(windowPos.y));
    sf::Vector2f worldPos = _window.mapPixelToCoords(pixelPos, _currentView);
    return {worldPos.x, worldPos.y};
}

vector2f_t SFMLDisplayManager::worldToWindow(
    const vector2f_t& worldPos) const {
    sf::Vector2f sfWorldPos(worldPos.x, worldPos.y);
    sf::Vector2i pixelPos = _window.mapCoordsToPixel(sfWorldPos, _currentView);
    return {static_cast<float>(pixelPos.x), static_cast<float>(pixelPos.y)};
}

}  // namespace RayTracer
