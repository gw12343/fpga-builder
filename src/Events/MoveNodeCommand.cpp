//
// Created by Gabe on 5/22/2026.
//

#include "MoveNodeCommand.h"
void MoveNodeCommand::execute() {
    m_node->last_pos = {FLT_MAX, FLT_MAX};
    m_node->start_pos = m_end_pos;
    m_node->m_is_dragging = false;
}
void MoveNodeCommand::undo() {
    m_node->last_pos = {FLT_MAX, FLT_MAX};
    m_node->start_pos = m_start_pos;
    m_node->m_is_dragging = false;
}

std::string MoveNodeCommand::str() { return "Move node: " + m_node->GetDisplayName() + "   guid: " + m_node->guid; }
