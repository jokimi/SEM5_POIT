const express = require('express');
const session = require('express-session');
const bodyParser = require('body-parser');

const app = express();
const PORT = 3000;

app.use(bodyParser.json());
app.use(session({
    secret: 'session-key',
    resave: false,
    saveUninitialized: true,
    cookie: { secure: false }
}));

app.post('/api/sum', (req, res) => {
    const { x, y } = req.body;

    if (typeof x !== 'number' || typeof y !== 'number' || x < 0 || y < 0) {
        return res.status(400).json({ error: 'N1 and N2 must be positive integers.' });
    }

    if (!req.session.sx) req.session.sx = 0;
    if (!req.session.sy) req.session.sy = 0;
    if (!req.session.count) req.session.count = 0;

    req.session.sx += x;
    req.session.sy += y;
    req.session.count++;

    if (req.session.count % 5 === 0) {
        const response = { sx: req.session.sx, sy: req.session.sy };
        req.session.sx = 0;
        req.session.sy = 0;
        return res.json(response);
    }

    res.json({ sx: req.session.sx, sy: req.session.sy });
});

app.listen(PORT, () => {
    console.log(`Server is running on http://localhost:${PORT}`);
});