//
// Created by gabed on 4/14/2026.
//

#pragma once

#include "ConfigurableBitWidthNode.h"
#include "Node.h"

class LiteralNode final : public ConfigurableBitWidthNode {
public:
    [[nodiscard]] std::string GetSerializationType() const override { return "LiteralNode"; }

    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }

    [[nodiscard]] nlohmann::json ToJson() const override {
        nlohmann::json j = ConfigurableBitWidthNode::ToJson();
        j["value"] = value;
        return j;
    }

    [[nodiscard]] ImVec4 GetUIColor() const override { return {0.373f, 0.369f, 0.353f, 1.0f}; }
    [[nodiscard]] int GetNodeWidth() const override { return 125; }

    void RenderInternals() override;


    // Pre-configured
    LiteralNode(Module *module, const std::string &guid, const int bit_width, const int val) :
        ConfigurableBitWidthNode(guid, module, "Literal", bit_width), value(val) {

        InitPinsAfterConfig();
    }

    explicit LiteralNode(Module *module) : ConfigurableBitWidthNode(module, "Literal"), value(0) {}

    void InitPinsAfterConfig() override {
        // Output
        pins.push_back((Pin){"Value", ax::NodeEditor::PinKind::Output, *this, 0, PinDataType(bits)});
    }


    int value;
};

inline void LiteralNode::RenderInternals() {
    ImGui::PushItemWidth(60);
    ImGui::SliderInt(("##" + guid).c_str(), &value, 0, powl(2, bits) - 1);
    if (value < 0) {
        value = 0;
    } else if (value > powl(2, bits) - 1) {
        value = powl(2, bits) - 1;
    }

    ImGui::PopItemWidth();
}
