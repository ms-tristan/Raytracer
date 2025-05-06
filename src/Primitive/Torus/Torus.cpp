/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Torus
*/

#include "Primitive/Torus/Torus.hpp"
#include <cmath>
#include <limits>
#include <complex>
#include <vector>
#include <algorithm>
#include <iostream> // Pour les logs de debug

namespace RayTracer {

Torus::Torus(const Math::Point3D &center, const Math::Vector3D &axis,
    double majorRadius, double minorRadius)
    : center(center), axis(axis.normalize()), majorRadius(majorRadius), minorRadius(minorRadius)
{
    std::cout << "[DEBUG] Torus created at (" << center.X << ", " << center.Y << ", " << center.Z
              << ") with majorRadius=" << majorRadius << " minorRadius=" << minorRadius << std::endl;
}

Torus::Torus(const Math::Point3D &center, const Math::Vector3D &axis,
    double majorRadius, double minorRadius, const std::shared_ptr<Material> &material)
    : center(center), axis(axis.normalize()), majorRadius(majorRadius), minorRadius(minorRadius), material(material)
{
    std::cout << "[DEBUG] Torus created at (" << center.X << ", " << center.Y << ", " << center.Z
              << ") with majorRadius=" << majorRadius << " minorRadius=" << minorRadius << " (with material)" << std::endl;
}

void Torus::translate(const Math::Vector3D &translation) {
    center = center + translation;
}

void Torus::rotateX(double degrees) {
    rotationX += degrees;
    // À compléter si tu veux appliquer la rotation à l'axe
}

void Torus::rotateY(double degrees) {
    rotationY += degrees;
    // À compléter si tu veux appliquer la rotation à l'axe
}

void Torus::rotateZ(double degrees) {
    rotationZ += degrees;
    // À compléter si tu veux appliquer la rotation à l'axe
}

std::shared_ptr<Material> Torus::getMaterial() const {
    return material;
}

std::shared_ptr<IPrimitive> Torus::clone() const {
    return std::make_shared<Torus>(*this);
}

void Torus::getLibConfigParams(std::shared_ptr<libconfig::Setting> setting) const {
    // À compléter selon ton système de sauvegarde de scène
}

// Solveur quartique naïf (cherche les racines réelles)
static std::vector<double> solveQuartic(double a, double b, double c, double d, double e) {
    std::vector<double> roots;
    if (std::abs(a) < 1e-8) {
        // Cubique ou moins
        if (std::abs(b) < 1e-8) {
            // Quadratique
            if (std::abs(c) < 1e-8) return roots;
            double delta = d * d - 4 * c * e;
            if (delta < 0) return roots;
            double sqrtDelta = std::sqrt(delta);
            roots.push_back((-d - sqrtDelta) / (2 * c));
            roots.push_back((-d + sqrtDelta) / (2 * c));
            return roots;
        }
        // Cubique : on ne traite pas ici pour la simplicité
        return roots;
    }
    // Quartique : méthode naïve par recherche sur un intervalle
    double tMin = -1000, tMax = 1000, step = 0.01;
    double prev = a*tMin*tMin*tMin*tMin + b*tMin*tMin*tMin + c*tMin*tMin + d*tMin + e;
    for (double t = tMin + step; t < tMax; t += step) {
        double val = a*t*t*t*t + b*t*t*t + c*t*t + d*t + e;
        if (prev * val < 0) { // Changement de signe = racine
            roots.push_back(t - step/2);
            if (roots.size() >= 4) break;
        }
        prev = val;
    }
    return roots;
}

std::optional<HitInfo> Torus::hit(const Ray &ray, double tMin, double tMax) {
    std::cout << "[DEBUG] Torus::hit called" << std::endl;

    // On suppose que l'axe du tore est aligné avec Z
    Math::Vector3D O = ray.origin - center;
    Math::Vector3D D = ray.direction;

    double R = majorRadius;
    double r = minorRadius;

    double ox = O.X, oy = O.Y, oz = O.Z;
    double dx = D.X, dy = D.Y, dz = D.Z;

    double sum_d_sq = dx*dx + dy*dy + dz*dz;
    double e = ox*ox + oy*oy + oz*oz - R*R - r*r;
    double f = ox*dx + oy*dy + oz*dz;
    double fourR2 = 4.0 * R * R;

    double a = sum_d_sq * sum_d_sq;
    double b = 4.0 * sum_d_sq * f;
    double c = 2.0 * sum_d_sq * e + 4.0 * f * f + fourR2 * dz * dz;
    double d = 4.0 * f * e + 2.0 * fourR2 * oz * dz;
    double e4 = e * e - fourR2 * (r * r - oz * oz);

    std::vector<double> roots = solveQuartic(a, b, c, d, e4);

    // Chercher la plus petite racine valide
    double t = std::numeric_limits<double>::max();
    for (double root : roots) {
        if (root > tMin && root < tMax && root < t)
            t = root;
    }
    if (t == std::numeric_limits<double>::max()) {
        std::cout << "[DEBUG] Torus::hit found no intersection" << std::endl;
        return std::nullopt;
    }

    // Calcul du point d'impact et de la normale
    Math::Point3D hitPoint = ray.origin + ray.direction * t;
    Math::Vector3D hit = hitPoint - center;
    double param = hit.X*hit.X + hit.Y*hit.Y + hit.Z*hit.Z + R*R - r*r;
    Math::Vector3D normal = Math::Vector3D({
        4.0 * hit.X * param - 8.0 * R * R * hit.X,
        4.0 * hit.Y * param - 8.0 * R * R * hit.Y,
        4.0 * hit.Z * param
    });
    normal = normal.normalize();

    std::cout << "[DEBUG] Torus::hit intersection at t=" << t << std::endl;

    HitInfo info;
    info.distance = t;
    info.hitPoint = hitPoint;
    info.normal = normal;
    info.primitive = shared_from_this();
    return info;
}

} // namespace RayTracer