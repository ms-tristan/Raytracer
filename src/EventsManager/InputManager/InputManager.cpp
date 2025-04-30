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

void InputManager::handleCameraMovement(std::shared_ptr<Camera> camera) {
    Math::Point3D screenCenter = camera->screen.origin +
        camera->screen.bottom_side * 0.5 + camera->screen.left_side * 0.5;
    Math::Vector3D forwardDir = (screenCenter - camera->origin).normalize();
    Math::Vector3D upDir = camera->screen.left_side.normalize();
    Math::Vector3D rightDir = forwardDir.cross(upDir).normalize();

    if (_eventsManager->isKeyPressed("Z")) {
        camera->translate(forwardDir * _moveSpeed);
    }
    if (_eventsManager->isKeyPressed("S")) {
        camera->translate(forwardDir * -_moveSpeed);
    }
    if (_eventsManager->isKeyPressed("Q")) {
        camera->translate(rightDir * -_moveSpeed);
    }
    if (_eventsManager->isKeyPressed("D")) {
        camera->translate(rightDir * _moveSpeed);
    }

    // if (_eventsManager->isKeyPressed("SPACE")) {
    //     camera->translate(upDir * _moveSpeed);
    // }
    // if (_eventsManager->isKeyPressed("LCONTROL")) {
    //     camera->translate(upDir * -_moveSpeed);
    // }

    if (_eventsManager->isKeyPressed("LEFT")) {
        camera->rotateY(_rotateSpeed);
    }
    if (_eventsManager->isKeyPressed("RIGHT")) {
        camera->rotateY(-_rotateSpeed);
    }
    if (_eventsManager->isKeyPressed("UP")) {
        camera->rotateX(_rotateSpeed);
    }
    if (_eventsManager->isKeyPressed("DOWN")) {
        camera->rotateX(-_rotateSpeed);
    }

    bool rightMouseIsPressed = sf::Mouse::isButtonPressed(sf::Mouse::Right);
    if (rightMouseIsPressed) {
        auto currentMousePos = _eventsManager->getMousePos();
        sf::Vector2i currentPos = {static_cast<int>(currentMousePos.x), static_cast<int>(currentMousePos.y)};

        if (_rightMouseWasPressed) {
            int deltaX = currentPos.x - _lastMousePos.x;
            int deltaY = currentPos.y - _lastMousePos.y;

            if (deltaX != 0) {
                camera->rotateY(-deltaX * _mouseRotateSensitivity);
            }
            if (deltaY != 0) {
                camera->rotateX(-deltaY * _mouseRotateSensitivity);
            }
        }
        _lastMousePos = currentPos;
    }
    _rightMouseWasPressed = rightMouseIsPressed;
}

void InputManager::handleObjectSelection(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera) {
    auto currentMousePos = _eventsManager->getMousePos();
    bool mouseIsPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);

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
        sf::Vector2i currentPos = {static_cast<int>(currentMousePos.x), static_cast<int>(currentMousePos.y)};

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
