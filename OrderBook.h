#include "OrderIdManager.h"
#include "OrderQueue.h"
#include <cstddef>
#include <map>

class OrderBook {
    public:
        void addOrder(Order& order);
        bool cancelOrder(Order& order);
        void matchOrders();
        OrderBook(OrderIdManager& manager);


    private:
        bool findFirstAvailableMarketSellOrder(Order& outOrder, float& priceLevel);
        bool findFirstAvailableLimitSellOrder(Order& outOrder, float& priceLevel);
        bool findFirstAvailableMarketBuyOrder(Order& outOrder, float& priceLevel);
        bool findFirstAvailableLimitBuyOrder(Order& outOrder, float& priceLevel);
        std::map<float, OrderQueue, std::greater<float> > buySide;
        std::map<float, OrderQueue> sellSide;
        OrderIdManager& idManager;

};
