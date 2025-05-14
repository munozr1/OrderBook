#include "zmqpp/context.hpp"
#include "zmqpp/socket_types.hpp"
#include <zmqpp/zmqpp.hpp>
#include <string>
#include <iostream>
#include "OrderBook.h"
#include "Order.h"
#include "OrderIdManager.h"
#include "order.pb.h"

int main() {

    /* * * * * * * * * * * * *
     * ZEROMQ INITIALIZATION *
     * * * * * * * * * * * * */
    const std::string endpoint = "tcp://*:5555";
    zmqpp::context ctx;
    zmqpp::socket_type type = zmqpp::socket_type::reply;
    zmqpp::socket socket  (ctx, type);
    std::cout << "Starting matching engine server" << std::endl;
    socket.bind(endpoint);


    /* * * * * * * * * * * * * * *
     * ORDER BOOK INITIALIZATION *
     * * * * * * * * * * * * * * */
    OrderIdManager idm;
    OrderBook engine(idm);


    /* * * * * * *
     * MAIN LOGIC*
     * * * * * * */
    std::cout << "Accepting Orders" << std::endl;
    while(1){
        zmqpp::message message;
        socket.receive(message);

        matching::Order_pb receivedOrder;
        receivedOrder.ParseFromArray(message.raw_data(), message.size(0));

        // parse into native cpp order
        Order nativeOrder;
        nativeOrder.price   = receivedOrder.price();
        nativeOrder.type    = receivedOrder.type();
        nativeOrder.side    = receivedOrder.side();

        // process order
        engine.addOrder(nativeOrder);
        std::cout << "received order" <<std::endl;
        socket.send("ACK");
        // loging
        std::cout << "ACK order" <<std::endl;
    }



    return 0;
}

