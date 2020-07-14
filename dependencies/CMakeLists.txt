execute_process( COMMAND git submodule update --init --recursive )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/whereami/src/ )
IF( WIN32 )
add_library( whereami STATIC ${CMAKE_CURRENT_SOURCE_DIR}/whereami/src/whereami.c ${CMAKE_CURRENT_SOURCE_DIR}/whereami/src/whereami.h )
ELSE()
add_library( whereami SHARED ${CMAKE_CURRENT_SOURCE_DIR}/whereami/src/whereami.c ${CMAKE_CURRENT_SOURCE_DIR}/whereami/src/whereami.h )
ENDIF()

# SDL2 
set( SDL_STATIC OFF CACHE BOOL "Build the static SDL2 library" )
set( SDL_SHARED ON CACHE BOOL "Build the shared SDL2 library" )
add_subdirectory( SDL2 )

# glew
set( glew-cmake_BUILD_SHARED ON CACHE BOOL "Build the shared glew library" )
set( glew-cmake_BUILD_STATIC OFF CACHE BOOL "Build the static glew library" )
set( glew-cmake_BUILD_SINGLE_CONTEXT ON CACHE BOOL "Build the single context glew library" )
set( glew-cmake_BUILD_MULTI_CONTEXT OFF CACHE BOOL "Build the multi context glew library" )

add_subdirectory( glew )

# freetype2
set( BUILD_SHARED_LIBS OFF )
add_subdirectory( freetype2 )

# bullet3
# Do this last
set( USE_MSVC_RUNTIME_LIBRARY_DLL ON )
set( BUILD_SHARED_LIBS OFF )
set( BUILD_UNIT_TESTS OFF CACHE BOOL "Do not build unit tests for bullet." )
set( BUILD_BULLET2_DEMOS OFF CACHE BOOL "no bullet2 demos." )
set( BUILD_CPU_DEMOS OFF CACHE BOOL "no bullet demos." )
set( BUILD_BULLET3 OFF CACHE BOOL "no bullet 3." )
add_subdirectory( bullet3 )
