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
        
        std::string fractalType = "mandelbrot";
        if (params.find("fractalType") != params.end()) {
            try {
                
                double fractalTypeValue = params.at("fractalType");
                int typeId = static_cast<int>(std::round(fractalTypeValue));
                
                std::cout << "Processing fractal with center ("
                          << center.X << ", " << center.Y << ", " << center.Z
                          << "), radius " << boundingRadius << std::endl;
                std::cout << "Processing fractalType value: " << fractalTypeValue
                          << " (rounded to: " << typeId << ")" << std::endl;
                
                switch (typeId) {
                    case 1: fractalType = "mandelbrot"; break;
                    case 2: fractalType = "julia"; break;
                    case 3: fractalType = "mandelbox"; break;
                    case 4: fractalType = "menger_sponge"; break;
                    case 5: fractalType = "sierpinski_tetrahedron"; break;
                    case 6: fractalType = "sierpinski_pyramid"; break;
                    case 7: fractalType = "quaternion_julia"; break;
                    case 8: fractalType = "qjulia_flower"; break;
                    case 9: fractalType = "qjulia_spiral"; break;
                    case 10: fractalType = "qjulia_cosmic"; break;
                    default: 
                        std::cerr << "Warning: Unknown fractal type ID: " << typeId
                                  << ". Using default type 'mandelbrot'." << std::endl;
                        fractalType = "mandelbrot";
                }
                
                std::cout << "Selected fractal type: " << fractalType << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "Error processing fractalType parameter: " << e.what() << std::endl;
                std::cerr << "Using default type 'mandelbrot'." << std::endl;
            }
        } else {
            std::cout << "No fractalType parameter found, using default: mandelbrot" << std::endl;
        }
        
        // Get iteration parameters with defaults
        int maxIterations = params.find("maxIterations") != params.end()
                            ? static_cast<int>(std::round(params.at("maxIterations"))) : 15;
        double bailout = params.find("bailout") != params.end()
                         ? params.at("bailout") : 4.0;
        
        std::cout << "Creating fractal with maxIterations=" << maxIterations
                  << ", bailout=" << bailout << std::endl;
        
        // Create the fractal primitive
        auto fractal = std::make_shared<Fractal>(center, boundingRadius,
                                              fractalType, maxIterations,
                                              bailout, material);
        
        // Set common parameters if they exist
        if (params.find("power") != params.end()) {
            fractal->setPower(params.at("power"));
            std::cout << "Setting power=" << params.at("power") << std::endl;
        }
        
        // Julia coordinate access fix
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
        
        // Set Quaternion Julia parameters if they exist
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
                case 'q': // Any quaternion julia
                    fractal->setQuaternionConstant(0.3, 0.5, 0.4, 0.2);
                    break;
                case 'f': // flower
                    fractal->setQuaternionConstant(0.4, 0.3, 0.2, 0.1);
                    break;
                case 's': // spiral
                    fractal->setQuaternionConstant(0.1, 0.6, 0.3, 0.2);
                    break;
                case 'c': // cosmic
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

extern "C" {
    IPrimitivePlugin* createPrimitivePlugin() {
        std::cout << "Creating fractal primitive plugin" << std::endl;
        return new FractalPlugin();
    }
}

}  // namespace RayTracer