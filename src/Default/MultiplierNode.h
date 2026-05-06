//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "ConfigurableBitWidthNode.h"


static auto MULTIPLIER_IN_PIN_A = "A";
static auto MULTIPLIER_IN_PIN_B = "B";

static auto MULTIPLIER_OUT_PIN_Q = "Q";

class MultiplierNode final : public ConfigurableBitWidthNode {
public:
    [[nodiscard]] std::string GetSerializationType() const override { return "MultiplierNode"; }

    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }

    [[nodiscard]] std::shared_ptr<Node> Clone() const override {
        return std::make_unique<MultiplierNode>(module, GUID::generate_guid(), bits);
    }

    static constexpr ImVec4 COLOR = {0.560f, 0.1f, 0.07f, 1.0f};
    [[nodiscard]] ImVec4 GetUIColor() const override { return COLOR; }


    // Pre-configured
    MultiplierNode(Module *module, const std::string &guid, const int bit_width) :
        ConfigurableBitWidthNode(guid, module, "Multiplier", bit_width) {
        InitPinsAfterConfig();
    }

    explicit MultiplierNode(Module *module) : ConfigurableBitWidthNode(module, "Multiplier") {}

    void InitPinsAfterConfig() override {
        int n = 0;
        // Inputs
        pins.push_back((Pin){MULTIPLIER_IN_PIN_A, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(bits)});
        pins.push_back((Pin){MULTIPLIER_IN_PIN_B, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(bits)});

        // Outputs
        pins.push_back((Pin){MULTIPLIER_OUT_PIN_Q, ax::NodeEditor::PinKind::Output, *this, n, PinDataType(bits)});
    }


    Pin GetAInputPin() { return FindPin(MULTIPLIER_IN_PIN_A).value(); }
    Pin GetBInputPin() { return FindPin(MULTIPLIER_IN_PIN_B).value(); }
};
