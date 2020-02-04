#pragma once

#include "typeDefinitions.hpp"
#include <mutex>
#include <string_view>
#include <optional>
#include <thread>
#include <vector>

#pragma pack(push, 1)

struct compileContext {
public:
    compileContext(const std::string_view);
    auto add(const std::string_view) -> void;

private:
    std::mutex mutex_;
    std::thread::id mainThreadId_;
    std::vector<std::thread> threads_;
};

#pragma pack(pop)