cmake_minimum_required(VERSION 3.16)

project(Jam)

if(UNIX)
	set(JAM_INSTALL_BY_DEFAULT ON)
else(UNIX)
	set(JAM_INSTALL_BY_DEFAULT OFF)
endif(UNIX)

option(JAM_INSTALL "Install JAM libs, includes, and CMake scripts" ${JAM_INSTALL_BY_DEFAULT})
option(JAM_BUILD_SHARED "Build JAM shared libraries" OFF)
option(JAM_BUILD_STATIC "Build JAM static libraries" ON)
option(JAM_BUILD_EXAMPLES "Build JAM examples" ON)

set(JAM_VERSION 1.0.0)
set(LIB_INSTALL_DIR lib${LIB_SUFFIX})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(JAM_thirdPartyLibs_PATH ${PROJECT_SOURCE_DIR}/dependencies/lib/x64)
else()
  set(JAM_thirdPartyLibs_PATH ${PROJECT_SOURCE_DIR}/dependencies/lib/win32)
endif()

# force Unicode over Multi-byte
if(MSVC)
    add_definitions(-DUNICODE -D_UNICODE)
	add_compile_options(/MP)
endif()

add_definitions(-DGC_NOT_DLL)

# The JAM library.
add_subdirectory(jam)

if(JAM_BUILD_EXAMPLES)
  add_subdirectory(tests/FallingBoxes)
  add_subdirectory(tests/SpritesDemo)
  add_subdirectory(tests/TestPrimitives)

  if(MSVC)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT FallingBoxes)
  endif(MSVC)
endif(JAM_BUILD_EXAMPLES)
