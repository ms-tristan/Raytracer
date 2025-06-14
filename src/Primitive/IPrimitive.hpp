// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** IPrimitive
*/

#ifndef SRC_PRIMITIVE_IPRIMITIVE_HPP_
  #define SRC_PRIMITIVE_IPRIMITIVE_HPP_
  #include <string>
  #include <memory>
  #include <optional>
  #include <libconfig.h++>
  #include "defs.hpp"
  #include "Material/Material.hpp"
  #include "Math/Vector3D/Vector3D.hpp"
  #include "Math/Point3D/Point3D.hpp"
  #include "Ray/Ray.hpp"
namespace RayTracer {
class IPrimitive {
 public:
    virtual ~IPrimitive() = default;

    virtual std::string getTypeName() const = 0;

    virtual void setSourceFile(const std::string& source) = 0;
    virtual std::string getSourceFile() const = 0;

    virtual void translate(const Math::Vector3D &translation) = 0;
    virtual void rotateX(double degrees) = 0;
    virtual void rotateY(double degrees) = 0;
    virtual void rotateZ(double degrees) = 0;
    virtual std::optional<HitInfo> hit(const Ray &ray,
      double tMin, double tMax) = 0;
    virtual std::shared_ptr<Material> getMaterial() const = 0;
    virtual std::shared_ptr<IPrimitive> clone() const = 0;
    virtual void getLibConfigParams(std::shared_ptr<libconfig::Setting> setting) const = 0;

    virtual Math::Point3D getPosition() const = 0;
};
}  // namespace RayTracer

#endif  // SRC_PRIMITIVE_IPRIMITIVE_HPP_
