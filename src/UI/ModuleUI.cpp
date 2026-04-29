//
// Created by Gabe on 4/28/2026.
//


#include <imgui_internal.h>
#include <iostream>
#include <optional>
#include <vector>

#include "CopyPasteManager.h"
#include "misc/cpp/imgui_stdlib.h"

#include "Default/InputNode.h"
#include "Default/Node.h"
#include "Default/OutputNode.h"
#include "GUID.h"


void Module::Render(const std::shared_ptr<ErrorManager> &error_manager,
                    const std::shared_ptr<CopyPasteManager> &copy_paste_manager) {


    {
        ed::SetCurrentEditor(context);
        PushStyleColor(ax::NodeEditor::StyleColor_Bg, ImVec4(0.125, 0.125, 0.125, 1));

        ed::Begin("Node Editor");


        copy_paste_manager->HandleCopyPaste(this, error_manager);


        if (ed::BeginCreate()) {
            ed::PinId inputPinId, outputPinId;
            if (ed::QueryNewLink(&inputPinId, &outputPinId)) {


                if (inputPinId && outputPinId) // both are valid, let's accept link
                {

                    auto out = GetPin(outputPinId);
                    auto in = GetPin(inputPinId);
                    if (out && in) {
                        if (!out->CanConnect(in.value())) {
                            ed::RejectNewItem(ImColor(255, 0, 0), 2.0f);

                        } else {

                            // ed::AcceptNewItem() return true when user release mouse button.
                            if (ed::AcceptNewItem()) {

                                Pin outPin = out.value();
                                Pin inPin = in.value();

                                auto b = CreateLink(outPin, inPin);

                                printf("LINK: %d\n", b);

                                printf("Connected %d bits to %d bits \n", outPin.GetDataType().GetBitWidth(),
                                       inPin.GetDataType().GetBitWidth());
                            }
                        }
                    }
                }
            }
        }


        if (ed::BeginDelete()) {
            ed::LinkId deletedLinkId;
            ed::NodeId deletedNodeId;


            while (QueryDeletedNode(&deletedNodeId)) {
                if (ed::AcceptDeletedItem()) {
                    for (auto &node: nodes) {
                        if (node->id == deletedNodeId) {

                            // check all pins on node
                            for (const auto &pin: node->pins) {
                                auto guid = pin.GetGuid();
                                std::erase_if(links, [guid](const Link &l) {
                                    return l.input_guid == guid || l.output_guid == guid;
                                });
                            }

                            std::erase_if(nodes, [deletedNodeId](const auto &n) { return n->id == deletedNodeId; });
                            break;
                        }
                    }
                }
            }

            while (QueryDeletedLink(&deletedLinkId)) {
                if (ed::AcceptDeletedItem()) {

                    std::erase_if(links, [deletedLinkId](const Link &l) { return l.id == deletedLinkId; });
                    break;
                }
            }
        }

        ed::EndDelete(); // Wrap up deletion action
        ed::EndCreate();
        RenderNodes(error_manager);
        RenderLinks();
        ed::End();
        ax::NodeEditor::PopStyleColor();
    }


    RenderModuleSettings();
}


void Module::RenderModuleSettings() {
    ImGui::Begin("Module Settings");

    ImGui::InputText("Module Name", &name);

    if (ImGui::BeginTable("IO TABLE", 2,
                          ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersInnerV |
                                  ImGuiTableFlags_SizingStretchSame)) {
        ImGui::TableNextColumn();


        const float footer_height = ImGui::GetFrameHeightWithSpacing();

        ImGui::BeginChild("Inputs", ImVec2(0, -footer_height), false);
        ImGui::Text("%d Input/s", inputs.size());


        for (int i = 0; i < inputs.size(); ++i) {
            std::string name = inputs[i].name;

            if (ImGui::InputText(("##INPUT" + std::to_string(i)).c_str(), &name)) {
                inputs[i].name = name;
            }
            ImGui::SameLine();

            ImGui::PushItemWidth(30);
            if (ImGui::InputInt(("##INPUTS-BITS" + std::to_string(i)).c_str(), &inputs.at(i).bits, 0, 0)) {
                for (const auto &node: nodes) {
                    if (node->GetSerializationType() != "InputNode")
                        continue;

                    const auto &input_node = std::dynamic_pointer_cast<InputNode>(node);
                    if (input_node->slot != i)
                        continue;

                    DeleteAllLinksConnected(input_node);

                    input_node->UpdateBits(inputs.at(i).bits);
                }
            }

            ImGui::PopItemWidth();

            ImGui::SameLine();
            if (ImGui::Button(("+##INPUTS-INSTANTIATE" + std::to_string(i)).c_str(), ImVec2(0, 0))) {
                nodes.push_back(std::make_unique<InputNode>(this, GUID::generate_guid(), i));
            }
        }


        ImGui::EndChild();
        ImGui::Separator();

        const float width = ImGui::GetColumnWidth() / 2.0f - ImGui::GetStyle().ItemSpacing.x / 2.0f;


        if (ImGui::Button("+##INPUTS-PLUS", ImVec2(width, 0))) {
            inputs.emplace_back("New Input", 1);
        }


        ImGui::SameLine();
        if (ImGui::Button("-##INPUTS-MINUS", ImVec2(width, 0))) {
            if (!inputs.empty())
                inputs.pop_back();
        }

        ImGui::TableNextColumn();


        ImGui::BeginChild("Outputs", ImVec2(0, -footer_height), false);
        ImGui::Text("%d Output/s      |        Bits", outputs.size());


        for (int i = 0; i < outputs.size(); i++) {
            std::string name = outputs.at(i).name;
            if (ImGui::InputText(("##OUTPUT" + std::to_string(i)).c_str(), &name)) {

                outputs[i].name = name;
            }
            ImGui::SameLine();

            ImGui::PushItemWidth(30);
            if (ImGui::InputInt(("##OUTPUTS-BITS" + std::to_string(i)).c_str(), &outputs.at(i).bits, 0, 0)) {
                for (const auto &node: nodes) {
                    if (node->GetSerializationType() != "OutputNode")
                        continue;

                    const auto &output_node = std::dynamic_pointer_cast<OutputNode>(node);
                    if (output_node->slot != i)
                        continue;

                    DeleteAllLinksConnected(output_node);

                    output_node->UpdateBits(outputs.at(i).bits);
                }
            }
            ImGui::PopItemWidth();

            ImGui::SameLine();
            if (ImGui::Button(("+##OUTPUTS-INSTANTIATE" + std::to_string(i)).c_str(), ImVec2(0, 0))) {
                nodes.push_back(std::make_unique<OutputNode>(this, GUID::generate_guid(), i));
            }
        }


        ImGui::EndChild();

        ImGui::Separator();

        if (ImGui::Button("+##OUTPUTS-PLUS", ImVec2(width, 0))) {
            outputs.emplace_back("New Output", 1);
        }
        ImGui::SameLine();
        if (ImGui::Button("-##OUTPUTS-MINUS", ImVec2(width, 0))) {
            if (!outputs.empty())
                outputs.pop_back();
        }


        ImGui::EndTable();
    }

    ImGui::End();
}
