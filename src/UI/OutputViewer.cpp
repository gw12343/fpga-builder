//
// Created by Gabe on 4/26/2026.
//

#include "OutputViewer.h"
#include <fstream>
#include "Module.h"

OutputViewer::OutputViewer() {
    auto lang = TextEditor::LanguageDefinition::CPlusPlus();

    const std::unordered_set<std::string> verilog_keywords = {"always",
                                                              "and",
                                                              "assign",
                                                              "automatic",
                                                              "begin",
                                                              "buf",
                                                              "bufif0",
                                                              "bufif1",
                                                              "case",
                                                              "casex",
                                                              "casez",
                                                              "cell",
                                                              "cmos",
                                                              "config",
                                                              "deassign",
                                                              "default",
                                                              "defparam",
                                                              "design",
                                                              "disable",
                                                              "edge",
                                                              "else",
                                                              "end",
                                                              "endcase",
                                                              "endconfig",
                                                              "endfunction",
                                                              "endgenerate",
                                                              "endmodule",
                                                              "endprimitive",
                                                              "endspecify",
                                                              "endtable",
                                                              "endtask",
                                                              "event",
                                                              "for",
                                                              "force",
                                                              "forever",
                                                              "fork",
                                                              "function",
                                                              "generate",
                                                              "genvar",
                                                              "highz0",
                                                              "highz1",
                                                              "if",
                                                              "ifnone",
                                                              "incdir",
                                                              "include",
                                                              "initial",
                                                              "inout",
                                                              "input",
                                                              "instance",
                                                              "integer",
                                                              "join",
                                                              "larger",
                                                              "liblist",
                                                              "library",
                                                              "localparam",
                                                              "macromodule",
                                                              "medium",
                                                              "module",
                                                              "nand",
                                                              "negedge",
                                                              "nmos",
                                                              "nor",
                                                              "noshow-cancelled",
                                                              "not",
                                                              "notif0",
                                                              "notif1",
                                                              "or",
                                                              "output",
                                                              "parameter",
                                                              "pmos",
                                                              "posedge",
                                                              "primitive",
                                                              "pull0",
                                                              "pull1",
                                                              "pullup",
                                                              "pulldown",
                                                              "pulsestyle_ondetect",
                                                              "pulsestyle_onevent",
                                                              "rcmos",
                                                              "real",
                                                              "realtime",
                                                              "reg",
                                                              "release",
                                                              "repeat",
                                                              "rnmos",
                                                              "rpmos",
                                                              "rtran",
                                                              "rtranif0",
                                                              "rtranif1",
                                                              "scalared",
                                                              "show-cancelled",
                                                              "signed",
                                                              "small",
                                                              "specify",
                                                              "specpa",
                                                              "strong0",
                                                              "strong1",
                                                              "supply0",
                                                              "supply1",
                                                              "table",
                                                              "task",
                                                              "time",
                                                              "tran",
                                                              "tranif0",
                                                              "tranif1",
                                                              "tri",
                                                              "tri0",
                                                              "tri1",
                                                              "triand",
                                                              "trior",
                                                              "trireg",
                                                              "use",
                                                              "vectored",
                                                              "wait",
                                                              "wand",
                                                              "weak0",
                                                              "weak1",
                                                              "while",
                                                              "wire",
                                                              "wor",
                                                              "xnor",
                                                              "xor"};


    lang.mKeywords = verilog_keywords;
    m_editor.SetLanguageDefinition(lang);
    m_editor.SetReadOnly(true);


    auto palette = TextEditor::GetDarkPalette();

    palette[static_cast<int>(TextEditor::PaletteIndex::Background)] = ImColor(18, 18, 20);
    palette[static_cast<int>(TextEditor::PaletteIndex::LineNumber)] = ImColor(110, 110, 110);
    palette[static_cast<int>(TextEditor::PaletteIndex::Cursor)] = ImColor(220, 220, 220);
    palette[static_cast<int>(TextEditor::PaletteIndex::Selection)] = ImColor(70, 20, 20, 180);
    palette[static_cast<int>(TextEditor::PaletteIndex::Default)] = ImColor(210, 210, 210);
    palette[static_cast<int>(TextEditor::PaletteIndex::Identifier)] = ImColor(190, 190, 190);
    palette[static_cast<int>(TextEditor::PaletteIndex::Keyword)] = ImColor(220, 70, 70);
    palette[static_cast<int>(TextEditor::PaletteIndex::Preprocessor)] = ImColor(200, 80, 80);
    palette[static_cast<int>(TextEditor::PaletteIndex::String)] = ImColor(200, 140, 90);
    palette[static_cast<int>(TextEditor::PaletteIndex::Number)] = ImColor(140, 200, 160);
    palette[static_cast<int>(TextEditor::PaletteIndex::Comment)] = ImColor(120, 130, 120);
    palette[static_cast<int>(TextEditor::PaletteIndex::Punctuation)] = ImColor(170, 170, 170);
    palette[static_cast<int>(TextEditor::PaletteIndex::KnownIdentifier)] = ImColor(120, 170, 220);
    palette[static_cast<int>(TextEditor::PaletteIndex::ErrorMarker)] = ImColor(255, 60, 60);
    palette[static_cast<int>(TextEditor::PaletteIndex::Breakpoint)] = ImColor(180, 40, 40);

    m_editor.SetPalette(palette);

    m_editor.SetText("TODO");
    // UpdateOutput(module);
}
void OutputViewer::UpdateOutput(const std::shared_ptr<Module> &module) {
    const std::ifstream file("../Project/Export/" + module->GetName() + ".v");
    std::stringstream buffer;
    buffer << file.rdbuf();

    m_editor.SetText(buffer.str());
}


void OutputViewer::Render() {
    ImGui::Begin("Output");
    m_editor.Render("TextEditor");
    ImGui::End();
}
