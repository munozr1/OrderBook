
#include "zmqpp/context.hpp"
#include "zmqpp/socket_types.hpp"
#include <zmqpp/zmqpp.hpp>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>


int main() {
    const std::string endpoint = "tcp://*:5555";

    zmqpp::context ctx;

    //generate pull socket
    zmqpp::socket_type type = zmqpp::socket_type::reply;
    zmqpp::socket socket  (ctx, type);


    //bind to socket

    socket.bind(endpoint);


    while(1){
        zmqpp::message message;
        socket.receive(message);
        std::string text;
        message >> text;

        //work
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Received Hello" <<std::endl;
        socket.send("World");
    }
    return 0;
}
