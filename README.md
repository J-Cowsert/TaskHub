# TaskHub

TaskHub builds into a static library and is meant to contain common code intended for use in creating task related imgui applications. TaskHubApp builds into an executable and links the core static library, as well as provides an include path to TaskHub code.

The `Scripts/` directory contains build scripts for Windows, and the `Vendor/` directory contains Premake binaries (currently version `5.0-beta2`).

## License
- UNLICENSE for this repository (see `UNLICENSE.txt` for more details)
- Premake is licensed under BSD 3-Clause (see included LICENSE.txt file for more details)
