// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Fractal Plugin implementation
*/

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <cmath>
#include "Primitive/Plugin/IPrimitivePlugin.hpp"
#include "Primitive/Fractal/Fractal.hpp"
#include "Primitive/Fractal/FractalType/FractalTypeFactory.hpp"
#include "Primitive/Fractal/FractalType/Julia/JuliaFractal.hpp"
#include "Primitive/Fractal/FractalType/QuaternionJulia/QuaternionJuliaFractal.hpp"
#include "Primitive/Fractal/FractalType/Mandelbox/MandelboxFractal.hpp"
#include "Primitive/Fractal/FractalType/Mandelbrot/MandelbrotFractal.hpp"
#include "Primitive/Fractal/FractalType/MengerSponge/MengerSpongeFractal.hpp"
#include "Primitive/Fractal/FractalType/Sierpinski/SierpinskiTetrahedronFractal.hpp"
#include "Math/Point3D/Point3D.hpp"
#include "Math/Vector3D/Vector3D.hpp"

namespace RayTracer {

// This function implements a more robust approach to determine the fractal type
std::string determineFractalType(const std::map<std::string, double>& params);

class FractalPlugin : public IPrimitivePlugin {
 public:
    ~FractalPlugin() override = default;

    std::string getTypeName() const override {
        return Fractal::getTypeNameStatic();
    }

