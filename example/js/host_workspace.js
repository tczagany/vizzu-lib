async function findFreePort() {
    var http = require('http');

    var server = http.createServer(function(req, res) {
        var done = finalhandler(req, res);
        serve(req, res, done);
    });
    server.listen(0, function() {});
    var port = server.address().port
    server.close()
    return port;
}

async function createServer(port) {
    var http = require('http');
    var finalhandler = require('finalhandler');
    var serveStatic = require('serve-static');

    var serve = serveStatic('../');
    var server = http.createServer(function(req, res) {
        var done = finalhandler(req, res);
        serve(req, res, done);
    });
    server.listen(port);
}


(async () => {
    try {
        var port = await findFreePort();
        console.log('Listening at http://127.0.0.1:' + String(port) + '/');
        createServer(port)
    } catch (e) {
        console.log(e);
    }
})();
  

