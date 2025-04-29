/*
** EPITECH PROJECT, 2025
** RayTracer
** File description:
** SFML implementation of IEventsManager
*/
#include <string>
#include "SFMLEventsManager.hpp"

namespace RayTracer {

SFMLEventsManager::SFMLEventsManager(sf::RenderWindow& window)
    : _window(window),
      _isWindowClosed(false),
      _isWindowResized(false),
      _resizedDimensions({0, 0}) {
    initKeyMap();
}

void SFMLEventsManager::initKeyMap() {
    _keyMap["A"] = sf::Keyboard::A;
    _keyMap["B"] = sf::Keyboard::B;
    _keyMap["C"] = sf::Keyboard::C;
    _keyMap["D"] = sf::Keyboard::D;
    _keyMap["E"] = sf::Keyboard::E;
    _keyMap["F"] = sf::Keyboard::F;
    _keyMap["G"] = sf::Keyboard::G;
    _keyMap["H"] = sf::Keyboard::H;
    _keyMap["I"] = sf::Keyboard::I;
    _keyMap["J"] = sf::Keyboard::J;
    _keyMap["K"] = sf::Keyboard::K;
    _keyMap["L"] = sf::Keyboard::L;
    _keyMap["M"] = sf::Keyboard::M;
    _keyMap["N"] = sf::Keyboard::N;
    _keyMap["O"] = sf::Keyboard::O;
    _keyMap["P"] = sf::Keyboard::P;
    _keyMap["Q"] = sf::Keyboard::Q;
    _keyMap["R"] = sf::Keyboard::R;
    _keyMap["S"] = sf::Keyboard::S;
    _keyMap["T"] = sf::Keyboard::T;
    _keyMap["U"] = sf::Keyboard::U;
    _keyMap["V"] = sf::Keyboard::V;
    _keyMap["W"] = sf::Keyboard::W;
    _keyMap["X"] = sf::Keyboard::X;
    _keyMap["Y"] = sf::Keyboard::Y;
    _keyMap["Z"] = sf::Keyboard::Z;
    _keyMap["0"] = sf::Keyboard::Num0;
    _keyMap["1"] = sf::Keyboard::Num1;
    _keyMap["2"] = sf::Keyboard::Num2;
    _keyMap["3"] = sf::Keyboard::Num3;
    _keyMap["4"] = sf::Keyboard::Num4;
    _keyMap["5"] = sf::Keyboard::Num5;
    _keyMap["6"] = sf::Keyboard::Num6;
    _keyMap["7"] = sf::Keyboard::Num7;
    _keyMap["8"] = sf::Keyboard::Num8;
    _keyMap["9"] = sf::Keyboard::Num9;
    _keyMap["ESCAPE"] = sf::Keyboard::Escape;
    _keyMap["SPACE"] = sf::Keyboard::Space;
    _keyMap["RETURN"] = sf::Keyboard::Return;
    _keyMap["BACKSPACE"] = sf::Keyboard::BackSpace;
    _keyMap["TAB"] = sf::Keyboard::Tab;
    _keyMap["LEFT"] = sf::Keyboard::Left;
    _keyMap["RIGHT"] = sf::Keyboard::Right;
    _keyMap["UP"] = sf::Keyboard::Up;
    _keyMap["DOWN"] = sf::Keyboard::Down;
    _keyMap["LSHIFT"] = sf::Keyboard::LShift;
    _keyMap["RSHIFT"] = sf::Keyboard::RShift;
    _keyMap["LCONTROL"] = sf::Keyboard::LControl;
    _keyMap["RCONTROL"] = sf::Keyboard::RControl;
}

bool SFMLEventsManager::isKeyPressed(std::string key) const {
    auto it = _keyMap.find(key);
    if (it == _keyMap.end()) {
        return false;
    }
    return sf::Keyboard::isKeyPressed(it->second);
}

vector2f_t SFMLEventsManager::getMousePos() {
    sf::Vector2i mousePos = sf::Mouse::getPosition(_window);
    return {static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)};
}

bool SFMLEventsManager::processEvents() {
    _isWindowClosed = false;
    _isWindowResized = false;

    while (_window.pollEvent(_event)) {
        switch (_event.type) {
            case sf::Event::Closed:
                _window.close();
                _isWindowClosed = true;
                return false;
            case sf::Event::Resized:
                _isWindowResized = true;
                _resizedDimensions.x = _event.size.width;
                _resizedDimensions.y = _event.size.height;
                break;
            default:
                break;
        }
    }
    return true;
}

bool SFMLEventsManager::isWindowClosed() const {
    return _isWindowClosed;
}

bool SFMLEventsManager::isWindowResized() const {
    return _isWindowResized;
}

vector2i_t SFMLEventsManager::getResizedDimensions() const {
    return _resizedDimensions;
}

}  // namespace RayTracer
