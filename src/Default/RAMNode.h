//
// Created by gabed on 4/14/2026.
//

#pragma once
#include <memory>
#include <string>


#include "ConfigurableDataAndSelectBitWidthNode.h"
#include "GUID.h"
#include "Node.h"

static auto RAM_IN_PIN_ADDRESS = "Address";
static auto RAM_IN_PIN_IN = "In";
static auto RAM_IN_PIN_LOAD = "Load";
static auto RAM_IN_PIN_CLOCK = "Clk";

class RAMNode final : public ConfigurableDataAndSelectBitWidthNode {
public:
    [[nodiscard]] std::string GetSerializationType() const override { return "RAMNode"; }

    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }

    [[nodiscard]] std::shared_ptr<Node> Clone() const override {
        return std::make_unique<RAMNode>(module, GUID::generate_guid(), data_bits, select_bits);
    }


    [[nodiscard]] int GetNodeWidth() const override { return 225; }

    static constexpr ImVec4 COLOR = {0.325f, 0.290f, 0.718f, 1.0f};
    [[nodiscard]] ImVec4 GetUIColor() const override { return COLOR; }


    RAMNode(Module *module, const std::string &guid, const int data_bits, const int select_bits) :
        ConfigurableDataAndSelectBitWidthNode(guid, module, "RAM", data_bits, select_bits) {
        InitPinsAfterConfig();
    }

    explicit RAMNode(Module *module) : ConfigurableDataAndSelectBitWidthNode(module, "RAM") {}

    void InitPinsAfterConfig() override {
        int n = 0;
        // Inputs
        pins.push_back((Pin){RAM_IN_PIN_ADDRESS, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(select_bits)});
        pins.push_back((Pin){RAM_IN_PIN_IN, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(data_bits)});
        pins.push_back((Pin){RAM_IN_PIN_LOAD, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(1)});
        pins.push_back((Pin){RAM_IN_PIN_CLOCK, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(1)});

        // Output
        pins.push_back((Pin){"Value", ax::NodeEditor::PinKind::Output, *this, n, PinDataType(data_bits)});
    }

    std::string GetDisplayName() const override {
        return name + " " + std::to_string(static_cast<int>(powl(2.0, select_bits))) + "x" + std::to_string(data_bits);
    }


    Pin GetAddressPin() { return FindPin(RAM_IN_PIN_ADDRESS).value(); }
    Pin GetInPin() { return FindPin(RAM_IN_PIN_IN).value(); }
    Pin GetLoadPin() { return FindPin(RAM_IN_PIN_LOAD).value(); }
    Pin GetClockPin() { return FindPin(RAM_IN_PIN_CLOCK).value(); }
};
