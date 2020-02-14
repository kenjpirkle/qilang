#pragma once

#include "type_definitions.hpp"

enum parser_state : u8 {
    working,
    finished,
    cancelled
};