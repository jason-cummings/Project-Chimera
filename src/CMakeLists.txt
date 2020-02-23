
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
set(BUILD_SHARED_LIBS TRUE)

file(GLOB_RECURSE TARGET_SRC
	*.cpp
)

file(GLOB_RECURSE TARGET_INC
	*.h
	*.hpp
)

set(TARGET_SRC ${TARGET_SRC} ${TARGET_INC})

add_executable(projectchimera ${TARGET_SRC})
target_link_libraries(projectchimera ${GAME_LIBS})
