// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Primitive Not Found Exception
*/

#ifndef SRC_EXCEPTION_PRIMITIVENOTFOUNDEXCEPTION_HPP_
    #define SRC_EXCEPTION_PRIMITIVENOTFOUNDEXCEPTION_HPP_

#include "Exception/BaseException.hpp"
#include <sstream>
#include <string>

namespace RayTracer {

/**
 * @brief Exception thrown when a primitive is not found
 */
class PrimitiveNotFoundException : public BaseException {
 public:
    /**
     * @brief Construct a new Primitive Not Found Exception object
     * 
     * @param primitiveType The type of primitive (e.g., "sphere", "cone")
     * @param primitiveId The identifier of the primitive (if available)
     */
    PrimitiveNotFoundException(const std::string& primitiveType,
                           const std::string& primitiveId = "") noexcept
        : BaseException(createMessage(primitiveType, primitiveId)) {}

 private:
    /**
     * @brief Create a descriptive error message
     * 
     * @param primitiveType The primitive type
     * @param primitiveId The primitive identifier (if available)
     * @return std::string Error message
     */
    static std::string createMessage(const std::string& primitiveType,
                                  const std::string& primitiveId) {
        std::stringstream ss;
        ss << "Primitive not found: type '" << primitiveType << "'";
        if (!primitiveId.empty()) {
            ss << ", id '" << primitiveId << "'";
        }
        return ss.str();
    }
};

}  // namespace RayTracer

#endif  // SRC_EXCEPTION_PRIMITIVENOTFOUNDEXCEPTION_HPP_
