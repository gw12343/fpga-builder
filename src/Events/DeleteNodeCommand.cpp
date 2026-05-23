//
// Created by Gabe on 5/22/2026.
//

#include "DeleteNodeCommand.h"
#include "Module.h"

#include <iostream>


void DeleteNodeCommand::execute() {
    m_module->RemoveNode(m_new_node->guid);

    for (const auto &l: m_displaced_links) {
        m_module->RemoveLink(l.id);
    }
}

void DeleteNodeCommand::undo() {
    m_module->AddNode(m_new_node);
    m_new_node->start_pos = m_new_node->last_pos;
    m_new_node->last_pos = {FLT_MAX, FLT_MAX};

    for (const auto &l: m_displaced_links) {
        m_module->AddLink(l);
    }
}

std::string DeleteNodeCommand::str() { return "Delete node: " + m_new_node->name + "    guid: " + m_new_node->guid; }
