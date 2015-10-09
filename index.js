var express = require('express'),
  app = express(),
  path = require('path'),
  http = require('http').Server(app),
  io = require('socket.io')(http);


app.use(express.static(path.join(__dirname, './static')));

app.get('/update', function (req, res, next) {
  console.log("got update: ", req.query.status);
  io.emit('update', req.query.status);
  res.json({success: true});
  next();
});

// socket.io handlers
io.on('connection', function (socket) {
  console.log('User connected. Socket id %s', socket.id);
  socket.on('disconnect', function () {
    console.log('User disconnected. %s. Socket id %s', socket.id);
  });
});

http.listen(process.env.PORT || 4000, function () {
  console.log('listening on: ' + process.env.PORT || 5000);
});

