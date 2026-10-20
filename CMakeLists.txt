cmake_minimum_required(VERSION 3.16)

if(APPLE AND NOT CMAKE_OSX_ARCHITECTURES)
	set(CMAKE_OSX_ARCHITECTURES arm64 CACHE STRING "macOS target architecture")
endif()

project(JamEngine VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(JAM_BUILD_STATIC "Build the static Jam library" ON)
option(JAM_BUILD_SHARED "Build the shared Jam library" OFF)
option(JAM_BUILD_EXAMPLES "Build the 14 example applications" ON)

if(NOT JAM_BUILD_STATIC AND NOT JAM_BUILD_SHARED)
	message(FATAL_ERROR "Enable at least one of JAM_BUILD_STATIC or JAM_BUILD_SHARED")
endif()

if(WIN32 AND NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
	message(FATAL_ERROR "Jam supports only Windows x86_64")
endif()

# Several pinned dependencies use older CMake policy versions. Keep their
# behavior compatible when configuring them with newer CMake releases.
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
include(FetchContent)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build fetched dependencies as static libraries" FORCE)

set(SDL_SHARED OFF CACHE BOOL "Build SDL as a shared library" FORCE)
set(SDL_STATIC ON CACHE BOOL "Build SDL as a static library" FORCE)
set(SDL_STATIC_PIC ON CACHE BOOL "Build SDL static library as position-independent code" FORCE)
set(SDL_TEST OFF CACHE BOOL "Build SDL tests" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Build Assimp tests" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "Build Assimp tools" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "Install Assimp" FORCE)
if(APPLE)
	# Assimp 5.0.1's bundled zlib mistakes TARGET_OS_MAC for classic Mac OS.
	set(ASSIMP_BUILD_ZLIB OFF CACHE BOOL "Use the zlib supplied by the macOS SDK" FORCE)
else()
	set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "Build Assimp's bundled zlib" FORCE)
endif()
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "Build only used Assimp importers" FORCE)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "Disable unused Assimp exporters" FORCE)
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "Build the OBJ importer" FORCE)
set(ASSIMP_BUILD_MD5_IMPORTER ON CACHE BOOL "Build the MD5 importer" FORCE)
set(ASSIMP_BUILD_OGRE_IMPORTER ON CACHE BOOL "Build the Ogre importer" FORCE)
set(ALSOFT_EXAMPLES OFF CACHE BOOL "Build OpenAL Soft examples" FORCE)
set(ALSOFT_UTILS OFF CACHE BOOL "Build OpenAL Soft utilities" FORCE)
set(ALSOFT_TESTS OFF CACHE BOOL "Build OpenAL Soft tests" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "Build dependency tests" FORCE)
set(BUILD_UTILS OFF CACHE BOOL "Build dependency utilities" FORCE)
set(CMAKE_DISABLE_FIND_PACKAGE_BZip2 TRUE)
set(CMAKE_DISABLE_FIND_PACKAGE_PNG TRUE)
set(CMAKE_DISABLE_FIND_PACKAGE_HarfBuzz TRUE)

