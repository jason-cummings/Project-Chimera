execute_process( COMMAND git submodule update --init --recursive )

# SDL2 
set( SDL_STATIC OFF CACHE BOOL "Build the static SDL2 library" )
set( SDL_SHARED ON CACHE BOOL "Build the shared SDL2 library" )
add_subdirectory( SDL2 )

# bullet3
# Do this last
set( USE_MSVC_RUNTIME_LIBRARY_DLL ON )
set( BUILD_SHARED_LIBS OFF )
set( BUILD_UNIT_TESTS OFF CACHE BOOL "Do not build unit tests for bullet." )
set( BUILD_BULLET2_DEMOS OFF CACHE BOOL "no bullet2 demos." )
set( BUILD_CPU_DEMOS OFF CACHE BOOL "no bullet demos." )
set( BUILD_BULLET3 OFF CACHE BOOL "no bullet 3." )
add_subdirectory( bullet3 )
