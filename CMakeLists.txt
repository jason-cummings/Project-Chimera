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

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake )

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )

LINK_DIRECTORIES( ${CMAKE_BINARY_DIR}/lib )

add_subdirectory( dependencies )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++17" )
find_package( OpenGL REQUIRED )


set( SDL2_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/SDL2/include/ )
set( SDL2_LIBRARY SDL2 )

set( BULLET_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/bullet3/src/ )
set( BULLET_LIBRARY BulletDynamics BulletCollision LinearMath )

set( STB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/stb/ )


set( GAME_LIBS
	${OPENGL_LIBRARY}
	${SDL2_LIBRARY}
	${BULLET_LIBRARY}
)
set( GAME_INCLUDES
	${SDL2_INCLUDE_DIR}
	${BULLET_INCLUDE_DIRS}
	${STB_INCLUDE_DIR}
)

include_directories( ${GAME_INCLUDES} )

add_subdirectory( src )
set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT projectchimera )
