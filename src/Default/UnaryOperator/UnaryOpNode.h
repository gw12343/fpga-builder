//
// Created by gabed on 4/15/2026.
//

#pragma once
#include "Default/ConfigurableBitWidthNode.h"
#include "Default/Node.h"

#define UNARY_OP_IN_PIN_A "A"


class UnaryOpNode : public ConfigurableBitWidthNode {
public:
    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }

    [[nodiscard]] int GetNodeWidth() const override { return 125; }

    static constexpr ImVec4 COLOR = {0.114f, 0.616f, 0.647f, 1.0f};
    [[nodiscard]] ImVec4 GetUIColor() const override { return COLOR; }


    [[nodiscard]] virtual std::string GetVerilogAssign(const std::string &out, const std::string &a) const {
        return "????";
    }


    // Pre-configured
    UnaryOpNode(Module *module, const std::string &guid, const std::string &name, const int bit_width) :
        ConfigurableBitWidthNode(guid, module, name, bit_width) {
        UnaryOpNode::InitPinsAfterConfig();
    }

    UnaryOpNode(Module *module, const std::string &name) : ConfigurableBitWidthNode(module, name) {}


    void InitPinsAfterConfig() override {
        // Input
        pins.push_back((Pin){UNARY_OP_IN_PIN_A, ax::NodeEditor::PinKind::Input, *this, 0, PinDataType(bits)});
        // Output
        pins.push_back((Pin){"Out", ax::NodeEditor::PinKind::Output, *this, 1, PinDataType(bits)});
    }

    Pin GetAInputPin() { return FindPin(UNARY_OP_IN_PIN_A).value(); }
};
