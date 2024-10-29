const express = require('express');
const readline = require('readline');

const app = express();
const port = 5000;

// Начальное состояние приложения
let state = 'norm';
let previousState = state;

// Middleware для обработки JSON и URL-encoded данных
app.use(express.json());
app.use(express.urlencoded({ extended: true }));

// Главная страница
app.get('/', (req, res) => {
    res.send(`
        <html>
            <head>
                <title>Состояние приложения</title>
            </head>
            <body>
                <h1>${state}</h1>
            </body>
        </html>
    `);
});

// Обработка POST-запроса для обновления состояния
app.post('/', (req, res) => {
    const newState = req.body.state;
    if (['norm', 'stop', 'test', 'idle'].includes(newState)) {
        previousState = state; // Сохраняем предыдущее состояние
        state = newState;
        console.log(`reg = ${previousState} -> ${state}`);
    } else if (newState !== 'exit') {
        console.log(`Некорректный ввод: ${newState}`);
    }
    res.redirect('/');
});

// Функция для обработки ввода с консоли
const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});

const promptUser = () => {
    rl.question(`${state} -> `, (answer) => {
        if (['norm', 'stop', 'test', 'idle'].includes(answer)) {
            previousState = state; // Сохраняем предыдущее состояние
            state = answer;
            console.log(`reg = ${previousState} -> ${state}`);
        } else if (answer === 'exit') {
            console.log('Завершение приложения...');
            rl.close();
            process.exit(0);
        } else {
            console.log(`Некорректный ввод: ${answer}`);
        }
        promptUser();
    });
};

// Запуск сервера
app.listen(port, () => {
    console.log(`Сервер запущен на http://localhost:${port}`);
    promptUser();
});