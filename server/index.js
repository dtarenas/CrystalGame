const express = require('express');
const http = require('http');
const socketio = require('socket.io');

const app = express();
const server = http.createServer(app);
const io = socketio(server);

io.on('connection', (socket) => {
  console.log('A new Socket connected');
})


app.get('/', (req, res, next) => {
  res.sendFile(__dirname + '/index.html')
})


const { SerialPort } = require('serialport');
const mySerialPort = new SerialPort(
  { path: 'COM3', baudRate: 9600 },
  (err) => {
    if (err) {
      return console.log('Error: ', err.message)
    }
  })

mySerialPort.write('main screen turn on', function (err) {
  if (err) {
    return console.log('Error on write: ', err.message);
  }
  console.log('message written');
});

mySerialPort.on('data', (data) => {
  console.log(data.toString());
  io.emit('arduino:data', {
    value: data.toString()
  });
});

mySerialPort.on('err', (err) => {
  console.log(err.message);
});

server.listen(3001, () => {
  console.log('listen 3001');
})