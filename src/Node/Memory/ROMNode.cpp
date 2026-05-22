//
// Created by Gabe on 5/21/2026.
//

#include "ROMNode.h"
#include "GUID.h"

void ROMNode::accept(Visitor &v, const int output_slot) { v.visit(*this, output_slot); }

std::shared_ptr<Node> ROMNode::Clone() const {
    return std::make_unique<ROMNode>(module, GUID::generate_guid(), data_bits, select_bits, m_rom_file);
}

nlohmann::json ROMNode::ToJson() const {
    nlohmann::json j = ConfigurableDataAndSelectBitWidthNode::ToJson();
    j["rom_file"] = m_rom_file;
    return j;
}

ROMNode::ROMNode(Module *module, const std::string &guid, const int data_bits, const int select_bits,
                 std::string data_file) :
    ConfigurableDataAndSelectBitWidthNode(guid, module, "ROM", data_bits, select_bits),
    m_rom_file(std::move(data_file)) {
    InitPinsAfterConfig();
}

ROMNode::ROMNode(Module *module) : ConfigurableDataAndSelectBitWidthNode(module, "ROM") {}

void ROMNode::InitPinsAfterConfig() {
    int n = 0;
    // Inputs
    pins.push_back((Pin){ROM_IN_PIN_ADDRESS, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(select_bits)});
    pins.push_back((Pin){ROM_IN_PIN_CLOCK, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(1)});

    // Output
    pins.push_back((Pin){"Value", ax::NodeEditor::PinKind::Output, *this, n, PinDataType(data_bits)});
}

std::string ROMNode::GetDisplayName() const {
    return name + " " + std::to_string(static_cast<int>(powl(2.0, select_bits))) + "x" + std::to_string(data_bits);
}

void ROMNode::RenderInternals() {
    ImGui::PushItemWidth(60);
    ImGui::InputText(("ROM file##" + guid).c_str(), &m_rom_file);
    ImGui::PopItemWidth();
}

Pin ROMNode::GetAddressPin() { return FindPin(ROM_IN_PIN_ADDRESS).value(); }
Pin ROMNode::GetClockPin() { return FindPin(ROM_IN_PIN_CLOCK).value(); }
