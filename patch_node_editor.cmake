file(READ "imgui_extra_math.inl" FILE_CONTENT)

string(REPLACE "\r\n" "\n" FILE_CONTENT "${FILE_CONTENT}")

string(REPLACE
        "inline ImVec2 operator*(const float lhs, const ImVec2& rhs)\n{\n    return ImVec2(lhs * rhs.x, lhs * rhs.y);\n}"
        "/* inline ImVec2 operator*(const float lhs, const ImVec2& rhs)\n{\n    return ImVec2(lhs * rhs.x, lhs * rhs.y);\n} */"
        PATCHED_CONTENT
        "${FILE_CONTENT}"
)
file(WRITE "imgui_extra_math.inl" "${PATCHED_CONTENT}")