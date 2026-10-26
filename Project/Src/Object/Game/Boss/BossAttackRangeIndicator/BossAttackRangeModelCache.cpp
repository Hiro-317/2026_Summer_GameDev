#include "BossAttackRangeModelCache.h"

#include <unordered_map>
#include <stdexcept>

#include "../../../../pch.h"

namespace {
    struct Entry { int handle; unsigned int users; };
    std::unordered_map<std::string, Entry>& Cache() {
        static std::unordered_map<std::string, Entry> cache;
        return cache;
    }
}

int BossAttackRangeModelCache::Acquire(const std::string& path) {
    auto& cache = Cache();
    auto it = cache.find(path);
    if (it != cache.end()) {
        ++it->second.users;
        return it->second.handle;
    }
    const int handle = MV1LoadModel(path.c_str());
    if (handle < 0) throw std::runtime_error("Failed to load range model: " + path);
    cache.emplace(path, Entry{handle, 1});
    return handle;
}

void BossAttackRangeModelCache::Release(const std::string& path) {
    auto& cache = Cache();
    auto it = cache.find(path);
    if (it == cache.end()) return;
    if (--it->second.users == 0) {
        MV1DeleteModel(it->second.handle);
        cache.erase(it);
    }
}
