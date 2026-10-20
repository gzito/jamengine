if(WIN32 AND TARGET SDL2main)
	link_libraries(SDL2main)
endif()

# TestSpritesZOrder example

if(JAM_BUILD_SHARED)
	link_libraries(Jam_shared)
else()
	link_libraries(Jam)
endif()

add_executable(TestSpritesZOrder TestSpritesZOrder.cpp TestSpritesZOrder.h)
jam_configure_example_resources(TestSpritesZOrder media)

include_directories( ${PROJECT_SOURCE_DIR}/jam/include )
include_directories( ${PROJECT_SOURCE_DIR}/jam/include/precomph )

add_compile_definitions(_USE_MATH_DEFINES)
add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

target_link_libraries(TestSpritesZOrder ${JAM_APP_THIRDPARTY_LIBRARIES})

