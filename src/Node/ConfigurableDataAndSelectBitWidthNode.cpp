//
// Created by Gabe on 5/21/2026.
//

#include "ConfigurableDataAndSelectBitWidthNode.h"

// Constructor for new node
ConfigurableDataAndSelectBitWidthNode::ConfigurableDataAndSelectBitWidthNode(Module *parent, const std::string &name) :
    Node(parent, name) {
    m_data_bits = defaultDataBitsToUse;
    m_select_bits = defaultSelectBitsToUse;
}
// Constructor for saved node
ConfigurableDataAndSelectBitWidthNode::ConfigurableDataAndSelectBitWidthNode(const std::string &saved_guid,
                                                                             Module *parent, const std::string &name,
                                                                             const int data_width,
                                                                             const int select_width) :
    Node(saved_guid, parent, name) {
    m_data_bits = data_width;
    m_select_bits = select_width;
}


// Save bit widths to json
[[nodiscard]] nlohmann::json ConfigurableDataAndSelectBitWidthNode::ToJson() const {
    nlohmann::json j = Node::ToJson();
    j["data_bits"] = m_data_bits;
    j["select_bits"] = m_select_bits;
    return j;
}

// Override configuration methods
void ConfigurableDataAndSelectBitWidthNode::RenderConfiguration() {
    if (ImGui::DragInt("# Data Bits", &m_data_bits, 1, 1, 16)) {
        defaultDataBitsToUse = m_data_bits;
    }
    if (ImGui::DragInt("# Select Bits", &m_select_bits, 1, 1, 4)) {
        defaultSelectBitsToUse = m_select_bits;
    }
}

[[nodiscard]] std::string ConfigurableDataAndSelectBitWidthNode::GetDisplayName() const {
    return name + " (" + std::to_string(static_cast<int>(powl(2, m_select_bits))) + "-to-1 x" +
           std::to_string(m_data_bits) + ")";
}
