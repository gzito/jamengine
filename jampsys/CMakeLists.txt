set(JAMPSYS_main_SRCS
	PSys.cpp
	PSys_Emitter.cpp
	PSys_Particle3D.cpp
	PSys_ParticleConfigurator.cpp
	PSys_ParticleSprite3D.cpp
)
set(JAMPSYS_main_HDRS
	PSys.h
	PSys_Emitter.h
	PSys_Globals.h
	PSys_Particle.hpp
	PSys_Particle3D.h
	PSys_ParticleConfigurator.h
	PSys_ParticleSprite3D.h
)
set(JAMPSYS_precomp_HDRS
	stdafx.h
)

include_directories( . )
include_directories( ../jam/include )
include_directories( ${PROJECT_SOURCE_DIR}/dependencies/include )
include_directories( ${PROJECT_SOURCE_DIR}/dependencies/include/SDL2 )
include_directories( ${PROJECT_SOURCE_DIR}/dependencies/include/boehmgc )

add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

set( JAMPSYS_THIRDPARTY_LIBRARIES
	opengl32
	debug debug/gc optimized release/gc
	debug debug/SDL2 optimized release/SDL2
	debug debug/SDL2main optimized release/SDL2main
	debug debug/OpenAL32 optimized release/OpenAL32
	debug debug/tinyxml optimized release/tinyxml
	debug debug/libvorbisfile optimized release/libvorbisfile
	debug debug/glew32d optimized release/glew32
	debug debug/Box2D optimized release/Box2D
	debug debug/assimp-vc140-mt optimized release/assimp-vc140-mt )

if(JAM_BUILD_SHARED)
	add_library(JamPSys_shared SHARED
		${JAMPSYS_main_SRCS}
		${JAMPSYS_main_HDRS}
	)

	set_target_properties(JamPSys_shared PROPERTIES
		OUTPUT_NAME "JamPSys"
		CLEAN_DIRECT_OUTPUT 1
		VERSION ${JAM_VERSION}
	)
	
	# has to be defined also for test projects, so public
	target_compile_definitions(JamPSys_shared PUBLIC JAMPSYS_DLL)
	# has to be defined only to library, so private
	target_compile_definitions(JamPSys_shared PRIVATE JAMPSYS_EXPORTS)

	target_precompile_headers(JamPSys_shared PRIVATE ${JAMPSYS_precomp_HDRS})
	target_link_directories(JamPSys_shared PUBLIC ${JAM_thirdPartyLibs_PATH})
	target_link_libraries(JamPSys_shared ${JAMPSYS_THIRDPARTY_LIBRARIES})

endif()

if(JAM_BUILD_STATIC)
	add_library(JamPSys STATIC
		${JAMPSYS_main_SRCS}
		${JAMPSYS_main_HDRS}
	)
	set_target_properties(JamPSys PROPERTIES
		OUTPUT_NAME "JamPSys_s"
		CLEAN_DIRECT_OUTPUT 1
		VERSION ${JAMPSYS_VERSION}
	)

	target_precompile_headers(JamPSys PRIVATE ${JAMPSYS_precomp_HDRS})
	target_link_directories(JamPSys PUBLIC ${JAM_thirdPartyLibs_PATH})
	target_link_libraries(JamPSys ${JAMPSYS_THIRDPARTY_LIBRARIES})
endif()

# These are used to create visual studio folders.
source_group("Source Files" FILES ${JAMPSYS_main_SRCS})
source_group("Header Files" FILES ${JAMPSYS_main_HDRS})

