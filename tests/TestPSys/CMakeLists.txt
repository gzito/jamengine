if(WIN32 AND TARGET SDL2main)
	link_libraries(SDL2main)
endif()

if(JAM_BUILD_SHARED)
	link_libraries(Jam_shared)
	link_libraries(JamPSys_shared)
else()
	link_libraries(Jam)
	link_libraries(JamPSys)
endif()

add_executable(TestPSys TestPSys.cpp PSysUtilities.cpp PSysUtilities.h PerformanceWindow.cpp PerformanceWindow.h)
jam_configure_example_resources(TestPSys media)

include_directories( ${PROJECT_SOURCE_DIR}/jam/include )
include_directories( ${PROJECT_SOURCE_DIR}/jam/include/precomph )
include_directories( ${PROJECT_SOURCE_DIR}/jampsys )

add_compile_definitions(_USE_MATH_DEFINES)
add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

target_link_libraries(TestPSys ${JAM_APP_THIRDPARTY_LIBRARIES})
