//
// Created by Gabe on 5/22/2026.
//
#pragma once

#include "Command.h"
#include "Node/Node.h"


class NumericParameterChangeCommand : public Command {
public:
    NumericParameterChangeCommand(std::shared_ptr<Module> module, std::shared_ptr<Node> node, long long before,
                                  long long after, int index) :
        Command(module), m_node(node), m_before(before), m_after(after), m_index(index) {}

    void execute() override;
    void undo() override;

    std::string str() override;

private:
    void SetValue(long long val, int index);

    std::shared_ptr<Node> m_node;
    long long m_before;
    long long m_after;
    int m_index;
};
