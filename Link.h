//
// Created by gabed on 4/14/2026.
//

#pragma once
#include <imgui.h>
#include <imgui_node_editor.h>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>


class Module;

class Link {
public:
    Link(Module *module, const std::string &output_guid, const std::string &input_guid);


    Link(Module *module, std::string saved_id, const std::string &output_guid, const std::string &input_guid);

    void Render() const;

    nlohmann::json to_json() const {
        return {{"output_guid", output_guid}, {"input_guid", input_guid}, {"id", id.Get()}};
    }

    ax::NodeEditor::LinkId id;
    Module *module;
    std::string output_guid;
    std::string input_guid;
    ImColor color;
};
