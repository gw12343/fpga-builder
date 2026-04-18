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
    Pin(std::string name, ax::NodeEditor::PinKind direction, Node& parent, int index);

    void Render() const;


    bool CanConnect(const Pin &other) const;
    std::optional<Pin> GetConnectedPin() const;


    std::string GetName() const { return name; }
    std::string GetGuid() const { return  guid;}
    ax::NodeEditor::PinKind GetDirection() const { return direction; }
    ax::NodeEditor::PinId GetId() const { return id; }
    Node& GetNode() const { return node; }
private:
    Node& node;
    ax::NodeEditor::PinId id;
    ax::NodeEditor::PinKind direction;
    std::string name;
    std::string guid;
};
