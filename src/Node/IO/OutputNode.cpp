//
// Created by gabed on 4/14/2026.
//

#include "OutputNode.h"

#include "Events/NumericParameterChangeCommand.h"
#include "GUID.h"
#include "Module.h"
#include "Pins/Pin.h"

#define IN_PIN_VALUE "Value"

nlohmann::json OutputNode::ToJson() const {
    nlohmann::json j = Node::ToJson();
    j["slot"] = slot;
    return j;
}

void OutputNode::accept(Visitor &v, const int output_slot) { v.visit(*this, output_slot); }

std::shared_ptr<Node> OutputNode::Clone() const {
    return std::make_unique<OutputNode>(module, GUID::generate_guid(), slot);
}

void OutputNode::RenderInternals() {
    ImGui::PushItemWidth(60);
    int old_val = slot;
    bool f = ImGui::SliderInt(("Output Slot##" + guid).c_str(), &slot, 0,
                              static_cast<int>(module->GetOutputs().size()) - 1);
    ImGui::PopItemWidth();

    if (f && slot != old_val) {
        auto cmd = std::make_shared<NumericParameterChangeCommand>(module->shared_from_this(), shared_from_this(),
                                                                   old_val, slot, 0);
        module->ExecuteCommand(cmd);
    }

    ImGui::Text("%s", module->GetOutputs()[slot].name.c_str());
}

Pin OutputNode::GetValueInputPin() { return FindPin(IN_PIN_VALUE).value(); }

OutputNode::OutputNode(Module *module, const std::string &guid, const int output) :
    Node(guid, module, "Output", {{IN_PIN_VALUE, PinDataType(module->GetOutputs()[output].bits)}}, {}) {
    slot = output;
}

void OutputNode::UpdateBits(const int bits) {
    pins.clear();
    const Pin p(IN_PIN_VALUE, ax::NodeEditor::PinKind::Input, *this, 0, PinDataType(bits));
    pins.push_back(p);
}
