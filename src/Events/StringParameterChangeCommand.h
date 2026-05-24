//
// Created by Gabe on 5/22/2026.
//
#pragma once

#include "Command.h"
#include "Node/Node.h"


class StringParameterChangeCommand : public Command {
public:
    StringParameterChangeCommand(std::shared_ptr<Module> module, std::shared_ptr<Node> node, std::string before,
                                 std::string after, int index) :
        Command(module), m_node(node), m_before(std::move(before)), m_after(std::move(after)), m_index(index) {}

    void execute() override;
    void undo() override;

    std::string str() override;

private:
    void SetValue(const std::string &val, int index);

    std::shared_ptr<Node> m_node;
    std::string m_before;
    std::string m_after;
    int m_index;
};
