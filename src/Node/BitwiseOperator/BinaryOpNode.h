//
// Created by gabed on 4/15/2026.
//

#pragma once

#include "Node/ConfigurableDataAndNumInputsNode.h"


class BinaryOpNode : public ConfigurableDataAndNumInputsNode {
public:
    void accept(Visitor &v, int output_slot) override;

    [[nodiscard]] int GetNodeWidth() const override { return 125; }

    static constexpr ImVec4 COLOR = {0.059f, 0.431f, 0.333f, 1.0f};
    [[nodiscard]] ImVec4 GetUIColor() const override { return COLOR; }


    [[nodiscard]] virtual std::string GetVerilogAssign(const std::string &out,
                                                       const std::vector<std::string> &inputs) const {
        return "????";
    }

    [[nodiscard]] bool IsSequential() const override { return false; }

    static std::string GetBitInPinName(int n);

    Pin GetInputPin(int n);

    void InitPinsAfterConfig() override;

    // Pre-configured
    BinaryOpNode(Module *module, const std::string &guid, const std::string &name, int bit_width, int num_inputs);
    // New node
    BinaryOpNode(Module *module, const std::string &name) : ConfigurableDataAndNumInputsNode(module, name) {}
};
