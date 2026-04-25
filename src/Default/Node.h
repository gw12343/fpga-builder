//
// Created by gabed on 4/14/2026.
//

#pragma once
#include <optional>
#include <vector>

#include <nlohmann/json.hpp>
#include <string>

#include "../cmake-build-debug/_deps/imgui_node_editor-src/imgui_node_editor.h"
#include "Codegen/Visitor.h"
#include "Pins/Pin.h"

class ErrorManager;
class Module;
namespace ed = ax::NodeEditor;

class Node {

public:
    virtual ~Node() = default;

    struct PinCreationData {
        std::string name;
        PinDataType type;
    };


    Node(Module *parent, const std::string &name, const std::vector<PinCreationData> &inputs,
         const std::vector<PinCreationData> &outputs);

    Node(std::string saved_guid, Module *parent, std::string name, const std::vector<PinCreationData> &inputs,
         const std::vector<PinCreationData> &outputs);


    virtual void accept(Visitor &v, int output_slot) = 0;
    void Render(const std::shared_ptr<ErrorManager> &error_manager);
    virtual void RenderInternals();

    [[nodiscard]] virtual bool HasConfiguration() const { return false; }

    [[nodiscard]] virtual std::string GetSerializationType() const = 0;
    [[nodiscard]] virtual int GetNodeWidth() const { return 175; };
    [[nodiscard]] virtual ImVec4 GetUIColor() const { return {1.0, 0.5, 0.5, 1.0}; }

    [[nodiscard]] virtual nlohmann::json ToJson() const {
        return {{"type", GetSerializationType()},
                {"guid", guid},
                {"name", name},
                {"id", id.Get()},
                {"x", last_pos.x},
                {"y", last_pos.y}};
    }

    std::optional<Pin> FindPin(const std::string &name);

    Module *module;
    std::string name;
    ed::NodeId id;
    std::string guid;
    std::vector<Pin> pins;

    ImVec2 last_pos;
    ImVec2 start_pos;
};
