//
// Created by gabed on 4/14/2026.
//

#include "OutputNode.h"

#include <iostream>

#include "Module.h"

#define IN_PIN_VALUE "Value"

void OutputNode::RenderInternals() {
    ImGui::PushItemWidth(60);
    ImGui::SliderInt(("Output Slot##" + guid).c_str(), &slot, 0, static_cast<int>(module->outputs.size()) - 1);
    ImGui::PopItemWidth();

    ImGui::Text("Selected: %s", module->outputs[slot].c_str());
}

Pin OutputNode::GetValueInputPin() { return FindPin(IN_PIN_VALUE).value(); }

OutputNode::OutputNode(Module *module, const std::string &guid, int output) :
    Node(guid, module, "Output", {{IN_PIN_VALUE, PinDataType(1)}}, {}) {
    slot = output;
}
