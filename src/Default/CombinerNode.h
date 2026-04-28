//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "ConfigurableBitWidthNode.h"
#include "Node.h"

static auto COMBINER_OUT_PIN_VAL = "Value";


class CombinerNode final : public ConfigurableBitWidthNode {
public:
    [[nodiscard]] std::string GetSerializationType() const override { return "CombinerNode"; }

    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }

    static constexpr ImVec4 color = {0.325f, 0.290f, 0.718f, 1.0f};
    [[nodiscard]] ImVec4 GetUIColor() const override { return color; }
    [[nodiscard]] int GetNodeWidth() const override { return 150; }


    static std::string GetBitInPinName(const int n) { return "Bit " + std::to_string(n); }

    CombinerNode(Module *module, const std::string &guid, const int data_width) :
        ConfigurableBitWidthNode(guid, module, "Combiner", data_width) {
        InitPinsAfterConfig();
    }

    explicit CombinerNode(Module *module) : ConfigurableBitWidthNode(module, "Combiner") {}


    void InitPinsAfterConfig() override {
        // Output
        pins.push_back((Pin){COMBINER_OUT_PIN_VAL, ax::NodeEditor::PinKind::Output, *this, 0, PinDataType(bits)});
        // Inputs
        int n = 1;
        for (int i = 0; i < bits; i++) {
            Pin new_input(GetBitInPinName(i), ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(1));
            pins.push_back(new_input);
        }
    }


    Pin GetBitInputPin(const int i) { return FindPin(GetBitInPinName(i)).value(); }
};