FetchContent_Declare(glm
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	GIT_TAG 0.9.9.6
	GIT_SHALLOW TRUE
)
FetchContent_Declare(box2d
	GIT_REPOSITORY https://github.com/erincatto/Box2D.git
	GIT_TAG v2.3.0
	GIT_SHALLOW TRUE
)
FetchContent_Declare(assimp
	GIT_REPOSITORY https://github.com/assimp/assimp.git
	GIT_TAG v5.0.1
	GIT_SHALLOW TRUE
)
FetchContent_Declare(glew
	URL https://github.com/nigels-com/glew/releases/download/glew-2.1.0/glew-2.1.0.tgz
	URL_HASH SHA256=04de91e7e6763039bc11940095cd9c7f880baba82196a7765f727ac05a993c95
)
FetchContent_Declare(ogg
	GIT_REPOSITORY https://github.com/xiph/ogg.git
	GIT_TAG v1.3.4
	GIT_SHALLOW TRUE
)
FetchContent_Declare(vorbis
	GIT_REPOSITORY https://github.com/xiph/vorbis.git
	GIT_TAG v1.3.6
	GIT_SHALLOW TRUE
)
FetchContent_Declare(openal_soft
	GIT_REPOSITORY https://github.com/kcat/openal-soft.git
	GIT_TAG openal-soft-1.20.1
	GIT_SHALLOW TRUE
)
FetchContent_Declare(sdl2
	GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
	GIT_TAG release-2.0.12
	GIT_SHALLOW TRUE
)
FetchContent_Declare(sdl2_ttf
	GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
	GIT_TAG release-2.0.15
	GIT_SHALLOW TRUE
)
FetchContent_Declare(freetype
	GIT_REPOSITORY https://github.com/freetype/freetype.git
	GIT_TAG VER-2-10-1
	GIT_SHALLOW TRUE
)
FetchContent_Declare(tinyxml2
	GIT_REPOSITORY https://github.com/leethomason/tinyxml2.git
	GIT_TAG 10.0.0
	GIT_SHALLOW TRUE
)
FetchContent_Declare(stb_image
	URL https://github.com/nothings/stb/archive/f056911.tar.gz
	URL_HASH SHA256=4d05c96640ae3a8cbdafdad8d344b50ce610802f78aee80154acdb8e266282e0
)
FetchContent_Declare(effolkronium_random
	GIT_REPOSITORY https://github.com/ilqvya/random.git
	GIT_TAG v1.5.0
	GIT_SHALLOW TRUE
)

# GLM is header-only for Jam. Its old CMake project enables a large test suite
# by default and applies strict warning flags globally, so expose the fetched
# headers through an interface target instead of building its optional tests.
FetchContent_GetProperties(glm)
if(NOT glm_POPULATED)
	FetchContent_Populate(glm)
endif()
add_library(JamGLM INTERFACE)
target_include_directories(JamGLM INTERFACE "${glm_SOURCE_DIR}")
add_library(glm::glm ALIAS JamGLM)

FetchContent_MakeAvailable(assimp ogg)