    std::shared_ptr<IPrimitive> createPrimitive(
        const std::map<std::string, double>& params,
        const std::shared_ptr<Material>& material) override {
        auto requiredParams = getRequiredParameters();
        for (const auto& param : requiredParams) {
            if (params.find(param) == params.end()) {
                std::cerr << "Error: Missing required parameter: " << param << std::endl;
                throw std::runtime_error("Missing required parameter: " + param);
            }
        }

        Math::Coords centerCoords {
            params.at("x"),
            params.at("y"),
            params.at("z")
        };
        Math::Point3D center(centerCoords);
        double boundingRadius = params.at("boundingRadius");
        std::string fractalType = determineFractalType(params);
        std::cout << "Selected fractal type: " << fractalType << std::endl;
        int maxIterations = params.find("maxIterations") != params.end()
                            ? static_cast<int>(std::round(params.at("maxIterations"))) : 15;
        double bailout = params.find("bailout") != params.end()
                         ? params.at("bailout") : 4.0;
        std::cout << " fractal with maxIterations=" << maxIterations
                  << ", bailout=" << bailout << std::endl;
        auto fractal = std::make_shared<Fractal>(center, boundingRadius,
                                              fractalType, maxIterations,
                                              bailout, material);
        if (params.find("power") != params.end()) {
            fractal->setPower(params.at("power"));
            std::cout << "Setting power=" << params.at("power") << std::endl;
        }
        if (fractalType == "julia" &&
            params.find("julia_x") != params.end() &&
            params.find("julia_y") != params.end() &&
            params.find("julia_z") != params.end()) {
            Math::Coords juliaCoords {
                params.at("julia_x"),
                params.at("julia_y"),
                params.at("julia_z")
            };
            fractal->setJuliaConstant(Math::Point3D(juliaCoords));
            std::cout << "Setting Julia constants: ("
                      << juliaCoords.Xcoords << ", "
                      << juliaCoords.Ycoords << ", "
                      << juliaCoords.Zcoords << ")" << std::endl;
        } else if (fractalType == "julia") {
            // Set default Julia constants if not specified
            Math::Coords juliaCoords {0.3, 0.5, 0.4};
            fractal->setJuliaConstant(Math::Point3D(juliaCoords));
            std::cout << "Setting default Julia constants: (0.3, 0.5, 0.4)" << std::endl;
        }
        if ((fractalType == "quaternion_julia" || fractalType == "qjulia_flower" ||
             fractalType == "qjulia_spiral" || fractalType == "qjulia_cosmic") &&
            params.find("qjulia_x") != params.end() &&
            params.find("qjulia_y") != params.end() &&
            params.find("qjulia_z") != params.end() &&
            params.find("qjulia_w") != params.end()) {
            double qx = params.at("qjulia_x");
            double qy = params.at("qjulia_y");
            double qz = params.at("qjulia_z");
            double qw = params.at("qjulia_w");
            fractal->setQuaternionConstant(qx, qy, qz, qw);
            std::cout << "Setting Quaternion Julia constants: ("
                      << qx << ", " << qy << ", " << qz << ", " << qw << ")" << std::endl;
        } else if (fractalType == "quaternion_julia" || fractalType == "qjulia_flower" ||
                   fractalType == "qjulia_spiral" || fractalType == "qjulia_cosmic") {
            // Set default Quaternion Julia constants if not specified
            switch (fractalType[0]) {
                case 'q':  // Any quaternion julia
                    fractal->setQuaternionConstant(0.3, 0.5, 0.4, 0.2);
                    break;
                case 'f':  // flower
                    fractal->setQuaternionConstant(0.4, 0.3, 0.2, 0.1);
                    break;
                case 's':  // spiral
                    fractal->setQuaternionConstant(0.1, 0.6, 0.3, 0.2);
                    break;
                case 'c':  // cosmic
                    fractal->setQuaternionConstant(0.1, 0.2, 0.7, 0.1);
                    break;
                default:
                    fractal->setQuaternionConstant(0.3, 0.5, 0.4, 0.2);
                    break;
            }
            std::cout << "Setting default Quaternion Julia constants based on type" << std::endl;
        }
        // Set Menger Sponge parameters if they exist
        if (fractalType == "menger_sponge" && params.find("menger_scale") != params.end()) {
            fractal->setMengerScale(params.at("menger_scale"));
            std::cout << "Setting Menger scale: " << params.at("menger_scale") << std::endl;
        } else if (fractalType == "menger_sponge") {
            // Set default Menger scale if not specified
            fractal->setMengerScale(3.0);
            std::cout << "Setting default Menger scale: 3.0" << std::endl;
        }
        // Set Sierpinski parameters if they exist
        if ((fractalType == "sierpinski_tetrahedron" || fractalType == "sierpinski_pyramid") &&
            params.find("sierpinski_scale") != params.end()) {
            bool useTetrahedron = fractalType == "sierpinski_tetrahedron";
            if (params.find("sierpinski_tetrahedron") != params.end()) {
                useTetrahedron = params.at("sierpinski_tetrahedron") > 0.5;
            }
            fractal->setSierpinskiParameters(params.at("sierpinski_scale"), useTetrahedron);
            std::cout << "Setting Sierpinski scale: " << params.at("sierpinski_scale")
                      << ", tetrahedron: " << (useTetrahedron ? "true" : "false") << std::endl;
        } else if (fractalType == "sierpinski_tetrahedron" || fractalType == "sierpinski_pyramid") {
            // Set default Sierpinski parameters if not specified
            bool useTetrahedron = fractalType == "sierpinski_tetrahedron";
            fractal->setSierpinskiParameters(2.0, useTetrahedron);
            std::cout << "Setting default Sierpinski parameters: scale=2.0, tetrahedron="
                      << (useTetrahedron ? "true" : "false") << std::endl;
        }
        // Set Mandelbox parameters if they exist
        if (fractalType == "mandelbox") {
            double scale = params.find("mandelbox_scale") != params.end()
                         ? params.at("mandelbox_scale") : 2.0;
            double minRadius = params.find("mandelbox_minRadius") != params.end()
                             ? params.at("mandelbox_minRadius") : 0.25;
            double foldingLimit = params.find("mandelbox_foldingLimit") != params.end()
                                ? params.at("mandelbox_foldingLimit") : 1.0;
            fractal->setMandelboxParameters(scale, minRadius, foldingLimit);
            std::cout << "Setting Mandelbox parameters: scale=" << scale
                      << ", minRadius=" << minRadius
                      << ", foldingLimit=" << foldingLimit << std::endl;
        }
        std::cout << "Fractal creation complete." << std::endl;
        return fractal;
    }

