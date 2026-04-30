    cmake_minimum_required(VERSION 3.28.1)
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
            GIT_PROGRESS TRUE GIT_SHALLOW TRUE
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


    add_executable(fpga_builder src/main.cpp
            src/Module.cpp
            src/Module.h
            src/UI/Renderer.cpp
            src/UI/Renderer.h
            src/Pins/Pin.cpp
            src/Pins/Pin.h
            src/GUID.cpp
            src/GUID.h
            src/Default/Node.cpp
            src/Default/Node.h
            src/Link.cpp
            src/Link.h
            src/Default/OutputNode.cpp
            src/Default/OutputNode.h
            src/Default/BinaryOperator/BinaryOpNode.h
            src/Default/InputNode.cpp
            src/Default/InputNode.h
            src/Codegen/Visitor.h
            src/Codegen/TraversePrint.cpp
            src/Codegen/TraversePrint.h
            src/Codegen/Codegen.cpp
            src/Codegen/Codegen.h
            src/Default/BinaryOperator/OrNode.h
            src/Default/BinaryOperator/AndNode.h
            src/CircuitSerializer.cpp
            src/CircuitSerializer.h
            src/Default/MultiplexerNode.h
            src/Default/BinaryOperator/AndNode.h
            src/Default/BinaryOperator/XOrNode.h
            src/Default/LiteralNode.h
            src/Default/BinaryOperator/NorNode.h
            src/Codegen/Codegen.cpp
            src/Codegen/Codegen.h
            src/Codegen/Codegen.h
            src/Default/DFFNode.h
            src/Default/DebounceNode.h
            src/Default/EdgeNode.h
            src/UI/ErrorManager.cpp
            src/UI/ErrorManager.h
            src/Pins/PinDataType.h
            src/UI/Lib/IconsFontAwesome6.h
            src/Default/ClockNode.h
            src/Default/UnaryOperator/UnaryOpNode.h
            src/Default/UnaryOperator/NotNode.h
            src/Pins/PinDataType.cpp
            src/Default/SplitterNode.h
            src/Default/CounterNode.h
            src/Default/CombinerNode.h
            src/Default/RegisterNode.h
            src/Default/AdderNode.h
            src/Default/ConfigurableBitWidthNode.h
            src/Default/ConfigurableBitWidthNode.h
            src/Default/ConfigurableDataAndSelectBitWidthNode.h
            src/UI/ConfigManager.cpp
            src/UI/ConfigManager.h
            src/UI/Toolbox.cpp
            src/UI/Toolbox.h
            src/UI/Topbar.cpp
            src/UI/Topbar.h
            src/UI/Lib/ImGuiNotify.h
            src/UI/Lib/TextEditor.h
            src/UI/Lib/TextEditor.cpp
            src/UI/OutputViewer.cpp
            src/UI/OutputViewer.h
            src/CopyPasteManager.cpp
            src/CopyPasteManager.h
            src/UI/ModuleUI.cpp
            src/Project/Project.h
            src/Project/Project.cpp
            src/Default/CustomModuleNode.h
    )

    target_include_directories(fpga_builder PUBLIC
            ${imgui_node_editor_SOURCE_DIR}
            ${CMAKE_SOURCE_DIR}/src
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
    endif ()