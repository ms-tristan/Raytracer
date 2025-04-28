/*
** EPITECH PROJECT, 2025
** RayTracer
** File description:
** SFML implementation of IEventsManager
*/

#ifndef SOURCES_CLIENT_CLASSES_EVENTS_SFMLEVENTSMANAGER_HPP_
#define SOURCES_CLIENT_CLASSES_EVENTS_SFMLEVENTSMANAGER_HPP_

#include <unordered_map>
#include <string>
#include <SFML/Graphics.hpp>
#include "./IEventsManager.hpp"

namespace RayTracer {

class SFMLEventsManager : public IEventsManager {
 public:
    SFMLEventsManager(sf::RenderWindow& window);
    ~SFMLEventsManager() override = default;

    bool isKeyPressed(std::string key) const override;
    vector2f_t getMousePos() override;

    bool processEvents() override;
    bool isWindowClosed() const override;
    bool isWindowResized() const override;
    vector2i_t getResizedDimensions() const override;

 private:
    sf::RenderWindow& _window;
    bool _isWindowClosed;
    bool _isWindowResized;
    vector2i_t _resizedDimensions;
    sf::Event _event;

    std::unordered_map<std::string, sf::Keyboard::Key> _keyMap;

    void initKeyMap();
};

}  // namespace RayTracer

#endif  // SOURCES_CLIENT_CLASSES_EVENTS_SFMLEVENTSMANAGER_HPP_
