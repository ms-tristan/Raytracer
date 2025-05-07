// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Invalid Operation Exception
*/

#ifndef SRC_EXCEPTION_INVALIDOPERATIONEXCEPTION_HPP_
    #define SRC_EXCEPTION_INVALIDOPERATIONEXCEPTION_HPP_

#include "Exception/BaseException.hpp"
#include <string>

namespace RayTracer {

/**
 * @brief Exception thrown when an invalid operation is attempted
 */
class InvalidOperationException : public BaseException {
 public:
    /**
     * @brief Construct a new Invalid Operation Exception object
     * 
     * @param operation The operation that was attempted
     * @param reason Why the operation is invalid
     */
    InvalidOperationException(const std::string& operation,
                             const std::string& reason) noexcept
        : BaseException("Invalid operation '" + operation + "': " + reason) {}

    /**
     * @brief Construct a new Invalid Operation Exception object
     * 
     * @param message Error message
     */
    explicit InvalidOperationException(const std::string& message) noexcept
        : BaseException(message) {}
};

}  // namespace RayTracer

#endif  // SRC_EXCEPTION_INVALIDOPERATIONEXCEPTION_HPP_
