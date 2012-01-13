var http = require('http');

http.createServer(function(request, response) {
    response.writeHead(200, {
        'Content-Type': 'multipart/x-mixed-replace; boundary=boundary',
        'Connection': 'keep-alive'
    });

    setInterval(function () {
        response.write('--boundary\r\n', 'ascii');
        response.write('Content-Type: text/plain\r\n', 'ascii');
        response.write('\r\n', 'ascii');

        var date = new Date();
        response.write(date.toString() + '\r\n\r\n', 'ascii');
    }, 1000);
}).listen(8080);
