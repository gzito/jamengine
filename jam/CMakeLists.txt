set(JAM_CORE_SRCS
	src/core/filesystem.cpp
	src/core/geom.cpp
	src/core/math.cpp	
)
set(JAM_CORE_HDRS
	include/jam/core/array.hpp
	include/jam/core/bmkextras.hpp
	include/jam/core/filesystem.h
	include/jam/core/geom.h
	include/jam/core/interfaces.hpp
	include/jam/core/math.h
	include/jam/core/rbtree.hpp
	include/jam/core/types.h
)

set(JAM_THIRDPARTY_SRCS
	src/thirdparty/stb_image.cpp
	src/thirdparty/HandleManager.cpp
)
set(JAM_THIRDPARTY_HDRS
	include/jam/thirdparty/Delegate.h
	include/jam/thirdparty/DelegateBase.h
	include/jam/thirdparty/dirent.hpp
	include/jam/thirdparty/MultiCastDelegate.h
	include/jam/thirdparty/random.h
	include/jam/thirdparty/SimpleIni.h
	include/jam/thirdparty/Handle.h
	include/jam/thirdparty/HandleManager.h
)

set(JAM_IMGUI_SRCS
	src/thirdparty/imgui/imgui.cpp
	src/thirdparty/imgui/imgui_demo.cpp
	src/thirdparty/imgui/imgui_draw.cpp
	src/thirdparty/imgui/imgui_impl_opengl3.cpp
	src/thirdparty/imgui/imgui_impl_sdl.cpp
	src/thirdparty/imgui/imgui_widgets.cpp	
)
set(JAM_IMGUI_HDRS
	include/jam/thirdparty/imgui/imconfig.h
	include/jam/thirdparty/imgui/imgui.h
	include/jam/thirdparty/imgui/imgui_impl_opengl3.h
	include/jam/thirdparty/imgui/imgui_impl_sdl.h
	include/jam/thirdparty/imgui/imgui_internal.h
	include/jam/thirdparty/imgui/imstb_rectpack.h
	include/jam/thirdparty/imgui/imstb_textedit.h
	include/jam/thirdparty/imgui/imstb_truetype.h
)

