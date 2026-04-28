//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "Node.h"


static auto COUTNER_IN_PIN_ENABLE = "Enable";
static auto COUTNER_IN_PIN_CLOCK = "Clk";
static auto COUTNER_IN_PIN_RESET = "Reset";
static auto COUTNER_IN_PIN_COUNT_UP = "Count Up";

class CounterNode final : public ConfigurableBitWidthNode {
public:
    [[nodiscard]] std::string GetSerializationType() const override { return "CounterNode"; }

    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }

    static constexpr ImVec4 color = {0.729f, 0.455f, 0.067f, 1.0f};
    [[nodiscard]] ImVec4 GetUIColor() const override { return color; }
    [[nodiscard]] int GetNodeWidth() const override { return 150; }

    // Pre-configured node
    CounterNode(Module *module, const std::string &guid, const int bit_width) :
        ConfigurableBitWidthNode(guid, module, "Counter", bit_width) {
        InitPinsAfterConfig();
    }
    // New node
    explicit CounterNode(Module *module) : ConfigurableBitWidthNode(module, "Counter") {}


    void InitPinsAfterConfig() override {
        int n = 0;
        // Inputs
        pins.push_back((Pin){COUTNER_IN_PIN_ENABLE, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(1)});
        pins.push_back((Pin){COUTNER_IN_PIN_CLOCK, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(1)});
        pins.push_back((Pin){COUTNER_IN_PIN_COUNT_UP, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(1)});
        pins.push_back((Pin){COUTNER_IN_PIN_RESET, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(1)});

        // Outputs
        pins.push_back((Pin){"Value", ax::NodeEditor::PinKind::Output, *this, n, PinDataType(bits)});
    }

    Pin GetEnablePin() { return FindPin(COUTNER_IN_PIN_ENABLE).value(); }
    Pin GetCountUpPin() { return FindPin(COUTNER_IN_PIN_COUNT_UP).value(); }
    Pin GetClkPin() { return FindPin(COUTNER_IN_PIN_CLOCK).value(); }
    Pin GetResetPin() { return FindPin(COUTNER_IN_PIN_RESET).value(); }
};
