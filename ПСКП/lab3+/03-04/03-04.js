const express = require('express');
const app = express();
const port = 5000;

// Асинхронная функция для вычисления факториала с использованием process.nextTick
const factorial = (n, callback) => {
    if (n < 0) return callback(new Error('Факториал отрицательных чисел не определен'));
    if (n === 0 || n === 1) return callback(null, 1); // Базовый случай

    // Рекурсивный случай
    process.nextTick(() => {
        factorial(n - 1, (err, result) => {
            if (err) return callback(err);
            callback(null, n * result);
        });
    });
};

// Обработка GET-запроса для расчета факториала
app.get('/fact', (req, res) => {
    const k = parseInt(req.query.k, 10);

    if (isNaN(k)) {
        return res.status(400).json({ error: 'Параметр k должен быть числом' });
    }

    factorial(k, (err, fact) => {
        if (err) return res.status(400).json({ error: err.message });
        res.json({ k: k, fact: fact });
    });
});

// Обработка GET-запроса для HTML-страницы
app.get('/', (req, res) => {
    res.send(`
        <!DOCTYPE html>
        <html lang="en">
        <head>
            <meta charset="UTF-8">
            <meta name="viewport" content="width=device-width, initial-scale=1.0">
            <title>Факториал</title>
        </head>
        <body>
            <h1>Результаты вычисления факториалов</h1>
            <div id="results"></div>
            <script>
                const resultsDiv = document.getElementById('results');
                const startTime = Date.now();

                const fetchFactorial = async (k) => {
                    const response = await fetch(\`/fact?k=\${k}\`);
                    const data = await response.json();
                    const elapsedTime = Date.now() - startTime;
                    resultsDiv.innerHTML += \`<p>\${k}. Результат: \${elapsedTime}-\${data.k}/\${data.fact}</p>\`;
                };

                const fetchAllFactorials = async () => {
                    for (let x = 0; x <= 20; x++) {
                        await fetchFactorial(x);
                    }
                    const totalTime = Date.now() - startTime;
                    resultsDiv.innerHTML += \`<h2>Общая продолжительность: \${totalTime} ms</h2>\`;
                };

                fetchAllFactorials();
            </script>
        </body>
        </html>
    `);
});

// Запуск сервера
app.listen(port, () => {
    console.log(`Сервер запущен на http://localhost:${port}`);
});