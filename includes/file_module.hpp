#pragma once

#include "f_string.hpp"
#include "module.hpp"

#pragma pack(push, 1)

struct file_module {
    f_string<23> file_path;
    module* module_ref;
};

#pragma pack(pop)