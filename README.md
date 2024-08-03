# TaskHub
### 🚧 Work in Progress
> **Codebase subject to change**
## Overview
TaskHub is a C++ framework designed to simplify the development of desktop applications using OpenGL & ImGui.
> **Note:** Currently, TaskHub supports Windows, with plans for future cross-platform support.
## Features
- **Modular Architecture:** Easily extend and customize the framework to suit your needs.
- **ImGui Integration:** Quickly create interactive and user-friendly interfaces.
- **Audio Support:** Manage and play audio files with miniaudio.

## Build
TaskHub builds into a static library and TaskHubApp builds into an executable and links to TaskHub.

<ins>**1. Downloading:**</ins>
To get started clone the repository with `git clone --recursive https://github.com/DexNipp/TaskHub.git`

If you cloned the repository non-recursively, you can use `git submodule update --init` to clone the required submodules.

<ins>**2. Configuring the dependencies:**</ins>
After cloning, run `Scripts/setup-windows.bat` to set up the Visual Studio solution and project files.

From there you can run the TaskHubApp project to see a basic example (Code is in App.cpp)

### 3rd party libaries
- [Dear ImGui](https://github.com/ocornut/imgui)
- [GLFW](https://github.com/glfw/glfw)
- [spdlog](https://github.com/gabime/spdlog)
- [miniaudio](https://github.com/mackron/miniaudio)

## License
- MIT License for this repository (see `LICENSE.txt` for more details)

---

## *Feedback*

If you notice any issues or areas for improvement, please feel free to open an issue. I'm eager for feedback and looking to improve. Your suggestions are greatly appreciated!

---
