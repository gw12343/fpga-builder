//
// Created by Gabe on 5/22/2026.
//

#include "NumericParameterChangeCommand.h"

#include "Node/BitwiseOperator/ShifterNode.h"
#include "Node/IO/InputNode.h"
#include "Node/IO/OutputNode.h"
#include "Node/Wiring/LiteralNode.h"

void NumericParameterChangeCommand::execute() { SetValue(m_after, m_index); }
void NumericParameterChangeCommand::undo() { SetValue(m_before, m_index); }

std::string NumericParameterChangeCommand::str() {
    return "change number ix: " + std::to_string(m_index) + "    from " + std::to_string(m_before) + " to " +
           std::to_string(m_after);
}


void NumericParameterChangeCommand::SetValue(long long val, int index) {
    if (m_node->GetSerializationType() == "LiteralNode") {
        auto literal_node = std::static_pointer_cast<LiteralNode>(m_node);
        literal_node->value = val;
    } else if (m_node->GetSerializationType() == "InputNode") {
        auto input_node = std::static_pointer_cast<InputNode>(m_node);
        input_node->slot = val;
    } else if (m_node->GetSerializationType() == "OutputNode") {
        auto output_node = std::static_pointer_cast<OutputNode>(m_node);
        output_node->slot = val;
    } else if (m_node->GetSerializationType() == "ShifterNode") {
        auto shifter_node = std::static_pointer_cast<ShifterNode>(m_node);
        shifter_node->type_index = val;
    }
}
