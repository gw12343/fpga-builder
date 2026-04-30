//
// Created by gabed on 4/14/2026.
//

#pragma once

#include <optional>
#include <string>

#include "PinDataType.h"
#include "imgui_node_editor.h"


class Node;

class Pin {

public:
    Pin(std::string name, ax::NodeEditor::PinKind direction, Node &parent, int index, PinDataType pin_data_type);

    void Render() const;


    [[nodiscard]] bool CanConnect(const Pin &other) const;
    [[nodiscard]] std::optional<Pin> GetConnectedPin() const;


    [[nodiscard]] std::string GetName() const { return name; }
    [[nodiscard]] std::string GetGuid() const { return guid; }
    [[nodiscard]] ax::NodeEditor::PinKind GetDirection() const { return direction; }
    [[nodiscard]] ax::NodeEditor::PinId GetId() const { return id; }
    [[nodiscard]] PinDataType GetDataType() const { return data_type; }

    [[nodiscard]] Node &GetNode() const { return node; }
    [[nodiscard]] int GetNodeIndex() const;

private:
    PinDataType data_type;
    Node &node;
    ax::NodeEditor::PinId id;
    ax::NodeEditor::PinKind direction;
    std::string name;
    std::string guid;
};
