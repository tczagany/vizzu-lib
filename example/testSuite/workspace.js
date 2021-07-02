var path = require('path');


class Workspace {
    
    #workspacePath;
    #workspacePort;
    #workspace;

    constructor(workspacePath) {
        if(path.isAbsolute(workspacePath)) {
            this.#workspacePath = workspacePath;
        } else {
            this.#workspacePath = __dirname + '/' + workspacePath;
        }
    }


    getWorkspacePort() {
        return this.#workspacePort
    }


    openWorkspace(port = this.#findFreePort()) {
        this.#workspacePort = port
        var http = require('http');
        var finalhandler = require('finalhandler');
        var serveStatic = require('serve-static');
    
        var serve = serveStatic(this.#workspacePath);
        this.#workspace = http.createServer(function(req, res) {
            var done = finalhandler(req, res);
            serve(req, res, done);
        });
        this.#workspace.listen(port);
    }

    closeWorkspace() {
        this.#workspace.close();
    }


    #findFreePort() {
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
}


module.exports = Workspace;
