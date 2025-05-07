// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Base Exception class
*/

#ifndef SRC_EXCEPTION_BASEEXCEPTION_HPP_
    #define SRC_EXCEPTION_BASEEXCEPTION_HPP_
    #include <string>
    #include "Exception/IException.hpp"

namespace RayTracer {

/**
 * @brief Base class for all exceptions in the Raytracer project
 */
class BaseException : public IException {
 private:
    std::string _message;
    int _errorCode;

 public:
    /**
     * @brief Construct a new Base Exception object
     *
     * @param message Error message
     * @param errorCode Error code (default: 84)
     */
    explicit BaseException(const std::string& message, int errorCode = 84) noexcept
        : _message(message), _errorCode(errorCode) {}

    /**
     * @brief Construct a new Base Exception object
     *
     * @param message Error message
     * @param errorCode Error code (default: 84)
     */
    explicit BaseException(const char* message, int errorCode = 84) noexcept
        : _message(message), _errorCode(errorCode) {}

    /**
     * @brief Get the error message
     *
     * @return const char* Error message
     */
    const char* what() const noexcept override {
        return _message.c_str();
    }

    /**
     * @brief Get the error code
     *
     * @return int Error code
     */
    int getErrorCode() const noexcept override {
        return _errorCode;
    }

    /**
     * @brief Virtual destructor
     */
    ~BaseException() noexcept override = default;
};

}  // namespace RayTracer

#endif  // SRC_EXCEPTION_BASEEXCEPTION_HPP_
