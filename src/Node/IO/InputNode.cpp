//
// Created by gabed on 4/14/2026.
//

#include "InputNode.h"

#include "Events/NumericParameterChangeCommand.h"
#include "Module.h"
#include "Pins/Pin.h"

nlohmann::json InputNode::ToJson() const {
    nlohmann::json j = Node::ToJson();
    j["slot"] = slot;
    return j;
}

std::shared_ptr<Node> InputNode::Clone() const {
    return std::make_unique<InputNode>(module, GUID::generate_guid(), slot);
}

void InputNode::RenderInternals() {
    ImGui::PushItemWidth(60);
    int old_val = slot;
    bool f = ImGui::SliderInt(("Input Slot##" + guid).c_str(), &slot, 0,
                              static_cast<int>(module->GetInputs().size()) - 1);
    ImGui::PopItemWidth();

    if (f && slot != old_val) {
        auto cmd = std::make_shared<NumericParameterChangeCommand>(module->shared_from_this(), shared_from_this(),
                                                                   old_val, slot, 0);
        module->ExecuteCommand(cmd);
    }

    ImGui::Text("%s", module->GetInputs()[slot].name.c_str());
}

InputNode::InputNode(Module *module, const std::string &guid, const int input) :
    Node(guid, module, "Input", {}, {{"Value", PinDataType(module->GetInputs()[input].bits)}}) {
    slot = input;
}

void InputNode::UpdateBits(const int bits) {
    pins.clear();
    const Pin p("Value", ax::NodeEditor::PinKind::Output, *this, 0, PinDataType(bits));
    pins.push_back(p);
}

void InputNode::accept(Visitor &v, const int output_slot) { v.visit(*this, output_slot); }
