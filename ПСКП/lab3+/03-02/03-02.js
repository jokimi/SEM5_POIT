const express = require('express');
const app = express();
const port = 5000;

// Рекурсивная функция для вычисления факториала
const factorial = (n) => {
    if (n < 0) return null; // Факториал отрицательных чисел не определен
    if (n === 0 || n === 1) return 1; // Базовый случай
    return n * factorial(n - 1); // Рекурсивный случай
};

// Обработка GET-запроса для расчета факториала
app.get('/fact', (req, res) => {
    const k = parseInt(req.query.k, 10);

    if (isNaN(k)) {
        return res.status(400).json({ error: 'Параметр k должен быть числом' });
    }

    const fact = factorial(k);
    res.json({ k: k, fact: fact });
});

// Запуск сервера
app.listen(port, () => {
    console.log(`Сервер запущен на http://localhost:${port}`);
});