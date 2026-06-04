//
// Created by Gabe on 4/28/2026.
//


#include <iostream>

#include "Events/ChangeIOWidthCommand.h"
#include "Events/Command.h"
#include "Events/CreateIOCommand.h"
#include "Events/DeleteIOCommand.h"
#include "Events/DeleteLinkCommand.h"
#include "Events/DeleteNodeCommand.h"
#include "Events/PasteCommand.h"
#include "Events/RenameIOCommand.h"
#include "GUID.h"
#include "Link.h"
#include "Module.h"
#include "Node/IO/InputNode.h"
#include "Node/IO/OutputNode.h"
#include "Node/Node.h"


void Module::Render(const std::shared_ptr<ErrorManager> &error_manager) {
    {
        SetCurrentEditor(m_context);
        PushStyleColor(ax::NodeEditor::StyleColor_Bg, ImVec4(0.125, 0.125, 0.125, 1));

        ax::NodeEditor::Begin("Node Editor");


        {
            if (!ImGui::GetIO().WantTextInput) {
                if (ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_Z)) {
                    Undo();
                } else if (ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_Y)) {
                    Redo();
                }

                // If copy
                if (ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_C)) {
                    std::cout << "copy nodes" << std::endl;

                    m_copied_nodes.clear();
                    m_copied_links.clear();


                    std::vector<ax::NodeEditor::NodeId> copied_node_ids;
                    std::vector<std::string> copied_node_guids;
                    copied_node_ids.resize(ax::NodeEditor::GetSelectedObjectCount());
                    const int node_count =
                            GetSelectedNodes(copied_node_ids.data(), static_cast<int>(copied_node_ids.size()));
                    copied_node_ids.resize(node_count);

                    for (const auto &n: copied_node_ids) {
                        auto node = GetNode(n);
                        m_copied_nodes.push_back(node.value()->shared_from_this());
                        copied_node_guids.push_back(node.value()->guid);
                    }

                    for (const auto &link: m_links) {
                        const auto &in_pin_op = GetPin(link.input_guid);
                        const auto &out_pin_op = GetPin(link.output_guid);
                        if (!in_pin_op || !out_pin_op)
                            continue;

                        auto &node_in = in_pin_op.value().GetNode();
                        auto &node_out = out_pin_op.value().GetNode();

                        if (std::ranges::find(copied_node_guids, node_in.guid) == copied_node_guids.end())
                            continue;
                        if (std::ranges::find(copied_node_guids, node_out.guid) == copied_node_guids.end())
                            continue;

                        m_copied_links.push_back(link);
                    }

                    std::cout << std::to_string(m_copied_nodes.size()) << " nodes" << std::endl;
                    std::cout << std::to_string(m_copied_links.size()) << " links" << std::endl;
                } else if (ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_V)) {
                    std::cout << "paste nodes" << std::endl;

                    std::vector<std::shared_ptr<Node>> paste_command_nodes;
                    std::vector<Link> paste_command_links;

                    std::map<std::string, std::string> guid_map;
                    std::map<std::string, std::shared_ptr<Node>> node_copies;
                    // Copy saved nodes
                    for (const auto &n: m_copied_nodes) {
                        auto node_copy = n->Clone();
                        node_copy->start_pos = {n->last_pos.x + 50, n->last_pos.y + 50};
                        node_copy->last_pos = {FLT_MAX, FLT_MAX};
                        node_copy->m_is_dragging = false;

                        guid_map[n->guid] = node_copy->guid;
                        node_copies[node_copy->guid] = node_copy;

                        paste_command_nodes.push_back(node_copy);
                        AddNode(node_copy);
                    }


                    // Copy saved links
                    for (const auto &l: m_copied_links) {
                        const auto &in_pin_op = GetPin(l.input_guid);
                        const auto &out_pin_op = GetPin(l.output_guid);


                        auto &node_in = in_pin_op.value().GetNode();
                        auto &node_out = out_pin_op.value().GetNode();


                        const auto &new_node_input_id = guid_map[node_in.guid];
                        const auto &new_node_output_id = guid_map[node_out.guid];


                        const auto &new_node_in_op = node_copies[new_node_input_id];
                        const auto &new_node_out_op = node_copies[new_node_output_id];


                        std::string in_suffix =
                                l.input_guid.substr(node_in.guid.size(), l.input_guid.size() - node_in.guid.size());

                        std::string new_in_id = new_node_in_op->guid + in_suffix;

                        std::string out_suffix =
                                l.output_guid.substr(node_out.guid.size(), l.output_guid.size() - node_out.guid.size());

                        std::string new_out_id = new_node_out_op->guid + out_suffix;

                        // Link link_copy{this, new_out_id, new_in_id};
                        // AddLink(link_copy);
                        paste_command_links.emplace_back(this, new_out_id, new_in_id);
                    }


                    auto cmd = std::make_shared<PasteCommand>(shared_from_this(), paste_command_nodes,
                                                              paste_command_links);
                    ExecuteCommand(cmd);
                }
            }
        }


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


            std::set<Link> displaced_links;
            std::vector<std::shared_ptr<Node>> deleted_nodes;

            while (QueryDeletedNode(&deletedNodeId)) {
                if (ax::NodeEditor::AcceptDeletedItem()) {
                    for (auto &node: m_nodes) {
                        if (node->id == deletedNodeId) {
                            // check all pins on node
                            for (const auto &pin: node->pins) {
                                auto guid = pin.GetGuid();

                                for (const auto &l: m_links) {
                                    if (l.input_guid == guid || l.output_guid == guid) {
                                        displaced_links.insert(l);
                                    }
                                }
                            }

                            deleted_nodes.push_back(node);
                        }
                    }
                }
            }

            if (!displaced_links.empty() || !deleted_nodes.empty()) {
                std::cout << " delete node cmd" << std::endl;
                auto cmd = std::make_shared<DeleteNodeCommand>(shared_from_this(), deleted_nodes, displaced_links);
                ExecuteCommand(cmd);
            }

            std::vector<Link> deleted_links;
            while (QueryDeletedLink(&deletedLinkId)) {
                if (ax::NodeEditor::AcceptDeletedItem()) {
                    for (const auto &l: m_links) {
                        if (l.id != deletedLinkId)
                            continue;

                        deleted_links.push_back(l);
                    }
                }
            }

            if (!deleted_links.empty()) {
                const auto cmd = std::make_shared<DeleteLinkCommand>(shared_from_this(), deleted_links);
                ExecuteCommand(cmd);
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
    RenderUndoRedoList();
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
            if (ImGui::InputText(("##INPUT" + std::to_string(i)).c_str(), &m_inputs[i].name)) {
                if (m_inputs[i].name != name) {
                    auto cmd = std::make_shared<RenameIOCommand>(shared_from_this(), i, true, name, m_inputs[i].name);
                    ExecuteCommand(cmd);
                }
            }
            ImGui::SameLine();

            ImGui::PushItemWidth(30);


            int old_bits = m_inputs.at(i).bits;
            if (ImGui::InputInt(("##INPUTS-BITS" + std::to_string(i)).c_str(), &m_inputs.at(i).bits, 0, 0)) {
                if (old_bits != m_inputs.at(i).bits) {


                    std::set<Link> displaced_links;


                    for (const auto &node: m_nodes) {
                        if (node->GetSerializationType() != "InputNode")
                            continue;

                        const auto &input_node = std::dynamic_pointer_cast<InputNode>(node);
                        if (input_node->slot != i)
                            continue;

                        for (const auto &pin: node->pins) {
                            for (const auto &link: m_links) {

                                if (link.input_guid == pin.GetGuid() || link.output_guid == pin.GetGuid()) {
                                    displaced_links.insert(link);
                                }
                            }
                        }
                    }


                    auto cmd = std::make_shared<ChangeIOWidthCommand>(shared_from_this(), i, true, old_bits,
                                                                      m_inputs.at(i).bits, displaced_links);
                    ExecuteCommand(cmd);
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
            const auto cmd = std::make_shared<CreateIOCommand>(shared_from_this(), IO("New Input", 1), true);
            ExecuteCommand(cmd);
        }


        ImGui::SameLine();
        if (ImGui::Button("-##INPUTS-MINUS", ImVec2(width, 0))) {
            const auto cmd = std::make_shared<DeleteIOCommand>(shared_from_this(), m_inputs.back(), true);
            ExecuteCommand(cmd);
        }

        ImGui::TableNextColumn();


        ImGui::BeginChild("Outputs", ImVec2(0, -footer_height), false);
        ImGui::Text("%d Output/s      |        Bits", m_outputs.size());


        for (int i = 0; i < m_outputs.size(); i++) {
            std::string name = m_outputs.at(i).name;
            if (ImGui::InputText(("##OUTPUT" + std::to_string(i)).c_str(), &m_outputs[i].name)) {
                if (m_outputs[i].name != name) {
                    auto cmd = std::make_shared<RenameIOCommand>(shared_from_this(), i, false, name, m_outputs[i].name);
                    ExecuteCommand(cmd);
                }
            }
            ImGui::SameLine();

            ImGui::PushItemWidth(30);

            int old_bits = m_outputs.at(i).bits;
            if (ImGui::InputInt(("##OUTPUTS-BITS" + std::to_string(i)).c_str(), &m_outputs.at(i).bits, 0, 0)) {
                if (old_bits != m_outputs.at(i).bits) {
                    auto cmd = std::make_shared<ChangeIOWidthCommand>(shared_from_this(), i, false, old_bits,
                                                                      m_outputs.at(i).bits, std::set<Link>());
                    ExecuteCommand(cmd);
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
            const auto cmd = std::make_shared<CreateIOCommand>(shared_from_this(), IO("New Output", 1), false);
            ExecuteCommand(cmd);
        }
        ImGui::SameLine();
        if (ImGui::Button("-##OUTPUTS-MINUS", ImVec2(width, 0))) {
            const auto cmd = std::make_shared<DeleteIOCommand>(shared_from_this(), m_outputs.back(), false);
            ExecuteCommand(cmd);
        }


        ImGui::EndTable();
    }

    ImGui::End();
}

void Module::RenderUndoRedoList() {
    ImGui::Begin("Undo Redo");

    if (ImGui::Button("Undo")) {
        Undo();
    }

    if (ImGui::Button("Redo")) {
        Redo();
    }

    ImGui::Text("undo stack (%d): ", m_undo_stack.size());

    int n = 0;
    for (const auto c: m_undo_stack) {
        ImGui::Button(("(u" + std::to_string(n++) + ") " + c->str()).c_str());
    }

    ImGui::Text("redo stack (%d): ", m_redo_stack.size());
    n = 0;
    for (const auto c: m_redo_stack) {
        ImGui::Button(("(r" + std::to_string(n++) + ") " + c->str()).c_str());
    }
    ImGui::End();
}
