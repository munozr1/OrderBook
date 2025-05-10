#ifndef ORDER_H
#define ORDER_H

#include <cstdint>
#include "time.h"

// Enum declarations
enum OrderType : uint8_t { market, limit };
enum OrderSide : uint8_t { buy, sell };

// Order struct definition
struct Order {
    time_t timestamp;   // 8 bytes
    float price;          // 4 bytes
    uint32_t id;          // 4 bytes
    uint8_t type;         // 1 byte
    uint8_t side;         // 1 byte
    uint8_t _padding[2];  // Pad to 24 bytes
};

#endif  // End the include guard
