const http = require('http');

const hostname = '127.0.0.1';
const port = 5000;

const server = http.createServer((req, res) => {
    if (req.url === '/api/name' && req.method === 'GET') {
        res.statusCode = 200;
        res.setHeader('Content-Type', 'text/plain; charset=utf-8');
        res.end('Козека Елизавета Максимовна');
    } else {
        res.statusCode = 404;
        res.setHeader('Content-Type', 'text/plain; charset=utf-8');
        res.end('Страница не найдена');
    }
});

server.listen(port, hostname, () => {
    console.log(`Сервер запущен на http://${hostname}:${port}/api/name`);
});