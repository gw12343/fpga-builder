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
    data_type(pin_data_type), node(parent), direction(direction), name(std::move(name)) {

    guid = parent.guid + "node" + std::to_string(index);
    id = GUID::to_id(guid);
}


void Pin::Render() const {
    BeginPin(id, direction);
    ImGui::Text(name.c_str());
    ax::NodeEditor::EndPin();
}

bool Pin::CanConnect(const Pin &other) const {
    if (other.id == id || other.direction == direction || other.node.id == node.id ||
        !other.GetDataType().CanConnect(data_type)) {
        return false;
    }

    return true;
}

std::optional<Pin> Pin::GetConnectedPin() const {
    for (const auto module = node.module; const auto &link: module->links) {
        if (link.input_guid == guid) {
            return module->GetPin(link.output_guid);
        }
        if (link.output_guid == guid) {
            return module->GetPin(link.input_guid);
        }
    }

    return std::nullopt;
}

int Pin::GetNodeIndex() const {
    for (int i = 0; i < node.pins.size(); i++) {
        if (const Pin &pin = node.pins[i]; pin.guid == guid) {
            return i;
        }
    }

    return -1;
}