set(JAM_MAIN_SRCS
	src/Achievement.cpp	src/Action.cpp	src/ActionEase.cpp	src/ActionInstant.cpp	src/ActionInterval.cpp	src/ActionManager.cpp
	src/Anim2d.cpp	src/Animation2dManager.cpp	src/Application.cpp	src/AudioManager.cpp	src/B2Sprite.cpp	src/Base64.cpp
	src/ButtonNode.cpp	src/Camera.cpp	src/Circle2f.cpp	src/CollisionManager.cpp	src/Color.cpp	src/Component.cpp
	src/Configurator.cpp	src/DeviceManager.cpp	src/Dir.cpp	src/Draw2d.cpp	src/Draw3dBatch.cpp	src/Draw3dManager.cpp
	src/DrawItem.cpp	src/DrawItemManager.cpp	src/Event.cpp	src/ExtAnimator.cpp	src/FrameBufferObject.cpp	src/GameManager.cpp
	src/GameObject.cpp	src/Gfx.cpp	src/Grabber.cpp	src/Grid.cpp	src/InputManager.cpp	src/Layer.cpp	src/Light.cpp
	src/Material.cpp	src/Mesh.cpp	src/Model.cpp	src/Node.cpp	src/Object.cpp	src/Pivot2d.cpp	src/Polygon2f.cpp
	src/Primitives.cpp	src/Quadtree.cpp	src/Randomizer.cpp	src/RefCountedObject.cpp	src/RenderBufferObject.cpp
	src/Resource.cpp	src/ResourceManager.cpp	src/Ring2f.cpp	src/Scene.cpp	src/ScrollingTile.cpp	src/Shader.cpp
	src/ShaderFile.cpp	src/SkinnedMesh.cpp	src/SkinnedModel.cpp	src/SkyBox.cpp	src/Sprite.cpp	src/SpriteBatch.cpp
	src/SpriteMesh.cpp	src/SpritePoolManager.cpp	src/SpriteRenderer.cpp	src/State.cpp	src/StateMachine.cpp	src/StridedVertexBuffer.cpp
	src/String.cpp	src/StringTokenizer.cpp	src/SysTimer.cpp	src/TextNode.cpp	src/Texture2D.cpp	src/Texture2DResource.cpp
	src/TextureCubemap.cpp	src/TightVertexBuffer.cpp	src/Timer.cpp	src/TMXLoader.cpp	src/Transform.cpp	src/VertexArrayObject.cpp
	src/VertexBufferObject.cpp	src/XmlResource.cpp
)
set(JAM_MAIN_HSRS
	include/jam/Achievement.h	include/jam/Action.h	include/jam/ActionEase.h	include/jam/ActionInstant.h	include/jam/ActionInterval.h
	include/jam/ActionManager.h	include/jam/Anim2d.h	include/jam/Animation2dManager.h	include/jam/Application.h	include/jam/AudioManager.h
	include/jam/B2Sprite.h	include/jam/Base64.h	include/jam/BaseManager.hpp	include/jam/ButtonNode.h	include/jam/Camera.h	include/jam/Circle2f.h
	include/jam/CollisionManager.h	include/jam/Color.h	include/jam/Component.h	include/jam/Configurator.h	include/jam/DeviceManager.h	include/jam/Dir.h
	include/jam/Draw2d.h	include/jam/Draw3dBatch.h	include/jam/Draw3dManager.h	include/jam/DrawItem.h	include/jam/DrawItemManager.h
	include/jam/Event.h	include/jam/ExtAnimator.h	include/jam/FrameBufferObject.h	include/jam/GameManager.h	include/jam/GameObject.h	include/jam/Gfx.h
	include/jam/Grabber.h	include/jam/Grid.h	include/jam/InputManager.h	include/jam/IVertexBuffer.hpp	include/jam/jam-config.h	include/jam/jam.h
	include/jam/Layer.h	include/jam/Light.h	include/jam/Material.h	include/jam/Mesh.h	include/jam/Model.h	include/jam/Node.h	include/jam/Object.h
	include/jam/ObjectPool.hpp	include/jam/Pivot2d.h	include/jam/Polygon2f.h	include/jam/Poolable.hpp	include/jam/Primitives.h	include/jam/Quadtree.h
	include/jam/Randomizer.h	include/jam/RefCountedObject.h	include/jam/RenderBufferObject.h	include/jam/Resource.h	include/jam/ResourceManager.h
	include/jam/Ring2f.h	include/jam/Scene.h	include/jam/ScrollingTile.h	include/jam/Shader.h	include/jam/ShaderFile.h	include/jam/Singleton.h
	include/jam/SkinnedMesh.h	include/jam/SkinnedModel.h	include/jam/SkyBox.h	include/jam/Sprite.h	include/jam/SpriteBatch.h
	include/jam/SpriteMesh.h	include/jam/SpritePoolManager.h	include/jam/SpriteRenderer.h	include/jam/State.h	include/jam/StateMachine.h
	include/jam/StridedVertexBuffer.h	include/jam/String.h	include/jam/StringTokenizer.h	include/jam/SysTimer.h	include/jam/TextNode.h
	include/jam/Texture2D.h	include/jam/Texture2DResource.h	include/jam/TextureCubemap.h	include/jam/TightVertexBuffer.h	include/jam/Timer.h
	include/jam/TMXLoader.h	include/jam/Transform.h	include/jam/VertexArrayObject.h	include/jam/VertexBufferObject.h	include/jam/XmlResource.h
	include/jam/Ref.hpp
)

set(JAM_PRECOMP_HDRS
	include/precomph/stdafx.h
	include/precomph/targetver.h
)

include_directories( include )
include_directories( include/precomph )
include_directories( include/jam/thirdparty/imgui )
include_directories( ${PROJECT_SOURCE_DIR}/dependencies/include )
include_directories( ${PROJECT_SOURCE_DIR}/dependencies/include/AL )
include_directories( ${PROJECT_SOURCE_DIR}/dependencies/include/vorbis )
include_directories( ${PROJECT_SOURCE_DIR}/dependencies/include/SDL2 )
include_directories( ${PROJECT_SOURCE_DIR}/dependencies/include/boehmgc )

add_compile_definitions(_USE_MATH_DEFINES)
add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

if(JAM_BUILD_SHARED)
	add_library(Jam_shared SHARED
		${JAM_CORE_SRCS}		${JAM_CORE_HDRS}
		${JAM_THIRDPARTY_SRCS}	${JAM_THIRDPARTY_HDRS}
		${JAM_IMGUI_SRCS}		${JAM_IMGUI_HDRS}
		${JAM_MAIN_SRCS}		${JAM_MAIN_HSRS}
	)

	set_target_properties(Jam_shared PROPERTIES
		OUTPUT_NAME "Jam"
		CLEAN_DIRECT_OUTPUT 1
		VERSION ${JAM_VERSION}
	)
	
	# has to be defined also for test projects, so public
	target_compile_definitions(Jam_shared PUBLIC JAM_DLL)
	# has to be defined only to library, so private
	target_compile_definitions(Jam_shared PRIVATE JAM_EXPORTS)

	target_precompile_headers(Jam_shared PRIVATE ${JAM_PRECOMP_HDRS})
	target_link_directories(Jam_shared PUBLIC ${JAM_THIRDPARTY_LIBRARY_PATH})
	target_link_libraries(Jam_shared ${JAM_THIRDPARTY_LIBRARIES})

