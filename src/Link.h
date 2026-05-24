//
// Created by gabed on 4/14/2026.
//

#pragma once

class Module;

class Link {
public:
    Link(Module *module, const std::string &output_guid, const std::string &input_guid);

    Link(Module *module, std::string saved_id, std::string output_guid, std::string input_guid);

    constexpr Link(Link &&other) = default;
    constexpr Link(const Link &other) = default;
    constexpr Link &operator=(const Link &) = default;

    void Render() const;

    bool operator==(const Link &other) const;

    bool operator<(const Link &other) const;


    [[nodiscard]] nlohmann::json to_json() const {
        return {{"output_guid", output_guid}, {"input_guid", input_guid}, {"id", id.Get()}};
    }

    ax::NodeEditor::LinkId id;
    Module *module;
    std::string output_guid;
    std::string input_guid;
    ImColor color;
};
