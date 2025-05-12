#include "Primitive/TangleCube/TangleCube.hpp"
#include <cmath>
#include <memory>
#include <algorithm>

namespace RayTracer {
TangleCube::TangleCube(const Math::Point3D &center, const Math::Vector3D &axis,
double size, double spacing)
: material(std::make_shared<Material>()), center(center),
axis(axis.normalize()), size(size), spacing(spacing) {}

TangleCube::TangleCube(const Math::Point3D &center, const Math::Vector3D &axis,
double size, double spacing, const std::shared_ptr<Material> &material)
: material(material), center(center), axis(axis.normalize()),
size(size), spacing(spacing) {}

void TangleCube::translate(const Math::Vector3D &translation) {
    center += translation;
}

void TangleCube::rotateX(double degrees) {
    rotationX += degrees;
    RayTracer::Rotate rotateX("x", degrees);
    center = rotateX.applyToPoint(center);
    axis = rotateX.applyToVector(axis).normalize();
}

void TangleCube::rotateY(double degrees) {
    rotationY += degrees;
    RayTracer::Rotate rotateY("y", degrees);
    center = rotateY.applyToPoint(center);
    axis = rotateY.applyToVector(axis).normalize();
}

void TangleCube::rotateZ(double degrees) {
    rotationZ += degrees;
    RayTracer::Rotate rotateZ("z", degrees);
    center = rotateZ.applyToPoint(center);
    axis = rotateZ.applyToVector(axis).normalize();
}

std::shared_ptr<Material> TangleCube::getMaterial() const {
    return material;
}

std::optional<HitInfo> TangleCube::hit(const Ray &ray, double tMin, double tMax) {
    Ray transformedRay = ray;
    if (rotationX != 0.0 || rotationY != 0.0 || rotationZ != 0.0) {
        Math::Point3D newOrigin = ray.origin;
        Math::Vector3D newDirection = ray.direction;

        if (rotationZ != 0.0) {
            RayTracer::Rotate rotateZ("z", -rotationZ);
            newOrigin = rotateZ.applyToPoint(newOrigin);
            newDirection = rotateZ.applyToVector(newDirection);
        }
        if (rotationY != 0.0) {
            RayTracer::Rotate rotateY("y", -rotationY);
            newOrigin = rotateY.applyToPoint(newOrigin);
            newDirection = rotateY.applyToVector(newDirection);
        }
        if (rotationX != 0.0) {
            RayTracer::Rotate rotateX("x", -rotationX);
            newOrigin = rotateX.applyToPoint(newOrigin);
            newDirection = rotateX.applyToVector(newDirection);
        }

        transformedRay = Ray(newOrigin, newDirection);
    }
    const int MAX_STEPS = 256;
    const double EPSILON = 0.0001;
    const double MAX_DIST = 20.0;

    auto tangleCubeSDF = [this](const Math::Vector3D& p) -> double {
        double scale = 1.5 / size;
        double x = p.X * scale;
        double y = p.Y * scale;
        double z = p.Z * scale;
        
        return (pow(x, 4) - 5.0 * x * x + 
                pow(y, 4) - 5.0 * y * y + 
                pow(z, 4) - 5.0 * z * z + 11.8);
    };

    auto calculateNormal = [&tangleCubeSDF](const Math::Vector3D& p) -> Math::Vector3D {
        const double eps = 0.0001;
        Math::Vector3D norm = Math::Vector3D(Math::Coords{
            .Xcoords = tangleCubeSDF(p + Math::Vector3D(Math::Coords{.Xcoords = eps, .Ycoords = 0, .Zcoords = 0})) -
                     tangleCubeSDF(p - Math::Vector3D(Math::Coords{.Xcoords = eps, .Ycoords = 0, .Zcoords = 0})),
            .Ycoords = tangleCubeSDF(p + Math::Vector3D(Math::Coords{.Xcoords = 0, .Ycoords = eps, .Zcoords = 0})) -
                     tangleCubeSDF(p - Math::Vector3D(Math::Coords{.Xcoords = 0, .Ycoords = eps, .Zcoords = 0})),
            .Zcoords = tangleCubeSDF(p + Math::Vector3D(Math::Coords{.Xcoords = 0, .Ycoords = 0, .Zcoords = eps})) -
                     tangleCubeSDF(p - Math::Vector3D(Math::Coords{.Xcoords = 0, .Ycoords = 0, .Zcoords = eps}))
        });
        return norm.normalize();
    };

    double boundingSize = size * 1.5;
    Math::Vector3D boxMin = Math::Vector3D(Math::Coords{
        .Xcoords = center.X - boundingSize,
        .Ycoords = center.Y - boundingSize,
        .Zcoords = center.Z - boundingSize
    });
    Math::Vector3D boxMax = Math::Vector3D(Math::Coords{
        .Xcoords = center.X + boundingSize,
        .Ycoords = center.Y + boundingSize,
        .Zcoords = center.Z + boundingSize
    });

    double tx1 = (boxMin.X - transformedRay.origin.X) / transformedRay.direction.X;
    double tx2 = (boxMax.X - transformedRay.origin.X) / transformedRay.direction.X;
    double tmin = std::min(tx1, tx2);
    double tmax = std::max(tx1, tx2);

    double ty1 = (boxMin.Y - transformedRay.origin.Y) / transformedRay.direction.Y;
    double ty2 = (boxMax.Y - transformedRay.origin.Y) / transformedRay.direction.Y;
    tmin = std::max(tmin, std::min(ty1, ty2));
    tmax = std::min(tmax, std::max(ty1, ty2));

    double tz1 = (boxMin.Z - transformedRay.origin.Z) / transformedRay.direction.Z;
    double tz2 = (boxMax.Z - transformedRay.origin.Z) / transformedRay.direction.Z;
    tmin = std::max(tmin, std::min(tz1, tz2));
    tmax = std::min(tmax, std::max(tz1, tz2));

    if (tmax < tmin || tmax < 0) {
        return std::nullopt;
    }
    tMin = std::max(tMin, tmin);
    tMax = std::min(tMax, tmax);

    double t = tMin;
    const int BINARY_STEPS = 20;
    bool hit_found = false;
    double closest_t = tMax;

    double prev_sdf = tangleCubeSDF(transformedRay.origin + transformedRay.direction * t - center);
    for (int i = 0; i < MAX_STEPS && t < tMax; i++) {
        double curr_t = t + i * (tMax - tMin) / MAX_STEPS;
        Math::Vector3D p = transformedRay.origin + transformedRay.direction * curr_t - center;
        double curr_sdf = tangleCubeSDF(p);
        
        if (prev_sdf * curr_sdf < 0) {
            double t_low = curr_t - (tMax - tMin) / MAX_STEPS;
            double t_high = curr_t;
            double mid_sdf;
            
            for (int j = 0; j < BINARY_STEPS; j++) {
                double t_mid = (t_low + t_high) * 0.5;
                Math::Vector3D mid_p = transformedRay.origin + transformedRay.direction * t_mid - center;
                mid_sdf = tangleCubeSDF(mid_p);
                
                if (std::abs(mid_sdf) < EPSILON) {
                    hit_found = true;
                    closest_t = t_mid;
                    break;
                }
                
                if (prev_sdf * mid_sdf < 0) {
                    t_high = t_mid;
                } else {
                    t_low = t_mid;
                    prev_sdf = mid_sdf;
                }
            }
            
            if (!hit_found) {
                closest_t = (t_low + t_high) * 0.5;
                hit_found = true;
            }
            
            break;
        }
        
        prev_sdf = curr_sdf;
    }
    
    if (!hit_found) {
        t = tMin;
        for (int i = 0; i < MAX_STEPS && t < tMax; i++) {
            Math::Vector3D p = transformedRay.origin + transformedRay.direction * t - center;
            double d = tangleCubeSDF(p);
            
            if (std::abs(d) < EPSILON) {
                hit_found = true;
                closest_t = t;
                break;
            }
            double step = std::abs(d) * 0.1;
            step = std::max(step, EPSILON * 10.0);
            
            t += step;
        }
    }

    if (!hit_found) {
        return std::nullopt;
    }

    Math::Point3D hitPoint = transformedRay.origin + transformedRay.direction * closest_t;
    Math::Vector3D rel_hit = hitPoint - center;
    Math::Vector3D final_normal = calculateNormal(rel_hit);

    if (rotationX != 0.0 || rotationY != 0.0 || rotationZ != 0.0) {
        if (rotationX != 0.0) {
            RayTracer::Rotate rotateX("x", rotationX);
            final_normal = rotateX.applyToVector(final_normal);
        }
        if (rotationY != 0.0) {
            RayTracer::Rotate rotateY("y", rotationY);
            final_normal = rotateY.applyToVector(final_normal);
        }
        if (rotationZ != 0.0) {
            RayTracer::Rotate rotateZ("z", rotationZ);
            final_normal = rotateZ.applyToVector(final_normal);
        }
    }

    HitInfo info;
    info.distance = closest_t;
    info.hitPoint = ray.origin + ray.direction * closest_t;
    info.normal = final_normal.normalize();
    info.primitive = shared_from_this();
    return info;
}

std::shared_ptr<IPrimitive> TangleCube::clone() const {
    auto copy = std::make_shared<TangleCube>(center, axis,
        size, spacing, material);
    copy->rotationX = rotationX;
    copy->rotationY = rotationY;
    copy->rotationZ = rotationZ;
    return copy;
}

void TangleCube::getLibConfigParams(std::shared_ptr<libconfig::Setting> setting) const {
    libconfig::Setting& pos = setting->add("position", libconfig::Setting::TypeGroup);
    pos.add("x", libconfig::Setting::TypeFloat) = center.X;
    pos.add("y", libconfig::Setting::TypeFloat) = center.Y;
    pos.add("z", libconfig::Setting::TypeFloat) = center.Z;

    libconfig::Setting& dir = setting->add("direction", libconfig::Setting::TypeGroup);
    dir.add("x", libconfig::Setting::TypeFloat) = axis.X;
    dir.add("y", libconfig::Setting::TypeFloat) = axis.Y;
    dir.add("z", libconfig::Setting::TypeFloat) = axis.Z;

    setting->add("size", libconfig::Setting::TypeFloat) = size;
    setting->add("spacing", libconfig::Setting::TypeFloat) = spacing;

    if (rotationX != 0.0 || rotationY != 0.0 || rotationZ != 0.0) {
        libconfig::Setting& rotation = setting->add("rotation", libconfig::Setting::TypeGroup);
        rotation.add("x", libconfig::Setting::TypeFloat) = rotationX;
        rotation.add("y", libconfig::Setting::TypeFloat) = rotationY;
        rotation.add("z", libconfig::Setting::TypeFloat) = rotationZ;
    }

    libconfig::Setting& mat = setting->add("material", libconfig::Setting::TypeGroup);

    libconfig::Setting& color = mat.add("color", libconfig::Setting::TypeGroup);
    color.add("r", libconfig::Setting::TypeFloat) = material->color.X;
    color.add("g", libconfig::Setting::TypeFloat) = material->color.Y;
    color.add("b", libconfig::Setting::TypeFloat) = material->color.Z;

    mat.add("ambient", libconfig::Setting::TypeFloat) = 0.1;
    mat.add("diffuse", libconfig::Setting::TypeFloat) = 0.9;
    mat.add("reflectivity", libconfig::Setting::TypeFloat) = material->reflectivity;
    mat.add("transparency", libconfig::Setting::TypeFloat) = material->transparency;
    mat.add("refractionIndex", libconfig::Setting::TypeFloat) = material->refractionIndex;
}
}  // namespace RayTracer