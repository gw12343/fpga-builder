//
// Created by Gabe on 5/23/2026.
//

#include "CreateLinkCommand.h"

#include "Module.h"

void CreateLinkCommand::execute() {
    m_module->AddLink(m_link);
    for (const auto &l: m_displaced_links) {
        m_module->RemoveLink(l.id);
    }
}
void CreateLinkCommand::undo() {
    m_module->RemoveLink(m_link.id);
    for (const auto &l: m_displaced_links) {
        m_module->AddLink(l);
    }
}

std::string CreateLinkCommand::str() {
    return "Create link: " + std::to_string(m_link.id.Get()) + "    from: " + m_link.input_guid + "  to  " +
           m_link.output_guid;
}
