//
// Created by Gabe on 5/21/2026.
//

#include "RamNode.h"
#include "Events/StringParameterChangeCommand.h"
#include "GUID.h"
#include "Module.h"

void RAMNode::accept(Visitor &v, const int output_slot) { v.visit(*this, output_slot); }


std::shared_ptr<Node> RAMNode::Clone() const {
    return std::make_unique<RAMNode>(module, GUID::generate_guid(), m_data_bits, m_select_bits, m_ram_init_file);
}

nlohmann::json RAMNode::ToJson() const {
    nlohmann::json j = ConfigurableDataAndSelectBitWidthNode::ToJson();
    j["ram_init_file"] = m_ram_init_file;
    return j;
}

RAMNode::RAMNode(Module *module, const std::string &guid, const int data_bits, const int select_bits,
                 std::string data_file) :
    ConfigurableDataAndSelectBitWidthNode(guid, module, "RAM", data_bits, select_bits),
    m_ram_init_file(std::move(data_file)) {
    InitPinsAfterConfig();
}

RAMNode::RAMNode(Module *module) : ConfigurableDataAndSelectBitWidthNode(module, "RAM") {}
void RAMNode::InitPinsAfterConfig() {
    int n = 0;
    // Inputs
    pins.push_back({RAM_IN_PIN_ADDRESS, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(m_select_bits)});
    pins.push_back({RAM_IN_PIN_IN, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(m_data_bits)});
    pins.push_back({RAM_IN_PIN_LOAD, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(1)});
    pins.push_back({RAM_IN_PIN_CLOCK, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(1)});

    // Output
    pins.push_back({"Value", ax::NodeEditor::PinKind::Output, *this, n, PinDataType(m_data_bits)});
}

std::string RAMNode::GetDisplayName() const {
    return name + " " + std::to_string(static_cast<int>(powl(2.0, m_select_bits))) + "x" + std::to_string(m_data_bits);
}

void RAMNode::RenderInternals() {
    ImGui::PushItemWidth(60);
    const std::string old_val = m_ram_init_file;
    const bool f = ImGui::InputText(("Init file##" + guid).c_str(), &m_ram_init_file);

    if (f && m_ram_init_file != old_val) {
        const auto cmd = std::make_shared<StringParameterChangeCommand>(module->shared_from_this(), shared_from_this(),
                                                                        old_val, m_ram_init_file, 0);
        module->ExecuteCommand(cmd);
    }


    ImGui::PopItemWidth();
}

Pin RAMNode::GetAddressPin() { return FindPin(RAM_IN_PIN_ADDRESS).value(); }
Pin RAMNode::GetInPin() { return FindPin(RAM_IN_PIN_IN).value(); }
Pin RAMNode::GetLoadPin() { return FindPin(RAM_IN_PIN_LOAD).value(); }
Pin RAMNode::GetClockPin() { return FindPin(RAM_IN_PIN_CLOCK).value(); }
