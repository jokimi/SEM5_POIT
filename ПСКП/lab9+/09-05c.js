const http = require('http');

const xmlData = `
<request>
    <_comment>lab10</_comment>
    <x>1</x>
    <y>2</y>
    <s>message</s>
    <m>
        <item>a</item>
        <item>b</item>
        <item>c</item>
    </m>
    <o>
        <surname>Kozeka</surname>
        <name>Elizaveta</name>
    </o>
</request>
`;

const options = {
    hostname: '127.0.0.1',
    port: 5000,
    path: '/',
    method: 'POST',
    headers: {
        'Content-Type': 'application/xml',
        'Content-Length': Buffer.byteLength(xmlData)
    }
};

const req = http.request(options, (res) => {
    console.log(`Статус ответа: ${res.statusCode}`);
    console.log(`Сообщение к статусу: ${res.statusMessage}`);
    let responseBody = '';
    res.on('data', (chunk) => {
        responseBody += chunk;
    });
    res.on('end', () => {
        console.log('Ответ сервера:');
        console.log(responseBody);
    });
});

req.on('error', (e) => {
    console.error(`Ошибка запроса: ${e.message}`);
});

req.write(xmlData);
req.end();

/*

<request>
    <_comment>lab8</_comment>
    <x>1</x>
    <y>2</y>
    <s>message</s>
    <m>
        <item>a</item>
        <item>b</item>
        <item>c</item>
    </m>
    <o>
        <surname>Kozeka</surname>
        <name>Elizaveta</name>
    </o>
</request>

*/