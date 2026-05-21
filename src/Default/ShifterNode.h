//
// Created by gabed on 4/14/2026.
//

#pragma once

#include "ConfigurableBitWidthNode.h"


static auto SHIFTER_IN_PIN_INPUT = "Input";
static auto SHIFTER_IN_PIN_DISTANCE = "Distance";

static auto SHIFTER_OUT_PIN_OUTPUT = "Output";


class ShifterNode final : public ConfigurableBitWidthNode {
public:
    [[nodiscard]] std::shared_ptr<Node> Clone() const override;
    [[nodiscard]] std::string GetSerializationType() const override { return "ShifterNode"; }
    [[nodiscard]] ImVec4 GetUIColor() const override { return COLOR; }
    [[nodiscard]] nlohmann::json ToJson() const override;

    void accept(Visitor &v, int output_slot) override;

    // Pre-configured
    ShifterNode(Module *module, const std::string &guid, int bit_width, int type);
    explicit ShifterNode(Module *module);

    [[nodiscard]] int GetShiftWidth() const;
    void InitPinsAfterConfig() override;
    void RenderInternals() override;


    Pin GetInputPin();
    Pin GetDistancePin();

    std::string GetShiftOperator(const std::string &in, const std::string &shft) const;


    static constexpr ImVec4 COLOR = {0.560f, 0.1f, 0.07f, 1.0f};
    const char *shift_type_names[5] = {"LSL", "LSR", "ASR", "ROL", "ROR"};
    int type_index = 0;
};
