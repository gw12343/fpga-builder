//
// Created by Gabe on 6/4/2026.
//

#include "ChangeIOWidthCommand.h"

#include "Link.h"
#include "Node/IO/InputNode.h"
#include "Node/IO/OutputNode.h"

void ChangeIOWidthCommand::execute() {
    if (m_is_input) {
        m_module->GetInputs()[m_io_index].bits = m_new_size;
    } else {
        m_module->GetOutputs()[m_io_index].bits = m_new_size;
    }

    for (const auto &l: m_displaced_links) {
        m_module->RemoveLink(l.id);
    }

    UpdateIONodes();
}

void ChangeIOWidthCommand::undo() {
    if (m_is_input) {
        m_module->GetInputs()[m_io_index].bits = m_old_size;
    } else {
        m_module->GetOutputs()[m_io_index].bits = m_old_size;
    }

    for (const auto &l: m_displaced_links) {
        m_module->AddLink(l);
    }

    UpdateIONodes();
}

std::string ChangeIOWidthCommand::str() {
    return "Change width IO: " + std::to_string(m_old_size) + " to " + std::to_string(m_new_size);
}

// Updates the pin data types on all corresponding io nodes
void ChangeIOWidthCommand::UpdateIONodes() {
    if (m_is_input) {
        for (const auto &node: m_module->GetNodes()) {
            if (node->GetSerializationType() != "InputNode")
                continue;

            const auto &input_node = std::dynamic_pointer_cast<InputNode>(node);
            if (input_node->slot != m_io_index)
                continue;

            // DeleteAllLinksConnected(input_node);

            input_node->UpdateBits(m_module->GetInputs().at(m_io_index).bits);
        }
    } else {
        for (const auto &node: m_module->GetNodes()) {
            if (node->GetSerializationType() != "OutputNode")
                continue;

            const auto &output_node = std::dynamic_pointer_cast<OutputNode>(node);
            if (output_node->slot != m_io_index)
                continue;

            // DeleteAllLinksConnected(input_node);

            output_node->UpdateBits(m_module->GetInputs().at(m_io_index).bits);
        }
    }
}
