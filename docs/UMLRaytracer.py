# script_python_raytracer_uml.py

uml_code = """
@startuml Raytracer_Complete

package Math {
    class Vector3D {
        +x: double
        +y: double
        +z: double
        +Vector3D()
        +Vector3D(x: double, y: double, z: double)
        +length(): double
        +dot(Vector3D): double
    }

    class Point3D {
        +x: double
        +y: double
        +z: double
        +Point3D()
        +Point3D(x: double, y: double, z: double)
    }

    Vector3D -- Point3D
}

package RayTracer {

    interface IPrimitive <<interface>> {
        +hit(ray: Ray): bool
    }

    class Sphere {
        +center: Point3D
        +radius: double
        +hit(ray: Ray): bool
    }

    class Plane {
        +position: Point3D
        +normal: Vector3D
        +hit(ray: Ray): bool
    }

    class Cylinder {
        +center: Point3D
        +radius: double
        +height: double
        +hit(ray: Ray): bool
    }

    class CompositePrimitive {
        -primitives: List<IPrimitive>
        +add(primitive: IPrimitive)
        +remove(primitive: IPrimitive)
        +hit(ray: Ray): bool
    }

    IPrimitive <|.. Sphere
    IPrimitive <|.. Plane
    IPrimitive <|.. Cylinder
    IPrimitive <|.. CompositePrimitive

    class PrimitiveFactory {
        +createPrimitive(type: string): IPrimitive
    }

    PrimitiveFactory --> IPrimitive

    class Ray {
        +origin: Point3D
        +direction: Vector3D
    }

    class Camera {
        +origin: Point3D
        +screen: Rectangle3D
        +ray(u: double, v: double): Ray
    }

    class Rectangle3D {
        +origin: Point3D
        +bottom_side: Vector3D
        +left_side: Vector3D
        +pointAt(u: double, v: double): Point3D
    }

    Camera -- Rectangle3D

    class SceneBuilder {
        -scene: CompositePrimitive
        +addSphere(center, radius)
        +addPlane(position, normal)
        +addCylinder(center, radius, height)
        +getScene(): CompositePrimitive
    }

    SceneBuilder --> CompositePrimitive
}

class SceneDirector {
    -builder: SceneBuilder
    +construct(): CompositePrimitive
}

SceneDirector --> SceneBuilder

@enduml
"""

# Génération du fichier UML avec PlantUML
def generate_uml_file(filename="raytracer_uml.puml"):
    with open(filename, "w") as file:
        file.write(uml_code)
    print(f"UML file '{filename}' has been generated successfully!")

if __name__ == '__main__':
    generate_uml_file()
