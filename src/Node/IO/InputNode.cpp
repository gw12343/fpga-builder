//
// Created by gabed on 4/14/2026.
//

#include "InputNode.h"

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
    ImGui::SliderInt(("Input Slot##" + guid).c_str(), &slot, 0, static_cast<int>(module->GetInputs().size()) - 1);
    ImGui::PopItemWidth();

    ImGui::Text("Selected: %s", module->GetInputs()[slot].name.c_str());
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
