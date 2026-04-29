//
// Created by gabed on 4/14/2026.
//

#include "GUID.h"

#include <iomanip>
#include <random>
#include <sstream>

std::string GUID::generate_guid() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution dis(0, 15);
    std::uniform_int_distribution dis2(8, 11);

    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (int i = 0; i < 16; ++i) {
        if (i == 6)
            ss << 4;
        else if (i == 8)
            ss << dis2(gen);
        else
            ss << dis(gen);
        ss << dis(gen);
    }
    return ss.str();
}

uint32_t GUID::to_id(const std::string &guid) {
    uint32_t hash = 2166136261u;
    for (const char c: guid) {
        hash ^= static_cast<uint8_t>(c);
        hash *= 16777619u;
    }
    return hash;
}
