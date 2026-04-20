//
// Created by gabed on 4/14/2026.
//

#pragma once
#include <optional>
#include <vector>

#include <string>
#include <nlohmann/json.hpp>

#include "../cmake-build-debug/_deps/imgui_node_editor-src/imgui_node_editor.h"
#include "../Pin.h"
#include "Codegen/Visitor.h"

class ErrorManager;
    class Module;
    namespace ed = ax::NodeEditor;

    class Node {

    public:
        virtual ~Node() = default;

        Node(Module* parent, const std::string &name, const std::vector<std::string> &inputs, const std::vector<std::string> &outputs);
        Node(std::string saved_guid, Module* parent, std::string name, std::vector<std::string> inputs, std::vector<std::string> outputs);


        virtual void accept(Visitor& v) = 0;
        void Render(const std::shared_ptr<ErrorManager>& error_manager);
        virtual void RenderInternals();
        [[nodiscard]] virtual std::string type() const = 0;

        [[nodiscard]] virtual nlohmann::json to_json() const {
            return {
                {"type", type()},
                {"guid", guid},
                {"name", name},
                {"id", id.Get()},
                {"x", last_pos.x},
                {"y", last_pos.y}
            };
        }

        std::optional<Pin> FindPin(const std::string& name);

        Module* module;
        std::string name;
        ed::NodeId id;
        std::string guid;
        std::vector<Pin> pins;

        ImVec2 last_pos;
        ImVec2 start_pos;
    };

