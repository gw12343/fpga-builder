//
// Created by gabed on 4/14/2026.
//

#pragma once
#include <string>

#include "Pins/Pin.h"


class GUID {
public:
    static std::string generate_guid();
    static uint32_t to_id(std::string guid);
};
