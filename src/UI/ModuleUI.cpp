//
// Created by Gabe on 4/28/2026.
//


#include <iostream>
#include "CopyPasteManager.h"

#include "Default/InputNode.h"
#include "Default/Node.h"
#include "Default/OutputNode.h"
#include "GUID.h"
#include "Link.h"


void Module::Render(const std::shared_ptr<ErrorManager> &error_manager,
                    const std::shared_ptr<CopyPasteManager> &copy_paste_manager) {


    {
        SetCurrentEditor(m_context);
        PushStyleColor(ax::NodeEditor::StyleColor_Bg, ImVec4(0.125, 0.125, 0.125, 1));

        ax::NodeEditor::Begin("Node Editor");


        copy_paste_manager->HandleCopyPaste(this, error_manager);


        if (ax::NodeEditor::BeginCreate()) {
            ax::NodeEditor::PinId inputPinId, outputPinId;
            if (QueryNewLink(&inputPinId, &outputPinId)) {


                if (inputPinId && outputPinId) // both are valid, let's accept link
                {

                    auto out = GetPin(outputPinId);
                    if (auto in = GetPin(inputPinId); out && in) {
                        if (!out->CanConnect(in.value())) {
                            ax::NodeEditor::RejectNewItem(ImColor(255, 0, 0), 2.0f);

                        } else {

                            // ax::NodeEditor::AcceptNewItem() return true when user release mouse button.
                            if (ax::NodeEditor::AcceptNewItem()) {

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


        if (ax::NodeEditor::BeginDelete()) {
            ax::NodeEditor::LinkId deletedLinkId;
            ax::NodeEditor::NodeId deletedNodeId;


            while (QueryDeletedNode(&deletedNodeId)) {
                if (ax::NodeEditor::AcceptDeletedItem()) {
                    for (auto &node: m_nodes) {
                        if (node->id == deletedNodeId) {

                            // check all pins on node
                            for (const auto &pin: node->pins) {
                                auto guid = pin.GetGuid();
                                std::erase_if(m_links, [guid](const Link &l) {
                                    return l.input_guid == guid || l.output_guid == guid;
                                });
                            }

                            std::erase_if(m_nodes, [deletedNodeId](const auto &n) { return n->id == deletedNodeId; });
                            break;
                        }
                    }
                }
            }

            while (QueryDeletedLink(&deletedLinkId)) {
                if (ax::NodeEditor::AcceptDeletedItem()) {

                    std::erase_if(m_links, [deletedLinkId](const Link &l) { return l.id == deletedLinkId; });
                    break;
                }
            }
        }

        ax::NodeEditor::EndDelete(); // Wrap up deletion action
        ax::NodeEditor::EndCreate();
        RenderNodes(error_manager);
        RenderLinks();
        ax::NodeEditor::End();
        ax::NodeEditor::PopStyleColor();
    }


    RenderModuleSettings();
}


void Module::RenderModuleSettings() {
    ImGui::Begin("Module Settings");

    std::string new_name = m_name;

    if (ImGui::InputText("Module Name", &new_name)) {
        Rename(new_name);
        std::cout << "rename module" << std::endl;
    }

    ImGui::InputText("Module GUID", &m_guid);

    if (ImGui::BeginTable("IO TABLE", 2,
                          ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersInnerV |
                                  ImGuiTableFlags_SizingStretchSame)) {
        ImGui::TableNextColumn();


        const float footer_height = ImGui::GetFrameHeightWithSpacing();

        ImGui::BeginChild("Inputs", ImVec2(0, -footer_height), false);
        ImGui::Text("%d Input/s", m_inputs.size());


        for (int i = 0; i < m_inputs.size(); ++i) {
            std::string name = m_inputs[i].name;

            if (ImGui::InputText(("##INPUT" + std::to_string(i)).c_str(), &name)) {
                m_inputs[i].name = name;
            }
            ImGui::SameLine();

            ImGui::PushItemWidth(30);
            if (ImGui::InputInt(("##INPUTS-BITS" + std::to_string(i)).c_str(), &m_inputs.at(i).bits, 0, 0)) {
                for (const auto &node: m_nodes) {
                    if (node->GetSerializationType() != "InputNode")
                        continue;

                    const auto &input_node = std::dynamic_pointer_cast<InputNode>(node);
                    if (input_node->slot != i)
                        continue;

                    DeleteAllLinksConnected(input_node);

                    input_node->UpdateBits(m_inputs.at(i).bits);
                }
            }

            ImGui::PopItemWidth();

            ImGui::SameLine();
            if (ImGui::Button(("+##INPUTS-INSTANTIATE" + std::to_string(i)).c_str(), ImVec2(0, 0))) {
                m_nodes.push_back(std::make_unique<InputNode>(this, GUID::generate_guid(), i));
            }
        }


        ImGui::EndChild();
        ImGui::Separator();

        const float width = ImGui::GetColumnWidth() / 2.0f - ImGui::GetStyle().ItemSpacing.x / 2.0f;


        if (ImGui::Button("+##INPUTS-PLUS", ImVec2(width, 0))) {
            m_inputs.emplace_back("New Input", 1);
        }


        ImGui::SameLine();
        if (ImGui::Button("-##INPUTS-MINUS", ImVec2(width, 0))) {
            if (!m_inputs.empty())
                m_inputs.pop_back();
        }

        ImGui::TableNextColumn();


        ImGui::BeginChild("Outputs", ImVec2(0, -footer_height), false);
        ImGui::Text("%d Output/s      |        Bits", m_outputs.size());


        for (int i = 0; i < m_outputs.size(); i++) {
            std::string name = m_outputs.at(i).name;
            if (ImGui::InputText(("##OUTPUT" + std::to_string(i)).c_str(), &name)) {
                m_outputs[i].name = name;
            }
            ImGui::SameLine();

            ImGui::PushItemWidth(30);
            if (ImGui::InputInt(("##OUTPUTS-BITS" + std::to_string(i)).c_str(), &m_outputs.at(i).bits, 0, 0)) {
                for (const auto &node: m_nodes) {
                    if (node->GetSerializationType() != "OutputNode")
                        continue;

                    const auto &output_node = std::dynamic_pointer_cast<OutputNode>(node);
                    if (output_node->slot != i)
                        continue;

                    DeleteAllLinksConnected(output_node);

                    output_node->UpdateBits(m_outputs.at(i).bits);
                }
            }
            ImGui::PopItemWidth();

            ImGui::SameLine();
            if (ImGui::Button(("+##OUTPUTS-INSTANTIATE" + std::to_string(i)).c_str(), ImVec2(0, 0))) {
                m_nodes.push_back(std::make_unique<OutputNode>(this, GUID::generate_guid(), i));
            }
        }


        ImGui::EndChild();

        ImGui::Separator();

        if (ImGui::Button("+##OUTPUTS-PLUS", ImVec2(width, 0))) {
            m_outputs.emplace_back("New Output", 1);
        }
        ImGui::SameLine();
        if (ImGui::Button("-##OUTPUTS-MINUS", ImVec2(width, 0))) {
            if (!m_outputs.empty())
                m_outputs.pop_back();
        }


        ImGui::EndTable();
    }

    ImGui::End();
}
