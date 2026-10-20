if(WIN32 AND TARGET SDL2main)
	link_libraries(SDL2main)
endif()

if(JAM_BUILD_SHARED)
	link_libraries(Jam_shared)
else()
	link_libraries(Jam)
endif()

add_executable(TestImgui TestImgui.cpp)
jam_configure_example_resources(TestImgui media)

include_directories( ${PROJECT_SOURCE_DIR}/jam/include )
include_directories( ${PROJECT_SOURCE_DIR}/jam/include/precomph )
include_directories( ${PROJECT_SOURCE_DIR}/jam/include/jam/thirdparty/imgui )

add_compile_definitions(_USE_MATH_DEFINES)
add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

target_link_libraries(TestImgui ${JAM_APP_THIRDPARTY_LIBRARIES} SDL2_ttf)
