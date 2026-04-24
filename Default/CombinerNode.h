//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "Node.h"

static auto COMBINER_OUT_PIN_VAL = "Value";


class CombinerNode final : public Node {
public:
    [[nodiscard]] std::string GetSerializationType() const override { return "CombinerNode"; }

    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }

    [[nodiscard]] nlohmann::json ToJson() const override {
        nlohmann::json j = Node::ToJson();
        j["bits"] = bits;
        return j;
    }

    [[nodiscard]] ImVec4 GetUIColor() const override { return {0.325f, 0.290f, 0.718f, 1.0f}; }
    [[nodiscard]] int GetNodeWidth() const override { return 150; }


    static std::string GetBitInPinName(const int n) { return "Bit " + std::to_string(n); }

    CombinerNode(Module *module, const std::string &guid, const int data_width) :
        bits(data_width), Node(guid, module, "Combiner", {}, {{COMBINER_OUT_PIN_VAL, PinDataType(data_width)}}) {

        int n = 1;
        for (int i = 0; i < bits; i++) {
            Pin new_input(GetBitInPinName(i), ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(1));
            pins.push_back(new_input);
        }
    }


    Pin GetBitInputPin(const int i) { return FindPin(GetBitInPinName(i)).value(); }

    int bits;
};
