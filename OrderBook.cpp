#include "OrderBook.h"
#include "Order.h"
#include "OrderIdManager.h"
#include <ctime>
#include <iostream>

/*
 * Constructor: set id manager
 */
OrderBook::OrderBook(OrderIdManager& manager) : idManager(manager) {
}

void OrderBook::addOrder(Order& order){
    OrderType type = (OrderType) order.type;
    OrderSide side = (OrderSide) order.side;
    order.timestamp = std::time(0); // do not let clients set timestamp themselves
    order.id = idManager.getNextOrderId();
    
    if (side == buy) {
        Order candidate;
        float priceLevel;

        if (type == market) {
            if (findFirstAvailableMarketSellOrder(candidate, priceLevel)){
                //1. notify user for approved buy order with order details (network call?)
                std::cout<< "Fulfilled market order @ " << priceLevel << std::endl;
                //2. remove the order
                OrderQueue& queue = sellSide[priceLevel];
                queue.removeFront();
                if (queue.isEmpty()) sellSide.erase(priceLevel);
                
            }
            // 1. notify the buyer of failed market buy order (network call?)
            else {std::cout<< "Fulfilled market order @ FAILED" << std::endl;}

        } else if (type == limit) {
            // match against sellSide, price <= order.price

            if (findFirstAvailableLimitSellOrder(candidate, priceLevel)){
                std::cout<< "Fulfilled limit order @ " << priceLevel << std::endl;
                OrderQueue& queue = sellSide[priceLevel];
                queue.removeFront();
                if (queue.isEmpty()) sellSide.erase(priceLevel);
            }
            else{
                //if no abailable limit sell order found, then add to the buySide
                buySide[priceLevel].addOrder(order);
            }
            
        }
    } else if (side == sell) {
        if (type == market) {
            // match against buySide, no price check
        } else if (type == limit) {
            // match against buySide, price >= order.price
        }
    }

}


bool OrderBook::findFirstAvailableMarketSellOrder(Order& outOrder, float& priceLevel){
    if (sellSide.empty()) return false; // check if price levels map is empty


    std::map<float, OrderQueue>::iterator it = sellSide.begin(); // iterator to the lowest price level
    OrderQueue& queue = it->second; // OrderQueue of first price level
    if (queue.isEmpty()) return false;
    priceLevel = it->first;
    outOrder = queue.front();


    return true;
}

bool OrderBook::findFirstAvailableLimitSellOrder(Order& candidate, float& priceLevel){
    std::map<float, OrderQueue>::iterator it = sellSide.begin();

    //iterate through price levels
    while(it != sellSide.end() && it->first <= priceLevel) {
        //iterate through price level queue
        OrderQueue& queue = it->second;
        if (!queue.isEmpty()) {
            candidate = queue.front();
            priceLevel = it->first;
            return true;
        }
        ++it;
    }
    
    return false;
}
