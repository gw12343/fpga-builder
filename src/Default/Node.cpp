//
// Created by gabed on 4/14/2026.
//


#define IMGUI_DEFINE_MATH_OPERATORS

#include "Node.h"

#include "ErrorManager.h"
#include "GUID.h"
#include "Module.h"
#include "Pins/Pin.h"

#include <utility>
#include "imgui.h"


Node::Node(Module *parent, const std::string &name, const std::vector<PinCreationData> &inputs,
           const std::vector<PinCreationData> &outputs) :
    Node(GUID::generate_guid(), parent, name, inputs, outputs) {}


Node::Node(std::string saved_guid, Module *parent, std::string in_name, const std::vector<PinCreationData> &inputs,
           const std::vector<PinCreationData> &outputs) {
    module = parent;
    guid = std::move(saved_guid);
    id = GUID::to_id(guid);
    name = std::move(in_name);

    InitPins(inputs, outputs);
}

// Non pin-creating constructors
Node::Node(Module *parent, const std::string &name) : Node(GUID::generate_guid(), parent, name) {}

Node::Node(std::string saved_guid, Module *parent, std::string name) : name(std::move(name)) {
    module = parent;
    guid = std::move(saved_guid);
    id = GUID::to_id(guid);
}


void Node::InitPins(const std::vector<PinCreationData> &inputs, const std::vector<PinCreationData> &outputs) {
    int i = 0;
    for (const auto &[name, type]: inputs) {
        Pin new_input(name, ax::NodeEditor::PinKind::Input, *this, i++, type);
        pins.push_back(new_input);
    }

    for (const auto &[name, type]: outputs) {
        Pin new_output(name, ax::NodeEditor::PinKind::Output, *this, i++, type);
        pins.push_back(new_output);
    }
}


void Node::Render(const std::shared_ptr<ErrorManager> &error_manager) {
    const bool is_error = error_manager->GetErrorNodeGuid() == guid;

    if (is_error) {
        PushStyleColor(ed::StyleColor_NodeBg, ImVec4(160 / 255.0, 60 / 255.0, 90 / 255.0, 255 / 255.0));
        PushStyleColor(ed::StyleColor_NodeBorder, ImVec4(200 / 255.0, 100 / 255.0, 140 / 255.0, 255 / 255.0));
    }

    PushStyleColor(ax::NodeEditor::StyleColor_PinRectBorder, ImVec4(160 / 255.0, 60 / 255.0, 90 / 255.0, 255 / 255.0));
    BeginNode(id);

    if (last_pos.x == FLT_MAX && last_pos.y == FLT_MAX) {
        SetNodePosition(id, start_pos);
    }
    last_pos = GetNodePosition(id);

    ImDrawList *drawList = ImGui::GetWindowDrawList();

    const auto nodeWidth = static_cast<float>(GetNodeWidth());
    constexpr int padding = 7;

    const char *label = GetDisplayName().c_str();
    const ImVec2 labelSize = ImGui::CalcTextSize(label);
    const ImVec2 titleMin = ImGui::GetCursorScreenPos();
    const auto titleMax = ImVec2(titleMin.x + nodeWidth, titleMin.y + labelSize.y + padding);

    // reserve space
    ImGui::Dummy(ImVec2(nodeWidth, labelSize.y + padding));

    // draw background directly onto the node's draw list
    drawList->AddRectFilled(
            titleMin, titleMax,
            IM_COL32(GetUIColor().x * 255.0, GetUIColor().y * 255.0, GetUIColor().z * 255.0, GetUIColor().w * 255.0));

    // draw centered label on top
    const float labelX = titleMin.x + (nodeWidth - labelSize.x) * 0.5f;
    const float labelY = titleMin.y + padding * 0.5f;
    drawList->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(labelX, labelY), IM_COL32(255, 255, 255, 255),
                      label);


    RenderInternals();

    ImGui::BeginGroup();

    const auto x = ImGui::GetCursorPosX();

    for (const auto &pin: pins) {
        if (pin.GetDirection() == ax::NodeEditor::PinKind::Input)
            pin.Render();
    }

    ImGui::EndGroup();

    ImGui::SameLine();

    ImGui::BeginGroup();
    for (const auto &pin: pins) {
        if (pin.GetDirection() == ax::NodeEditor::PinKind::Output) {
            ImGui::SetCursorPosX(x + static_cast<float>(GetNodeWidth()) - ImGui::CalcTextSize(pin.GetName().c_str()).x);
            pin.Render();
        }
    }

    ImGui::EndGroup();


    ed::EndNode();

    ed::PopStyleColor(1);
    if (is_error) {
        ed::PopStyleColor(2);
    }
}

void Node::RenderInternals() {}

std::optional<Pin> Node::FindPin(const std::string &name) {
    for (const auto &pin: pins) {
        if (pin.GetName() == name)
            return pin;
    }
    return std::nullopt;
}
