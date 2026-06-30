//
// Created by Gabe on 5/21/2026.
//


#include "ShifterNode.h"
#include "Events/NumericParameterChangeCommand.h"
#include "GUID.h"
#include "Module.h"

std::shared_ptr<Node> ShifterNode::Clone() const {
    return std::make_unique<ShifterNode>(module, GUID::generate_guid(), bits, type_index);
}
nlohmann::json ShifterNode::ToJson() const {
    nlohmann::json j = ConfigurableBitWidthNode::ToJson();
    j["type_index"] = type_index;
    return j;
}

void ShifterNode::accept(Visitor &v, const int output_slot) { v.visit(*this, output_slot); }

ShifterNode::ShifterNode(Module *module, const std::string &guid, const int bit_width, const int type) :
    ConfigurableBitWidthNode(guid, module, "Shifter", bit_width), type_index(type) {
    InitPinsAfterConfig();
}
ShifterNode::ShifterNode(Module *module) : ConfigurableBitWidthNode(module, "Shifter") {}


int ShifterNode::GetShiftWidth() const { return ceill(log2(bits)); }

void ShifterNode::InitPinsAfterConfig() {
    int n = 0;
    // Inputs
    pins.push_back({SHIFTER_IN_PIN_INPUT, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(bits)});
    pins.push_back({SHIFTER_IN_PIN_DISTANCE, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(GetShiftWidth())});

    // Outputs
    pins.push_back({SHIFTER_OUT_PIN_OUTPUT, ax::NodeEditor::PinKind::Output, *this, n, PinDataType(bits)});
}

Pin ShifterNode::GetInputPin() { return FindPin(SHIFTER_IN_PIN_INPUT).value(); }
Pin ShifterNode::GetDistancePin() { return FindPin(SHIFTER_IN_PIN_DISTANCE).value(); }

std::string ShifterNode::GetShiftOperator(const std::string &in, const std::string &shft) const {
    if (type_index == 0)
        return in + " << " + shft;
    if (type_index == 1)
        return in + " >> " + shft;
    if (type_index == 2)
        return "$signed(" + in + ") >>> " + shft;
    if (type_index == 3)
        return "(" + in + " << " + shft + ") | (" + in + " >> (" + std::to_string(bits) + " - " + shft + "))";
    if (type_index == 4)
        return "(" + in + " >> " + shft + ") | (" + in + " << (" + std::to_string(bits) + " - " + shft + "))";

    return "err";
}

void ShifterNode::RenderInternals() {
    ImGui::PushItemWidth(150);
    int old_val = type_index;
    bool f = ImGui::SliderInt(("##" + guid).c_str(), &type_index, 0, IM_ARRAYSIZE(shift_type_names) - 1);
    ImGui::PopItemWidth();

    if (f && type_index != old_val) {
        auto cmd = std::make_shared<NumericParameterChangeCommand>(module->shared_from_this(), shared_from_this(),
                                                                   old_val, type_index, 0);
        module->ExecuteCommand(cmd);
    }

    ImGui::Text("%s", shift_type_names[type_index]);
}
