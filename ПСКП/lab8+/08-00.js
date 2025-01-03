const http = require('http');
const url = require('url');
const serverSockets = new Set();
const {Service} = require('./controller');

const server = http.createServer();

const httpHandler = ((req, res) => {
    const service = new Service(server, serverSockets);
    if (req.method === 'GET') {
        switch (url.parse(req.url, true).pathname.split('/')[1]) {
            case '':
                res.writeHead(200, {'Content-Type': 'text/html; charset=utf-8'});
                res.end('<h1>Lab08</h1>');
                break;
            case 'connection':
                service.handleConnection(req, res);
                break;
            case 'headers':
                service.handleHeaders(req, res);
                break;
            case 'parameter':
                service.handleParameters(req, res);
                break;
            case 'close':
                service.handleClose(req, res);
                break;
            case 'socket':
                service.handleSockets(req, res);
                break;
            case 'req-data':
                service.handleReqData(req, res);
                break;
            case 'resp-status':
                service.handleResponseStatus(req, res);
                break;
            case 'formparameter':
                service.handleFormParameter(req, res);
                break;
            case 'files':
                service.handleFiles(req, res);
                break;
            case 'upload':
                service.handleUpload(req, res);
                break;
            default:
                res.writeHead(404, {'Content-Type': 'text/html; charset=utf-8'});
                res.end('<h1>Not found</h1>')
        }
    }
    else if (req.method === 'POST') {
        switch (url.parse(req.url).pathname.split('/')[1]) {
            case 'formparameter':
                service.handleFormParameter(req, res);
                break;
            case 'json':
                service.handleJSON(req, res);
                break;
            case 'xml':
                service.handleXML(req, res);
                break;
            case 'upload':
                service.handleUpload(req, res);
                break;
            default:
                res.writeHead(404, {'Content-Type': 'text/html; charset=utf-8'});
                res.end('<h1>Not found</h1>')
        }
    }
    else {
        res.writeHead(409, 'Incorrect method', {'Content-Type': 'text/html; charset=utf-8'});
        res.end('<h1>Incorrect request method</h1>')
    }
});

server.on('request', httpHandler);
server.on('connection', socket => {
    serverSockets.add(socket);
})
server.listen(5000, '127.0.0.1', () => console.log("Server is running at http://127.0.0.1:5000"));