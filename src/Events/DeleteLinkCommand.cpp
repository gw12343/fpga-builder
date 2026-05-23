//
// Created by Gabe on 5/23/2026.
//

#include "DeleteLinkCommand.h"

#include "Module.h"

void DeleteLinkCommand::execute() { m_module->RemoveLink(m_link.id); }
void DeleteLinkCommand::undo() { m_module->AddLink(m_link); }

std::string DeleteLinkCommand::str() {
    return "Delete link: " + std::to_string(m_link.id.Get()) + "    from: " + m_link.input_guid + "  to  " +
           m_link.output_guid;
}
