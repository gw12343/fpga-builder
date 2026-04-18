    cmake_minimum_required(VERSION 3.30)
    project(fpga_builder)

    set(CMAKE_CXX_STANDARD 20)

    include(FetchContent)

    FetchContent_Declare(
            imgui
            GIT_REPOSITORY https://github.com/ocornut/imgui.git
            GIT_TAG docking
    )

    FetchContent_Declare(
            SDL3
            GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
            GIT_TAG release-3.2.2
    )

    FetchContent_Declare(
            imgui_node_editor
            GIT_REPOSITORY https://github.com/thedmd/imgui-node-editor.git
            GIT_TAG master
    )

    FetchContent_Declare(json
            GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
            GIT_PROGRESS TRUE  GIT_SHALLOW TRUE
            GIT_TAG v3.11.2)

    FetchContent_MakeAvailable(json)

    set(SDL_SHARED OFF CACHE BOOL "" FORCE)
    set(SDL_STATIC ON CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(imgui SDL3 imgui_node_editor json)

    find_package(OpenGL REQUIRED)

    set(IMGUI_SOURCES
            ${imgui_SOURCE_DIR}/imgui.cpp
            ${imgui_SOURCE_DIR}/imgui_demo.cpp
            ${imgui_SOURCE_DIR}/imgui_draw.cpp
            ${imgui_SOURCE_DIR}/imgui_tables.cpp
            ${imgui_SOURCE_DIR}/imgui_widgets.cpp
            ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
            ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
            ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
    )


    add_library(imgui STATIC ${IMGUI_SOURCES})

    target_include_directories(imgui PUBLIC
            ${imgui_SOURCE_DIR}
            ${imgui_SOURCE_DIR}/backends
            ${SDL3_SOURCE_DIR}/include
            ${imgui_SOURCE_DIR}/misc/cpp/
    )

    add_library(imgui_node_editor STATIC
            ${imgui_node_editor_SOURCE_DIR}/crude_json.cpp
            ${imgui_node_editor_SOURCE_DIR}/imgui_canvas.cpp
            ${imgui_node_editor_SOURCE_DIR}/imgui_node_editor.cpp
            ${imgui_node_editor_SOURCE_DIR}/imgui_node_editor_api.cpp
    )

    target_include_directories(imgui_node_editor PUBLIC
            ${imgui_node_editor_SOURCE_DIR}
            ${imgui_SOURCE_DIR}
    )


    add_executable(fpga_builder main.cpp
            Module.cpp
            Module.h
            Renderer.cpp
            Renderer.h
            Pin.cpp
            Pin.h
            GUID.cpp
            GUID.h
            Default/Node.cpp
            Default/Node.h
            Link.cpp
            Link.h
            Default/OutputNode.cpp
            Default/OutputNode.h
            Default/BinaryOperator/BinaryOpNode.cpp
            Default/BinaryOperator/BinaryOpNode.h
            Default/InputNode.cpp
            Default/InputNode.h
            Codegen/Visitor.h
            Codegen/TraversePrint.cpp
            Codegen/TraversePrint.h
            Codegen/Codegen.cpp
            Codegen/Codegen.h
            Default/BinaryOperator/OrNode.h
            Default/BinaryOperator/AndNode.h
            CircuitSerializer.cpp
            CircuitSerializer.h
            Default/MultiplexerNode.h
            Default/BinaryOperator/AndNode.h
            Default/BinaryOperator/XOrNode.h
            Default/LiteralNode.h
    )

    target_include_directories(fpga_builder PUBLIC
            ${imgui_node_editor_SOURCE_DIR}
            ${CMAKE_SOURCE_DIR}
    )

    target_link_libraries(fpga_builder PRIVATE
            imgui
            SDL3::SDL3-static
            OpenGL::GL
            imgui_node_editor
            nlohmann_json::nlohmann_json
    )

    if (WIN32)
        target_link_libraries(fpga_builder PRIVATE
                user32 gdi32 winmm imm32 ole32 oleaut32 version uuid
        )
    endif()