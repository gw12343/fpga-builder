//
// Created by gabed on 4/14/2026.
//

#include "Node.h"

#include <utility>

#include "ErrorManager.h"
#include "../GUID.h"
#include "../Pin.h"
#include "../Module.h"


Node::Node(Module* parent, const std::string &name, const std::vector<std::string> &inputs, const std::vector<std::string> &outputs) : Node(GUID::generate_guid(), parent, name, inputs, outputs) {

}

Node::Node(std::string saved_guid, Module* parent, std::string name, std::vector<std::string> inputs, std::vector<std::string> outputs) : name(name) {
    module = parent;
    guid = std::move(saved_guid);
    id = GUID::to_id(guid);


    int i = 0;
    for (const auto& input_name : inputs) {
        Pin new_input(input_name, ax::NodeEditor::PinKind::Input, *this, i++);
        pins.push_back(new_input);
    }

    for (const auto& output_name : outputs) {
        Pin new_output(output_name, ax::NodeEditor::PinKind::Output, *this, i++);
        pins.push_back(new_output);
    }


}


void Node::Render(const std::shared_ptr<ErrorManager>& error_manager) {
    bool is_error = error_manager->GetErrorNodeGuid() == guid;

    if(is_error) {
        ed::PushStyleColor(ed::StyleColor_NodeBg, ImVec4(160/255.0, 60/255.0, 90/255.0, 255/255.0));
        ed::PushStyleColor(ed::StyleColor_NodeBorder, ImVec4(200/255.0, 100/255.0, 140/255.0, 255/255.0));
    }
    BeginNode(id);

    if (last_pos.x == FLT_MAX && last_pos.y == FLT_MAX) {
        SetNodePosition(id, start_pos);
    }
    last_pos = GetNodePosition(id);

    ImGui::Text(name.c_str());

    RenderInternals();

    ImGui::BeginGroup();
    for (const auto& pin : pins) {
        if (pin.GetDirection() == ax::NodeEditor::PinKind::Input) pin.Render();
    }

    ImGui::EndGroup();
    ImGui::SameLine();



    ImGui::BeginGroup();
    for (const auto& pin : pins) {
        if (pin.GetDirection() == ax::NodeEditor::PinKind::Output) pin.Render();
    }

    ImGui::EndGroup();




    ed::EndNode();

    if(is_error) {
        ed::PopStyleColor(2);
    }
}

void Node::RenderInternals() {

}

std::optional<Pin> Node::FindPin(const std::string &name) {
    for (const auto& pin : pins) {
        if (pin.GetName() == name) return pin;
    }
    return std::nullopt;
}
