#include "Order.h"
#include <cstddef>
#include <deque>

class OrderQueue {
    public:
        void addOrder(Order& order);
        Order removeOrder();
        bool removeOrderById(uint32_t orderId);
        void removeFront();
        Order front() const;
        bool isEmpty() const;
        std::size_t size() const;
    private:
        std::deque<Order> orders;
};
