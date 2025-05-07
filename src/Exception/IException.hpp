// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Exception Interface
*/

#ifndef SRC_EXCEPTION_IEXCEPTION_HPP_
    #define SRC_EXCEPTION_IEXCEPTION_HPP_

#include <string>
#include <exception>

namespace RayTracer {

/**
 * @brief Interface for all exceptions in the Raytracer project
 */
class IException : public std::exception {
 public:
    /**
     * @brief Get the error message
     *
     * @return const char* Error message
     */
    virtual const char* what() const noexcept = 0;

    /**
     * @brief Get the error code
     *
     * @return int Error code (default: 84)
     */
    virtual int getErrorCode() const noexcept = 0;

    /**
     * @brief Virtual destructor
     */
    virtual ~IException() noexcept = default;
};

}  // namespace RayTracer

#endif  // SRC_EXCEPTION_IEXCEPTION_HPP_
