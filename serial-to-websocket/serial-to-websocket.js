const ws = require('ws');
const SerialPort = require('serialport')
const port = new SerialPort('/dev/cu.SLAB_USBtoUART', 
{
    baudRate: 250000
})

const server = new ws.Server({ port: process.env.PORT || 3000 });

server.broadcast = (message, excluding) => {
  server.clients.forEach((client) => {
    if (client.readyState === ws.OPEN && (!excluding || excluding !== client)) {
      client.send(message);
    }
  });
};


const Readline = require('@serialport/parser-readline')
const parser = new Readline()
port.pipe(parser)
parser.on('data', (data) => {
  server.broadcast(data);
  console.log(data);
});
server.connection_count = 0;

server.on('connection', (socket) => {
  console.log(`Client connect; count: ${++server.connection_count}`);

  socket.on('close', () => {
    console.log(`Client disconnect; count: ${--server.connection_count}`);
  });
});