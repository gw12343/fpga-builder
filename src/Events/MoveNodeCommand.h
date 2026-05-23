//
// Created by Gabe on 5/22/2026.
//
#pragma once

#include "Command.h"
#include "Node/Node.h"


class MoveNodeCommand : public Command {
public:
    MoveNodeCommand(std::shared_ptr<Module> module, std::shared_ptr<Node> node, ImVec2 start, ImVec2 end) :
        Command(module), m_node(node), m_start_pos(start), m_end_pos(end) {}

    void execute() override;
    void undo() override;
    std::string str() override;

private:
    std::shared_ptr<Node> m_node;
    ImVec2 m_start_pos;
    ImVec2 m_end_pos;
};
