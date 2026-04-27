//
// Created by Gabe on 4/26/2026.
//

#pragma once
#include "Lib/TextEditor.h"


class Module;
class OutputViewer {
public:
    explicit OutputViewer(const std::shared_ptr<Module> &module);
    void UpdateOutput(const std::shared_ptr<Module> &module);
    void Render();

private:
    TextEditor::ErrorMarkers markers;
    TextEditor editor;
};
