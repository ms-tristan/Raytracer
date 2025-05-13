// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** InputManager implementation
*/
#include <iostream>
#include <memory>
#include "InputManager.hpp"
#include "../../Ray/Ray.hpp"

namespace RayTracer {

InputManager::InputManager(std::shared_ptr<IEventsManager> eventsManager, int windowWidth, int windowHeight)
    : _eventsManager(eventsManager)
    , _windowWidth(windowWidth)
    , _windowHeight(windowHeight)
    , _moveSpeed(0.4)
    , _rotateSpeed(4.0)
    , _mouseRotateSensitivity(0.3)
    , _mouseWasPressed(false)
    , _rightMouseWasPressed(false)
    , _isDragging(false)
    , _selectedPrimitive(nullptr) {
}

void InputManager::processInput(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera) {
    _eventsManager->processEvents();

    Math::Point3D screenCenter = camera->screen.origin +
        camera->screen.bottom_side * 0.5 + camera->screen.left_side * 0.5;
    Math::Vector3D forwardDir = (screenCenter - camera->origin).normalize();
    Math::Vector3D upDir = camera->screen.left_side.normalize();
    Math::Vector3D rightDir = forwardDir.cross(upDir).normalize();

    handleCameraMovement(camera);
    handleObjectSelection(scene, camera);
    handleObjectScrolling(scene, camera);
    if (_isDragging && _selectedPrimitive) {
        handleObjectDragging(camera);
    }
}

void InputManager::setupCameraCommands(std::shared_ptr<Camera> camera) {
    Math::Point3D screenCenter = camera->screen.origin +
        camera->screen.bottom_side * 0.5 + camera->screen.left_side * 0.5;
    Math::Vector3D forwardDir = (screenCenter - camera->origin).normalize();
    Math::Vector3D upDir = camera->screen.left_side.normalize();
    Math::Vector3D rightDir = forwardDir.cross(upDir).normalize();

    _cameraCommands.clear();

    _cameraCommands.push_back(std::make_shared<CameraTranslateCommand>(
        _eventsManager, camera, "Z", forwardDir, _moveSpeed));
    _cameraCommands.push_back(std::make_shared<CameraTranslateCommand>(
        _eventsManager, camera, "S", forwardDir * -1.0, _moveSpeed));
    _cameraCommands.push_back(std::make_shared<CameraTranslateCommand>(
        _eventsManager, camera, "Q", rightDir * -1.0, _moveSpeed));
    _cameraCommands.push_back(std::make_shared<CameraTranslateCommand>(
        _eventsManager, camera, "D", rightDir, _moveSpeed));
    _cameraCommands.push_back(std::make_shared<CameraTranslateCommand>(
        _eventsManager, camera, "SPACE", Math::Vector3D(Math::Coords{0.0, 1.0, 0.0}), _moveSpeed));
    _cameraCommands.push_back(std::make_shared<CameraTranslateCommand>(
        _eventsManager, camera, "LCONTROL", Math::Vector3D(Math::Coords{0.0, -1.0, 0.0}), _moveSpeed));

    _cameraCommands.push_back(std::make_shared<CameraRotateCommand>(
        _eventsManager, camera, "LEFT", false, _rotateSpeed));
    _cameraCommands.push_back(std::make_shared<CameraRotateCommand>(
        _eventsManager, camera, "RIGHT", false, -_rotateSpeed));
    _cameraCommands.push_back(std::make_shared<CameraRotateCommand>(
        _eventsManager, camera, "UP", true, _rotateSpeed));
    _cameraCommands.push_back(std::make_shared<CameraRotateCommand>(
        _eventsManager, camera, "DOWN", true, -_rotateSpeed));

    _cameraCommands.push_back(std::make_shared<MouseRotateCommand>(
        _eventsManager, camera, _lastMousePos, _rightMouseWasPressed, _mouseRotateSensitivity));
}

void InputManager::handleCameraMovement(std::shared_ptr<Camera> camera) {
    setupCameraCommands(camera);

    double speedMultiplier = _eventsManager->isKeyPressed("LSHIFT") ? 3.5 : 1.0;
    for (auto& command : _cameraCommands) {
        auto translateCmd = std::dynamic_pointer_cast<CameraTranslateCommand>(command);
        if (translateCmd) {
            translateCmd->setSpeed(_moveSpeed * speedMultiplier);
        }

        if (command->isActive()) {
            _moving = true;
            command->execute();
        }
    }

    bool rightMouseIsPressed = _eventsManager->isButtonPressed("RIGHT");
    if (!rightMouseIsPressed) {
        _rightMouseWasPressed = false;
    }
}

void InputManager::handleObjectSelection(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera) {
    auto currentMousePos = _eventsManager->getMousePos();
    bool mouseIsPressed = _eventsManager->isButtonPressed("LEFT");

    if (mouseIsPressed && !_mouseWasPressed) {
        double u = static_cast<double>(currentMousePos.x) / (_windowWidth - 1);
        double v = static_cast<double>((_windowHeight - 1) - currentMousePos.y) / (_windowHeight - 1);

        Ray ray = camera->ray(u, v);
        auto hit = scene->trace(ray);

        if (hit && hit->primitive) {
            _isDragging = true;
            _selectedPrimitive = hit->primitive;
            _dragStartPos = {static_cast<int>(currentMousePos.x), static_cast<int>(currentMousePos.y)};
        }
    } else if (!mouseIsPressed && _isDragging) {
        _isDragging = false;
        _selectedPrimitive = nullptr;
    }

    _mouseWasPressed = mouseIsPressed;
}

void InputManager::handleObjectDragging(std::shared_ptr<Camera> camera) {
    auto currentMousePos = _eventsManager->getMousePos();
    int deltaX = currentMousePos.x - _dragStartPos.x;
    int deltaY = currentMousePos.y - _dragStartPos.y;

    if (deltaX != 0 || deltaY != 0) {
        Math::Point3D screenCenter = camera->screen.origin +
            camera->screen.bottom_side * 0.5 + camera->screen.left_side * 0.5;
        Math::Vector3D forwardDir = (screenCenter - camera->origin).normalize();
        Math::Vector3D upDir = camera->screen.left_side.normalize();
        Math::Vector3D rightDir = forwardDir.cross(upDir).normalize();

        Math::Vector3D moveVec = rightDir * (deltaX * 0.01) + upDir * (-deltaY * 0.01);
        _selectedPrimitive->translate(moveVec);
        _dragStartPos = {static_cast<int>(currentMousePos.x), static_cast<int>(currentMousePos.y)};
    }
}

void InputManager::handleObjectScrolling(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera) {
    float mouseOffset = _eventsManager->getMouseWheelDelta();

    if (static_cast<int>(mouseOffset) != 0) {
        auto currentMousePos = _eventsManager->getMousePos();
        double u = static_cast<double>(currentMousePos.x) / (_windowWidth - 1);
        double v = static_cast<double>((_windowHeight - 1) - currentMousePos.y) / (_windowHeight - 1);

        Ray ray = camera->ray(u, v);
        auto hit = scene->trace(ray);

        if (hit && hit->primitive) {
            Math::Point3D screenCenter = camera->screen.origin +
                camera->screen.bottom_side * 0.5 + camera->screen.left_side * 0.5;
            Math::Vector3D viewDir = (screenCenter - camera->origin).normalize();

            Math::Vector3D moveVec = viewDir * mouseOffset * 0.2;
            hit->primitive->translate(moveVec);
        }
    }
}

}  // namespace RayTracer
