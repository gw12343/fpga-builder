//
// Created by gabed on 4/14/2026.
//

#include "InputNode.h"

#include "Module.h"

void InputNode::RenderInternals() {
    ImGui::PushItemWidth(60);
    ImGui::SliderInt(("Input Slot##" + guid).c_str(), &slot, 0, static_cast<int>(module->inputs.size()) - 1);
    ImGui::PopItemWidth();

    ImGui::Text("Selected: %s", module->inputs[slot].c_str());
}

InputNode::InputNode(Module *module, const std::string &guid, const int input) :
    Node(guid, module, "Input", {}, {{"Value", PinDataType(1)}}) {
    slot = input;
}
