//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "Node/Node.h"


static auto DEBOUNCE_IN_PIN_D = "D";
static auto DEBOUNCE_IN_PIN_CLK = "Clk";

class DebounceNode final : public Node {
public:
    [[nodiscard]] std::shared_ptr<Node> Clone() const override;
    [[nodiscard]] std::string GetSerializationType() const override { return "DebounceNode"; }
    [[nodiscard]] int GetNodeWidth() const override { return 100; }
    [[nodiscard]] ImVec4 GetUIColor() const override { return COLOR; }
    [[nodiscard]] bool IsSequential() const override { return true; }

    DebounceNode(Module *module, const std::string &guid);

    explicit DebounceNode(Module *module);


    void accept(Visitor &v, int output_slot) override;

    Pin GetDPin();
    Pin GetClockPin();

    static constexpr ImVec4 COLOR = {0.729f, 0.455f, 0.067f, 1.0f};
};
