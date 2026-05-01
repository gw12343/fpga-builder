//
// Created by Gabe on 4/29/2026.
//

#pragma once

#include <iostream>


#include "GUID.h"
#include "Module.h"
#include "Node.h"
#include "Project/Project.h"

class Module;
class CustomModuleNode final : public Node {
public:
    [[nodiscard]] std::string GetSerializationType() const override { return "CustomNode"; }

    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }


    // Constructor for new node
    explicit CustomModuleNode(Module *parent) : Node(parent, "Custom") { module_guid = ""; }
    // Constructor for saved node
    CustomModuleNode(Module *parent, const std::string &saved_guid, const std::string &saved_module_guid) :
        Node(saved_guid, parent, "Custom") {
        module_guid = saved_module_guid;
        CustomModuleNode::InitPinsAfterConfig();
    }


    void InitPinsAfterConfig() override {
        pins.clear();
        if (!module_ref.has_value())
            return;

        const auto &target_module = module_ref.value();

        int n = 0;
        for (const auto &[name, bits]: target_module->GetInputs()) {
            pins.push_back((Pin){name, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(bits)});
        }
        for (const auto &[name, bits]: target_module->GetOutputs()) {
            pins.push_back((Pin){name, ax::NodeEditor::PinKind::Output, *this, n++, PinDataType(bits)});
        }
    }

    [[nodiscard]] std::shared_ptr<Node> Clone() const override {
        return std::make_unique<CustomModuleNode>(module, GUID::generate_guid(), module_guid);
    }

    static constexpr ImVec4 COLOR = {1.0f, 84.0f / 255.0f, 252.0f / 255.0f, 1.0f};
    [[nodiscard]] ImVec4 GetUIColor() const override { return COLOR; }
    [[nodiscard]] int GetNodeWidth() const override { return 250; }

    // Save bit width to json
    [[nodiscard]] nlohmann::json ToJson() const override {
        nlohmann::json j = Node::ToJson();
        j["module_guid"] = module_guid;
        return j;
    }

    // Override configuration methods
    [[nodiscard]] bool HasConfiguration() const override { return true; }

    void RenderConfiguration() override {

        if (ImGui::InputText("Module GUID", &module_guid)) {
            module_ref = module->GetProject()->GetModule(module_guid);

            std::cout << "updated node ref to " + (module_ref.has_value() ? module_ref.value()->GetName() : "missing")
                      << std::endl;
        }
    }

    // Helpers
    [[nodiscard]] std::string GetDisplayName() const override {
        return module_ref.has_value() ? module_ref.value()->GetName() : "missing";
    }


    std::string module_guid;
    std::optional<std::shared_ptr<Module>> module_ref;
};
