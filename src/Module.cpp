//
// Created by gabed on 4/13/2026.
//

#include "Module.h"

#include <imgui_internal.h>
#include <iostream>
#include <optional>

#include <algorithm>
#include <utility>
#include <vector>
#include "misc/cpp/imgui_stdlib.h"

#include "Default/InputNode.h"
#include "Default/Node.h"
#include "Default/OutputNode.h"
#include "GUID.h"


Module::Module(std::string name) : name(std::move(name)) {
    config.SettingsFile = (name + ".json").c_str();
    context = CreateEditor(&config);
}

Module::~Module() {
    if (context) {
        DestroyEditor(context);
        context = nullptr;
    }
}

std::vector<ed::NodeId> copied_nodes;
std::vector<ed::LinkId> selected_nodes;
std::map<unsigned long long, ImVec2> copied_node_positions;

void Module::Render(const std::shared_ptr<ErrorManager> &error_manager) {
    ed::SetCurrentEditor(context);

    ImGuiWindowClass window_class;
    window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
    ImGui::SetNextWindowClass(&window_class);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    {
        ImGui::Begin("Node Editor Win", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoTitleBar);

        ax::NodeEditor::PushStyleColor(ax::NodeEditor::StyleColor_Bg, ImVec4(0.125, 0.125, 0.125, 1));

        ed::Begin("Node Editor");


        if (ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_C)) {
            copied_nodes.resize(ed::GetSelectedObjectCount());
            selected_nodes.resize(ed::GetSelectedObjectCount());

            int nodeCount = ed::GetSelectedNodes(copied_nodes.data(), static_cast<int>(copied_nodes.size()));
            int linkCount = ed::GetSelectedLinks(selected_nodes.data(), static_cast<int>(selected_nodes.size()));

            copied_nodes.resize(nodeCount);
            selected_nodes.resize(linkCount);
            copied_node_positions.clear();
            for (auto n: copied_nodes) {
                copied_node_positions[n.Get()] = ax::NodeEditor::GetNodePosition(n);
            }

        } else if (ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_V)) {
            std::map<std::string, std::string> guid_map;
            std::vector<std::string> old_nodes;


            ax::NodeEditor::ClearSelection();
            // copy each node and put in map
            for (const auto &n: copied_nodes) {
                const auto &curr_node = GetNode(n);
                if (!curr_node.has_value())
                    continue;

                const auto &curr_node_val = curr_node.value();
                old_nodes.push_back(curr_node_val->guid);
                auto copy = curr_node_val->Clone();

                copy->start_pos = ImVec2(copied_node_positions[n.Get()].x + 40, copied_node_positions[n.Get()].y + 40);
                copy->last_pos = {FLT_MAX, FLT_MAX};

                guid_map[curr_node_val->guid] = copy->guid;

                // Render to init node before selecting
                copy->Render(error_manager);
                ax::NodeEditor::SelectNode(copy->id.Get(), true);

                nodes.push_back(copy);
            }

            // get the links that are between only the nodes selected
            std::vector<std::string> old_links;

            struct link_copy_data {
                Link l;
                Node &in;
                Node &out;
            };

            std::vector<link_copy_data> links_to_copy;
            for (const auto &link: links) {
                const auto &in_pin_op = GetPin(link.input_guid);
                const auto &out_pin_op = GetPin(link.output_guid);
                if (!in_pin_op || !out_pin_op)
                    continue;

                auto &node_in = in_pin_op.value().GetNode();
                auto &node_out = out_pin_op.value().GetNode();

                if (std::ranges::find(old_nodes, node_in.guid) == old_nodes.end())
                    continue;
                if (std::ranges::find(old_nodes, node_out.guid) == old_nodes.end())
                    continue;

                links_to_copy.push_back({link, node_in, node_out});
            }


            for (const auto &[link, in_node, out_node]: links_to_copy) {
                std::cout << "copying " << link.id.Get() << " link" << std::endl;
                std::cout << "i " << link.input_guid << std::endl;
                std::cout << "o " << link.output_guid << std::endl;


                const auto &new_node_input_id = guid_map[in_node.guid];
                const auto &new_node_output_id = guid_map[out_node.guid];

                const auto &new_node_in_op = GetNode(new_node_input_id);
                const auto &new_node_out_op = GetNode(new_node_output_id);

                if (!new_node_in_op)
                    continue;
                if (!new_node_out_op)
                    continue;


                std::string in_suffix =
                        link.input_guid.substr(in_node.guid.size(), link.input_guid.size() - in_node.guid.size());

                std::string new_in_id = new_node_in_op.value()->guid + in_suffix;

                std::string out_suffix =
                        link.output_guid.substr(out_node.guid.size(), link.output_guid.size() - out_node.guid.size());

                std::string new_out_id = new_node_out_op.value()->guid + out_suffix;
                std::cout << "i " << new_in_id << std::endl;
                std::cout << "o " << new_out_id << std::endl;


                links.emplace_back(this, new_out_id, new_in_id);
            }
        }


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

        ImGui::End();
    }
    ImGui::PopStyleVar();
    ed::SetCurrentEditor(nullptr);
    RenderIOList();
}


