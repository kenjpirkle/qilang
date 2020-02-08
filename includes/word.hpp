#pragma once

#include "type_definitions.hpp"

using namespace std;

#pragma pack(push, 1)

struct word {
    char* start;
    char* end;
    size_t hash;
};

#pragma pack(pop)