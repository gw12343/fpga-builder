//
// Created by gabed on 4/14/2026.
//

#include "InputNode.h"

#include "Module.h"
#include "Pins/Pin.h"

void InputNode::RenderInternals() {
    ImGui::PushItemWidth(60);
    ImGui::SliderInt(("Input Slot##" + guid).c_str(), &slot, 0, static_cast<int>(module->inputs.size()) - 1);
    ImGui::PopItemWidth();

    ImGui::Text("Selected: %s", module->inputs[slot].name.c_str());
}

InputNode::InputNode(Module *module, const std::string &guid, const int input) :
    Node(guid, module, "Input", {}, {{"Value", PinDataType(module->inputs[input].bits)}}) {
    slot = input;
}

void InputNode::UpdateBits(const int bits) {
    pins.clear();
    const Pin p("Value", ax::NodeEditor::PinKind::Output, *this, 0, PinDataType(bits));
    pins.push_back(p);
}
