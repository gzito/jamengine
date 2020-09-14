# jamengine

## What is jamengine
A simple-to-use C++ game engine built upon OpenGL 3.2 and SDL 2

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
