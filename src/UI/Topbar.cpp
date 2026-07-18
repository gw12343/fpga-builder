//
// Created by Gabe on 4/26/2026.
//

#include "Topbar.h"

#include <iostream>

#include "Codegen/Codegen.h"
#include "GUID.h"
#include "OutputViewer.h"
#include "Project/Project.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

void DownloadString(const std::string &content, const std::string &filename) {
#ifdef __EMSCRIPTEN__
    EM_ASM(
            {
                // Convert the C++ memory pointers back to JavaScript strings
                var js_content = UTF8ToString($0);
                var js_filename = UTF8ToString($1);

                // Create the Blob and download link
                var blob = new Blob([js_content], {
                    type:
                        'text/plain'
                });
                var url = URL.createObjectURL(blob);

                var a = document.createElement('a');
                a.style.display = 'none';
                a.href = url;
                a.download = js_filename;

                document.body.appendChild(a);
                a.click();

                // Cleanup
                document.body.removeChild(a);
                URL.revokeObjectURL(url);
            },
            content.c_str(), filename.c_str()); // Pass the C-strings into $0 and $1
#else
    std::cerr << "ERROR cannot download file outside of web build";
#endif
}

void Topbar::Render(const std::shared_ptr<Project> &project, const std::shared_ptr<ErrorManager> &error_manager,
                    const std::shared_ptr<OutputViewer> &output_viewer) {
    ImGui::Begin("Options");

#ifndef __EMSCRIPTEN__

    if (ImGui::Button("Export Project")) {
        for (const auto module: project->GetModules()) {
            Codegen c(error_manager);
            const auto verilog = c.GenerateCode(module);
            // Last module wins in the viewer; files are still written under Export/.
            output_viewer->UpdateOutput(verilog);
        }
    }
    ImGui::SameLine();
#endif

    if (const auto module = project->GetSelectedModule(); module.has_value()) {


        if (ImGui::Button("Generate Verilog")) {
            Codegen c(error_manager);
            // Always feed the viewer from the in-memory result. Re-reading Export/*.v
            // breaks on the web build whenever the virtual FS write fails (common on
            // GitHub Pages where Project/Export is not preloaded and cannot be created
            // by a bare ofstream).
            const auto verilog = c.GenerateCode(module.value());
            output_viewer->UpdateOutput(verilog);
        }
#ifdef __EMSCRIPTEN__
        ImGui::SameLine();
        if (ImGui::Button("Export Verilog")) {
            Codegen c(error_manager);
            const auto verilog = c.GenerateCode(module.value());
            output_viewer->UpdateOutput(verilog);
            DownloadString(verilog, module.value()->GetName() + ".v");
        }
#endif
#ifndef __EMSCRIPTEN__
        ImGui::SameLine();

        if (ImGui::Button("Save Project")) {
            project->Save();
        }
#endif

        ImGui::SameLine();

        if (ImGui::Button("New Module")) {
            const auto m = std::make_shared<Module>(project.get(), "new_module", GUID::generate_guid());
            project->RegisterModule(m);
        }
    }

    ImGui::End();
}
