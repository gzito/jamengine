cmake_minimum_required(VERSION 3.16)

project(Jam)

#if(UNIX)
#	set(JAM_INSTALL_BY_DEFAULT ON)
#else(UNIX)
#	set(JAM_INSTALL_BY_DEFAULT OFF)
#endif(UNIX)

#option(JAM_INSTALL "Install JAM libs, includes, and CMake scripts" ${JAM_INSTALL_BY_DEFAULT})
#option(JAM_BUILD_SHARED "Build JAM shared libraries" OFF)
option(JAM_BUILD_STATIC "Build JAM static libraries" ON)
option(JAM_BUILD_EXAMPLES "Build JAM examples" ON)

set(JAM_VERSION 1.0.0)
set(JAMPSYS_VERSION 1.0.0)
set(LIB_INSTALL_DIR lib${LIB_SUFFIX})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(JAM_THIRDPARTY_LIBRARY_PATH ${PROJECT_SOURCE_DIR}/dependencies/lib/x64)
	set(JAM_THIRDPARTY_BINARY_PATH  ${PROJECT_SOURCE_DIR}/dependencies/bin/x64)

	set(JAM_THIRDPARTY_LIBRARIES
		opengl32
		SDL2
		SDL2main
		OpenAL32
		debug debug/tinyxml optimized release/tinyxml
		debug debug/vorbisfile optimized release/vorbisfile
		release/glew32
		debug debug/Box2D optimized release/Box2D
		debug debug/assimp-vc142-mtd optimized release/assimp-vc142-mt )
else()
	set(JAM_THIRDPARTY_LIBRARY_PATH ${PROJECT_SOURCE_DIR}/dependencies/lib/win32)
	set(JAM_THIRDPARTY_BINARY_PATH  ${PROJECT_SOURCE_DIR}/dependencies/bin/win32)

	set(JAM_THIRDPARTY_LIBRARIES
		opengl32
		SDL2
		SDL2main
		OpenAL32
		debug debug/tinyxml optimized release/tinyxml
		debug debug/vorbisfile optimized release/vorbisfile
		debug debug/glew32d optimized release/glew32
		debug debug/Box2D optimized release/Box2D
		debug debug/assimp-vc140-mt optimized release/assimp-vc140-mt )
endif()

# force Unicode over Multi-byte
if(MSVC)
    add_definitions(-DUNICODE -D_UNICODE)
	add_compile_options(/MP)
endif()

add_definitions(-DGC_NOT_DLL)

# The JAM library.
add_subdirectory(jam)
add_subdirectory(jampsys)

if(JAM_BUILD_EXAMPLES)
  add_subdirectory(tests/FallingBoxes)
  add_subdirectory(tests/SpritesDemo)
  add_subdirectory(tests/TestInputManager)
  add_subdirectory(tests/TestPrimitives)
  add_subdirectory(tests/TestPSys)
  add_subdirectory(tests/TestImgui)
  add_subdirectory(tests/TestNormalMapping)
  add_subdirectory(tests/TestMesh)
  add_subdirectory(tests/TestCubeMap)
  add_subdirectory(tests/TestSkinnedMesh)
  add_subdirectory(tests/TestFrameBuffer)
  add_subdirectory(tests/TestEvents)
  add_subdirectory(tests/TestSpritesZorder)
  
  if(MSVC)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT FallingBoxes)
  endif(MSVC)
endif(JAM_BUILD_EXAMPLES)
