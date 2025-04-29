// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** SceneFileWatcher implementation
*/
#include <string>
#include <algorithm>
#include <iostream>
#include "SceneFileWatcher.hpp"

namespace RayTracer {

SceneFileWatcher::SceneFileWatcher(const std::string& filepath)
: filepath(filepath) {
    lastModified = getFileModificationTime();
}

void SceneFileWatcher::attach(std::shared_ptr<IObserver> observer) {
    observers.push_back(observer);
}

void SceneFileWatcher::detach(std::shared_ptr<IObserver> observer) {
    observers.erase(
        std::remove(observers.begin(), observers.end(), observer),
        observers.end());
}

void SceneFileWatcher::notify() const {
    for (const auto& observer : observers) {
        observer->update();
    }
}

bool SceneFileWatcher::checkForChanges() {
    time_t currentModTime = getFileModificationTime();
    if (currentModTime != lastModified) {
        lastModified = currentModTime;
        notify();
        return true;
    }
    return false;
}

time_t SceneFileWatcher::getFileModificationTime() const {
    struct stat fileInfo;
    if (stat(filepath.c_str(), &fileInfo) != 0) {
        std::cerr << "Error: Unable to access file: " << filepath << std::endl;
        return 0;
    }
    return fileInfo.st_mtime;
}

std::string SceneFileWatcher::getFilePath() const {
    return filepath;
}

}  // namespace RayTracer
