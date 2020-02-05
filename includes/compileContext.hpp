#pragma once

#include "typeDefinitions.hpp"
#include "monoAllocator.hpp"
#include <unordered_map>
#include <mutex>
#include <string_view>
#include <queue>
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
    std::vector<bool> threadStates_;
    std::queue<std::string_view> fileQueue_;
    monoAllocator<std::string, 8> moduleAlloc_;
    std::unordered_map<std::string_view, std::string*> modules_;
};

#pragma pack(pop)