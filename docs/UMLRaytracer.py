# script_python_raytracer_uml.py

uml_code = """
@startuml Raytracer_Complete

' -- CLASSES AND STRUCTURE --

package Math {
    class Vector3D {
        +x: double
        +y: double
        +z: double
        +Vector3D()
        +Vector3D(x: double, y: double, z: double)
        +length(): double
        +dot(Vector3D): double
        +normalize(): Vector3D
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

    ' -- Primitive Classes --
    interface IPrimitive <<interface>> {
        +getTypeName(): string
        +translate(translation: Vector3D): void
        +rotateX(degrees: double): void
        +rotateY(degrees: double): void
        +rotateZ(degrees: double): void
        +hit(ray: Ray, tMin: double, tMax: double): optional<HitInfo>
        +getMaterial(): Material
        +clone(): IPrimitive
        +getLibConfigParams(setting: libconfig::Setting): void
    }

    abstract class APrimitive {
        #material: Material
        #rotationX: double
        #rotationY: double
        #rotationZ: double
        +APrimitive()
        +APrimitive(material: Material)
        +getMaterial(): Material
        +rotateX(degrees: double): void
        +rotateY(degrees: double): void
        +rotateZ(degrees: double): void
    }

    class Sphere {
        +center: Point3D
        +radius: double
        +Sphere(center: Point3D, radius: double)
        +Sphere(center: Point3D, radius: double, material: Material)
        +getTypeNameStatic(): string
        +getTypeName(): string
        +translate(translation: Vector3D): void
        +hit(ray: Ray, tMin: double, tMax: double): optional<HitInfo>
        +clone(): IPrimitive
    }

    class Plane {
        +position: Point3D
        +normal: Vector3D
        +Plane(position: Point3D, normal: Vector3D)
        +Plane(position: Point3D, normal: Vector3D, material: Material)
        +getTypeNameStatic(): string
        +getTypeName(): string
        +translate(translation: Vector3D): void
        +rotateX(degrees: double): void
        +rotateY(degrees: double): void
        +rotateZ(degrees: double): void
        +hit(ray: Ray, tMin: double, tMax: double): optional<HitInfo>
        +clone(): IPrimitive
    }

    class Cylinder {
        +center: Point3D
        +axis: Vector3D
        +radius: double
        +height: double
        +Cylinder(center: Point3D, axis: Vector3D, radius: double, height: double)
        +Cylinder(center: Point3D, axis: Vector3D, radius: double, height: double, material: Material)
        +getTypeNameStatic(): string
        +getTypeName(): string
        +translate(translation: Vector3D): void
        +rotateX(degrees: double): void
        +rotateY(degrees: double): void
        +rotateZ(degrees: double): void
        +hit(ray: Ray, tMin: double, tMax: double): optional<HitInfo>
        +clone(): IPrimitive
    }

    class Cone {
        +apex: Point3D
        +axis: Vector3D
        +radius: double
        +height: double
        +Cone(apex: Point3D, axis: Vector3D, radius: double, height: double)
        +Cone(apex: Point3D, axis: Vector3D, radius: double, height: double, material: Material)
        +getTypeNameStatic(): string
        +getTypeName(): string
        +translate(translation: Vector3D): void
        +rotateX(degrees: double): void
        +rotateY(degrees: double): void
        +rotateZ(degrees: double): void
        +hit(ray: Ray, tMin: double, tMax: double): optional<HitInfo>
        +clone(): IPrimitive
    }

    class Box {
        +center: Point3D
        +dimensions: Vector3D
        +Box(center: Point3D, dimensions: Vector3D)
        +Box(center: Point3D, dimensions: Vector3D, material: Material)
        +getTypeNameStatic(): string
        +getTypeName(): string
        +translate(translation: Vector3D): void
        +rotateX(degrees: double): void
        +rotateY(degrees: double): void
        +rotateZ(degrees: double): void
        +hit(ray: Ray, tMin: double, tMax: double): optional<HitInfo>
        +clone(): IPrimitive
    }

    class CompositePrimitive {
        -primitives: List<IPrimitive>
        -material: Material
        +CompositePrimitive(material: Material)
        +getTypeNameStatic(): string
        +getTypeName(): string
        +translate(translation: Vector3D): void
        +rotateX(degrees: double): void
        +rotateY(degrees: double): void
        +rotateZ(degrees: double): void
        +hit(ray: Ray, tMin: double, tMax: double): optional<HitInfo>
        +getMaterial(): Material
        +clone(): IPrimitive
        +add(primitive: IPrimitive): void
        +remove(primitive: IPrimitive): void
        +getPrimitives(): List<IPrimitive>
    }

    IPrimitive <|.. APrimitive
    APrimitive <|-- Sphere
    APrimitive <|-- Plane
    APrimitive <|-- Cylinder
    APrimitive <|-- Cone
    APrimitive <|-- Box
    IPrimitive <|.. CompositePrimitive

    ' -- Primitive Plugin System --
    interface IPrimitivePlugin <<interface>> {
        +getTypeName(): string
        +createPrimitive(params: Map<string, double>, material: Material): IPrimitive
        +getRequiredParameters(): List<string>
    }

    class SpherePlugin {
        +getTypeName(): string
        +createPrimitive(params: Map<string, double>, material: Material): IPrimitive
        +getRequiredParameters(): List<string>
    }

    class PlanePlugin {
        +getTypeName(): string
        +createPrimitive(params: Map<string, double>, material: Material): IPrimitive
        +getRequiredParameters(): List<string>
    }

    class CylinderPlugin {
        +getTypeName(): string
        +createPrimitive(params: Map<string, double>, material: Material): IPrimitive
        +getRequiredParameters(): List<string>
    }

    class ConePlugin {
        +getTypeName(): string
        +createPrimitive(params: Map<string, double>, material: Material): IPrimitive
        +getRequiredParameters(): List<string>
    }

    class BoxPlugin {
        +getTypeName(): string
        +createPrimitive(params: Map<string, double>, material: Material): IPrimitive
        +getRequiredParameters(): List<string>
    }

    IPrimitivePlugin <|.. SpherePlugin
    IPrimitivePlugin <|.. PlanePlugin
    IPrimitivePlugin <|.. CylinderPlugin
    IPrimitivePlugin <|.. ConePlugin
    IPrimitivePlugin <|.. BoxPlugin

    class PrimitivePluginManager {
        -loadedPlugins: Map<string, PluginInfo>
        -instance: PrimitivePluginManager
        +getInstance(): PrimitivePluginManager
        +loadPlugin(path: string): bool
        +unloadPlugin(typeName: string): bool
        +getPlugin(typeName: string): IPrimitivePlugin
        +getLoadedPluginNames(): List<string>
        +createPrimitive(typeName: string, params: Map<string, double>, material: Material): IPrimitive
        +loadAllPlugins(directory: string): bool
    }

    class PrimitiveFactory {
        -creators: Map<string, CreatorFunc>
        -isInitialized: bool
        +createPrimitive(type: string, params: Map<string, double>, material: Material): IPrimitive
        +registerPrimitive(type: string, creator: CreatorFunc): void
        +getAvailablePrimitiveTypes(): List<string>
    }

    PrimitivePluginManager --> IPrimitivePlugin
    PrimitiveFactory --> IPrimitive

    class PrimitiveDecorator {
        #wrappedPrimitive: IPrimitive
        +PrimitiveDecorator(primitive: IPrimitive)
        +translate(translation: Vector3D): void
        +rotateX(degrees: double): void
        +rotateY(degrees: double): void
        +rotateZ(degrees: double): void
        +hit(ray: Ray, tMin: double, tMax: double): optional<HitInfo>
        +getMaterial(): Material
        +getLibConfigParams(setting: libconfig::Setting): void
    }

    class MaterialDecorator {
        -overrideMaterial: Material
        +MaterialDecorator(primitive: IPrimitive, material: Material)
        +getMaterial(): Material
        +clone(): IPrimitive
        +getLibConfigParams(setting: libconfig::Setting): void
    }

    PrimitiveDecorator --> IPrimitive
    PrimitiveDecorator <|-- MaterialDecorator

    ' -- Ray and Camera --
    class Ray {
        +origin: Point3D
        +direction: Vector3D
        +Ray(origin: Point3D, direction: Vector3D)
        +at(t: double): Point3D
    }

    class Rectangle3D {
        +origin: Point3D
        +bottom_side: Vector3D
        +left_side: Vector3D
        +Rectangle3D(origin: Point3D, bottom_side: Vector3D, left_side: Vector3D)
        +pointAt(u: double, v: double): Point3D
    }

    class Camera {
        +origin: Point3D
        +screen: Rectangle3D
        +Camera()
        +ray(u: double, v: double): Ray
        +translate(translation: Vector3D): void
    }

    Camera --> Rectangle3D
    Camera --> Ray

    ' -- Light System --
    interface ILight <<interface>> {
        +getLightDirection(point: Point3D): Vector3D
        +getLightColor(): Vector3D
        +clone(): ILight
        +getLibConfigParams(setting: libconfig::Setting): void
    }

    abstract class ALight {
        #color: Vector3D
        +ALight()
        +ALight(color: Vector3D)
        +getLightColor(): Vector3D
    }

    class PointLight {
        +position: Point3D
        +constantAttenuation: double
        +linearAttenuation: double
        +quadraticAttenuation: double
        +PointLight(position: Point3D, color: Vector3D, constant: double, linear: double, quadratic: double)
        +getLightDirection(point: Point3D): Vector3D
        +getLightColor(point: Point3D): Vector3D
        +clone(): ILight
        +getLibConfigParams(setting: libconfig::Setting): void
    }

    class DirectionalLight {
        +direction: Vector3D
        +DirectionalLight(direction: Vector3D, color: Vector3D)
        +getLightDirection(point: Point3D): Vector3D
        +clone(): ILight
        +getLibConfigParams(setting: libconfig::Setting): void
    }

    class AmbientLight {
        +color: Vector3D
        +AmbientLight()
        +AmbientLight(color: Vector3D)
        +getLibConfigParams(setting: libconfig::Setting): void
        +getLightColor(): Vector3D
    }

    class LightDecorator {
        #wrappedLight: ILight
        +LightDecorator(light: ILight)
        +getLightDirection(point: Point3D): Vector3D
        +getLightColor(): Vector3D
        +getLibConfigParams(setting: libconfig::Setting): void
    }

    class ColorLightDecorator {
        -colorFilter: Vector3D
        +ColorLightDecorator(light: ILight, filter: Vector3D)
        +getLightColor(): Vector3D
        +clone(): ILight
        +getLibConfigParams(setting: libconfig::Setting): void
    }

    class LightFactory {
        -creators: Map<string, CreatorFunc>
        -isInitialized: bool
        -registerBuiltInLights(): void
        +createLight(type: string, params: Map<string, double>): ILight
        +registerLight(type: string, creator: CreatorFunc): void
        +getAvailableLightTypes(): List<string>
        +createAmbientLight(color: Vector3D): AmbientLight
    }
    
    ILight <|.. ALight
    ALight <|-- PointLight
    ALight <|-- DirectionalLight
    ILight <|.. LightDecorator
    LightDecorator <|-- ColorLightDecorator
    LightFactory --> ILight

    ' -- Shader System --
    interface IShader <<interface>> {
        +apply(color: Vector3D, hitInfo: HitInfo, ray: Ray): Vector3D
        +clone(): IShader
        +getLibConfigParams(setting: libconfig::Setting): void
        +getType(): string
    }

    abstract class AShader {
        #type: string
        +AShader(shaderType: string)
        +getType(): string
        +getLibConfigParams(setting: libconfig::Setting): void
    }

    class BasicShader {
        +BasicShader()
        +apply(color: Vector3D, hitInfo: HitInfo, ray: Ray): Vector3D
        +clone(): IShader
    }

    class PhongShader {
        -specularStrength: double
        -shininess: double
        +PhongShader(specularStrength: double, shininess: double)
        +apply(color: Vector3D, hitInfo: HitInfo, ray: Ray): Vector3D
        +clone(): IShader
        +getLibConfigParams(setting: libconfig::Setting): void
    }

    class ToonShader {
        -levels: int
        -edgeThreshold: double
        +ToonShader(levels: int, edgeThreshold: double)
        +apply(color: Vector3D, hitInfo: HitInfo, ray: Ray): Vector3D
        +clone(): IShader
        +getLibConfigParams(setting: libconfig::Setting): void
    }

    class FresnelShader {
        -fresnelPower: double
        -rimColor: Vector3D
        +FresnelShader(fresnelPower: double, rimColor: Vector3D)
        +apply(color: Vector3D, hitInfo: HitInfo, ray: Ray): Vector3D
        +clone(): IShader
        +getLibConfigParams(setting: libconfig::Setting): void
    }

    class ShaderDecorator {
        #wrappedShader: IShader
        +ShaderDecorator(shader: IShader)
        +apply(color: Vector3D, hitInfo: HitInfo, ray: Ray): Vector3D
        +getType(): string
        +getLibConfigParams(setting: libconfig::Setting): void
    }

    class FresnelDecorator {
        -fresnelPower: double
        -rimColor: Vector3D
        +FresnelDecorator(shader: IShader, fresnelPower: double, rimColor: Vector3D)
        +apply(color: Vector3D, hitInfo: HitInfo, ray: Ray): Vector3D
        +clone(): IShader
        +getLibConfigParams(setting: libconfig::Setting): void
        +getType(): string
    }

    interface IShaderPlugin <<interface>> {
        +getTypeName(): string
        +createShader(params: Map<string, double>): IShader
        +getRequiredParameters(): List<string>
    }

    class PhongShaderPlugin {
        +getTypeName(): string
        +createShader(params: Map<string, double>): IShader
        +getRequiredParameters(): List<string>
    }

    class ToonShaderPlugin {
        +getTypeName(): string
        +createShader(params: Map<string, double>): IShader
        +getRequiredParameters(): List<string>
    }

    class FresnelShaderPlugin {
        +getTypeName(): string
        +createShader(params: Map<string, double>): IShader
        +getRequiredParameters(): List<string>
    }

    class ShaderPluginManager {
        -loadedPlugins: Map<string, PluginInfo>
        -instance: ShaderPluginManager
        +getInstance(): ShaderPluginManager
        +loadPlugin(path: string): bool
        +unloadPlugin(typeName: string): bool
        +getPlugin(typeName: string): IShaderPlugin
        +getLoadedPluginNames(): List<string>
        +createShader(typeName: string, params: Map<string, double>): IShader
        +loadAllPlugins(directory: string): bool
    }

    class ShaderFactory {
        +ShaderFactory()
        +createShader(type: string, setting: libconfig::Setting): IShader
        +createShaderFromSetting(setting: libconfig::Setting): IShader
        +registerShader(type: string, creator: ShaderCreator): void
        -registerBuiltInShaders(): void
    }

    IShader <|.. AShader
    AShader <|-- BasicShader
    AShader <|-- PhongShader
    AShader <|-- ToonShader
    AShader <|-- FresnelShader
    IShader <|.. ShaderDecorator
    ShaderDecorator <|-- FresnelDecorator
    IShaderPlugin <|.. PhongShaderPlugin
    IShaderPlugin <|.. ToonShaderPlugin
    IShaderPlugin <|.. FresnelShaderPlugin
    ShaderPluginManager --> IShaderPlugin
    ShaderFactory --> IShader

    ' -- Post-processing System --
    interface IPostProcess <<interface>> {
        +apply(frameBuffer: vector<Vector3D>, width: int, height: int): vector<Vector3D>
        +clone(): IPostProcess
        +getType(): string
    }

    class PostProcessFactory {
        -creators: Map<string, CreatorFunc>
        -isInitialized: bool
        +createPostProcess(type: string, params: Map<string, double>): IPostProcess
        +registerPostProcess(type: string, creator: CreatorFunc): void
        +getAvailablePostProcessTypes(): vector<string>
    }
    
    PostProcessFactory --> IPostProcess

    ' -- Scene Management --
    class Scene {
        -primitives: vector<IPrimitive>
        -lights: vector<ILight>
        -shaders: vector<IShader>
        -postProcessEffects: vector<IPostProcess>
        -ambientLight: AmbientLight
        -camera: Camera
        +setCamera(cam: Camera): void
        +getCamera(): Camera
        +setAmbientLight(light: AmbientLight): void
        +addPrimitive(primitive: IPrimitive): void
        +addLight(light: ILight): void
        +addShader(shader: IShader): void
        +addPostProcess(postProcess: IPostProcess): void
        +trace(ray: Ray): optional<HitInfo>
        +isInShadow(hitPoint: Point3D, lightDir: Vector3D, light: ILight): bool
        +computeColor(ray: Ray): Vector3D
        +applyPostProcessingToFrameBuffer(frameBuffer: vector<Vector3D>, width: int, height: int): vector<Vector3D>
        +writeColor(color: Vector3D): void
        +getLibConfigParams(setting: libconfig::Setting): void
        +getPrimitives(): vector<IPrimitive>
        +getLights(): vector<ILight>
        +getShaders(): vector<IShader>
        +getPostProcessEffects(): vector<IPostProcess>
        +getAmbientLight(): AmbientLight
    }

    class SceneBuilder {
        -scene: Scene
        +addPrimitive(primitive: IPrimitive): void
        +addLight(light: ILight): void
        +addShader(shader: IShader): void
        +addPostProcess(postProcess: IPostProcess): void
        +setCamera(camera: Camera): void
        +setAmbientLight(light: AmbientLight): void
        +getScene(): Scene
    }

    class SceneDirector {
        -builder: SceneBuilder
        +construct(): Scene
    }
    
    interface IObserver <<interface>> {
        +update(): void
    }

    class SceneFileWatcher {
        -observers: List<IObserver>
        +attach(observer: IObserver): void
        +detach(observer: IObserver): void
        +notify(): void
    }

    SceneBuilder --> Scene
    SceneDirector --> SceneBuilder
    Scene --> Camera
    Scene --> AmbientLight
    Scene --> IPrimitive
    Scene --> ILight
    Scene --> IShader
    Scene --> IPostProcess
    SceneFileWatcher --> IObserver

    ' -- Rendering System --
    interface IRenderer <<interface>> {
        +drawScene(scene: Scene, camera: Camera): void
    }
    
    class Renderer {
        +Renderer()
        +drawScene(scene: Scene, camera: Camera): void
    }
    
    IRenderer <|.. Renderer
    Renderer --> Scene

    ' -- Data structures --
    class HitInfo {
        +distance: double
        +hitPoint: Point3D
        +normal: Vector3D
        +primitive: IPrimitive
    }

    class Material {
        +ambient: Vector3D
        +diffuse: Vector3D
        +specular: Vector3D
        +shininess: double
    }
    
    Material -- IPrimitive

    ' -- Transform System --
    interface ITransformation <<interface>> {
        +apply(primitive: IPrimitive): void
    }
    
    class Rotate {
        +Rotate(axis: string, angle: double)
        +apply(primitive: IPrimitive): void
    }
    
    class Scale {
        +Scale(factor: Vector3D)
        +apply(primitive: IPrimitive): void
    }
    
    class Translate {
        +Translate(offset: Vector3D)
        +apply(primitive: IPrimitive): void
    }
    
    ITransformation <|.. Rotate
    ITransformation <|.. Scale
    ITransformation <|.. Translate
}

@enduml

@startuml Raytracer_LogicFlow
start
:Read Scene File;
:Parse with libconfig++;
:Configure Scene Builder;
:Build Scene;
:For each pixel;
:Cast Ray via Camera;
:Check hit with primitives;
:Apply shaders;
:Calculate color;
:Apply post-processing effects;
:Write image to file;
stop
@enduml
"""

# Generate the UML file with PlantUML
def generate_uml_file(filename="raytracer_uml.puml"):
    with open(filename, "w") as file:
        file.write(uml_code)
    print(f"UML file '{filename}' has been generated successfully!")

if __name__ == '__main__':
    generate_uml_file()