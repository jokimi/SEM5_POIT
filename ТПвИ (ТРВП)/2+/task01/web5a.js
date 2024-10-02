const Koa = require('koa');
const Router = require('koa-router');
const fs = require("fs").promises;
const path = require("path");
const cors = require('koa-cors');

const app = new Koa();
const router = new Router();
const port = 3000;

app.use(cors());
app.use(require('koa-bodyparser')());

router.get('/', async (ctx) => {
    ctx.body = 'Welcome! Please go to /web5a.html';
});

router.get('/web5a.html', async (ctx) => {
    const filePath = path.join(__dirname, 'web5a.html');
    const text = await fs.readFile(filePath, 'utf8');
    ctx.body = text;
});

router.post('/calculate', async (ctx) => {
    const x = parseInt(ctx.request.headers['x-value-x']);
    const y = parseInt(ctx.request.headers['x-value-y']);
    if (!isNaN(x) && !isNaN(y)) {
        const z = x + y;
        ctx.set('X-Value-z', z.toString()); // Устанавливаем заголовок
        ctx.set('Access-Control-Expose-Headers', 'X-Value-z'); // Разрешаем доступ к заголовку
        ctx.status = 200; // Успех
    } else {
        ctx.status = 400; // Неверный ввод
        ctx.body = 'Invalid input';
    }
});

app.use(router.routes());
app.use(router.allowedMethods());

app.listen(port, () => console.log(`Server is running at http://localhost:${port}`));