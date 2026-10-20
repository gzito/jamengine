# Building Jam Engine

## Requirements

- CMake 3.16 or newer and Git.
- An Internet connection on the first configure: CMake downloads the GitHub dependencies listed in `CMakeLists.txt` into `build/<configuration>/_deps`.
- macOS: Xcode Command Line Tools (`xcode-select --install`). The Apple Silicon commands below build for `arm64`.
- Windows: an x86_64 toolchain. MSVC x64 and MinGW-w64 x86_64 are supported. For MinGW, `gcc`, `g++`, and `mingw32-make` must be on `PATH`.

The Jam and JamPSys libraries are static by default, and the example applications are enabled. The commands below use separate build directories for Debug and Release.

## macOS (Apple Silicon)

Run these commands from the repository root:

```sh
cmake -S . -B build/macos-debug \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_OSX_ARCHITECTURES=arm64 \
  -DJAM_BUILD_STATIC=ON -DJAM_BUILD_SHARED=OFF -DJAM_BUILD_EXAMPLES=ON
cmake --build build/macos-debug --parallel
```

For Release, use `build/macos-release` and `-DCMAKE_BUILD_TYPE=Release`.

## Windows x86_64 with MinGW-w64

Run these commands from an x86_64 MinGW-w64 terminal, at the repository root:

```sh
cmake -S . -B build/windows-debug -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug -DJAM_BUILD_STATIC=ON -DJAM_BUILD_SHARED=OFF -DJAM_BUILD_EXAMPLES=ON
cmake --build build/windows-debug --parallel
```

For Release, use `build/windows-release` and `-DCMAKE_BUILD_TYPE=Release`.

### Alternative: Visual Studio

From a terminal where CMake and Visual Studio 2022 are available:

```powershell
cmake -S . -B build/windows-vs -G "Visual Studio 17 2022" -A x64 -DJAM_BUILD_STATIC=ON -DJAM_BUILD_SHARED=OFF -DJAM_BUILD_EXAMPLES=ON
cmake --build build/windows-vs --config Debug --parallel
```

For Release, replace `Debug` with `Release` in the last command.

## Useful options

- Libraries only: add `-DJAM_BUILD_EXAMPLES=OFF` when configuring.
- Build shared libraries as well: set `-DJAM_BUILD_SHARED=ON` (the static build remains enabled unless you set `-DJAM_BUILD_STATIC=OFF`).
- To change the compiler, generator, or configuration, use a new build directory, or remove the previous build directory and configure again.

Shaders and example resources are copied automatically to `build/<configuration>/resources` during the build.
