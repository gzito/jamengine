# jamengine

Simple C++ game engine

## How to build

You need CMake (at least version 3.16) and Visual Studio 2019

Unpack _jam-dependencies.zip_ file into project root folder.

Run **cmake-gui**: 

- _Where is the sorce code_: select project root folder ;
- _Where to build the binaries_: select a folder of your choice ;


Leave default settings as they are and click **Configure**.

Choose your Visual Studio toolchain and select Win32 as platform. Then click **Generate**. Lastly open Visual Studio Project and you are done.


## Tests
The project has been tested as static win32 library on Windows 10.  The x64 build hasn't been tested yet.

Also library wasn't tested on other platforms, but should work on Linux.
