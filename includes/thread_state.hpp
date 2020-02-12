#pragma once

#include "type_definitions.hpp"

enum thread_state : u8 {
    working,
    finished,
    cancelled
};