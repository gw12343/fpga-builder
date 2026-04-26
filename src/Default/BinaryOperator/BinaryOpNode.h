//
// Created by gabed on 4/15/2026.
//

#pragma once
#include "Default/ConfigurableBitWidthNode.h"
#include "Default/Node.h"

#define BINARY_OP_IN_PIN_A "A"
#define BINARY_OP_IN_PIN_B "B"

class BinaryOpNode : public ConfigurableBitWidthNode {
public:
    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }

    [[nodiscard]] int GetNodeWidth() const override { return 125; }

    [[nodiscard]] ImVec4 GetUIColor() const override { return {0.059f, 0.431f, 0.333f, 1.0f}; }

    [[nodiscard]] virtual std::string GetVerilogAssign(const std::string &out, const std::string &a,
                                                       const std::string &b) const {
        return "????";
    }


    // Pre-configured
    BinaryOpNode(Module *module, const std::string &guid, const std::string &name, const int bit_width) :
        ConfigurableBitWidthNode(guid, module, name, bit_width) {
        BinaryOpNode::InitPinsAfterConfig();
    }

    BinaryOpNode(Module *module, const std::string &name) : ConfigurableBitWidthNode(module, name) {}


    void InitPinsAfterConfig() override {
        int n = 0;
        // Inputs
        pins.push_back((Pin){BINARY_OP_IN_PIN_A, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(bits)});
        pins.push_back((Pin){BINARY_OP_IN_PIN_B, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(bits)});

        // Outputs
        pins.push_back((Pin){"Out", ax::NodeEditor::PinKind::Output, *this, n, PinDataType(bits)});
    }

    Pin GetAInputPin() { return FindPin(BINARY_OP_IN_PIN_A).value(); }
    Pin GetBInputPin() { return FindPin(BINARY_OP_IN_PIN_B).value(); }
};
