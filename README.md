# 🎇 Raytracer - B-OOP-400
A **modular C++ raytracer** that simulates light to render realistic 3D scenes. This project is built as part of the **B-OOP-400** curriculum and focuses on clean architecture, extensibility, and performance.

---

## 🧠 Project Overview
Ray tracing is a rendering technique that traces the path of light as pixels and simulates its interaction with objects. The goal is to generate a digital image from a scene description file without using a graphical interface.

This raytracer supports:
- Rendering basic primitives like spheres and planes
- Lighting with ambient and directional light
- Scene configuration from an external file
- Output to `.ppm` image files
- Clean and extensible architecture with C++ interfaces and design patterns

---

## 📦 Features
### ✅ **Mandatory**
- [x] Primitives: **Sphere**, **Plane**
- [x] Transformations: **Translation**
- [x] Light: **Ambient**, **Directional**
- [x] Flat color material
- [x] Scene loaded via external config file (libconfig++)
- [x] Output rendered image to `.ppm` file (no GUI)

### 🧪 **Optional (Bonus)**
- [ ] Additional primitives: **Cylinder**, **Cone**
- [ ] Rotation transformations
- [ ] Drop shadows
- [ ] Plugins (`.so`) for dynamic primitive loading
- [ ] GUI display (SFML)
- [ ] Phong model, reflections, textures

---

## 🛠️ Technologies
- **Language**: C++17
- **Build Systems**: Makefile or CMake
- **Scene Parsing**: [libconfig++](https://hyperrealm.github.io/libconfig/)
- **Optional Display**: [SFML 2.5.1](https://www.sfml-dev.org/)

---

## 🚀 Getting Started
### 📥 Clone the repository
```bash
git clone git@github.com:SamTess/Raytracer.git
cd Raytracer
```

### 📦 Install dependencies
```bash
sudo apt update
sudo apt install -y g++ cmake libconfig++-dev libsfml-dev
```

### 🛠️ Build the project
```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
cd ..
```

### 🎨 Run the raytracer
```bash
./raytracer <scene_file.cfg> [--graphic]
```
In graphic mode, a window will open displaying the scene. You can move in it with ZQSD, turn with the arrows,
and move object by dragging them with the mouse.
Else output will be saved as a .ppm image.

### 📜 Example scene file
Scene files are written in libconfig++ format. Example:
```cfg
camera: {
  resolution = { width = 1920; height = 1080; };
  position = { x = 0; y = -100; z = 20; };
  rotation = { x = 0; y = 0; z = 0; };
  fieldOfView = 72.0;
  fieldOfView = 72.0;
};

primitives: {
  spheres = (
    { x = 60; y = 5; z = 40; r = 25; color = { r = 255; g = 64; b = 64; }; },
    { x = -40; y = 20; z = -10; r = 35; color = { r = 64; g = 255; b = 64; }; }
  );

  planes = (
    { axis = "Z"; position = -20; color = { r = 64; g = 64; b = 255; }; }
  );
};

lights: {
  ambient = 0.4;
  diffuse = 0.6;

  point = (
    { x = 400; y = 100; z = 500; }
  );

  directional = ();
};
```

## 📁 Project Structure
```plaintext
raytracer/
├── src/                # All C++ sources and headers
│   ├── core/           # Camera, Scene, Ray, Renderer
│   ├── primitives/     # Sphere, Plane, etc.
│   ├── lights/         # AmbientLight, DirectionalLight
│   ├── utils/          # Vectors, Colors, Math utils
│   ├── config/         # Scene loader
│   └── main.cpp        # Entry point
├── scenes/             # Sample scene files (.cfg)
├── screenshots/        # Output PPM images
├── plugins/            # Bonus plugins (.so)
├── bonus/              # Bonus implementations
├── Makefile            # Make build system
├── CMakeLists.txt      # CMake alternative
├── .gitignore
└── README.md
```

### 🧩 Architecture
- Interfaces used for primitives and lights
- Supports plugins via .so dynamic libraries (optional)
- Includes Factory, Builder, or Composite patterns


#### ✨ Credits

Made as part of the Epitech B-OOP-400 project by :
- Samuel Tesson <samuel.tesson@epitech.eu>
- Clément Talneau <clement.talneau@epitech.eu>
- Tristan Masse <tristan.masse@epitech.eu>
- Alex Augereau <alex.augereau@epitech.eu>
