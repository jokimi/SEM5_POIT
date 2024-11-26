import { createServer } from 'http';
import fs from 'fs';
import nodemailer from 'nodemailer';
import querystring from 'querystring';

const transporter = nodemailer.createTransport({
    service: 'gmail',
    auth: {
        user: 'lizakozeka@gmail.com',
        pass: 'svrfbtvljzisqgom'
    },
});

const server = createServer((req, res) => {
    if (req.url === '/' && req.method === 'GET') {
        fs.readFile('index.html', (err, data) => {
            if (err) {
                res.writeHead(500, { 'Content-Type': 'text/plain' });
                res.end('Ошибка сервера!');
            }
            else {
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
            const params = querystring.parse(body);
            const { from, to, message } = params;
            if (!from || !to || !message) {
                res.writeHead(400, { 'Content-Type': 'application/json' });
                return res.end(JSON.stringify({ message: 'Некорректные данные' }));
            }
            const mailOptions = {
                from: from,
                to: to,
                subject: 'task 2',
                text: message,
            };
            transporter.sendMail(mailOptions, (error, info) => {
                if (error) {
                    console.error('Ошибка при отправке письма:', error);
                    res.writeHead(500, { 'Content-Type': 'application/json' });
                    res.end(JSON.stringify({ message: 'Ошибка при отправке письма' }));
                }
                else {
                    console.log('Письмо отправлено:', info.response);
                    res.writeHead(200, { 'Content-Type': 'application/json' });
                    res.end(JSON.stringify({ message: 'Письмо отправлено успешно!' }));
                }
            });
        });
    }
    else {
        res.writeHead(404);
        res.end('Страница не найдена');
    }
});

server.listen(3000, () => {
    console.log('Сервер запущен на http://localhost:3000/');
});