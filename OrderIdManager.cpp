#include "OrderIdManager.h"
#include <cstdint>


uint32_t OrderIdManager::getNextOrderId() {
    return counter.fetch_add(1, std::memory_order_relaxed);
}
