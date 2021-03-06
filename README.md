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
You have to download dependencies yourself and build them from sources.

 -  glm 0.9.9.6
 -  Box2D 2.3.0
 -  assimp 5
 -  glew 2.1.0
 -  ogg 1.3.4
 -  vorbis 1.3.6
 -  openal-soft 1.20.1
 -  SDL2 2.0.12
 -  SDL_ttf 2.0.15
 -  tinyxml 2.6.2

Prebuilt win32 and win64 binaries dependencies are provided for your convenience at https://github.com/gzito/jamengine-dependencies

## How to build

You need CMake (at least version 3.16) and Visual Studio 2019

Unpack _jam-dependencies.zip_ file into project root folder.

Run **cmake-gui**: 

- _Where is the sorce code_: select project root folder ;
- _Where to build the binaries_: select a folder of your choice ;


Leave default settings as they are and click **Configure**.

Choose your Visual Studio toolchain and select Win32 or x64 as platform. Then click **Generate**. Lastly open Visual Studio Project and you are done.


## Tests
The project has been tested as static library on Windows 10.

Also library wasn't tested on other platforms, but should work on Linux.

## License
Licensed under MIT
