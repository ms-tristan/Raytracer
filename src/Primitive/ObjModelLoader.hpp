// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ObjModelLoader - loads .obj files and creates triangle primitives
*/

#ifndef SRC_PRIMITIVE_OBJMODELLOADER_HPP_
    #define SRC_PRIMITIVE_OBJMODELLOADER_HPP_
    #include <string>
    #include <vector>
    #include <memory>
    #include "Primitive/IPrimitive.hpp"
    #include "Material/Material.hpp"

namespace RayTracer {

class ObjModelLoader {
public:
    static std::shared_ptr<IPrimitive>
    loadObjModel(const std::string& path, const std::shared_ptr<Material>& material);
};

}  // namespace RayTracer

#endif  // SRC_PRIMITIVE_OBJMODELLOADER_HPP_
