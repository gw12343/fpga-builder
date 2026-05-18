//
// Created by gabed on 4/14/2026.
//

#pragma once
#include <imgui.h>
#include <imgui_node_editor.h>
#include <memory>
#include <string>


#include "ConfigurableBitWidthNode.h"
#include "GUID.h"
#include "Node.h"

static auto SHIFTER_IN_PIN_INPUT = "Input";
static auto SHIFTER_IN_PIN_DISTANCE = "Distance";

static auto SHIFTER_OUT_PIN_OUTPUT = "Output";


class ShifterNode final : public ConfigurableBitWidthNode {
public:
    [[nodiscard]] std::string GetSerializationType() const override { return "ShifterNode"; }

    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }

    [[nodiscard]] std::shared_ptr<Node> Clone() const override {
        return std::make_unique<ShifterNode>(module, GUID::generate_guid(), bits, type_index);
    }

    int type_index = 0;

    [[nodiscard]] nlohmann::json ToJson() const override {
        nlohmann::json j = ConfigurableBitWidthNode::ToJson();
        j["type_index"] = type_index;
        return j;
    }

    static constexpr ImVec4 COLOR = {0.560f, 0.1f, 0.07f, 1.0f};
    [[nodiscard]] ImVec4 GetUIColor() const override { return COLOR; }


    // Pre-configured
    ShifterNode(Module *module, const std::string &guid, const int bit_width, const int type) :
        ConfigurableBitWidthNode(guid, module, "Shifter", bit_width), type_index(type) {
        InitPinsAfterConfig();
    }

    explicit ShifterNode(Module *module) : ConfigurableBitWidthNode(module, "Shifter") {}

    [[nodiscard]] int GetShiftWidth() const { return ceill(log2(bits)); }

    void InitPinsAfterConfig() override {
        int n = 0;
        // Inputs
        pins.push_back((Pin){SHIFTER_IN_PIN_INPUT, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(bits)});
        pins.push_back((Pin){SHIFTER_IN_PIN_DISTANCE, ax::NodeEditor::PinKind::Input, *this, n++,
                             PinDataType(GetShiftWidth())});

        // Outputs
        pins.push_back((Pin){SHIFTER_OUT_PIN_OUTPUT, ax::NodeEditor::PinKind::Output, *this, n, PinDataType(bits)});
    }


    Pin GetInputPin() { return FindPin(SHIFTER_IN_PIN_INPUT).value(); }
    Pin GetDistancePin() { return FindPin(SHIFTER_IN_PIN_DISTANCE).value(); }


    const char *shift_type_names[5] = {"LSL", "LSR", "ASR", "ROL", "ROR"};

    std::string GetShiftOperator(const std::string &in, const std::string &shft) const {
        if (type_index == 0)
            return in + " << " + shft;
        if (type_index == 1)
            return in + " >> " + shft;
        if (type_index == 2)
            return in + " >>> " + shft;
        if (type_index == 3)
            return "(" + in + " << " + shft + ") | (" + in + " >> (" + std::to_string(bits) + " - " + shft + "))";
        if (type_index == 4)
            return "(" + in + " >> " + shft + ") | (" + in + " << (" + std::to_string(bits) + " - " + shft + "))";

        return "err";
    }

    void RenderInternals() override {
        ImGui::PushItemWidth(150);
        ImGui::SliderInt(("##" + guid).c_str(), &type_index, 0, IM_ARRAYSIZE(shift_type_names) - 1);
        ImGui::PopItemWidth();
        ImGui::Text("%s", shift_type_names[type_index]);
    }
};
