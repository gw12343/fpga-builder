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


void Module::Render(const std::shared_ptr<ErrorManager> &error_manager) {
    ed::SetCurrentEditor(context);

    ImGuiWindowClass window_class;
    window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
    ImGui::SetNextWindowClass(&window_class);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    {
        ImGui::Begin("Node Editor Win", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoTitleBar);
        ed::Begin("Node Editor");


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
    ImGui::Begin("IO");


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
