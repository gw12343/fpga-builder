//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "Node/ConfigurableDataAndSelectBitWidthNode.h"

static auto MP_IN_PIN_SELECT = "Sel";

class MultiplexerNode final : public ConfigurableDataAndSelectBitWidthNode {
public:
    [[nodiscard]] std::shared_ptr<Node> Clone() const override;
    [[nodiscard]] std::string GetSerializationType() const override { return "MultiplexerNode"; }
    [[nodiscard]] ImVec4 GetUIColor() const override { return COLOR; }
    [[nodiscard]] int GetNodeWidth() const override { return 225; }

    MultiplexerNode(Module *module, const std::string &guid, int data_bits, int select_bits);
    explicit MultiplexerNode(Module *module);


    void accept(Visitor &v, int output_slot) override;
    void InitPinsAfterConfig() override;

    [[nodiscard]] int GetNumOptions() const;
    Pin GetInputPin(int n);
    Pin GetSelectInputPin();

    static constexpr ImVec4 COLOR = {0.325f, 0.290f, 0.718f, 1.0f};
};
