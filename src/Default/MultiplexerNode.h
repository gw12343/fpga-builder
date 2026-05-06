//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "ConfigurableDataAndSelectBitWidthNode.h"
#include "Node.h"

static auto MP_IN_PIN_SELECT = "Sel";

class MultiplexerNode final : public ConfigurableDataAndSelectBitWidthNode {
public:
    [[nodiscard]] std::string GetSerializationType() const override { return "MultiplexerNode"; }

    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }

    [[nodiscard]] std::shared_ptr<Node> Clone() const override {
        return std::make_unique<MultiplexerNode>(module, GUID::generate_guid(), data_bits, select_bits);
    }

    [[nodiscard]] int GetNodeWidth() const override { return 225; }

    static constexpr ImVec4 COLOR = {0.325f, 0.290f, 0.718f, 1.0f};
    [[nodiscard]] ImVec4 GetUIColor() const override { return COLOR; }


    MultiplexerNode(Module *module, const std::string &guid, const int data_bits, const int select_bits) :
        ConfigurableDataAndSelectBitWidthNode(guid, module, "Multiplexer", data_bits, select_bits) {
        InitPinsAfterConfig();
    }

    explicit MultiplexerNode(Module *module) : ConfigurableDataAndSelectBitWidthNode(module, "Multiplexer") {}

    void InitPinsAfterConfig() override {
        // Inputs
        int n = 0;
        for (int i = 0; i < GetNumOptions(); i++) {
            Pin new_input("In " + std::to_string(n), ax::NodeEditor::PinKind::Input, *this, n++,
                          PinDataType(data_bits));
            pins.push_back(new_input);
        }
        pins.push_back((Pin){MP_IN_PIN_SELECT, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(select_bits)});

        // Output
        pins.push_back((Pin){"Value", ax::NodeEditor::PinKind::Output, *this, n, PinDataType(data_bits)});
    }

    [[nodiscard]] int GetNumOptions() const { return static_cast<int>(powl(2.0, select_bits)); }

    Pin GetInputPin(const int n) { return pins[n]; }
    Pin GetSelectInputPin() { return FindPin(MP_IN_PIN_SELECT).value(); }
};
