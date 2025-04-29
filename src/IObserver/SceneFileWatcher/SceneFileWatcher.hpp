// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** SceneFileWatcher
*/

#ifndef SRC_IOBSERVER_SCENEFILEWATCHER_SCENEFILEWATCHER_HPP_
#define SRC_IOBSERVER_SCENEFILEWATCHER_SCENEFILEWATCHER_HPP_
#include <sys/stat.h>
#include <string>
#include <vector>
#include <memory>
#include "IObserver/IObserver.hpp"

namespace RayTracer {
class SceneFileWatcher {
 private:
    std::string filepath;
    std::vector<std::shared_ptr<IObserver>> observers;
    time_t lastModified;

    time_t getFileModificationTime() const;

 public:
    explicit SceneFileWatcher(const std::string& filepath);

    void attach(std::shared_ptr<IObserver> observer);
    void detach(std::shared_ptr<IObserver> observer);
    void notify() const;

    bool checkForChanges();
    std::string getFilePath() const;
};
}  // namespace RayTracer

#endif  // SRC_IOBSERVER_SCENEFILEWATCHER_SCENEFILEWATCHER_HPP_
