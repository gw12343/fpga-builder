//
// Created by Gabe on 4/30/2026.
//

#ifndef PCH_H
#define PCH_H

#include <algorithm>
#include <array>
#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_node_editor.h>
#include "UI/Lib/ImGuiNotify.h"
#include "misc/cpp/imgui_stdlib.h"

#include <nlohmann/json.hpp>


#ifdef __EMSCRIPTEN__
// root of the virtual filesystem
inline const std::string ASSET_BASE_PATH = "/";
#else
inline const std::string ASSET_BASE_PATH = "";
#endif

#endif // PCH_H
