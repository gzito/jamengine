# jamengine

## What is jamengine
A simple-to-use C++ game engine built upon OpenGL 3.2 and SDL 2

## Features

 - Uses intrusive reference counted pointers for managing memory
 - Spritesheets support from Zwoptex or TexturePacker
 - SpriteBatch class available ported from XNA/Monogame
 - Tilemaps created with Tiled
 - Many formats supported for 3D model loading via assimp
 - Loads BMP, JPG, and PNG files via stb-image
 - Audio support via openal-soft

## Dependencies
Dependencies will automatically be downloaded and built from their sources.
See [dependencies.txt](./dependencies.txt)

## How to build
See [BUILDING.md](./BUILDING.md)

## Tests
The project has been tested as static library on Windows and macos.

Also library wasn't tested on other platforms, but should work on Linux.

## Acknoledgements
See [ACKNOLEDGEMENTS.txt](./ACKNOLEDGEMENTS.txt)

## License
Licensed under MIT
