# Atom Engine

## Overview
This C++ Engine is a lightweight, modular, and extensible framework designed for building high-performance games. It provides the foundation for 2D and 3D rendering, input handling, dynamic system management, and more, while maintaining a clean and flexible architecture.

## Features
- **Cross-platform support** (Windows, macOS, Linux)
- **Input handling** for keyboard, mouse, and game controllers
- **Modular architecture** to easily extend or replace systems
- **Resource management** for textures, models, and audio
- **Extension support** for specialized scripts for projects

## Getting Started

### Requirements
| Component    | Windows                                | macOS                                        | Linux (Ubuntu)                       |
| ------------ | -------------------------------------- | -------------------------------------------- | ------------------------------------ |
| **OS**       | Windows 10+                            | macOS 10.14+                                 | Ubuntu 20.04+                        |
| **CPU**      | x64 architecture with SSE2 support     | Apple silicon (M1 or above) required         | x64 architecture with SSE2 support   |
| **Graphics** | GPU with DirectX 10, 11, or 12 support | Metal-capable Intel/AMD GPU or Apple silicon | GPU supporting OpenGL 3.2+ or Vulkan |

### Build Instructions

1. Create a build directory and navigate into it:
   ```bash
   mkdir projects
   cd projects
   ```
2. Clone the repository:
   ```bash
   git clone https://github.com/Frogpants/engine.git
   ```
3. Build the engine:
   ```bash
   make
   ```
4. Run the sample application to verify everything works:
   ```bash
   make run
   ```

## Contributing
Contributions are welcome! Please follow these guidelines:  
- Fork the repository and create a feature branch.
- Write clear, well-documented code.
- Submit a pull request describing your changes.

## License
This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.
