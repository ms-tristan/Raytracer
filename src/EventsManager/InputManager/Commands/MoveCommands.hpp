#ifndef SRC_EVENTSMANAGER_INPUTMANAGER_COMMANDS_MOVECOMMANDS_HPP_
#define SRC_EVENTSMANAGER_INPUTMANAGER_COMMANDS_MOVECOMMANDS_HPP_

#include <memory>
#include <string>
#include "ICommand.hpp"
#include "../../../Camera/Camera.hpp"
#include "../../../Math/Vector3D/Vector3D.hpp"
#include "../../IEventsManager.hpp"

namespace RayTracer {

class KeyCommand : public ICommand {
 protected:
    std::shared_ptr<IEventsManager> _eventsManager;
    std::string _keyName;

 public:
    KeyCommand(std::shared_ptr<IEventsManager> eventsManager, const std::string& keyName)
        : _eventsManager(eventsManager), _keyName(keyName) {}

    bool isActive() const override {
        return _eventsManager->isKeyPressed(_keyName);
    }
};

class CameraTranslateCommand : public KeyCommand {
 private:
    std::shared_ptr<Camera> _camera;
    Math::Vector3D _direction;
    double _speed;

 public:
    CameraTranslateCommand(
        std::shared_ptr<IEventsManager> eventsManager,
        std::shared_ptr<Camera> camera,
        const std::string& keyName,
        const Math::Vector3D& direction,
        double speed)
        : KeyCommand(eventsManager, keyName), _camera(camera), _direction(direction), _speed(speed) {}

    void execute() override {
        _camera->translate(_direction * _speed);
    }

    void setSpeed(double speed) {
        _speed = speed;
    }
};

class CameraRotateCommand : public KeyCommand {
 private:
    std::shared_ptr<Camera> _camera;
    bool _isXAxis;
    double _degrees;

 public:
    CameraRotateCommand(
        std::shared_ptr<IEventsManager> eventsManager,
        std::shared_ptr<Camera> camera,
        const std::string& keyName,
        bool isXAxis,
        double degrees)
        : KeyCommand(eventsManager, keyName), _camera(camera), _isXAxis(isXAxis), _degrees(degrees) {}

    void execute() override {
        if (_isXAxis) {
            _camera->rotateX(_degrees);
        } else {
            _camera->rotateY(_degrees);
        }
    }
};

class MouseRotateCommand : public ICommand {
 private:
    std::shared_ptr<IEventsManager> _eventsManager;
    std::shared_ptr<Camera> _camera;
    sf::Vector2i& _lastMousePos;
    bool& _rightMouseWasPressed;
    double _sensitivity;

 public:
    MouseRotateCommand(
        std::shared_ptr<IEventsManager> eventsManager,
        std::shared_ptr<Camera> camera,
        sf::Vector2i& lastMousePos,
        bool& rightMouseWasPressed,
        double sensitivity)
        : _eventsManager(eventsManager), _camera(camera)
        , _lastMousePos(lastMousePos)
        , _rightMouseWasPressed(rightMouseWasPressed)
        , _sensitivity(sensitivity) {}

    bool isActive() const override {
        return _eventsManager->isButtonPressed("RIGHT");
    }

    void execute() override {
        auto currentMousePos = _eventsManager->getMousePos();
        sf::Vector2i currentPos = {static_cast<int>(currentMousePos.x), static_cast<int>(currentMousePos.y)};

        if (_rightMouseWasPressed) {
            int deltaX = currentPos.x - _lastMousePos.x;
            int deltaY = currentPos.y - _lastMousePos.y;

            if (deltaX != 0) {
                _camera->rotateY(-deltaX * _sensitivity);
            }
            if (deltaY != 0) {
                _camera->rotateX(-deltaY * _sensitivity);
            }
        }
        _lastMousePos = currentPos;
        _rightMouseWasPressed = true;
    }
};

}  // namespace RayTracer

#endif  // SRC_EVENTSMANAGER_INPUTMANAGER_COMMANDS_MOVECOMMANDS_HPP_
