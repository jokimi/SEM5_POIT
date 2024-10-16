const express = require('express');
const cookieParser = require('cookie-parser');
const bodyParser = require('body-parser');

const app = express();
const PORT = 3000;

app.use(cookieParser());
app.use(bodyParser.json());

app.post('/api/sum', (req, res) => {
    const { x, y } = req.body;

    // Валидация входных данных
    if (typeof x !== 'number' || typeof y !== 'number' || x < 0 || y < 0) {
        return res.status(400).json({ error: 'N1 and N2 must be positive integers.' });
    }

    let sx = 0;
    let sy = 0;

    if (req.cookies.sx) {
        sx = parseInt(req.cookies.sx, 10);
    }
    if (req.cookies.sy) {
        sy = parseInt(req.cookies.sy, 10);
    }

    sx += x;
    sy += y;

    let count = parseInt(req.cookies.count || '0', 10) + 1;

    res.cookie('sx', sx);
    res.cookie('sy', sy);
    res.cookie('count', count);

    if (count % 5 === 0) {
        const response = { sx: sx, sy: sy };
        res.cookie('sx', 0);
        res.cookie('sy', 0);
        return res.json(response);
    } else {
        return res.json({ sx: sx, sy: sy });
    }
});

// Запуск сервера
app.listen(PORT, () => {
    console.log(`Server is running on http://localhost:${PORT}`);
});