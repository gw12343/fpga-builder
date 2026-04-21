//
// Created by gabed on 4/14/2026.
//

#pragma once

#include <optional>
#include <string>

#include "imgui_node_editor.h"

class Node;
namespace ed = ax::NodeEditor;

class Pin {

public:
    Pin(std::string name, ax::NodeEditor::PinKind direction, Node &parent, int index);

    void Render() const;


    [[nodiscard]] bool CanConnect(const Pin &other) const;
    [[nodiscard]] std::optional<Pin> GetConnectedPin() const;


    [[nodiscard]] std::string GetName() const { return name; }
    [[nodiscard]] std::string GetGuid() const { return guid; }
    [[nodiscard]] ax::NodeEditor::PinKind GetDirection() const { return direction; }
    [[nodiscard]] ax::NodeEditor::PinId GetId() const { return id; }

    [[nodiscard]] Node &GetNode() const { return node; }
    [[nodiscard]] int GetNodeIndex() const;

private:
    Node &node;
    ax::NodeEditor::PinId id;
    ax::NodeEditor::PinKind direction;
    std::string name;
    std::string guid;
};
