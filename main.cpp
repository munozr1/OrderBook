#include <zmqpp/zmqpp.hpp>
#include <iostream>
#include "OrderBook.h"
#include "Order.h"
#include "OrderIdManager.h"

int main() {
    Order order;
    order.id = 1;
    order.price = 100.0;
    order.type = market;
    order.side = buy;
    order.timestamp = 1627359200;

    std::cout << "Order ID: " << order.id << std::endl;
    return 0;
}

