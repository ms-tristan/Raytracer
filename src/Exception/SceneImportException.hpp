// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Scene Import Exception
*/

#ifndef SRC_EXCEPTION_SCENEIMPORTEXCEPTION_HPP_
    #define SRC_EXCEPTION_SCENEIMPORTEXCEPTION_HPP_

#include "Exception/BaseException.hpp"
#include <sstream>
#include <string>

namespace RayTracer {

/**
 * @brief Exception thrown when there's an error importing a scene
 */
class SceneImportException : public BaseException {
 public:
    /**
     * @brief Construct a new Scene Import Exception object
     * 
     * @param scenePath Path to the scene file that couldn't be imported
     * @param reason The reason for the import failure
     */
    SceneImportException(const std::string& scenePath,
                         const std::string& reason) noexcept
        : BaseException(createMessage(scenePath, reason)) {}

 private:
    /**
     * @brief Create a descriptive error message
     * 
     * @param scenePath Scene file path
     * @param reason Reason for import failure
     * @return std::string Error message
     */
    static std::string createMessage(const std::string& scenePath,
                                  const std::string& reason) {
        std::stringstream ss;
        ss << "Failed to import scene '" << scenePath << "': " << reason;
        return ss.str();
    }
};

}  // namespace RayTracer

#endif  // SRC_EXCEPTION_SCENEIMPORTEXCEPTION_HPP_
