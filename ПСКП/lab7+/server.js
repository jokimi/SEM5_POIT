const http = require('http');
const staticDir = './static';
const StaticServer = require('./m07-01')(staticDir);

const PORT = 5000;

const server = new StaticServer();

const httpServer = http.createServer((req, res) => {
    server.handleRequest(req, res);
});

httpServer.listen(PORT, () => {
    console.log(`Server is running on http://localhost:${PORT}`);
});