set(FILES
        renderer_3D.h
        renderer_3D.cpp
        engine/shader.cpp
        engine/shader.h

        engine/texture.cpp
        engine/texture.h

        engine/material.cpp
        engine/material.h

        engine/mesh.cpp
        engine/mesh.h

        engine/object.cpp
        engine/object.h

        engine/camera.cpp
        engine/camera.h

        engine/obj_loader.cpp
        engine/obj_loader.h

        engine/light.cpp
        engine/light.h

        engine/window.cpp
        engine/window.h

        engine/scene.cpp
        engine/scene.h

        engine/point_light.cpp
        engine/point_light.h

        engine/directional_light.cpp
        engine/directional_light.h

        engine/vertex.h

        engine/sky_box.cpp
        engine/sky_box.h

        engine/animation_handler.cpp
        engine/animation_handler.h

        vendor/imgui_impl_glfw.cpp
        vendor/imgui_impl_glfw.h

        vendor/imgui_impl_opengl3.cpp
        vendor/imgui_impl_opengl3.h)

add_library(3d_engine ${FILES})
target_link_libraries(3d_engine CONAN_PKG::glew CONAN_PKG::glfw CONAN_PKG::soil2 CONAN_PKG::imgui game)
target_include_directories(3d_engine INTERFACE ..)
