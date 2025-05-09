// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Value Range Exception
*/

#ifndef SRC_EXCEPTION_VALUERANGEEXCEPTION_HPP_
    #define SRC_EXCEPTION_VALUERANGEEXCEPTION_HPP_
    #include <sstream>
    #include <string>
    #include "Exception/BaseException.hpp"


namespace RayTracer {

/**
 * @brief Exception thrown when a value is out of the valid range
 */
class ValueRangeException : public BaseException {
 public:
    /**
     * @brief Construct a new Value Range Exception object
     *
     * @param paramName The name of the parameter that is out of range
     * @param value The invalid value
     * @param minValue The minimum valid value
     * @param maxValue The maximum valid value
     */
    ValueRangeException(const std::string& paramName, double value,
                        double minValue, double maxValue) noexcept
        : BaseException(createMessage(paramName, value, minValue, maxValue)) {}

 private:
    /**
     * @brief Create a descriptive error message
     *
     * @param paramName Parameter name
     * @param value Invalid value
     * @param minValue Minimum valid value
     * @param maxValue Maximum valid value
     * @return std::string Error message
     */
    static std::string createMessage(const std::string& paramName, double value,
                                    double minValue, double maxValue) {
        std::stringstream ss;
        ss << "Value out of range: " << paramName << " = " << value
           << " (valid range: [" << minValue << ", " << maxValue << "])";
        return ss.str();
    }
};

}  // namespace RayTracer

#endif  // SRC_EXCEPTION_VALUERANGEEXCEPTION_HPP_
