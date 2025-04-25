/*
** EPITECH PROJECT, 2025
** RayTracer
** File description:
** IEventsManager
*/

#ifndef SOURCES_SHARED_CLASSES_INTERFACES_IEVENTSMANAGER_HPP_
#define SOURCES_SHARED_CLASSES_INTERFACES_IEVENTSMANAGER_HPP_

#include "../defs.hpp"

#include <vector>
#include <string>

namespace RayTracer {

class IEventsManager {
 public:
  virtual ~IEventsManager() = default;
  virtual bool isKeyPressed(std::string key) const = 0;
  virtual vector2f_t getMousePos() = 0;
};
}  // namespace RayTracer

#endif  // SOURCES_SHARED_CLASSES_INTERFACES_IEVENTSMANAGER_HPP_
