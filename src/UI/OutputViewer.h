//
// Created by Gabe on 4/26/2026.
//

#pragma once
#include <regex>
#include "Lib/TextEditor.h"


class Module;
class OutputViewer {
public:
    explicit OutputViewer();

    /// Show already-generated Verilog (preferred — works on web without FS writes).
    void UpdateOutput(const std::string &verilog_source);

    /// Try to load `Export/<module>.v` from the project workspace (desktop / after a successful write).
    void UpdateOutput(const std::shared_ptr<Module> &module);

    void Render();

private:
    TextEditor::ErrorMarkers m_markers;
    TextEditor m_editor;
};
