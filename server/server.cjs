const zmq = require('zeromq');
const protobuf = require("protobufjs");
const ws = require("ws");

let root;
let Order;

async function init(){
    root = await protobuf.load("../order.proto");
    Order = root.lookupType("matching.Order_pb");
    console.log('Connecting to matching engine server...');
}

async function testEngine(id){
    const engineConnection = new zmq.Request();
    engineConnection.connect('tcp://localhost:5555');
    // Helper function to create an order
    function createOrder(price, type, side) {
      const errMsg = Order.verify({ price, type, side });
      if (errMsg) throw Error(errMsg);
      return Order.create({ price, type, side });
    }

    for (let i = 0; i < 10; i++) {
      const order = createOrder(100.0 + i, 1, 0); // LIMIT BUY orders with increasing price
      const messageBuffer = Order.encode(order).finish();

      console.log(`Sending Order ${i}`);
      await engineConnection.send(messageBuffer);

      const [result] = await engineConnection.receive();
      console.log(`TO${id} Received ${i}:`, result.toString());
    }

}

async function tests(){
    await Promise.all([testEngine(1), testEngine(2)])
}

init().then(() => tests());


async function addOrder(orderJSON, engineConnection){
    function createOrder(price, type, side) {
      const errMsg = Order.verify({ price, type, side });
      if (errMsg) throw Error(errMsg);
      return Order.create({ price, type, side });
    }

    const order = createOrder(orderJSON.price, orderJSON.type, orderJSON.side);
    const messageBuffer = Order.encode(order).finish();

    console.log(`Sending Order ${i}`);
    await engineConnection.send(messageBuffer);

    const [result] = await engineConnection.receive();
    console.log('ACK');
}


const socket = new ws.WebSocketServer({port: 3000});
socket.on('connection', function connection(ws) {
    /*
     * INIT CONNECTION TO MATCHING ENGINE
     */
    const engineConnection = new zmq.Request();
    engineConnection.connect('tcp://localhost:5555');


    /*
     * SUBMIT CLIENT ORDER
     */
    ws.on('message', async function incoming(message){
        try{
            const data = JSON.parse(message);
            switch(data.action){
                case 'addOrder':
                    await addOrder(data.order, engineConnection);
                    ws.send(JSON.stringify({
                        status: "success",
                        action: "addOrder",
                        orderId: data.orderId
                    }));
                default:
                    console.warn("Unhandled action");
            }
        }catch (err){
            console.error("Failed to handle message => ", err);
        }
    });
})
