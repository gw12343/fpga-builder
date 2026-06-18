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
    [[nodiscard]] std::shared_ptr<Node> Clone() const override;
    [[nodiscard]] nlohmann::json ToJson() const override;
    [[nodiscard]] ImVec4 GetUIColor() const override { return COLOR; }
    [[nodiscard]] int GetNodeWidth() const override { return 250; }
    [[nodiscard]] const std::vector<Pin> &GetPins() { return pins; }
    [[nodiscard]] bool HasConfiguration() const override { return true; }
    [[nodiscard]] std::string GetDisplayName() const override;

    [[nodiscard]] bool IsSequential() const override { return true; }

    // Constructor for new node
    explicit CustomModuleNode(Module *parent);
    // Constructor for saved node
    CustomModuleNode(Module *parent, const std::string &saved_guid, const std::string &saved_module_guid);


    void InitPinsAfterConfig() override;
    void RenderConfiguration() override;
    void accept(Visitor &v, int output_slot) override;


    std::string module_guid;
    std::optional<std::shared_ptr<Module>> module_ref;

    static constexpr ImVec4 COLOR = {1.0f, 84.0f / 255.0f, 252.0f / 255.0f, 1.0f};
};
