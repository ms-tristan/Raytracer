// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Main with demonstration of design patterns
*/

#include <iostream>
#include <memory>
#include <algorithm>
#include <string>
#include <thread>
#include <vector>
#include <mutex>

#include <SFML/Graphics.hpp>

// Display Manager
#include "./Renderer/DisplayManager/SFMLDisplayManager.hpp"
#include "./Renderer/Renderer.hpp"

// Events Manager
#include "./EventsManager/SFMLEventsManager.hpp"

// Scene components
#include "Scene/Scene.hpp"
#include "Scene/SceneBuilder/SceneBuilder.hpp"
#include "Scene/SceneDirector/SceneDirector.hpp"
#include "Camera/Camera.hpp"
#include "Rectangle3D/Rectangle3D.hpp"

// Primitives
#include "Primitive/Sphere/Sphere.hpp"
#include "Primitive/Cylinder/Cylinder.hpp"
#include "Primitive/Cone/Cone.hpp"
#include "Primitive/Plane/Plane.hpp"
#include "Primitive/CompositePrimitive/CompositePrimitive.hpp"

// Lights and Materials
#include "Material/Material.hpp"
#include "Light/DirectionalLight/DirectionalLight.hpp"
#include "Light/AmbientLight/AmbientLight.hpp"
#include "Light/PointLight/PointLight.hpp"

// Math utilities
#include "Math/Vector3D/Vector3D.hpp"
#include "Math/Point3D/Point3D.hpp"
#include "Ray/Ray.hpp"


int main(int argc, char **argv) {
    const int image_width = 800;
    const int image_height = 600;

    RayTracer::SceneDirector director;
    std::unique_ptr<RayTracer::Scene> scene;
    std::string sceneFile = "scenes/default_scene.cfg";

    if (argc > 1) {
        sceneFile = argv[1];
        std::cout << "Loading scene from file: " << sceneFile << std::endl;
        scene = director.createSceneFromFile(sceneFile);
    } else {
        std::cout << "No scene file specified, using default scene." << std::endl;
        scene = director.createDefaultScene();
    }

    RayTracer::Camera camera = scene->getCamera();

    RayTracer::SFMLDisplayManager displayManager;

    RayTracer::SFMLEventsManager eventsManager(displayManager.getWindow());

    displayManager.initialize(image_width, image_height, "Raytracer", false);

    RayTracer::Renderer renderer(displayManager);

    bool mouseWasPressed = false;
    bool rightMouseWasPressed = false;
    sf::Vector2i lastMousePos;

    bool isDragging = false;
    const RayTracer::IPrimitive* selectedPrimitive = nullptr;
    sf::Vector2i dragStartPos;
    Math::Point3D objectOriginalPos;

    while (displayManager.isWindowOpen()) {

        renderer.drawScene(*scene, camera);
        eventsManager.processEvents();

        Math::Point3D screenCenter = camera.screen.origin +
            camera.screen.bottom_side * 0.5 + camera.screen.left_side * 0.5;
        Math::Vector3D forwardDir = (screenCenter - camera.origin).normalize();

        Math::Vector3D upDir = camera.screen.left_side.normalize();
        Math::Vector3D rightDir = forwardDir.cross(upDir).normalize();

        const double moveSpeed = 0.4;
        const double rotateSpeed = 4.0;
        const double mouseRotateSensitivity = 0.3;

        bool mouseIsPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
        auto currentMousePos = eventsManager.getMousePos();

        if (mouseIsPressed && !mouseWasPressed) {
            double u = static_cast<double>(currentMousePos.x) / (image_width - 1);
            double v = static_cast<double>((image_height - 1) - currentMousePos.y) / (image_height - 1);

            RayTracer::Ray ray = camera.ray(u, v);
            auto hit = scene->trace(ray);

            if (hit && hit->primitive) {
                isDragging = true;
                selectedPrimitive = hit->primitive;
                dragStartPos = {static_cast<int>(currentMousePos.x), static_cast<int>(currentMousePos.y)};
            }
        }
        else if (mouseIsPressed && isDragging && selectedPrimitive) {
            int deltaX = currentMousePos.x - dragStartPos.x;
            int deltaY = currentMousePos.y - dragStartPos.y;

            if (deltaX != 0 || deltaY != 0) {
                Math::Vector3D moveVec = rightDir * (deltaX * 0.01) + upDir * (-deltaY * 0.01);
                const_cast<RayTracer::IPrimitive*>(selectedPrimitive)->translate(moveVec);
                dragStartPos = {static_cast<int>(currentMousePos.x), static_cast<int>(currentMousePos.y)};
            }
        }
        else if (!mouseIsPressed && isDragging) {
            isDragging = false;
            selectedPrimitive = nullptr;
        }

        mouseWasPressed = mouseIsPressed;

        bool rightMouseIsPressed = sf::Mouse::isButtonPressed(sf::Mouse::Right);
        if (rightMouseIsPressed) {
            sf::Vector2i currentPos = {static_cast<int>(currentMousePos.x), static_cast<int>(currentMousePos.y)};

            if (rightMouseWasPressed) {

                int deltaX = currentPos.x - lastMousePos.x;
                int deltaY = currentPos.y - lastMousePos.y;

                if (deltaX != 0) {
                    camera.rotateY(-deltaX * mouseRotateSensitivity);
                }
                if (deltaY != 0) {
                    camera.rotateX(-deltaY * mouseRotateSensitivity);
                }
            }
            lastMousePos = currentPos;
        }
        rightMouseWasPressed = rightMouseIsPressed;

        if (eventsManager.isKeyPressed("Z")) {
            camera.translate(forwardDir * moveSpeed);
        }
        if (eventsManager.isKeyPressed("S")) {
            camera.translate(forwardDir * -moveSpeed);
        }
        if (eventsManager.isKeyPressed("Q")) {
            camera.translate(rightDir * -moveSpeed);
        }
        if (eventsManager.isKeyPressed("D")) {
            camera.translate(rightDir * moveSpeed);
        }

        if (eventsManager.isKeyPressed("SPACE")) {
            camera.translate(upDir * moveSpeed);
        }
        if (eventsManager.isKeyPressed("LCONTROL")) {
            camera.translate(upDir * -moveSpeed);
        }

        if (eventsManager.isKeyPressed("LEFT")) {
            camera.rotateY(rotateSpeed);
        }
        if (eventsManager.isKeyPressed("RIGHT")) {
            camera.rotateY(-rotateSpeed);
        }
        if (eventsManager.isKeyPressed("UP")) {
            camera.rotateX(rotateSpeed);
        }
        if (eventsManager.isKeyPressed("DOWN")) {
            camera.rotateX(-rotateSpeed);
        }
        if (eventsManager.isKeyPressed("ESCAPE")) {
            displayManager.closeWindow();
        }
    }

    scene->setCamera(camera);

    if (!director.saveSceneToFile(*scene, sceneFile)) {
        std::cerr << "Failed to save the scene to " << sceneFile << std::endl;
    } else {
        std::cout << "Scene saved successfully to " << sceneFile << std::endl;
    }

    return 0;
}
