//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "ConfigurableBitWidthNode.h"
#include "Node.h"

static auto SPLITTER_IN_PIN_VAL = "Value";


class SplitterNode final : public ConfigurableBitWidthNode {
public:
    [[nodiscard]] std::string GetSerializationType() const override { return "SplitterNode"; }

    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }

    static constexpr ImVec4 color = {0.325f, 0.290f, 0.718f, 1.0f};
    [[nodiscard]] ImVec4 GetUIColor() const override { return color; }
    [[nodiscard]] int GetNodeWidth() const override { return 150; }


    static std::string GetBitOutPinName(const int n) { return "Bit " + std::to_string(n); }

    // Pre-configured
    SplitterNode(Module *module, const std::string &guid, const int bit_width) :
        ConfigurableBitWidthNode(guid, module, "Splitter", bit_width) {
        InitPinsAfterConfig();
    }

    explicit SplitterNode(Module *module) : ConfigurableBitWidthNode(module, "Splitter") {}


    void InitPinsAfterConfig() override {
        // Inputs
        pins.push_back((Pin){SPLITTER_IN_PIN_VAL, ax::NodeEditor::PinKind::Input, *this, 0, PinDataType(bits)});

        // Outputs
        int n = 1;
        for (int i = 0; i < bits; i++) {
            Pin new_output(GetBitOutPinName(i), ax::NodeEditor::PinKind::Output, *this, n++, PinDataType(1));
            pins.push_back(new_output);
        }
    }


    Pin GetInputPin() { return FindPin(SPLITTER_IN_PIN_VAL).value(); }
    Pin GetBitOutputPin(const int i) { return FindPin(GetBitOutPinName(i)).value(); }
};
