//
// Created by Gabe on 4/26/2026.
//

#include "Topbar.h"

#include <imgui.h>

#include "CircuitSerializer.h"
#include "Codegen/Codegen.h"
#include "Codegen/TraversePrint.h"
#include "OutputViewer.h"
#include "misc/cpp/imgui_stdlib.h"


void Topbar::Render(std::optional<std::shared_ptr<Module>> module, const std::shared_ptr<ErrorManager> &error_manager,
                    const std::shared_ptr<OutputViewer> &output_viewer) {
    ImGui::Begin("Options");

    if (module.has_value()) {
        if (ImGui::Button("Print Circuit")) {
            TraversePrint v;
            module.value()->nodes[0]->accept(v, 0);
        }

        ImGui::SameLine();

        if (ImGui::Button("Export Circuit")) {
            Codegen c(error_manager);
            c.GenerateCode(module.value());

            output_viewer->UpdateOutput(module.value());
        }

        ImGui::SameLine();

        if (ImGui::Button("Save Circuit")) {
            CircuitSerializer::SaveModule(module.value(), "../Project/circuit.json");
        }

        ImGui::SameLine();

        if (ImGui::Button("Load Circuit")) {
            module = CircuitSerializer::LoadModule("../Project/circuit.json");
        }

        ImGui::SameLine();
    }

    ImGui::End();
}
