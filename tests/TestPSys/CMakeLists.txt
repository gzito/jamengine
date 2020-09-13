# TestPSys example

if(JAM_BUILD_SHARED)
	link_libraries(Jam_shared)
	link_libraries(JamPSys_shared)
else()
	link_libraries(Jam)
	link_libraries(JamPSys)
endif()

add_executable(TestPSys TestPSys.cpp PSysUtilities.cpp PSysUtilities.h)

include_directories( ${PROJECT_SOURCE_DIR}/jam/include )
include_directories( ${PROJECT_SOURCE_DIR}/jam/include/precomph )
include_directories( ${PROJECT_SOURCE_DIR}/jampsys )
include_directories( ${PROJECT_SOURCE_DIR}/dependencies/include )
include_directories( ${PROJECT_SOURCE_DIR}/dependencies/include/SDL2 )
include_directories( ${PROJECT_SOURCE_DIR}/dependencies/include/boehmgc )

add_compile_definitions(_USE_MATH_DEFINES)
add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

set_property(TARGET TestPSys PROPERTY 
	VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

file(TO_NATIVE_PATH ${PROJECT_SOURCE_DIR} NATIVE_PROJECT_SOURCE_DIR)
set_property(TARGET TestPSys PROPERTY
	VS_DEBUGGER_ENVIRONMENT "PATH=${NATIVE_PROJECT_SOURCE_DIR}\\dependencies\\bin\\win32\\$(Configuration)"
)

target_link_libraries(TestPSys ${JAM_THIRDPARTY_LIBRARIES})
