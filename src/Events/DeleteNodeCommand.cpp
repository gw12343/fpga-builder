//
// Created by Gabe on 5/22/2026.
//

#include "DeleteNodeCommand.h"
#include "Module.h"

#include <iostream>


void DeleteNodeCommand::execute() {
    for (const auto &deleted_node: m_deleted_nodes) {
        m_module->RemoveNode(deleted_node->guid);
    }

    for (const auto &l: m_displaced_links) {
        m_module->RemoveLink(l.id);
    }
}

void DeleteNodeCommand::undo() {
    for (const auto &deleted_node: m_deleted_nodes) {
        m_module->AddNode(deleted_node);
        deleted_node->start_pos = deleted_node->last_pos;
        deleted_node->last_pos = {FLT_MAX, FLT_MAX};
    }
    for (const auto &l: m_displaced_links) {
        m_module->AddLink(l);
    }
}

std::string DeleteNodeCommand::str() {
    return "Delete " + std::to_string(m_deleted_nodes.size()) + " nodes and " +
           std::to_string(m_displaced_links.size()) + " links";
}
