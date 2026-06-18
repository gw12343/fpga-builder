//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "Node/ConfigurableDataAndSelectBitWidthNode.h"

static auto ROM_IN_PIN_ADDRESS = "Address";
static auto ROM_IN_PIN_CLOCK = "Clk";

class ROMNode final : public ConfigurableDataAndSelectBitWidthNode {
public:
    [[nodiscard]] std::string GetSerializationType() const override { return "ROMNode"; }
    [[nodiscard]] std::shared_ptr<Node> Clone() const override;
    [[nodiscard]] nlohmann::json ToJson() const override;
    [[nodiscard]] int GetNodeWidth() const override { return 225; }
    [[nodiscard]] ImVec4 GetUIColor() const override { return COLOR; }
    [[nodiscard]] bool IsSequential() const override { return !m_async_read; } // if syncronous

    void accept(Visitor &v, int output_slot) override;


    ROMNode(Module *module, const std::string &guid, int data_bits, int select_bits, std::string data_file,
            bool async_read);
    explicit ROMNode(Module *module);

    void InitPinsAfterConfig() override;
    void RenderInternals() override;

    std::string GetDisplayName() const override;
    Pin GetAddressPin();
    Pin GetClockPin();

    const std::string &GetRomFile() const { return m_rom_file; }

    static constexpr ImVec4 COLOR = {0.325f, 0.290f, 0.718f, 1.0f};

    std::string m_rom_file;
    bool m_async_read = false;
};