    std::vector<std::string> getRequiredParameters() const override {
        return {"x", "y", "z", "boundingRadius"};
    }
};

// This function implements a more robust approach to determine the fractal type
std::string determineFractalType(const std::map<std::string, double>& params) {
    // Default to mandelbrot
    std::string fractalType = "mandelbrot";
    if (params.find("fractalType") != params.end()) {
        double fractalTypeValue = params.at("fractalType");
        // Check if it's NaN (which would indicate it might be a string)
        if (std::isnan(fractalTypeValue)) {
            std::cout << "FractalType is detected as a string (NaN value)" << std::endl;
            // Debug: List all parameters we have
            std::cout << "Available parameters: ";
            for (const auto& pair : params) {
                std::cout << pair.first << "=" << pair.second << " ";
            }
            std::cout << std::endl;
            // Forced detection of quaternion_julia based on qjulia parameters
            if (params.find("qjulia_x") != params.end() &&
                params.find("qjulia_y") != params.end() &&
                params.find("qjulia_z") != params.end() &&
                params.find("qjulia_w") != params.end()) {
                fractalType = "quaternion_julia";
                std::cout << "Forced detection of quaternion_julia based on parameters" << std::endl;
                return fractalType;
            }
            // Try direct string mapping
            for (const auto& pair : params) {
                // Look for special parameters that might indicate the fractal type as a string
                if (pair.first.find("fractalType_") == 0) {
                    std::string typeName = pair.first.substr(12);  // Remove "fractalType_" prefix
                    // Check for known types
                    if (typeName == "string_mandelbrot" || typeName == "mandelbrot") {
                        fractalType = "mandelbrot";
                        std::cout << "Detected string fractalType: mandelbrot" << std::endl;
                    } else if (typeName == "string_julia" || typeName == "julia") {
                        fractalType = "julia";
                        std::cout << "Detected string fractalType: julia" << std::endl;
                    } else if (typeName == "string_mandelbox" || typeName == "mandelbox") {
                        fractalType = "mandelbox";
                        std::cout << "Detected string fractalType: mandelbox" << std::endl;
                    } else if (typeName == "string_menger_sponge" || typeName == "menger_sponge") {
                        fractalType = "menger_sponge";
                        std::cout << "Detected string fractalType: menger_sponge" << std::endl;
                    } else if (typeName == "string_sierpinski_tetrahedron" || typeName == "sierpinski_tetrahedron") {
                        fractalType = "sierpinski_tetrahedron";
                        std::cout << "Detected string fractalType: sierpinski_tetrahedron" << std::endl;
                    } else if (typeName == "string_quaternion_julia" || typeName == "quaternion_julia") {
                        fractalType = "quaternion_julia";
                        std::cout << "Detected string fractalType: quaternion_julia" << std::endl;
                    } else {
                        std::cout << "Unknown fractal type string: " << typeName << std::endl;
                    }
                    // Once we found a type, no need to check further
                    if (fractalType != "mandelbrot") {
                        return fractalType;
                    }
                }
            }
        } else {
            // It's a numeric value, use the original approach
            int typeId = static_cast<int>(std::round(fractalTypeValue));
            // Map numeric IDs to type names
            switch (typeId) {
                case 1: return "mandelbrot";
                case 2: return "julia";
                case 3: return "mandelbox";
                case 4: return "menger_sponge";
                case 5: return "sierpinski_tetrahedron";
                case 6: return "sierpinski_pyramid";
                case 7: return "quaternion_julia";
                case 8: return "qjulia_flower";
                case 9: return "qjulia_spiral";
                case 10: return "qjulia_cosmic";
                default:
                    std::cerr << "Unknown fractal type ID: " << typeId << ". Using default type." << std::endl;
                    break;  // Will use default "mandelbrot"
            }
        }
    }
    // Check for type-specific parameters to infer the fractal type as a fallback
    if (params.find("julia_x") != params.end() && params.find("julia_y") != params.end()) {
        fractalType = "julia";
        std::cout << "Detected Julia type from julia_x and julia_y parameters" << std::endl;
    } else if (params.find("mandelbox_scale") != params.end() || params.find("mandelbox_minRadius") != params.end()) {
        fractalType = "mandelbox";
        std::cout << "Detected Mandelbox type from mandelbox_* parameters" << std::endl;
    } else if (params.find("menger_scale") != params.end()) {
        fractalType = "menger_sponge";
        std::cout << "Detected Menger Sponge type from menger_scale parameter" << std::endl;
    } else if (params.find("sierpinski_scale") != params.end()) {
        fractalType = "sierpinski_tetrahedron";
        std::cout << "Detected Sierpinski type from sierpinski_scale parameter" << std::endl;
    } else if (params.find("qjulia_x") != params.end() || params.find("qjulia_w") != params.end()) {
        fractalType = "quaternion_julia";
        std::cout << "Detected Quaternion Julia type from qjulia_* parameters" << std::endl;
    }
    return fractalType;
}

extern "C" {
    IPrimitivePlugin* createPrimitivePlugin() {
        return new FractalPlugin();
    }
}

}  // namespace RayTracer
