//
// Created by Gabe on 5/21/2026.
//

#include "ROMNode.h"
#include "Events/StringParameterChangeCommand.h"
#include "GUID.h"
#include "Module.h"

void ROMNode::accept(Visitor &v, const int output_slot) { v.visit(*this, output_slot); }

std::shared_ptr<Node> ROMNode::Clone() const {
    return std::make_unique<ROMNode>(module, GUID::generate_guid(), m_data_bits, m_select_bits, m_rom_file, m_async_read);
}

nlohmann::json ROMNode::ToJson() const {
    nlohmann::json j = ConfigurableDataAndSelectBitWidthNode::ToJson();
    j["rom_file"] = m_rom_file;
    j["async_read"] = m_async_read;
    return j;
}

ROMNode::ROMNode(Module *module, const std::string &guid, const int data_bits, const int select_bits,
                 std::string data_file, bool async_read) :
    ConfigurableDataAndSelectBitWidthNode(guid, module, "ROM", data_bits, select_bits),
    m_rom_file(std::move(data_file)),
    m_async_read(async_read) {
    InitPinsAfterConfig();
}

ROMNode::ROMNode(Module *module) : ConfigurableDataAndSelectBitWidthNode(module, "ROM") {}

void ROMNode::InitPinsAfterConfig() {
    int n = 0;
    // Inputs
    pins.push_back({ROM_IN_PIN_ADDRESS, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(m_select_bits)});
    pins.push_back({ROM_IN_PIN_CLOCK, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(1)});

    // Output
    pins.push_back({"Value", ax::NodeEditor::PinKind::Output, *this, n, PinDataType(m_data_bits)});
}

std::string ROMNode::GetDisplayName() const {
    return name + " " + std::to_string(static_cast<int>(powl(2.0, m_select_bits))) + "x" + std::to_string(m_data_bits);
}

void ROMNode::RenderInternals() {
    ImGui::PushItemWidth(60);
    const std::string old_val = m_rom_file;
    const bool f = ImGui::InputText(("ROM file##" + guid).c_str(), &m_rom_file);

    if (f && m_rom_file != old_val) {
        const auto cmd = std::make_shared<StringParameterChangeCommand>(module->shared_from_this(), shared_from_this(),
                                                                        old_val, m_rom_file, 0);
        module->ExecuteCommand(cmd);
    }


    std::string checkbox_text = "Async Read##" + guid;
    ImGui::Checkbox(checkbox_text.c_str(), &m_async_read);

    ImGui::PopItemWidth();
}

Pin ROMNode::GetAddressPin() { return FindPin(ROM_IN_PIN_ADDRESS).value(); }
Pin ROMNode::GetClockPin() { return FindPin(ROM_IN_PIN_CLOCK).value(); }