endif()

if(JAM_BUILD_STATIC)
	add_library(Jam STATIC
		${JAM_CORE_SRCS}		${JAM_CORE_HDRS}
		${JAM_THIRDPARTY_SRCS}	${JAM_THIRDPARTY_HDRS}
		${JAM_IMGUI_SRCS}		${JAM_IMGUI_HDRS}
		${JAM_MAIN_SRCS}		${JAM_MAIN_HSRS}
	)
	set_target_properties(Jam PROPERTIES
		OUTPUT_NAME "Jam_s"
		CLEAN_DIRECT_OUTPUT 1
		VERSION ${JAM_VERSION}
	)

	target_precompile_headers(Jam PRIVATE ${JAM_PRECOMP_HDRS})
	target_link_directories(Jam PUBLIC ${JAM_THIRDPARTY_LIBRARY_PATH})
	target_link_libraries(Jam ${JAM_THIRDPARTY_LIBRARIES})
endif()

# These are used to create visual studio folders.
source_group("Source Files" FILES ${JAM_MAIN_SRCS})
source_group("Source Files\\core" FILES ${JAM_CORE_SRCS} )
source_group("Source Files\\thirdparty" FILES ${JAM_THIRDPARTY_SRCS} )
source_group("Source Files\\thirdparty\\imgui" FILES ${JAM_IMGUI_SRCS} )
source_group("Header Files" FILES ${JAM_MAIN_HSRS})
source_group("Header Files\\core" FILES ${JAM_CORE_HDRS})
source_group("Header Files\\thirdparty" FILES ${JAM_THIRDPARTY_HDRS})
source_group("Header Files\\thirdparty\\imgui" FILES ${JAM_IMGUI_HDRS})

#if(JAM_INSTALL)
	# install headers
#	install(FILES ${BOX2D_General_HDRS} DESTINATION include/Box2D)
#	install(FILES ${BOX2D_Collision_HDRS} DESTINATION include/Box2D/Collision)
#	install(FILES ${BOX2D_Shapes_HDRS} DESTINATION include/Box2D/Collision/Shapes)
#	install(FILES ${BOX2D_Common_HDRS} DESTINATION include/Box2D/Common)
#	install(FILES ${BOX2D_Dynamics_HDRS} DESTINATION include/Box2D/Dynamics)
#	install(FILES ${BOX2D_Contacts_HDRS} DESTINATION include/Box2D/Dynamics/Contacts)
#	install(FILES ${BOX2D_Joints_HDRS} DESTINATION include/Box2D/Dynamics/Joints)
#	install(FILES ${BOX2D_Rope_HDRS} DESTINATION include/Box2D/Rope)

	# install libraries
#	if(BOX2D_BUILD_SHARED)
#		install(TARGETS Box2D_shared EXPORT Box2D-targets 
#                 LIBRARY DESTINATION ${LIB_INSTALL_DIR}
#                 ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
#                 RUNTIME DESTINATION bin)
#	endif()
#	if(BOX2D_BUILD_STATIC)
#		install(TARGETS Box2D EXPORT Box2D-targets DESTINATION ${LIB_INSTALL_DIR})
#	endif()

	# install build system hooks for third-party apps
#	install(EXPORT Box2D-targets DESTINATION ${LIB_INSTALL_DIR}/Box2D)

#       set (BOX2D_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include)
#       set (BOX2D_INCLUDE_DIRS ${BOX2D_INCLUDE_DIR} )
#       set (BOX2D_LIBRARY_DIRS ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR})
#       set (BOX2D_LIBRARY Box2D)
#       set (BOX2D_LIBRARIES ${BOX2D_LIBRARY})
#       set (BOX2D_USE_FILE ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/cmake/Box2D/UseBox2D.cmake)
#       configure_file(Box2DConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/Box2DConfig.cmake @ONLY ESCAPE_QUOTES)
#       install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Box2DConfig.cmake UseBox2D.cmake DESTINATION ${LIB_INSTALL_DIR}/cmake/Box2D)
#endif(JAM_INSTALL)
