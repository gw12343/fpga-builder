//
// Created by gabed on 4/14/2026.
//

#pragma once
#include <string>

#include <cstdint>

class GUID {
public:
    static std::string generate_guid();
    static uint32_t to_id(const std::string &guid);
};
