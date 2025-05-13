#include <zmqpp/zmqpp.hpp>
//#include <iostream>
#include "OrderBook.h"
#include "Order.h"
#include "OrderIdManager.h"

int main() {
    Order order;
    order.price = 100.0;
    order.type = limit;
    order.side = buy;

    Order order2;
    order2.price = 100.0;
    order2.type = market;
    order2.side = sell;


    // init order manager
    OrderIdManager idm;
    // init order book
    OrderBook book(idm);

    book.addOrder(order);
    book.addOrder(order2);


    return 0;
}

