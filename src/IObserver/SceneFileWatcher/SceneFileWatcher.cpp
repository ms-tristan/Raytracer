// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** SceneFileWatcher
*/
#include <algorithm>
#include <iostream>
#include <string>
#include "SceneFileWatcher.hpp"

namespace RayTracer {

SceneFileWatcher::SceneFileWatcher(const std::string& path)
: filePath(path), running(false) {
    struct stat fileInfo;
    if (stat(filePath.c_str(), &fileInfo) != 0) {
        std::cerr << "Warning: Could not stat file " << filePath << std::endl;
        lastModified = 0;
    } else {
        lastModified = fileInfo.st_mtime;
    }
}

SceneFileWatcher::~SceneFileWatcher() {
    stopWatching();
}

void SceneFileWatcher::attach(IObserver* observer) {
    observers.push_back(observer);
}

void SceneFileWatcher::detach(IObserver* observer) {
    observers.erase(
        std::remove(observers.begin(), observers.end(), observer),
        observers.end());
}

void SceneFileWatcher::notify() {
    for (auto observer : observers) {
        observer->update();
    }
}

bool SceneFileWatcher::checkForChanges() {
    struct stat fileInfo;
    if (stat(filePath.c_str(), &fileInfo) != 0) {
        return false;
    }

    if (fileInfo.st_mtime > lastModified) {
        lastModified = fileInfo.st_mtime;
        return true;
    }

    return false;
}

void SceneFileWatcher::watchLoop() {
    while (running) {
        if (checkForChanges()) {
            notify();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void SceneFileWatcher::startWatching() {
    if (!running) {
        running = true;
        watchThread = std::thread(&SceneFileWatcher::watchLoop, this);
    }
}

void SceneFileWatcher::stopWatching() {
    if (running) {
        running = false;
        if (watchThread.joinable()) {
            watchThread.join();
        }
    }
}

}  // namespace RayTracer
