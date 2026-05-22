//
// Created by gabed on 4/14/2026.
//

#pragma once

#include "Node/ConfigurableDataAndSelectBitWidthNode.h"

static auto RAM_IN_PIN_ADDRESS = "Address";
static auto RAM_IN_PIN_IN = "In";
static auto RAM_IN_PIN_LOAD = "Load";
static auto RAM_IN_PIN_CLOCK = "Clk";

class RAMNode final : public ConfigurableDataAndSelectBitWidthNode {
public:
    [[nodiscard]] std::string GetSerializationType() const override { return "RAMNode"; }
    [[nodiscard]] std::shared_ptr<Node> Clone() const override;
    [[nodiscard]] int GetNodeWidth() const override { return 225; }
    [[nodiscard]] ImVec4 GetUIColor() const override { return COLOR; }
    [[nodiscard]] std::string GetDisplayName() const override;

    RAMNode(Module *module, const std::string &guid, const int data_bits, const int select_bits);
    explicit RAMNode(Module *module);

    void accept(Visitor &v, int output_slot) override;
    void InitPinsAfterConfig() override;

    Pin GetAddressPin();
    Pin GetInPin();
    Pin GetLoadPin();
    Pin GetClockPin();

    static constexpr ImVec4 COLOR = {0.325f, 0.290f, 0.718f, 1.0f};
};
