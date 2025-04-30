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
   InputManager(std::shared_ptr<IEventsManager> eventsManager, int windowWidth, int windowHeight);
   ~InputManager() = default;

   void processInput(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera);

 private:
   void handleCameraMovement(std::shared_ptr<Camera> camera);
   void handleObjectSelection(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera);
   void handleObjectDragging(std::shared_ptr<Camera> camera);
   void handleObjectScrolling(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera);

   std::shared_ptr<IEventsManager> _eventsManager;
   int _windowWidth;
   int _windowHeight;

   double _moveSpeed;
   double _rotateSpeed;
   double _mouseRotateSensitivity;

   bool _mouseWasPressed;
   bool _rightMouseWasPressed;
   sf::Vector2i _lastMousePos;

   bool _isDragging;
   std::shared_ptr<IPrimitive> _selectedPrimitive;
   sf::Vector2i _dragStartPos;
};

}  // namespace RayTracer

#endif  // SRC_EVENTSMANAGER_INPUTMANAGER_INPUTMANAGER_HPP_
