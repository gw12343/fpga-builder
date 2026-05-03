//
// Created by gabed on 4/14/2026.
//

#pragma once
#include <string>


#include "ConfigurableBitWidthNode.h"
#include "GUID.h"
#include "Node.h"

static auto SUBTRACTOR_IN_PIN_A = "A";
static auto SUBTRACTOR_IN_PIN_B = "B";

static auto SUBTRACTOR_OUT_PIN_Q = "Q";
static auto SUBTRACTOR_OUT_PIN_COUT = "Carry Out";

class SubtractorNode final : public ConfigurableBitWidthNode {
public:
    [[nodiscard]] std::string GetSerializationType() const override { return "SubtractorNode"; }

    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }

    [[nodiscard]] std::shared_ptr<Node> Clone() const override {
        return std::make_unique<SubtractorNode>(module, GUID::generate_guid(), bits);
    }

    static constexpr ImVec4 COLOR = {0.560f, 0.1f, 0.07f, 1.0f};
    [[nodiscard]] ImVec4 GetUIColor() const override { return COLOR; }


    // Pre-configured
    SubtractorNode(Module *module, const std::string &guid, const int bit_width) :
        ConfigurableBitWidthNode(guid, module, "Subtractor", bit_width) {
        InitPinsAfterConfig();
    }

    explicit SubtractorNode(Module *module) : ConfigurableBitWidthNode(module, "Subtractor") {}

    void InitPinsAfterConfig() override {
        int n = 0;
        // Inputs
        pins.push_back((Pin){SUBTRACTOR_IN_PIN_A, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(bits)});
        pins.push_back((Pin){SUBTRACTOR_IN_PIN_B, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(bits)});

        // Outputs
        pins.push_back((Pin){SUBTRACTOR_OUT_PIN_Q, ax::NodeEditor::PinKind::Output, *this, n++, PinDataType(bits)});
        pins.push_back((Pin){SUBTRACTOR_OUT_PIN_COUT, ax::NodeEditor::PinKind::Output, *this, n, PinDataType(1)});

        // Find output pin ids
        SUBTRACTOR_Q_ID = FindPin(SUBTRACTOR_OUT_PIN_Q).value().GetNodeIndex();
        SUBTRACTOR_COUT_ID = FindPin(SUBTRACTOR_OUT_PIN_COUT).value().GetNodeIndex();
    }


    int SUBTRACTOR_Q_ID = -1;
    int SUBTRACTOR_COUT_ID = -1;

    Pin GetAInputPin() { return FindPin(SUBTRACTOR_IN_PIN_A).value(); }
    Pin GetBInputPin() { return FindPin(SUBTRACTOR_IN_PIN_B).value(); }
};
