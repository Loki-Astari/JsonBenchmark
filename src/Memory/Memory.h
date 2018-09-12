#include <map>
#include <unordered_map>
#include <cstddef>

struct MemoryInfo
{
    std::size_t mallocCount     = 0;
    std::size_t reallocCount    = 0;
    std::size_t freeCount       = 0;
    std::size_t currentSize     = 0;
    std::size_t peakSize        = 0;

    std::map<std::size_t, std::pair<std::size_t, std::size_t>>  countData;
    std::unordered_map<void*, std::size_t>                      ptrInfo;

    static bool alreadyIn;
    static bool active;

    static void alloc(void* ptr, std::size_t size, bool malloc);
    static void free(void* ptr);
    static MemoryInfo& getInstance();

    MemoryInfo(MemoryInfo const&) = delete;
    MemoryInfo operator=(MemoryInfo const&) = delete;
    MemoryInfo(){}
};
