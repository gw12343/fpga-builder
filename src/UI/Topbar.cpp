//
// Created by Gabe on 4/26/2026.
//

#include "Topbar.h"

#include "Codegen/Codegen.h"
#include "GUID.h"
#include "OutputViewer.h"
#include "Project/Project.h"


void Topbar::Render(const std::shared_ptr<Project> &project, const std::shared_ptr<ErrorManager> &error_manager,
                    const std::shared_ptr<OutputViewer> &output_viewer) {
    ImGui::Begin("Options");

    if (const auto module = project->GetSelectedModule(); module.has_value()) {


        if (ImGui::Button("Export Circuit")) {
            Codegen c(error_manager);
            c.GenerateCode(module.value());

            output_viewer->UpdateOutput(module.value());
        }

        ImGui::SameLine();

        if (ImGui::Button("Save Project")) {
            project->Save();
        }

        ImGui::SameLine();


        if (ImGui::Button("New Module")) {
            const auto m = std::make_shared<Module>(project.get(), "new_module", GUID::generate_guid());
            project->RegisterModule(m);
        }
    }

    ImGui::End();
}
