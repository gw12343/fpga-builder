//
// Created by Gabe on 5/22/2026.
//

#include "StringParameterChangeCommand.h"

#include "Node/Memory/RAMNode.h"
#include "Node/Memory/ROMNode.h"


void StringParameterChangeCommand::execute() { SetValue(m_after, m_index); }
void StringParameterChangeCommand::undo() { SetValue(m_before, m_index); }

std::string StringParameterChangeCommand::str() {
    return "change str ix: " + std::to_string(m_index) + "    from '" + m_before + "' to '" + m_after + "'";
}


void StringParameterChangeCommand::SetValue(const std::string &val, int index) {
    if (m_node->GetSerializationType() == "ROMNode") {
        auto rom_node = std::static_pointer_cast<ROMNode>(m_node);
        rom_node->m_rom_file = val;
    } else if (m_node->GetSerializationType() == "RAMNode") {
        auto rom_node = std::static_pointer_cast<RAMNode>(m_node);
        rom_node->m_ram_init_file = val;
    }
}
