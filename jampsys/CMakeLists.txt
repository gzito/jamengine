set(JAMPSYS_MAIN_SRCS
	PSys.cpp
	PSys_Emitter.cpp
	PSys_Particle3D.cpp
	PSys_ParticleConfigurator.cpp
	PSys_ParticleSprite3D.cpp
)
set(JAMPSYS_MAIN_HDRS
	PSys.h
	PSys_Emitter.h
	PSys_Globals.h
	PSys_Particle.hpp
	PSys_Particle3D.h
	PSys_ParticleConfigurator.h
	PSys_ParticleSprite3D.h
)
set(JAMPSYS_PRECOMP_HDRS
	stdafx.h
)

include_directories( . )
include_directories( ../jam/include )
include_directories( ${PROJECT_SOURCE_DIR}/dependencies/include )
include_directories( ${PROJECT_SOURCE_DIR}/dependencies/include/SDL2 )
include_directories( ${PROJECT_SOURCE_DIR}/dependencies/include/boehmgc )

add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

if(JAM_BUILD_SHARED)
	add_library(JamPSys_shared SHARED
		${JAMPSYS_MAIN_SRCS}
		${JAMPSYS_MAIN_HDRS}
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

	target_precompile_headers(JamPSys_shared PRIVATE ${JAMPSYS_PRECOMP_HDRS})
	target_link_directories(JamPSys_shared PUBLIC ${JAM_THIRDPARTY_LIBRARY_PATH})

endif()

if(JAM_BUILD_STATIC)
	add_library(JamPSys STATIC
		${JAMPSYS_MAIN_SRCS}
		${JAMPSYS_MAIN_HDRS}
	)
	set_target_properties(JamPSys PROPERTIES
		OUTPUT_NAME "JamPSys_s"
		CLEAN_DIRECT_OUTPUT 1
		VERSION ${JAMPSYS_VERSION}
	)

	target_precompile_headers(JamPSys PRIVATE ${JAMPSYS_PRECOMP_HDRS})
	target_link_directories(JamPSys PUBLIC ${JAM_THIRDPARTY_LIBRARY_PATH})
endif()

# These are used to create visual studio folders.
source_group("Source Files" FILES ${JAMPSYS_MAIN_SRCS})
source_group("Header Files" FILES ${JAMPSYS_MAIN_HDRS})