bool Module::CreateLink(const Pin &a, const Pin &b) {
    const std::string out_pin_guid = a.GetDirection() == ax::NodeEditor::PinKind::Input ? a.GetGuid() : b.GetGuid();
    const std::string in_pin_guid = a.GetDirection() == ax::NodeEditor::PinKind::Output ? a.GetGuid() : b.GetGuid();

    printf("%s   %s\n", out_pin_guid.c_str(), in_pin_guid.c_str());

    for (const auto &link: links) {
        if (link.output_guid == out_pin_guid && link.input_guid == in_pin_guid) {
            // link already exists
            return false;
        }
        if (link.output_guid == out_pin_guid) {
            // remove old connection to this input and replace with current
            std::erase_if(links, [&](const Link &l) { return l.id == link.id; });
        }
    }

    if (b.CanConnect(a)) {
        links.emplace_back(this, out_pin_guid, in_pin_guid);
        return true;
    }

    return false;
}

std::optional<Node *> Module::GetNode(const std::string &guid) const {
    for (const auto &node: nodes) {
        if (node->guid == guid) {
            return node.get();
        }
    }
    return std::nullopt;
}

std::optional<Node *> Module::GetNode(const ax::NodeEditor::NodeId &id) const {
    for (const auto &node: nodes) {
        if (node->id == id) {
            return node.get();
        }
    }
    return std::nullopt;
}

std::optional<Pin> Module::GetPin(const std::string &guid) {
    for (const auto &node: nodes) {
        for (auto pin: node->pins) {
            if (pin.GetGuid() == guid) {
                return pin;
            }
        }
    }
    return std::nullopt;
}

std::optional<Pin> Module::GetPin(const ax::NodeEditor::PinId &id) {
    for (const auto &node: nodes) {
        for (auto pin: node->pins) {
            if (pin.GetId() == id) {
                return pin;
            }
        }
    }
    return std::nullopt;
}

void Module::RenderIOList() {
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
            std::string name = inputs[i];

            if (ImGui::InputText(("##INPUT" + std::to_string(i)).c_str(), &name)) {
                inputs[i] = name;
            }

            ImGui::SameLine();
            if (ImGui::Button(("+##INPUTS-INSTANTIATE" + std::to_string(i)).c_str(), ImVec2(0, 0))) {
                nodes.push_back(std::make_unique<InputNode>(this, GUID::generate_guid(), i));
            }
        }


        ImGui::EndChild();
        ImGui::Separator();

        const float width = ImGui::GetColumnWidth() / 2.0f - ImGui::GetStyle().ItemSpacing.x / 2.0f;


        if (ImGui::Button("+##INPUTS-PLUS", ImVec2(width, 0))) {
            inputs.emplace_back("New Input");
        }
        ImGui::SameLine();
        if (ImGui::Button("-##INPUTS-MINUS", ImVec2(width, 0))) {
            if (!inputs.empty())
                inputs.pop_back();
        }

        ImGui::TableNextColumn();


        ImGui::BeginChild("Outputs", ImVec2(0, -footer_height), false);
        ImGui::Text("%d Output/s", outputs.size());


        for (int i = 0; i < outputs.size(); i++) {
            std::string name = outputs.at(i);
            if (ImGui::InputText(("##OUTPUT" + std::to_string(i)).c_str(), &name)) {

                outputs[i] = name;
            }

            ImGui::SameLine();
            if (ImGui::Button(("+##OUTPUTS-INSTANTIATE" + std::to_string(i)).c_str(), ImVec2(0, 0))) {
                nodes.push_back(std::make_unique<OutputNode>(this, GUID::generate_guid(), i));
            }
        }


        ImGui::EndChild();

        ImGui::Separator();

        if (ImGui::Button("+##OUTPUTS-PLUS", ImVec2(width, 0))) {
            outputs.emplace_back("New Output");
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

void Module::RenderNodes(const std::shared_ptr<ErrorManager> &error_manager) const {
    for (const auto &node: nodes)
        node->Render(error_manager);
}

void Module::RenderLinks() const {
    for (const auto &link: links)
        link.Render();
}
