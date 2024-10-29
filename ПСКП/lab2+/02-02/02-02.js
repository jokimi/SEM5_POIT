const http = require('http');
const fs = require('fs');
const path = require('path');

const hostname = '127.0.0.1';
const port = 5000;

const server = http.createServer((req, res) => {
    if (req.url === '/png') {
        const filePath = path.join(__dirname, 'pic.png');
        fs.readFile(filePath, (err, data) => {
            if (err) {
                res.statusCode = 500;
                res.setHeader('Content-Type', 'text/plain; charset=utf-8');
                res.end('Ошибка чтения файла');
                return;
            }
            res.statusCode = 200;
            res.setHeader('Content-Type', 'image/png; charset=utf-8');
            res.end(data);
        });
    } else {
        res.statusCode = 404;
        res.setHeader('Content-Type', 'text/plain; charset=utf-8');
        res.end('Страница не найдена');
    }
});

server.listen(port, hostname, () => {
    console.log(`Сервер запущен на http://${hostname}:${port}/png`);
});