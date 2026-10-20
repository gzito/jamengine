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

 -  glm 0.9.9.6
 -  Box2D 2.3.0
 -  assimp 5
 -  glew 2.1.0
 -  ogg 1.3.4
 -  vorbis 1.3.6
 -  openal-soft 1.20.1
 -  SDL2 2.0.12
 -  SDL_ttf 2.0.15
 -  tinyxml2 10.0.0


## How to build

See [BUILDING.md](./BUILDING.md)


## Tests
The project has been tested as static library on Windows and macos.

Also library wasn't tested on other platforms, but should work on Linux.

## License
Licensed under MIT
