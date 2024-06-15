# TaskHub
## Overview
TaskHub is a C++ framework designed to simplify the development of task-related desktop applications using ImGui.
> **Note:** Currently, TaskHub supports Windows, with plans for future cross-platform support.
## Features
- **Modular Architecture:** Easily extend and customize the framework to suit your needs.
- **ImGui Integration:** Quickly create interactive and user-friendly interfaces.
- **Audio Support:** Manage and play audio files seamlessly with miniaudio.

## Build
TaskHub builds into a static library and TaskHubApp builds into an executable and links the core static library. The `Scripts/` directory contains setup script for Windows (Currently only supports Visual Studio).

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
- UNLICENSE for this repository (see `UNLICENSE.txt` for more details)
