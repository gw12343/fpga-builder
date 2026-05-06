//
// Created by Gabe on 4/30/2026.
//

#include "Default/Node.h"
#include "ErrorManager.h"

void Node::Render(const std::shared_ptr<ErrorManager> &error_manager) {
    const bool is_error = error_manager->GetErrorNodeGuid() == guid;

    if (is_error) {
        PushStyleColor(ax::NodeEditor::StyleColor_NodeBg, ImVec4(160 / 255.0, 60 / 255.0, 90 / 255.0, 255 / 255.0));
        PushStyleColor(ax::NodeEditor::StyleColor_NodeBorder,
                       ImVec4(200 / 255.0, 100 / 255.0, 140 / 255.0, 255 / 255.0));
    }

    PushStyleColor(ax::NodeEditor::StyleColor_PinRectBorder, ImVec4(160 / 255.0, 60 / 255.0, 90 / 255.0, 255 / 255.0));
    BeginNode(id);

    if (last_pos.x == FLT_MAX && last_pos.y == FLT_MAX) {
        SetNodePosition(id, start_pos);
    }
    last_pos = GetNodePosition(id);

    ImDrawList *drawList = ImGui::GetWindowDrawList();

    const auto nodeWidth = static_cast<float>(GetNodeWidth());
    constexpr int PADDING = 7;

    const char *label = GetDisplayName().c_str();
    const ImVec2 labelSize = ImGui::CalcTextSize(label);
    const ImVec2 titleMin = ImGui::GetCursorScreenPos();
    const auto titleMax = ImVec2(titleMin.x + nodeWidth, titleMin.y + labelSize.y + PADDING);

    // reserve space
    ImGui::Dummy(ImVec2(nodeWidth, labelSize.y + PADDING));

    // draw background directly onto the node's draw list
    drawList->AddRectFilled(
            titleMin, titleMax,
            IM_COL32(GetUIColor().x * 255.0, GetUIColor().y * 255.0, GetUIColor().z * 255.0, GetUIColor().w * 255.0));

    // draw centered label on top
    const float labelX = titleMin.x + (nodeWidth - labelSize.x) * 0.5f;
    const float labelY = titleMin.y + PADDING * 0.5f;
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


    ax::NodeEditor::EndNode();

    ax::NodeEditor::PopStyleColor(1);
    if (is_error) {
        ax::NodeEditor::PopStyleColor(2);
    }
}

void Node::RenderInternals() {}
