add_subdirectory(3d)
add_subdirectory(2d)

set(FILES
        console/console.h
        console/console.cpp
        )

add_library(renderers ${FILES})
target_link_libraries(renderers 2d_engine 3d_engine game)
target_include_directories(renderers INTERFACE ..)
