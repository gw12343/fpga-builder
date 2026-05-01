//
// Created by gabed on 4/14/2026.
//

#include "Pin.h"
#include "Default/Node.h"
#include "GUID.h"
#include "Link.h"
#include "Module.h"

#include <utility>


Pin::Pin(std::string name, const ax::NodeEditor::PinKind direction, Node &parent, const int index,
         const PinDataType pin_data_type) :
    m_data_type(pin_data_type), m_node(parent), m_direction(direction), m_name(std::move(name)) {

    m_guid = parent.guid + "node" + std::to_string(index);
    m_id = GUID::to_id(m_guid);
}


void Pin::Render() const {
    BeginPin(m_id, m_direction);
    ImGui::Text(m_name.c_str());
    ax::NodeEditor::EndPin();
}

bool Pin::CanConnect(const Pin &other) const {
    if (other.m_id == m_id || other.m_direction == m_direction || other.m_node.id == m_node.id ||
        !other.GetDataType().CanConnect(m_data_type)) {
        return false;
    }

    return true;
}

std::optional<Pin> Pin::GetConnectedPin() const {
    for (const auto module = m_node.module; const auto &link: module->GetLinks()) {
        if (link.input_guid == m_guid) {
            return module->GetPin(link.output_guid);
        }
        if (link.output_guid == m_guid) {
            return module->GetPin(link.input_guid);
        }
    }

    return std::nullopt;
}

int Pin::GetNodeIndex() const {
    for (int i = 0; i < m_node.pins.size(); i++) {
        if (const Pin &pin = m_node.pins[i]; pin.m_guid == m_guid) {
            return i;
        }
    }

    return -1;
}
