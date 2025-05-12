#ifndef SRC_PRIMITIVE_TANGLECUBE_TANGLECUBE_HPP_
    #define SRC_PRIMITIVE_TANGLECUBE_TANGLECUBE_HPP_
    
    #include <string>
    #include <memory>
    #include "Primitive/IPrimitive.hpp"
    #include "Transformation/Rotate/Rotate.hpp"
    #include <libconfig.h++>

namespace RayTracer {
class TangleCube : public IPrimitive, public std::enable_shared_from_this<TangleCube> {
 private:
    std::shared_ptr<Material> material;
    double rotationX = 0.0;
    double rotationY = 0.0;
    double rotationZ = 0.0;
    std::string sourceFile = "";

 public:
    Math::Point3D center;
    Math::Vector3D axis;
    double size;
    double spacing;

    TangleCube(const Math::Point3D &center, const Math::Vector3D &axis,
        double size, double spacing);
    TangleCube(const Math::Point3D &center, const Math::Vector3D &axis,
        double size, double spacing,
        const std::shared_ptr<Material> &material);
    ~TangleCube() override = default;

    static std::string getTypeNameStatic() {
        return "tanglecubes";
    }

    std::string getTypeName() const override {
        return TangleCube::getTypeNameStatic();
    }

    void translate(const Math::Vector3D &translation) override;
    void rotateX(double degrees) override;
    void rotateY(double degrees) override;
    void rotateZ(double degrees) override;
    std::optional<HitInfo> hit(const Ray &ray,
        double tMin, double tMax) override;
    std::shared_ptr<Material> getMaterial() const override;
    std::shared_ptr<IPrimitive> clone() const override;
    void getLibConfigParams(std::shared_ptr<libconfig::Setting> setting) const override;

    void setSourceFile(const std::string& source) {
        sourceFile = source;
    }
    
    std::string getSourceFile() const {
        return sourceFile;
    }
};
}  // namespace RayTracer

#endif  // SRC_PRIMITIVE_TANGLECUBE_TANGLECUBE_HPP_