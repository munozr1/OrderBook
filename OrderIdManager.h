#pragma once
#include <atomic>
#include <cstdint>
#include <string>

class OrderIdManager {
public:
    uint32_t getNextOrderId();

private:
    std::atomic<uint32_t> counter;
    std::string storagePath;
    void saveCounter(uint32_t value);
    uint32_t loadCounter();
};

