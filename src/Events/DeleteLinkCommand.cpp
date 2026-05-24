//
// Created by Gabe on 5/23/2026.
//

#include "DeleteLinkCommand.h"

#include "Module.h"

void DeleteLinkCommand::execute() {
    for (const auto &link: m_links) {
        m_module->RemoveLink(link.id);
    }
}
void DeleteLinkCommand::undo() {
    for (const auto &link: m_links) {
        m_module->AddLink(link);
    }
}

std::string DeleteLinkCommand::str() { return "Delete " + std::to_string(m_links.size()) + " links"; }
