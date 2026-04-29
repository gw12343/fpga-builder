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

    ImGui::Text("Selected: %s", module->outputs[slot].name.c_str());
}

Pin OutputNode::GetValueInputPin() { return FindPin(IN_PIN_VALUE).value(); }

OutputNode::OutputNode(Module *module, const std::string &guid, const int output) :
    Node(guid, module, "Output", {{IN_PIN_VALUE, PinDataType(module->outputs[output].bits)}}, {}) {
    slot = output;
}

void OutputNode::UpdateBits(const int bits) {
    pins.clear();
    const Pin p(IN_PIN_VALUE, ax::NodeEditor::PinKind::Input, *this, 0, PinDataType(bits));
    pins.push_back(p);
}
