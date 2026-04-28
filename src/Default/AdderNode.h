//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "ConfigurableBitWidthNode.h"
#include "Node.h"

static auto ADDER_IN_PIN_A = "A";
static auto ADDER_IN_PIN_B = "B";
static auto ADDER_IN_PIN_CIN = "Carry In";

static auto ADDER_OUT_PIN_Q = "Q";
static auto ADDER_OUT_PIN_COUT = "Carry Out";

class AdderNode final : public ConfigurableBitWidthNode {
public:
    [[nodiscard]] std::string GetSerializationType() const override { return "AdderNode"; }

    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }

    static constexpr ImVec4 color = {0.560f, 0.1f, 0.07f, 1.0f};
    [[nodiscard]] ImVec4 GetUIColor() const override { return color; }

    // Pre-configured
    AdderNode(Module *module, const std::string &guid, const int bit_width) :
        ConfigurableBitWidthNode(guid, module, "Adder", bit_width) {
        InitPinsAfterConfig();
    }

    explicit AdderNode(Module *module) : ConfigurableBitWidthNode(module, "Adder") {}

    void InitPinsAfterConfig() override {
        int n = 0;
        // Inputs
        pins.push_back((Pin){ADDER_IN_PIN_A, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(bits)});
        pins.push_back((Pin){ADDER_IN_PIN_B, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(bits)});
        pins.push_back((Pin){ADDER_IN_PIN_CIN, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(1)});

        // Outputs
        pins.push_back((Pin){ADDER_OUT_PIN_Q, ax::NodeEditor::PinKind::Output, *this, n++, PinDataType(bits)});
        pins.push_back((Pin){ADDER_OUT_PIN_COUT, ax::NodeEditor::PinKind::Output, *this, n, PinDataType(1)});

        // Find output pin ids
        ADDER_Q_ID = FindPin(ADDER_OUT_PIN_Q).value().GetNodeIndex();
        ADDER_COUT_ID = FindPin(ADDER_OUT_PIN_COUT).value().GetNodeIndex();
    }


    int ADDER_Q_ID = -1;
    int ADDER_COUT_ID = -1;

    Pin GetAInputPin() { return FindPin(ADDER_IN_PIN_A).value(); }
    Pin GetBInputPin() { return FindPin(ADDER_IN_PIN_B).value(); }
    Pin GetCarryInputPin() { return FindPin(ADDER_IN_PIN_CIN).value(); }
};
