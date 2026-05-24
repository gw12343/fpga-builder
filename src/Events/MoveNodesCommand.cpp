//
// Created by Gabe on 5/22/2026.
//

#include "MoveNodesCommand.h"

void MoveNodesCommand::execute() {
    for (auto &[node, start, end]: m_node_moves) {
        node->last_pos = {FLT_MAX, FLT_MAX};
        node->start_pos = end;
        node->m_is_dragging = false;
    }
}
void MoveNodesCommand::undo() {
    for (auto &[node, start, end]: m_node_moves) {
        node->last_pos = {FLT_MAX, FLT_MAX};
        node->start_pos = start;
        node->m_is_dragging = false;
    }
}

std::string MoveNodesCommand::str() { return "Move " + std::to_string(m_node_moves.size()) + " nodes"; }
