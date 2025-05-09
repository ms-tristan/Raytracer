// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** File IO Exception
*/

#ifndef SRC_EXCEPTION_FILEIOEXCEPTION_HPP_
    #define SRC_EXCEPTION_FILEIOEXCEPTION_HPP_
    #include <sstream>
    #include <string>
    #include "Exception/BaseException.hpp"


namespace RayTracer {

/**
 * @brief Exception thrown when there's an error with file I/O operations
 */
class FileIOException : public BaseException {
 public:
    /**
     * @brief Construct a new File IO Exception object
     *
     * @param filePath Path to the file that caused the error
     * @param operation The operation that failed (e.g., "read", "write", "open")
     * @param reason Additional information about the failure
     */
    FileIOException(const std::string& filePath, const std::string& operation,
                   const std::string& reason = "") noexcept
        : BaseException(createMessage(filePath, operation, reason)) {}

 private:
    /**
     * @brief Create a descriptive error message
     *
     * @param filePath File path
     * @param operation Operation that failed
     * @param reason Additional information
     * @return std::string Error message
     */
    static std::string createMessage(const std::string& filePath,
                                  const std::string& operation,
                                  const std::string& reason) {
        std::stringstream ss;
        ss << "File " << operation << " error for '" << filePath << "'";
        if (!reason.empty())
            ss << ": " << reason;
        return ss.str();
    }
};

}  // namespace RayTracer

#endif  // SRC_EXCEPTION_FILEIOEXCEPTION_HPP_
