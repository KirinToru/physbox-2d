<h1 align="center">PhysBox 2D</h1>
<h3 align="center">A 2D Physics Sandbox Game</h3>
<p align="center">
<img src="https://img.shields.io/badge/C++-23-00599C?style=flat-square&logo=c%2B%2B">
<img src="https://img.shields.io/badge/SFML-v4.4.2-8CC445?logo=SFML&style=flat-square">
<img src="https://img.shields.io/badge/Box2D-v3.1.1-blue?style=flat-square">
<a href="LICENSE"><img src="https://img.shields.io/badge/license-Source%20Available-informational?style=flat-square"/></a>
</p>

PhysBox 2D is a two-dimentional physics-based sandbox game. It features rigid body dynamics, wind simulation, and interactive object spawning. Built with C++23, SFML 4.4.2, and Box2D v3.1.1.

### Contents

- [Features](#features)
- [Controls](#controls)
- [Tech Stack](#tech-stack)
- [Build](#build)
- [Documentation](#documentation)
- [Generating API Docs (Doxygen)](#generating-api-docs-doxygen)
- [Used Resources](#used-resources)
- [License](#license)

## Features

- **Robust Physics Engine:** Integration with Box2D for stable and accurate rigid body dynamics.
- **Interactive Sandbox:** A spawn menu allowing players to select and drop various physical objects into the world using the mouse.
- **Wind Simulation:** Environmental wind forces that realistically affect player movement and physics objects based on their mass and properties.

## [Controls](docs/controls.jpg)

## Tech Stack

| Category | Tool |
|----------|------|
| Standard | C++23 |
| Build System | CMake 4.4.2 |
| Libraries | SFML 3.1.0, Box2D v3.1.1 |
| Platform | Windows x64, Linux, macOS |

## Build

### Prerequisites

- **C++23** compatible compiler (e.g., MSVC, GCC 14+)
- **CMake** 4.4.2+
- *Note: SFML and Box2D are automatically downloaded and built via CMake FetchContent.*

### Windows (Visual Studio) / CMake

```shell
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

The executable will be generated in the `build` folder.

## Documentation

- **[Roadmap](docs/ROADMAP.md)** - Planned features and progress tracking

## Generating API Docs (Doxygen)

This project includes a [Doxyfile](docs/Doxyfile) for generating HTML API documentation. Run `doxygen docs/Doxyfile` from the root directory.

## Used Resources

- [SFML 3.1.0 Documentation](https://www.sfml-dev.org/documentation/3.1.0/)
- [Tiled Documentation](https://doc.mapeditor.org/en/stable/)
- [Box2D Documentation](https://box2d.org/documentation/)
- [CMake Documentation](https://cmake.org/documentation/)

## License

This project uses a **Source Available License**. See [LICENSE](LICENSE) for full details.
This project uses the external libraries [SFML](https://www.sfml-dev.org/) and [Box2D](https://box2d.org/), which are licensed under their respective open-source licenses.
