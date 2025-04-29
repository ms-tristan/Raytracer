// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** InputManager - handles input for camera and scene object interactions
*/

#ifndef SRC_EVENTSMANAGER_INPUTMANAGER_INPUTMANAGER_HPP_
#define SRC_EVENTSMANAGER_INPUTMANAGER_INPUTMANAGER_HPP_

#include <memory>
#include <SFML/Graphics.hpp>
#include "../IEventsManager.hpp"
#include "../../Scene/Scene.hpp"
#include "../../Camera/Camera.hpp"
#include "../../Primitive/IPrimitive.hpp"
#include "../../Math/Vector3D/Vector3D.hpp"
#include "../../Math/Point3D/Point3D.hpp"

namespace RayTracer {

class InputManager {
 public:
    InputManager(IEventsManager& eventsManager, int windowWidth, int windowHeight);
    ~InputManager() = default;

    void processInput(Scene& scene, Camera& camera);

 private:
    void handleCameraMovement(Camera& camera);
    void handleObjectSelection(Scene& scene, Camera& camera);
    void handleObjectDragging(Camera& camera);

    IEventsManager& _eventsManager;
    int _windowWidth;
    int _windowHeight;

    double _moveSpeed;
    double _rotateSpeed;
    double _mouseRotateSensitivity;

    bool _mouseWasPressed;
    bool _rightMouseWasPressed;
    sf::Vector2i _lastMousePos;

    bool _isDragging;
    const IPrimitive* _selectedPrimitive;
    sf::Vector2i _dragStartPos;
};

}  // namespace RayTracer

#endif  // SRC_EVENTSMANAGER_INPUTMANAGER_INPUTMANAGER_HPP_