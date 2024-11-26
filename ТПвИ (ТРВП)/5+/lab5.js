const express = require('express');
const fs = require('fs');
const path = require('path');

const app = express();

// Функция для установки заголовков кеширования
function setCacheHeaders(res, cacheType) {
    const now = new Date();

    switch (cacheType) {
        case 'no-store':
            res.set('Cache-Control', 'no-store');
            break;
        case 'max-age':
            res.set('Cache-Control', 'max-age=3600');
            break;
        case 'expired':
            res.set('Expires', now.toUTCString());
            break;
        case 'last-modified':
            res.set('Last-Modified', new Date().toUTCString());
            break;
        case 'etag':
            res.set('Etag', '12345');
            break;
        default:
            res.set('Cache-Control', 'no-cache');
    }
}

// Хэндлер для PNG-изображений
app.get('/image', (req, res) => {
    const cacheParam = req.query.cache_parm || 'no-cache';
    setCacheHeaders(res, cacheParam);

    const imagePath = path.join(__dirname, 'public', 'image.png');
    res.setHeader('Content-Type', 'image/png');
    fs.createReadStream(imagePath).pipe(res);
});

// Хэндлер для JavaScript
app.get('/script.js', (req, res) => {
    const cacheParam = req.query.cache_parm || 'no-cache';
    setCacheHeaders(res, cacheParam);

    const jsContent = `
        console.log('Hello from the script!');
        document.body.style.backgroundColor = 'pink';
    `;

    res.setHeader('Content-Type', 'application/javascript');
    res.send(jsContent);
});

// Хэндлер для CSS
app.get('/style.css', (req, res) => {
    const cacheParam = req.query.cache_parm || 'no-cache';
    setCacheHeaders(res, cacheParam);

    const cssContent = `
        body {
            font-family: Arial, sans-serif;
            color: #333;
        }
    `;

    res.setHeader('Content-Type', 'text/css');
    res.send(cssContent);
});

// Страница для демонстрации
app.get('/', (req, res) => {
    res.send(`
        <!DOCTYPE html>
        <html lang="en">
        <head>
            <meta charset="UTF-8">
            <meta name="viewport" content="width=device-width, initial-scale=1.0">
            <title>HTTP Handlers Demo</title>
            <link rel="stylesheet" href="/style.css?cache_parm=max-age">
        </head>
        <body>
            <h1>HTTP Handlers Demo</h1>
            <img src="/image?cache_parm=etag" alt="Dynamic PNG">
            <script src="/script.js?cache_parm=no-store"></script>
        </body>
        </html>
    `);
});

// Запуск сервера
const PORT = 3000;
app.listen(PORT, () => {
    console.log(`Server running at http://localhost:${PORT}`);
});