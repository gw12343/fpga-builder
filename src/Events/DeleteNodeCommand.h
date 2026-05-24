//
// Created by Gabe on 5/22/2026.
//
#pragma once

#include <set>
#include <vector>


#include "Command.h"
#include "Link.h"
#include "Node/Node.h"


class DeleteNodeCommand : public Command {
public:
    DeleteNodeCommand(std::shared_ptr<Module> module, std::vector<std::shared_ptr<Node>> deleted_nodes,
                      std::set<Link> displaced_links) :
        Command(module), m_deleted_nodes(deleted_nodes), m_displaced_links(displaced_links) {}


    void execute() override;
    void undo() override;
    std::string str() override;

private:
    std::vector<std::shared_ptr<Node>> m_deleted_nodes;
    std::set<Link> m_displaced_links;
};
