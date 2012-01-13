#!/usr/bin/node

var net = require('net');

var clients = [];

net.createServer(function (c) {

    clients.push(c);

    c.on('data', function (data) {

        for (i = 0; i < clients.length; i++) {
            var client = clients[i];

            if (client == c)
                continue;

            client.write(data);
        }

    });

    c.on('end', function () {
        var index = clients.indexOf(c);
        clients.splice(index, 1);
    });

}).listen(8081, '127.0.0.1')