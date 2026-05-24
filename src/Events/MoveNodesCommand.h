//
// Created by Gabe on 5/22/2026.
//
#pragma once

#include "Command.h"
#include "Node/Node.h"

struct NodeMove {
    std::shared_ptr<Node> node;
    ImVec2 start_pos;
    ImVec2 end_pos;
};

class MoveNodesCommand : public Command {
public:
    MoveNodesCommand(std::shared_ptr<Module> module, std::vector<NodeMove> node_moves) :
        Command(module), m_node_moves(node_moves) {}

    void execute() override;
    void undo() override;
    std::string str() override;

private:
    std::vector<NodeMove> m_node_moves;
};
