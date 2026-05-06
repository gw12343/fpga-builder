//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "Node.h"


static auto REGISTER_IN_PIN_ENABLE = "Enable";
static auto REGISTER_IN_PIN_CLOCK = "Clk";
static auto REGISTER_IN_PIN_RESET = "Reset";
static auto REGISTER_IN_PIN_D = "D";

class RegisterNode final : public ConfigurableBitWidthNode {
public:
    [[nodiscard]] std::string GetSerializationType() const override { return "RegisterNode"; }

    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }

    [[nodiscard]] std::shared_ptr<Node> Clone() const override {
        return std::make_unique<RegisterNode>(module, GUID::generate_guid(), bits);
    }

    static constexpr ImVec4 COLOR = {0.729f, 0.455f, 0.067f, 1.0f};
    [[nodiscard]] ImVec4 GetUIColor() const override { return COLOR; }
    [[nodiscard]] int GetNodeWidth() const override { return 150; }


    // Pre-configured node
    RegisterNode(Module *module, const std::string &guid, const int bit_width) :
        ConfigurableBitWidthNode(guid, module, "Register", bit_width) {
        InitPinsAfterConfig();
    }
    // New node
    explicit RegisterNode(Module *module) : ConfigurableBitWidthNode(module, "Register") {}


    void InitPinsAfterConfig() override {
        int n = 0;
        // Inputs
        pins.push_back((Pin){REGISTER_IN_PIN_ENABLE, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(1)});
        pins.push_back((Pin){REGISTER_IN_PIN_CLOCK, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(1)});
        pins.push_back((Pin){REGISTER_IN_PIN_RESET, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(1)});
        pins.push_back((Pin){REGISTER_IN_PIN_D, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(bits)});

        // Outputs
        pins.push_back((Pin){"Q", ax::NodeEditor::PinKind::Output, *this, n, PinDataType(bits)});
    }

    Pin GetEnablePin() { return FindPin(REGISTER_IN_PIN_ENABLE).value(); }
    Pin GetClkPin() { return FindPin(REGISTER_IN_PIN_CLOCK).value(); }
    Pin GetDPin() { return FindPin(REGISTER_IN_PIN_D).value(); }
    Pin GetResetPin() { return FindPin(REGISTER_IN_PIN_RESET).value(); }
};
