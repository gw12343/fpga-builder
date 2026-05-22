//
// Created by gabed on 4/15/2026.
//

#pragma once

#include "../ConfigurableBitWidthNode.h"

#define UNARY_OP_IN_PIN_A "A"


class UnaryOpNode : public ConfigurableBitWidthNode {
public:
    [[nodiscard]] int GetNodeWidth() const override { return 125; }
    [[nodiscard]] ImVec4 GetUIColor() const override { return COLOR; }

    // Pre-configured
    UnaryOpNode(Module *module, const std::string &guid, const std::string &name, const int bit_width);
    // New
    UnaryOpNode(Module *module, const std::string &name);


    [[nodiscard]] virtual std::string GetVerilogAssign(const std::string &out, const std::string &a) const;
    void InitPinsAfterConfig() override;

    void accept(Visitor &v, int output_slot) override;


    Pin GetAInputPin();

    static constexpr ImVec4 COLOR = {0.114f, 0.616f, 0.647f, 1.0f};
};
