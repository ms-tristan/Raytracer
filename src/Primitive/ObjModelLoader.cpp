// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ObjModelLoader - loads .obj files and creates triangle primitives
*/
#include <vector>
#include <memory>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Math/Point3D/Point3D.hpp"
#include "Primitive/Plugin/PrimitivePluginManager.hpp"
#include "Primitive/CompositePrimitive/CompositePrimitive.hpp"
#include "ObjModelLoader.hpp"

namespace RayTracer {

std::shared_ptr<IPrimitive>
ObjModelLoader::loadObjModel(const std::string& path, const std::shared_ptr<Material>& material) {
    auto composite = std::make_shared<CompositePrimitive>(material);
    std::vector<Math::Point3D> vertices;
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Failed to open OBJ file: " << path << std::endl;
        return composite;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;
        if (prefix == "v") {
            double x, y, z;
            iss >> x >> y >> z;
            vertices.emplace_back(Math::Coords{x, y, z});
        } else if (prefix == "f") {
            std::vector<int> indices;
            std::string token;
            while (iss >> token) {
                std::istringstream tokenStream(token);
                std::string idxStr;
                std::getline(tokenStream, idxStr, '/');
                int idx = std::stoi(idxStr);
                // Handle negative indices (relative to end of list)
                if (idx < 0) idx = static_cast<int>(vertices.size()) + idx + 1;
                indices.push_back(idx - 1); // OBJ is 1-based
            }
            // Fan triangulation for faces with >3 vertices
            for (size_t i = 1; i + 1 < indices.size(); ++i) {
                int idx0 = indices[0];
                int idx1 = indices[i];
                int idx2 = indices[i + 1];
                if (idx0 >= 0 && idx1 >= 0 && idx2 >= 0 &&
                    idx0 < (int)vertices.size() && idx1 < (int)vertices.size() && idx2 < (int)vertices.size()) {
                    std::map<std::string, double> params = {
                        {"v1x", vertices[idx0].X},
                        {"v1y", vertices[idx0].Y},
                        {"v1z", vertices[idx0].Z},
                        {"v2x", vertices[idx1].X},
                        {"v2y", vertices[idx1].Y},
                        {"v2z", vertices[idx1].Z},
                        {"v3x", vertices[idx2].X},
                        {"v3y", vertices[idx2].Y},
                        {"v3z", vertices[idx2].Z}
                    };
                    auto pluginManager = PrimitivePluginManager::getInstance();
                    try {
                        if (!pluginManager->getPlugin("triangles"))
                            pluginManager->loadAllPlugins("plugins/primitives");
                    } catch (...) {
                        pluginManager->loadAllPlugins("plugins/primitives");
                    }
                    auto tri = pluginManager->createPrimitive("triangles", params, material);
                    composite->add(tri);
                }
            }
        }
    }
    return composite;
}

}  // namespace RayTracer
