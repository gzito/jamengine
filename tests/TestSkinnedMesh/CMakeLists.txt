if(WIN32 AND TARGET SDL2main)
	link_libraries(SDL2main)
endif()

if(JAM_BUILD_SHARED)
	link_libraries(Jam_shared)
else()
	link_libraries(Jam)
endif()

add_executable(TestSkinnedMesh TestSkinnedMesh.cpp)
jam_configure_example_resources(TestSkinnedMesh models)

include_directories( ${PROJECT_SOURCE_DIR}/jam/include )
include_directories( ${PROJECT_SOURCE_DIR}/jam/include/precomph )

add_compile_definitions(_USE_MATH_DEFINES)
add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

target_link_libraries(TestSkinnedMesh ${JAM_APP_THIRDPARTY_LIBRARIES})

