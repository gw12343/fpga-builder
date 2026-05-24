//
// Created by Gabe on 5/22/2026.
//

#include "MoveNodeCommand.h"
void MoveNodeCommand::execute() {
    for (auto &[node, start, end]: m_node_moves) {
        node->last_pos = {FLT_MAX, FLT_MAX};
        node->start_pos = end;
        node->m_is_dragging = false;
    }
}
void MoveNodeCommand::undo() {
    for (auto &[node, start, end]: m_node_moves) {
        node->last_pos = {FLT_MAX, FLT_MAX};
        node->start_pos = start;
        node->m_is_dragging = false;
    }
}

std::string MoveNodeCommand::str() { return "Move " + std::to_string(m_node_moves.size()) + " nodes"; }
