// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Fractal Type Factory
*/

#ifndef SRC_PRIMITIVE_FRACTAL_FRACTALTYPE_FRACTALTYPEFACTORY_HPP_
    #define SRC_PRIMITIVE_FRACTAL_FRACTALTYPE_FRACTALTYPEFACTORY_HPP_
    #include <string>
    #include <memory>
    #include <unordered_map>
    #include <functional>
    #include "Primitive/Fractal/FractalType/IFractalType.hpp"
    #include "Primitive/Fractal/FractalType/Mandelbrot/MandelbrotFractal.hpp"
    #include "Primitive/Fractal/FractalType/Julia/JuliaFractal.hpp"
    #include "Primitive/Fractal/FractalType/Mandelbox/MandelboxFractal.hpp"
    #include "Primitive/Fractal/FractalType/MengerSponge/MengerSpongeFractal.hpp"
    #include "Primitive/Fractal/FractalType/Sierpinski/SierpinskiTetrahedronFractal.hpp"
    #include "Primitive/Fractal/FractalType/QuaternionJulia/QuaternionJuliaFractal.hpp"

namespace RayTracer {

class FractalTypeFactory {
 private:
    FractalTypeFactory() {
        registerType("mandelbrot", []() { return std::make_shared<MandelbrotFractal>(); });
        registerType("1.0", []() { return std::make_shared<MandelbrotFractal>(); });

        registerType("julia", []() { return std::make_shared<JuliaFractal>(); });
        registerType("2.0", []() { return std::make_shared<JuliaFractal>(); });

        registerType("mandelbox", []() { return std::make_shared<MandelboxFractal>(); });
        registerType("3.0", []() { return std::make_shared<MandelboxFractal>(); });

        registerType("menger_sponge", []() { return std::make_shared<MengerSpongeFractal>(); });
        registerType("4.0", []() { return std::make_shared<MengerSpongeFractal>(); });

        registerType("sierpinski_tetrahedron", []() { return std::make_shared<SierpinskiTetrahedronFractal>(); });
        registerType("5.0", []() { return std::make_shared<SierpinskiTetrahedronFractal>(); });

        registerType("quaternion_julia", []() { return std::make_shared<QuaternionJuliaFractal>(); });
        registerType("6.0", []() { return std::make_shared<QuaternionJuliaFractal>(); });
    }
    std::unordered_map<std::string, std::function<std::shared_ptr<IFractalType>()>> creators;
    void registerType(const std::string& name, std::function<std::shared_ptr<IFractalType>()> creator) {
        creators[name] = creator;
    }

 public:
    static FractalTypeFactory& getInstance() {
        static FractalTypeFactory instance;
        return instance;
    }
    FractalTypeFactory(const FractalTypeFactory&) = delete;
    void operator=(const FractalTypeFactory&) = delete;

    std::shared_ptr<IFractalType> createFractalType(const std::string& name) {
        auto it = creators.find(name);
        if (it != creators.end()) {
            return it->second();
        }
        return std::make_shared<MandelbrotFractal>();
    }
};

}  // namespace RayTracer

#endif  // SRC_PRIMITIVE_FRACTAL_FRACTALTYPE_FRACTALTYPEFACTORY_HPP_
