//
// Created by Gabe on 4/19/2026.
//

#pragma once

#include "Default/Node.h"


class ErrorManager {
public:
    void Render(const std::optional<std::shared_ptr<Module>> &module);
    void ThrowError(const std::string &msg, const Node &node);
    [[nodiscard]] const std::string &GetErrorNodeGuid() const { return m_error_node_guid; }

private:
    bool m_error_present = false;
    std::string m_error_msg;
    std::string m_error_node_guid;
};
