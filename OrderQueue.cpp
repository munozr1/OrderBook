#include "OrderQueue.h"
#include <cstddef>
#include <deque>


/*
 * Use this function to add orders if not immediatly able to fulfill
 */
void OrderQueue::addOrder(Order& order) {
    orders.push_back(order);
}

/*
 * Use this to fulfill market orders. (removes from front of queue)
 */
Order OrderQueue::removeOrder(){
    if (!orders.empty()) {
        Order frontOrder = orders.front();
        orders.pop_front();   // Remove the front element from the deque
        return frontOrder;    // Return the removed element
    }
    throw std::out_of_range("Attempt to remove from empty queue");

}



/*
 * Use this function to fulfill orders
 * Use this function to cancel orders
 */
bool OrderQueue::removeOrderById(uint32_t orderId){
    bool erased = false;
    for (std::deque<Order>::iterator it = orders.begin(); it != orders.end();){
        if (it->id == orderId){
            it = orders.erase(it);
            erased = true;
            break;
        }
        else ++it;
    }

    return erased;
}



/*
 * Use this function to peek into the first element of the queue
 */
Order OrderQueue::front() const {
    if (orders.empty()) {
        throw std::runtime_error("Attempted to access front of empty queue.");
    }
    return orders.front();
}



bool OrderQueue::isEmpty() const {
    return orders.empty();
}


std::size_t OrderQueue::size() const {
    return orders.size();
}


void OrderQueue::removeFront() {
    if (!orders.empty()) {
        orders.pop_front();
    }
}

