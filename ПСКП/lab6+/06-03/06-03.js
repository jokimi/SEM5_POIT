import { createServer } from 'http';
import fs from 'fs';
import { send } from './m0603.js';
import querystring from 'querystring';

const server = createServer((req, res) => {
    if (req.url === '/' && req.method === 'GET') {
        fs.readFile('06.html', (err, data) => {
            if (err) {
                res.writeHead(500, { 'Content-Type': 'text/plain' });
                res.end('Ошибка сервера!');
            } else {
                res.writeHead(200, { 'Content-Type': 'text/html' });
                res.end(data);
            }
        });
    }
    else if (req.url === '/send' && req.method === 'POST') {
        let body = '';
        req.on('data', chunk => {
            body += chunk.toString();
        });
        req.on('end', () => {
            console.log('Полученные данные:', body);
            try {
                const parsedData = querystring.parse(body);
                const { message } = parsedData;
                if (!message) {
                    res.writeHead(400, { 'Content-Type': 'application/json' });
                    return res.end(JSON.stringify({ message: 'Некорректные данные' }));
                }
                send(message)
                    .then(() => {
                        res.writeHead(200, { 'Content-Type': 'application/json' });
                        res.end(JSON.stringify({ message: 'Письмо отправлено успешно!' }));
                    })
                    .catch((error) => {
                        console.error('Ошибка при отправке письма:', error);
                        res.writeHead(500, { 'Content-Type': 'application/json' });
                        res.end(JSON.stringify({ message: 'Ошибка при отправке письма' }));
                    });
            } catch (error) {
                console.error('Ошибка при обработке данных:', error);
                res.writeHead(400, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify({ message: 'Некорректный формат данных' }));
            }
        });
    } else {
        res.writeHead(404);
        res.end('Страница не найдена');
    }
});

server.listen(3000, () => {
    console.log('Сервер запущен на http://localhost:3000/');
});