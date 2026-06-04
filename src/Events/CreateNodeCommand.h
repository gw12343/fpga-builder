//
// Created by Gabe on 5/22/2026.
//

#pragma once

#include "Command.h"
#include "Node/Node.h"


class CreateNodeCommand : public Command {
public:
    CreateNodeCommand(std::shared_ptr<Module> module, std::shared_ptr<Node> new_node) :
        Command(module), m_new_node(new_node) {}


    void execute() override;
    void undo() override;
    std::string str() override;

private:
    std::shared_ptr<Node> m_new_node;
};
