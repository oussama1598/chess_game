add_subdirectory(common)
add_subdirectory(3d)
add_subdirectory(2d)

set(FILES
        console/console.h
        console/console.cpp)

add_library(renderers ${FILES})
target_link_libraries(renderers PUBLIC 2d_engine 3d_engine game)
target_include_directories(renderers INTERFACE ..)

add_custom_target(copy_assets ALL
        COMMAND ${CMAKE_COMMAND} -E remove_directory
        ${CMAKE_BINARY_DIR}/bin/assets/
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets/
        ${CMAKE_BINARY_DIR}/bin/assets/
        COMMENT "Copying runtime assets")

add_dependencies(renderers copy_assets)
