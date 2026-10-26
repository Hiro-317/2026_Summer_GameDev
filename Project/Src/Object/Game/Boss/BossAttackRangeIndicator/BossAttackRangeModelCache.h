#pragma once
#include <string>

class BossAttackRangeModelCache final {
public:
    static int Acquire(const std::string& path);
    static void Release(const std::string& path);
    BossAttackRangeModelCache() = delete;
};
