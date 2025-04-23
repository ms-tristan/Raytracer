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
    #include <thread>
    #include <atomic>
    #include <chrono>
    #include "IObserver/IObserver.hpp"


namespace RayTracer {
class SceneFileWatcher {
 private:
    std::string filePath;
    std::vector<IObserver*> observers;
    time_t lastModified;
    std::thread watchThread;
    std::atomic<bool> running;

    void watchLoop();

 public:
    explicit SceneFileWatcher(const std::string& path);
    ~SceneFileWatcher();

    void attach(IObserver* observer);
    void detach(IObserver* observer);
    void notify();

    bool checkForChanges();
    void startWatching();
    void stopWatching();
};
}  // namespace RayTracer

#endif  // SRC_IOBSERVER_SCENEFILEWATCHER_SCENEFILEWATCHER_HPP_
