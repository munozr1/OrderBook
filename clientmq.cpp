#include <zmqpp/zmqpp.hpp>
#include <string>
#include <iostream>
#include <vector>
#include "order.pb.h"


int main(int argc, char *argv[]) {
    /* * * * * * * * * * 
     * INIT TEST ORDERS*
     * * * * * * * * * */
  std::vector<matching::Order_pb> orders;
  float base_price = 100.0;

  /* SET BUY ORDERS */
  for(int i = 0; i < 10; i++) {
      matching::Order_pb new_order;
      new_order.set_price(base_price + i);
      new_order.set_type(matching::LIMIT);
      new_order.set_side(matching::BUY);
      orders.push_back(new_order);
  }

  /* SET SELL ORDERS */ 
  for(int i = 0; i < 10; i++) {
      matching::Order_pb new_order;
      new_order.set_price(base_price + i);
      new_order.set_type(matching::LIMIT);
      new_order.set_side(matching::SELL);
      orders.push_back(new_order);
  }

  /* * * * * * * * *
   * initialize 0MQ*
   * * * * * * * * */
  const std::string endpoint = "tcp://localhost:5555";
  zmqpp::context context;
  zmqpp::socket_type type = zmqpp::socket_type::req;
  zmqpp::socket socket (context, type);

  /* open the connection */
  std::cout << "Connecting to matching engine server…" << std::endl;
  socket.connect(endpoint);
  int request_nbr;

  /* SEND ORERS */
  std::cout << "Sending Orders" << std::endl;
  for (int i = 0; i < orders.size(); i++) {
    matching::Order_pb curr_order = orders[i];
    std::cout << "Sending order" << std::endl;
    /* serialize message */
    std::string buffer;
    if (!curr_order.SerializeToString(&buffer)){
        std::cout << "Failed to serialize order #" << i <<std::endl;
        continue;
    }
    /* compose message */
    zmqpp::message message;
    message.add_raw(buffer.data(), buffer.size());
    socket.send(message);

    /* wait for reply */
    std::string reply_buffer;
    socket.receive(reply_buffer);
    std::cout << "replied #" << i << std::endl;
    
  }

}

