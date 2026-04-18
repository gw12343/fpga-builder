//
// Created by gabed on 4/14/2026.
//

#include "InputNode.h"

#include <iostream>

#include "Module.h"

void InputNode::RenderInternals() {
        ImGui::PushItemWidth(60);
        ImGui::SliderInt(("Input Slot##" + guid).c_str(), &slot, 0, module->inputs.size() - 1);
        ImGui::PopItemWidth();

        ImGui::Text("Selected: %s", module->inputs[slot].c_str());

}

InputNode::InputNode(Module* module, const std::string &guid, const int input) : Node(guid, module, "InputNode", {}, {"Value"}){
        slot = input;
}
