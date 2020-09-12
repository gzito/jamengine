# TestImgui example

if(JAM_BUILD_SHARED)
	link_libraries(Jam_shared)
else()
	link_libraries(Jam)
endif()

add_executable(TestImgui TestImgui.cpp)

include_directories( ${PROJECT_SOURCE_DIR}/jam/include )
include_directories( ${PROJECT_SOURCE_DIR}/jam/include/precomph )
include_directories( ${PROJECT_SOURCE_DIR}/jam/include/jam/thirdparty/imgui )
include_directories( ${PROJECT_SOURCE_DIR}/dependencies/include )
include_directories( ${PROJECT_SOURCE_DIR}/dependencies/include/SDL2 )
include_directories( ${PROJECT_SOURCE_DIR}/dependencies/include/boehmgc )

add_compile_definitions(_USE_MATH_DEFINES)
add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

set( JAM_THIRDPARTY_LIBRARIES
	opengl32
	debug debug/gc optimized release/gc
	debug debug/SDL2 optimized release/SDL2
	debug debug/SDL2main optimized release/SDL2main
	debug debug/OpenAL32 optimized release/OpenAL32
	debug debug/tinyxml optimized release/tinyxml
	debug debug/libvorbisfile optimized release/libvorbisfile
	debug debug/glew32d optimized release/glew32
	debug debug/Box2D optimized release/Box2D
	debug debug/assimp-vc140-mt optimized release/assimp-vc140-mt
	release/SDL2_ttf
)

set_property(TARGET TestImgui PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${SOURCE_DIR})

target_link_libraries(TestImgui ${JAM_THIRDPARTY_LIBRARIES})
