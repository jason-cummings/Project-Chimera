
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

# add_custom_command(TARGET projectchimera POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#       ${CMAKE_CURRENT_SOURCE_DIR}/Assets
#       $<TARGET_FILE_DIR:projectchimera>/Assets)

add_custom_target(copy_assets ALL DEPENDS projectchimera COMMAND ${CMAKE_COMMAND} -E copy_directory
      ${CMAKE_CURRENT_SOURCE_DIR}/Assets
      $<TARGET_FILE_DIR:projectchimera>/Assets)
