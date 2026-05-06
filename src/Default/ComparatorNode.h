//
// Created by gabed on 4/14/2026.
//

#pragma once
#include <imgui.h>
#include <string>


#include "ConfigurableBitWidthNode.h"
#include "GUID.h"
#include "Node.h"

static auto COMPARATOR_IN_PIN_A = "A";
static auto COMPARATOR_IN_PIN_B = "B";

static auto COMPARATOR_OUT_PIN_GREATER = ">";
static auto COMPARATOR_OUT_PIN_EQUAL = "=";
static auto COMPARATOR_OUT_PIN_LESS = "<";

class ComparatorNode final : public ConfigurableBitWidthNode {
public:
    [[nodiscard]] std::string GetSerializationType() const override { return "ComparatorNode"; }

    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }

    [[nodiscard]] std::shared_ptr<Node> Clone() const override {
        return std::make_unique<ComparatorNode>(module, GUID::generate_guid(), bits);
    }

    static constexpr ImVec4 COLOR = {0.560f, 0.1f, 0.07f, 1.0f};
    [[nodiscard]] ImVec4 GetUIColor() const override { return COLOR; }


    // Pre-configured
    ComparatorNode(Module *module, const std::string &guid, const int bit_width) :
        ConfigurableBitWidthNode(guid, module, "Comparator", bit_width) {
        InitPinsAfterConfig();
    }

    explicit ComparatorNode(Module *module) : ConfigurableBitWidthNode(module, "Comparator") {}

    void InitPinsAfterConfig() override {
        int n = 0;
        // Inputs
        pins.push_back((Pin){COMPARATOR_IN_PIN_A, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(bits)});
        pins.push_back((Pin){COMPARATOR_IN_PIN_B, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(bits)});

        // Outputs
        pins.push_back((Pin){COMPARATOR_OUT_PIN_GREATER, ax::NodeEditor::PinKind::Output, *this, n++, PinDataType(1)});
        pins.push_back((Pin){COMPARATOR_OUT_PIN_EQUAL, ax::NodeEditor::PinKind::Output, *this, n++, PinDataType(1)});
        pins.push_back((Pin){COMPARATOR_OUT_PIN_LESS, ax::NodeEditor::PinKind::Output, *this, n, PinDataType(1)});

        // Find output pin ids
        COMPARATOR_G_ID = FindPin(COMPARATOR_OUT_PIN_GREATER).value().GetNodeIndex();
        COMPARATOR_E_ID = FindPin(COMPARATOR_OUT_PIN_EQUAL).value().GetNodeIndex();
        COMPARATOR_L_ID = FindPin(COMPARATOR_OUT_PIN_LESS).value().GetNodeIndex();
    }


    int COMPARATOR_G_ID = -1;
    int COMPARATOR_E_ID = -1;
    int COMPARATOR_L_ID = -1;

    Pin GetAInputPin() { return FindPin(ADDER_IN_PIN_A).value(); }
    Pin GetBInputPin() { return FindPin(ADDER_IN_PIN_B).value(); }
    Pin GetCarryInputPin() { return FindPin(ADDER_IN_PIN_CIN).value(); }
};
