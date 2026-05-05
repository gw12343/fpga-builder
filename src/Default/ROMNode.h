//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "ConfigurableDataAndSelectBitWidthNode.h"
#include "Node.h"

static auto ROM_IN_PIN_ADDRESS = "Address";
static auto ROM_IN_PIN_CLOCK = "Clk";

class ROMNode final : public ConfigurableDataAndSelectBitWidthNode {
public:
    [[nodiscard]] std::string GetSerializationType() const override { return "ROMNode"; }

    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }

    [[nodiscard]] std::shared_ptr<Node> Clone() const override {
        return std::make_unique<ROMNode>(module, GUID::generate_guid(), data_bits, select_bits, m_rom_file);
    }

    [[nodiscard]] nlohmann::json ToJson() const override {
        nlohmann::json j = ConfigurableDataAndSelectBitWidthNode::ToJson();
        j["rom_file"] = m_rom_file;
        return j;
    }

    [[nodiscard]] int GetNodeWidth() const override { return 225; }

    static constexpr ImVec4 COLOR = {0.325f, 0.290f, 0.718f, 1.0f};
    [[nodiscard]] ImVec4 GetUIColor() const override { return COLOR; }


    ROMNode(Module *module, const std::string &guid, const int data_bits, const int select_bits,
            std::string data_file) :
        ConfigurableDataAndSelectBitWidthNode(guid, module, "ROM", data_bits, select_bits),
        m_rom_file(std::move(data_file)) {
        InitPinsAfterConfig();
    }

    explicit ROMNode(Module *module) : ConfigurableDataAndSelectBitWidthNode(module, "ROM") {}

    void InitPinsAfterConfig() override {
        int n = 0;
        // Inputs
        pins.push_back((Pin){ROM_IN_PIN_ADDRESS, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(select_bits)});
        pins.push_back((Pin){ROM_IN_PIN_CLOCK, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(1)});

        // Output
        pins.push_back((Pin){"Value", ax::NodeEditor::PinKind::Output, *this, n, PinDataType(data_bits)});
    }

    std::string GetDisplayName() const override {
        return name + " " + std::to_string(static_cast<int>(powl(2.0, select_bits))) + "x" + std::to_string(data_bits);
    }

    void RenderInternals() override {
        ImGui::PushItemWidth(60);
        ImGui::InputText(("ROM file##" + guid).c_str(), &m_rom_file);
        ImGui::PopItemWidth();
    }

    Pin GetAddressPin() { return FindPin(ROM_IN_PIN_ADDRESS).value(); }
    Pin GetClockPin() { return FindPin(ROM_IN_PIN_CLOCK).value(); }

    const std::string &GetRomFile() const { return m_rom_file; }

private:
    std::string m_rom_file;
};
