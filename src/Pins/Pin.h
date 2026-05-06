//
// Created by gabed on 4/14/2026.
//

#pragma once

#include "PinDataType.h"


class Node;

class Pin {

public:
    Pin(std::string name, ax::NodeEditor::PinKind direction, Node &parent, int index, PinDataType pin_data_type);

    void Render() const;


    [[nodiscard]] bool CanConnect(const Pin &other) const;
    [[nodiscard]] std::optional<Pin> GetConnectedPin() const;


    [[nodiscard]] std::string GetName() const { return m_name; }
    [[nodiscard]] std::string GetGuid() const { return m_guid; }
    [[nodiscard]] ax::NodeEditor::PinKind GetDirection() const { return m_direction; }
    [[nodiscard]] ax::NodeEditor::PinId GetId() const { return m_id; }
    [[nodiscard]] PinDataType GetDataType() const { return m_data_type; }

    [[nodiscard]] Node &GetNode() const { return m_node; }
    [[nodiscard]] int GetNodeIndex() const;

private:
    PinDataType m_data_type;
    Node &m_node;
    ax::NodeEditor::PinId m_id;
    ax::NodeEditor::PinKind m_direction;
    std::string m_name;
    std::string m_guid;
};
