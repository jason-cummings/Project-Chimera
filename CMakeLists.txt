#
#   Project Chimera
#        Jason Cummings
#        Alex Burnley
#        Riley Kirkham
#
#   CMake architecture and files modified from https://github.com/Shervanator/Engine
#

cmake_minimum_required( VERSION 3.6 )

project( Chimera )
set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

# set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake )

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )

LINK_DIRECTORIES( ${CMAKE_BINARY_DIR}/lib )

add_subdirectory( dependencies )

set( SDL2_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/SDL2/include/ )
set( SDL2_LIBRARY SDL2 )

set( GLEW_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glew/include/ )
set( GLEW_LIBRARIES glewd ) #libglew_shared

set( BULLET_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/bullet3/src/ )
set( BULLET_LIBRARY BulletDynamics BulletCollision LinearMath )

set( STB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/stb/ )

set( GLM_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glm/ )

set( WHEREAMI_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/whereami/src/ )
set( WHEREAMI_LIBRARY whereami )

set( FREETYPE_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/freetype2/include/ )
set( FREETYPE_LIBRARIES freetyped )

if( WIN32 )
	set( CMAKE_CXX_FLAGS "/W3 /EHsc" )
else()
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g ${CXX_WARNING_FLAG} -std=c++17" )
endif()

find_package( OpenGL REQUIRED )
find_package( PNG REQUIRED )
find_package( BZIP2 REQUIRED )
find_package( ZLIB REQUIRED )

set( GAME_LIBS
	${OPENGL_LIBRARY}
	${SDL2_LIBRARY}
	${GLEW_LIBRARIES}
	${WHEREAMI_LIBRARY}
	${BULLET_LIBRARY}
	${PNG_LIBRARY}
	${ZLIB_LIBRARY}
	${BZIP2_LIBRARY}
	${FREETYPE_LIBRARIES}
)
set( GAME_INCLUDES
	${GLM_INCLUDE_DIRS}
	${STB_INCLUDE_DIR}
	${SDL2_INCLUDE_DIR}
	${GLEW_INCLUDE_DIRS}
	${WHEREAMI_INCLUDE_DIRS}
	${BULLET_INCLUDE_DIRS}
	${FREETYPE_INCLUDE_DIRS}
)

# Only include boost on Mac
if( APPLE )
	find_package( Boost COMPONENTS filesystem REQUIRED )
	set( GAME_LIBS ${GAME_LIBS} ${Boost_LIBRARIES} )
	set( GAME_INCLUDES ${GAME_INCLUDES} ${Boost_INCLUDE_DIRS} )
endif()


include_directories( ${GAME_INCLUDES} )

add_subdirectory( src )
set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT projectchimera )
