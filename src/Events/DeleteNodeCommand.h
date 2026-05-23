//
// Created by Gabe on 5/22/2026.
//
#pragma once

#include <vector>


#include "Command.h"
#include "Link.h"
#include "Node/Node.h"


class DeleteNodeCommand : public Command {
public:
    DeleteNodeCommand(std::shared_ptr<Module> module, std::shared_ptr<Node> new_node,
                      std::vector<Link> displaced_links) :
        Command(module), m_new_node(new_node), m_displaced_links(displaced_links) {}


    void execute() override;
    void undo() override;
    std::string str() override;

private:
    std::shared_ptr<Node> m_new_node;
    std::vector<Link> m_displaced_links;
};
