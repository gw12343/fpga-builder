//
// Created by Gabe on 4/19/2026.
//

#pragma once
#include <string>

#include "Default/Node.h"


class ErrorManager {
public:
    void Render(const std::shared_ptr<Module>& module);
    void ThrowError(const std::string& msg, const Node& node);
    [[nodiscard]] const std::string& GetErrorNodeGuid() const { return error_node_guid; };
private:
    bool error_present = false;
    std::string error_msg;
    std::string error_node_guid;
};
