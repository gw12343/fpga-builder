//
// Created by gabed on 4/14/2026.
//

#pragma once
#include <imgui.h>
#include <optional>
#include <vector>

#include <nlohmann/json.hpp>
#include <string>


#include "Codegen/Visitor.h"
#include "Pins/Pin.h"
#include "Pins/PinDataType.h"


class ErrorManager;
class Module;

class Node {

public:
    virtual ~Node() = default;

    struct PinCreationData {
        std::string name;
        PinDataType type;
    };


    Node(Module *parent, const std::string &name, const std::vector<PinCreationData> &inputs,
         const std::vector<PinCreationData> &outputs);

    Node(std::string saved_guid, Module *parent, std::string in_name, const std::vector<PinCreationData> &inputs,
         const std::vector<PinCreationData> &outputs);

    Node(Module *parent, const std::string &name);
    Node(std::string saved_guid, Module *parent, std::string name);

    void InitPins(const std::vector<PinCreationData> &inputs, const std::vector<PinCreationData> &outputs);

    virtual void accept(Visitor &v, int output_slot) = 0;

    // Rendering
    void Render(const std::shared_ptr<ErrorManager> &error_manager);
    virtual void RenderInternals();
    [[nodiscard]] virtual int GetNodeWidth() const { return 175; };
    [[nodiscard]] virtual ImVec4 GetUIColor() const { return {1.0, 0.5, 0.5, 1.0}; }

    // Configuration
    [[nodiscard]] virtual bool HasConfiguration() const { return false; }
    virtual void RenderConfiguration() {}
    virtual void InitPinsAfterConfig() {}

    // Serialization
    [[nodiscard]] virtual std::string GetSerializationType() const = 0;
    [[nodiscard]] virtual nlohmann::json ToJson() const {
        return {{"type", GetSerializationType()},
                {"guid", guid},
                {"name", name},
                {"id", id.Get()},
                {"x", last_pos.x},
                {"y", last_pos.y}};
    }

    // Helpers
    std::optional<Pin> FindPin(const std::string &name);

    Module *module;
    std::string name;
    ed::NodeId id;
    std::string guid;
    std::vector<Pin> pins;

    ImVec2 last_pos;
    ImVec2 start_pos;
};