# Vorbis 1.3.6 locates Ogg by variables rather than CMake targets. Point it at
# the source-built target so its libraries remain in the generated build graph.
set(OGG_ROOT "${ogg_SOURCE_DIR}" CACHE PATH "Fetched libogg source" FORCE)
set(OGG_INCLUDE_DIRS "${ogg_SOURCE_DIR}/include" CACHE PATH "Fetched libogg headers" FORCE)
set(OGG_LIBRARIES ogg CACHE STRING "Fetched libogg target" FORCE)
FetchContent_MakeAvailable(vorbis)
target_include_directories(vorbisfile PUBLIC
	$<BUILD_INTERFACE:${vorbis_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)
if(APPLE)
	# libogg 1.3.4's macOS type header includes sys/types.h, which no longer
	# supplies the fixed-width integer types on current macOS SDKs.
	target_compile_options(ogg PUBLIC -include stdint.h)
	target_compile_options(vorbis PRIVATE -include stdint.h)
endif()

if(APPLE)
	# FreeType 2.10.1's bundled zlib conflicts with Apple's TARGET_OS_MAC macro.
	# Use the zlib supplied by the macOS SDK for this platform.
	set(FT_WITH_ZLIB ON CACHE BOOL "Use macOS SDK zlib in FreeType" FORCE)
endif()
FetchContent_MakeAvailable(openal_soft sdl2 freetype tinyxml2 stb_image)
if(MINGW AND TARGET SDL2-static)
	# SDL 2.0.12 marks API declarations as dllexport even for static builds.
	# Suppress those exports when SDL is linked into the Jam DLL.
	target_compile_definitions(SDL2-static PRIVATE DECLSPEC=)
endif()
target_include_directories(OpenAL PUBLIC
	$<BUILD_INTERFACE:${openal_soft_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)

add_library(JamSTBImage INTERFACE)
target_include_directories(JamSTBImage INTERFACE "${stb_image_SOURCE_DIR}")

FetchContent_GetProperties(effolkronium_random)
if(NOT effolkronium_random_POPULATED)
	FetchContent_Populate(effolkronium_random)
endif()
add_library(JamRandom INTERFACE)
target_include_directories(JamRandom INTERFACE
	"${effolkronium_random_SOURCE_DIR}/include")

# OpenAL Soft 1.20.1 configures its native code generators in a nested CMake
# invocation, which does not inherit CMAKE_POLICY_VERSION_MINIMUM from here.
# Raise that subproject's minimum to the same compatibility floor used above.
set(JAM_OPENAL_NATIVE_TOOLS_CMAKE
	"${openal_soft_SOURCE_DIR}/native-tools/CMakeLists.txt")
file(READ "${JAM_OPENAL_NATIVE_TOOLS_CMAKE}" JAM_OPENAL_NATIVE_TOOLS_CONTENT)
string(REPLACE
	"cmake_minimum_required(VERSION 3.0.2)"
	"cmake_minimum_required(VERSION 3.5)"
	JAM_OPENAL_NATIVE_TOOLS_CONTENT
	"${JAM_OPENAL_NATIVE_TOOLS_CONTENT}"
)
file(WRITE "${JAM_OPENAL_NATIVE_TOOLS_CMAKE}" "${JAM_OPENAL_NATIVE_TOOLS_CONTENT}")

# SDL 2.0.12 marks a declaration-after-statement warning as an error in an
# Objective-C source that Apple Clang accepts as valid Objective-C.
if(APPLE AND TARGET SDL2-static)
	target_compile_options(SDL2-static PRIVATE -Wno-error=declaration-after-statement)
endif()

# Box2D 2.3.0 predates its CMake build. Build its library sources directly.
FetchContent_GetProperties(box2d)
if(NOT box2d_POPULATED)
	FetchContent_Populate(box2d)
endif()
file(GLOB_RECURSE JAM_BOX2D_SOURCES CONFIGURE_DEPENDS
	"${box2d_SOURCE_DIR}/Box2D/Box2D/*.cpp")
add_library(JamBox2D STATIC ${JAM_BOX2D_SOURCES})
target_include_directories(JamBox2D PUBLIC "${box2d_SOURCE_DIR}/Box2D")
add_library(Box2D::Box2D ALIAS JamBox2D)

# GLEW 2.1.0 keeps its CMake project under build/cmake.
FetchContent_GetProperties(glew)
if(NOT glew_POPULATED)
	FetchContent_Populate(glew)
endif()
add_subdirectory("${glew_SOURCE_DIR}/build/cmake" "${glew_BINARY_DIR}")
target_include_directories(glew_s PUBLIC
	$<BUILD_INTERFACE:${glew_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)

# SDL_ttf 2.0.15's CMake file requires an installed SDL2 package. Create the
# same library target from its source and link it to the fetched SDL and FreeType.
FetchContent_GetProperties(sdl2_ttf)
if(NOT sdl2_ttf_POPULATED)
	FetchContent_Populate(sdl2_ttf)
endif()
add_library(SDL2_ttf STATIC "${sdl2_ttf_SOURCE_DIR}/SDL_ttf.c")
target_include_directories(SDL2_ttf PUBLIC "${sdl2_ttf_SOURCE_DIR}")
target_link_libraries(SDL2_ttf PUBLIC SDL2-static freetype)

find_package(OpenGL REQUIRED)

if(TARGET assimp::assimp)
	set(JAM_ASSIMP_TARGET assimp::assimp)
else()
	set(JAM_ASSIMP_TARGET assimp)
endif()

set(JAM_THIRDPARTY_LIBRARIES
	SDL2-static
	glew_s
	OpenAL
	vorbisfile
	ogg
	${JAM_ASSIMP_TARGET}
	glm::glm
	Box2D::Box2D
	tinyxml2
	JamSTBImage
	JamRandom
	OpenGL::GL
)
set(JAM_APP_THIRDPARTY_LIBRARIES ${JAM_THIRDPARTY_LIBRARIES})
if(WIN32 AND TARGET SDL2main)
	# Ensure MinGW selects the SDL Windows entry point before scanning archives.
	# Example targets link SDL2main before Jam in their directory settings.
	target_link_options(SDL2main INTERFACE -mwindows "-Wl,-u,WinMain")
elseif(WIN32)
	message(FATAL_ERROR "SDL2main is required to build Windows applications")
endif()

if(APPLE)
	add_compile_definitions(GL_SILENCE_DEPRECATION)
endif()
add_compile_definitions(
	SI_NO_CONVERSION
	SI_Case=SI_GenericCase
	SI_NoCase=SI_GenericNoCase
)
if(MSVC)
	add_compile_definitions(UNICODE _UNICODE _CRT_SECURE_NO_WARNINGS)
	add_compile_options(/MP)
else()
	add_compile_definitions(_USE_MATH_DEFINES)
endif()
add_compile_definitions($<$<CONFIG:Debug>:_DEBUG>)

set(JAM_VERSION 1.0.0)
set(JAMPSYS_VERSION 1.0.0)

# Stage the shared shader set for Jam and every example that links it.
add_custom_target(JamShaderResources
	COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/resources/shaders"
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${PROJECT_SOURCE_DIR}/jam/shaders"
		"${PROJECT_BINARY_DIR}/resources/shaders"
	COMMENT "Copying Jam shaders to the build resources directory"
	VERBATIM
)

function(jam_configure_example_resources target)
	set(test_resource_root "${PROJECT_BINARY_DIR}/resources/${target}")
	target_compile_definitions(${target} PRIVATE
		"JAM_TEST_RESOURCE_DIR=\"${test_resource_root}\"")
	add_dependencies(${target} JamShaderResources)
	set_property(TARGET ${target} PROPERTY
		VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_BINARY_DIR}")

	if(ARGN)
		set(resource_copy_commands)
		foreach(resource_dir IN LISTS ARGN)
			set(resource_source "${CMAKE_CURRENT_SOURCE_DIR}/${resource_dir}")
			if(NOT IS_DIRECTORY "${resource_source}")
				message(FATAL_ERROR
					"${target} declares missing resource directory: ${resource_source}")
			endif()
			list(APPEND resource_copy_commands
				COMMAND ${CMAKE_COMMAND} -E copy_directory
					"${resource_source}" "${test_resource_root}/${resource_dir}")
		endforeach()

		add_custom_target(${target}_resources
			COMMAND ${CMAKE_COMMAND} -E make_directory "${test_resource_root}"
			${resource_copy_commands}
			COMMENT "Copying ${target} resources"
			VERBATIM
		)
		add_dependencies(${target} ${target}_resources)
	endif()
endfunction()

add_subdirectory(jam)
if(JAM_BUILD_SHARED)
	set(JAM_CONSUMER_TARGET Jam_shared)
else()
	set(JAM_CONSUMER_TARGET Jam)
endif()
add_subdirectory(jampsys)

if(JAM_BUILD_EXAMPLES)
	set(JAM_EXAMPLE_DIRS
		FallingBoxes
		SpritesDemo
		TestInputManager
		TestPrimitives
		TestPSys
		TestImgui
		TestNormalMapping
		TestMesh
		TestCubeMap
		TestSkinnedMesh
		TestFrameBuffer
		TestEvents
		TestSpritesZOrder
		TestSpriteFont
	)
	foreach(example_dir IN LISTS JAM_EXAMPLE_DIRS)
		add_subdirectory("tests/${example_dir}")
	endforeach()

	if(MSVC)
		set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT FallingBoxes)
	endif()
endif()
