//
// Created by gabed on 4/15/2026.
//

#pragma once
#include "Default/ConfigurableDataAndNumInputsNode.h"
#include "Default/Node.h"
#include "Pins/Pin.h"


class BinaryOpNode : public ConfigurableDataAndNumInputsNode {
public:
    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }

    [[nodiscard]] int GetNodeWidth() const override { return 125; }

    static constexpr ImVec4 COLOR = {0.059f, 0.431f, 0.333f, 1.0f};
    [[nodiscard]] ImVec4 GetUIColor() const override { return COLOR; }


    [[nodiscard]] virtual std::string GetVerilogAssign(const std::string &out,
                                                       const std::vector<std::string> &inputs) const {
        return "????";
    }


    // Pre-configured
    BinaryOpNode(Module *module, const std::string &guid, const std::string &name, const int bit_width,
                 const int num_inputs) :
        ConfigurableDataAndNumInputsNode(guid, module, name, bit_width, num_inputs) {
        BinaryOpNode::InitPinsAfterConfig();
    }

    BinaryOpNode(Module *module, const std::string &name) : ConfigurableDataAndNumInputsNode(module, name) {}

    static std::string GetBitInPinName(const int n) { return "Input " + std::to_string(n); }


    void InitPinsAfterConfig() override {
        int n = 0;
        // Inputs
        for (int i = 0; i < inputs_size; i++) {
            Pin new_input(GetBitInPinName(i), ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(data_bits));
            pins.push_back(new_input);
        }

        // Output
        pins.push_back((Pin){"Out", ax::NodeEditor::PinKind::Output, *this, n, PinDataType(data_bits)});
    }

    Pin GetInputPin(const int n) { return FindPin(GetBitInPinName(n)).value(); }
};
