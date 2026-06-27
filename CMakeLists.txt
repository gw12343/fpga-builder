    cmake_minimum_required(VERSION 3.28.1)
    project(fpga_builder)

    set(CMAKE_CXX_STANDARD 20)

    include(FetchContent)

    FetchContent_Declare(
            imgui
            GIT_REPOSITORY https://github.com/ocornut/imgui.git
            GIT_TAG docking
    )

    if (NOT EMSCRIPTEN)
        FetchContent_Declare(
                SDL3
                GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
                GIT_TAG release-3.2.2
        )
        set(SDL_SHARED OFF CACHE BOOL "" FORCE)
        set(SDL_STATIC ON CACHE BOOL "" FORCE)
    endif ()

    FetchContent_Declare(
            imgui_node_editor
            GIT_REPOSITORY https://github.com/thedmd/imgui-node-editor.git
            GIT_TAG master
    )

    FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
    FetchContent_MakeAvailable(json)

    if (NOT EMSCRIPTEN)
        FetchContent_MakeAvailable(imgui SDL3 imgui_node_editor json)
        find_package(OpenGL REQUIRED)
    else ()
        FetchContent_MakeAvailable(imgui imgui_node_editor json)
    endif ()


    set(IMGUI_SOURCES
            ${imgui_SOURCE_DIR}/imgui.cpp
            ${imgui_SOURCE_DIR}/imgui_demo.cpp
            ${imgui_SOURCE_DIR}/imgui_draw.cpp
            ${imgui_SOURCE_DIR}/imgui_tables.cpp
            ${imgui_SOURCE_DIR}/imgui_widgets.cpp
            ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
            ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
            ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
            src/Events/CreateNodeCommand.h
    )


    add_library(imgui STATIC ${IMGUI_SOURCES})

    if (EMSCRIPTEN)
        target_include_directories(imgui PUBLIC
                ${imgui_SOURCE_DIR}
                ${imgui_SOURCE_DIR}/backends
                ${imgui_SOURCE_DIR}/misc/cpp/
        )

        target_compile_options(imgui PUBLIC "-sUSE_SDL=3")
    else ()
        target_include_directories(imgui PUBLIC
                ${imgui_SOURCE_DIR}
                ${imgui_SOURCE_DIR}/backends
                ${SDL3_SOURCE_DIR}/include
                ${imgui_SOURCE_DIR}/misc/cpp/
        )
    endif ()


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


    add_executable(fpga_builder
            src/main.cpp

            src/CircuitSerializer.h src/CircuitSerializer.cpp
            src/GUID.h src/GUID.cpp
            src/Link.h src/Link.cpp
            src/Module.h src/Module.cpp
            src/pch.h

            src/Codegen/Codegen.h src/Codegen/Codegen.cpp
            src/Codegen/ConstExprEvaluator.h src/Codegen/ConstExprEvaluator.cpp
            src/Codegen/ResultSpace.h
            src/Codegen/Visitor.h

            src/Node/Arithmetic/AdderNode.h src/Node/Arithmetic/AdderNode.cpp
            src/Node/Arithmetic/MultiplierNode.h src/Node/Arithmetic/MultiplierNode.cpp
            src/Node/Arithmetic/SubtractorNode.h src/Node/Arithmetic/SubtractorNode.cpp

            src/Node/BitwiseOperator/AndNode.h src/Node/BitwiseOperator/AndNode.cpp
            src/Node/BitwiseOperator/BinaryOpNode.h src/Node/BitwiseOperator/BinaryOpNode.cpp
            src/Node/BitwiseOperator/NandNode.h src/Node/BitwiseOperator/NandNode.cpp
            src/Node/BitwiseOperator/NorNode.h src/Node/BitwiseOperator/NorNode.cpp
            src/Node/BitwiseOperator/OrNode.h src/Node/BitwiseOperator/OrNode.cpp
            src/Node/BitwiseOperator/XOrNode.h src/Node/BitwiseOperator/XOrNode.cpp

            src/Node/Memory/CounterNode.h src/Node/Memory/CounterNode.cpp
            src/Node/Memory/DFFNode.h src/Node/Memory/DFFNode.cpp
            src/Node/Memory/RAMNode.h src/Node/Memory/RAMNode.cpp
            src/Node/Memory/RegisterNode.h src/Node/Memory/RegisterNode.cpp
            src/Node/Memory/ROMNode.h src/Node/Memory/ROMNode.cpp

            src/Node/BitwiseOperator/NotNode.h src/Node/BitwiseOperator/NotNode.cpp
            src/Node/BitwiseOperator/UnaryOpNode.h src/Node/BitwiseOperator/UnaryOpNode.cpp

            src/Node/Wiring/LiteralNode.h src/Node/Wiring/LiteralNode.cpp
            src/Node/Wiring/MultiplexerNode.h src/Node/Wiring/MultiplexerNode.cpp

            src/Node/Wiring/BitSelectorNode.h src/Node/Wiring/BitSelectorNode.cpp
            src/Node/Default/ClockNode.h src/Node/Default/ClockNode.cpp
            src/Node/Wiring/CombinerNode.h src/Node/Wiring/CombinerNode.cpp
            src/Node/Default/ComparatorNode.h src/Node/Default/ComparatorNode.cpp
            src/Node/ConfigurableBitRangeNode.h src/Node/ConfigurableBitRangeNode.cpp
            src/Node/ConfigurableBitWidthNode.h src/Node/ConfigurableBitWidthNode.cpp
            src/Node/ConfigurableDataAndNumInputsNode.h src/Node/ConfigurableDataAndNumInputsNode.cpp
            src/Node/ConfigurableDataAndSelectBitWidthNode.h src/Node/ConfigurableDataAndSelectBitWidthNode.cpp
            src/Node/CustomModuleNode.h src/Node/CustomModuleNode.cpp
            src/Node/Default/DebounceNode.h src/Node/Default/DebounceNode.cpp
            src/Node/Default/DecoderNode.h src/Node/Default/DecoderNode.cpp
            src/Node/Default/EdgeNode.h src/Node/Default/EdgeNode.cpp
            src/Node/IO/InputNode.h src/Node/IO/InputNode.cpp
            src/Node/Node.h src/Node/Node.cpp
            src/Node/IO/OutputNode.h src/Node/IO/OutputNode.cpp
            src/Node/BitwiseOperator/ShifterNode.h src/Node/BitwiseOperator/ShifterNode.cpp
            src/Node/Wiring/SplitterNode.h src/Node/Wiring/SplitterNode.cpp

            src/Pins/Pin.h src/Pins/Pin.cpp
            src/Pins/PinDataType.h src/Pins/PinDataType.cpp

            src/Project/Project.h src/Project/Project.cpp

            src/UI/ConfigManager.h src/UI/ConfigManager.cpp
            src/UI/ErrorManager.h src/UI/ErrorManager.cpp
            src/UI/ModuleUI.cpp
            src/UI/NodeUI.cpp
            src/UI/OutputViewer.h src/UI/OutputViewer.cpp
            src/UI/ProjectUI.cpp
            src/UI/Renderer.h src/UI/Renderer.cpp
            src/UI/Toolbox.h src/UI/Toolbox.cpp
            src/UI/Topbar.h src/UI/Topbar.cpp

            src/UI/Lib/IconsFontAwesome6.h
            src/UI/Lib/ImGuiNotify.h
            src/UI/Lib/TextEditor.h src/UI/Lib/TextEditor.cpp
            src/Events/Command.h
            src/Events/CreateNodeCommand.cpp
            src/Events/MoveNodesCommand.h
            src/Events/MoveNodesCommand.cpp
            src/Events/CreateLinkCommand.h
            src/Events/CreateLinkCommand.cpp
            src/Events/DeleteLinkCommand.cpp
            src/Events/DeleteLinkCommand.h
            src/Events/DeleteNodeCommand.h
            src/Events/DeleteNodeCommand.cpp
            src/Events/PasteCommand.h
            src/Events/PasteCommand.cpp
            src/Events/NumericParameterChangeCommand.h
            src/Events/NumericParameterChangeCommand.cpp
            src/Events/StringParameterChangeCommand.h
            src/Events/StringParameterChangeCommand.cpp
            src/Node/Wiring/InputTunnelNode.h
            src/Node/Wiring/InputTunnelNode.cpp
            src/Node/Wiring/OutputTunnelNode.h
            src/Node/Wiring/OutputTunnelNode.cpp
            src/Node/Wiring/ConfigurableTunnelNode.h
            src/Node/Wiring/ConfigurableTunnelNode.cpp
            src/Events/CreateIOCommand.h
            src/Events/CreateIOCommand.cpp
            src/Events/DeleteIOCommand.h
            src/Events/DeleteIOCommand.cpp
            src/Events/RenameIOCommand.h
            src/Events/RenameIOCommand.cpp
            src/Events/RenameIOCommand.cpp
            src/Events/ChangeIOWidthCommand.h
            src/Events/ChangeIOWidthCommand.cpp
            src/Events/ChangeIOWidthCommand.cpp
    )

    target_include_directories(fpga_builder PUBLIC
            ${imgui_node_editor_SOURCE_DIR}
            ${CMAKE_SOURCE_DIR}/src
    )

    target_link_libraries(fpga_builder PRIVATE
            imgui
            imgui_node_editor
            nlohmann_json::nlohmann_json
    )

    if (EMSCRIPTEN)
        # Convert paths safely
        file(TO_CMAKE_PATH "${CMAKE_SOURCE_DIR}/resources" CLEAN_RESOURCES_DIR)
        file(TO_CMAKE_PATH "${CMAKE_SOURCE_DIR}/Project" CLEAN_PROJECT_DIR)
        file(TO_CMAKE_PATH "${CMAKE_SOURCE_DIR}/imgui.ini" CLEAN_INI_FILE)

        target_compile_options(fpga_builder PRIVATE "-O3")

        # Use target_link_options with SHELL: for flags that contain spaces
        target_link_options(fpga_builder PRIVATE
                "-sUSE_SDL=3"
                "-sUSE_WEBGL2=1"
                "-sFULL_ES3=1"
                "-sWASM=1"
                "-sFORCE_FILESYSTEM=1"
                "SHELL:--preload-file ${CLEAN_RESOURCES_DIR}@/resources"
                "SHELL:--preload-file ${CLEAN_PROJECT_DIR}@/Project"
                "SHELL:--preload-file ${CLEAN_INI_FILE}@/imgui.ini"
        )

        # Also pass dependencies down to imgui target
        target_link_options(imgui PRIVATE "-sUSE_SDL=3" "-sUSE_WEBGL2=1")

        set_target_properties(fpga_builder PROPERTIES SUFFIX ".html")
    else ()
        # Desktop libs
        target_link_libraries(fpga_builder PRIVATE
                SDL3::SDL3-static
                OpenGL::GL
        )
    endif ()


    target_precompile_headers(fpga_builder PRIVATE ${CMAKE_SOURCE_DIR}/src/pch.h)

    if (WIN32 AND NOT EMSCRIPTEN)
        target_link_libraries(fpga_builder PRIVATE
                user32 gdi32 winmm imm32 ole32 oleaut32 version uuid
        )
    endif ()