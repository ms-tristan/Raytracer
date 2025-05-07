// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Config Parse Exception
*/

#ifndef SRC_EXCEPTION_CONFIGPARSEEXCEPTION_HPP_
    #define SRC_EXCEPTION_CONFIGPARSEEXCEPTION_HPP_

#include "Exception/BaseException.hpp"
#include <sstream>
#include <string>

namespace RayTracer {

/**
 * @brief Exception thrown when there's an error parsing a configuration
 */
class ConfigParseException : public BaseException {
 public:
    /**
     * @brief Construct a new Config Parse Exception object
     * 
     * @param configPath Path to the configuration file (if applicable)
     * @param line Line number where the error occurred (0 if unknown)
     * @param message Error message
     */
    ConfigParseException(const std::string& configPath, int line,
                        const std::string& message) noexcept
        : BaseException(createMessage(configPath, line, message)) {}

    /**
     * @brief Construct a new Config Parse Exception object
     * 
     * @param message Error message
     */
    explicit ConfigParseException(const std::string& message) noexcept
        : BaseException(message) {}

 private:
    /**
     * @brief Create a descriptive error message
     * 
     * @param configPath Configuration file path
     * @param line Line number
     * @param message Error message
     * @return std::string Formatted error message
     */
    static std::string createMessage(const std::string& configPath,
                                  int line,
                                  const std::string& message) {
        std::stringstream ss;
        ss << "Configuration parse error";
        
        if (!configPath.empty()) {
            ss << " in '" << configPath << "'";
        }
        
        if (line > 0) {
            ss << " at line " << line;
        }
        
        ss << ": " << message;
        return ss.str();
    }
};

}  // namespace RayTracer

#endif  // SRC_EXCEPTION_CONFIGPARSEEXCEPTION_HPP_
