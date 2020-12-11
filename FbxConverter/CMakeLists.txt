#
#   Fbx Converter for Project Chimera
#        Jason Cummings
#        Alex Burnley
#        Riley Kirkham
#

cmake_minimum_required( VERSION 3.6 )

project( FbxConverter )
set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake )

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )

LINK_DIRECTORIES( ${CMAKE_BINARY_DIR}/lib )

set( GLM_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../dependencies/glm/ )

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
set(BUILD_SHARED_LIBS TRUE)

if( WIN32 )
	set( CMAKE_CXX_FLAGS "/W3 /EHsc" )
else()
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g ${CXX_WARNING_FLAG} -std=c++17" )
endif()

find_path(FBXSDK_ROOT_DIR
          NAMES include/fbxsdk.h
          HINTS ${EXTERNAL_LIBRARY_DIR}
          PATH_SUFFIXES Autodesk/FBX\ SDK
                        Autodesk/FBX
                        Autodesk/FBX/FBX\ SDK/latest
                        Autodesk/FBX/FBX\ SDK/2020.0.1
                        Autodesk/FBX/FBX\ SDK/2020.1
						Autodesk/FBX\ SDK/2020.0.1
						Autodesk/FBX\ SDK/2020.1
						Autodesk/FBX\ SDK/latest
		  REQUIRED
)
set(FBXSDK_INCLUDE_DIR "${FBXSDK_ROOT_DIR}/include")
message("-- Found FbxSdk includes at ${FBXSDK_INCLUDE_DIR}")

find_library(FBXSDK_LIBRARY_PATH libfbxsdk HINTS $ENV{PATH} REQUIRED)
message("-- Found FbxSdk library at ${FBXSDK_LIBRARY_PATH}")

add_library(libfbxsdk STATIC IMPORTED)
set_target_properties(libfbxsdk PROPERTIES
	IMPORTED_LOCATION ${FBXSDK_LIBRARY_PATH}
    INTERFACE_COMPILE_DEFINITIONS "FBXSDK_SHARED"
)

set( CONVERTER_LIBS
	libfbxsdk
)

set( CONVERTER_INCLUDES
	${FBXSDK_INCLUDE_DIR}
	${GLM_INCLUDE_DIRS}
)

# Only include boost on Mac
if( APPLE )
	find_package( Boost COMPONENTS filesystem REQUIRED )
	set( CONVERTER_LIBS ${CONVERTER_LIBS} ${Boost_LIBRARIES} )
	set( CONVERTER_INCLUDES ${CONVERTER_INCLUDES} ${Boost_INCLUDE_DIRS} )
endif()

include_directories( ${CONVERTER_INCLUDES} )


file(GLOB_RECURSE TARGET_SRC
	src/*.cpp
)

file(GLOB_RECURSE TARGET_INC
	src/*.h
	src/*.hpp
)

set(TARGET_SRC ${TARGET_SRC} ${TARGET_INC})

add_executable(FbxParser ${TARGET_SRC})
target_link_libraries(FbxParser ${CONVERTER_LIBS})
