#pragma once

#include "typeDefinitions.hpp"

#pragma pack(push, 1)

struct word {
    char* start;
    char* end;
    std::size_t hash;
};

#pragma pack(pop)