const axios = require('axios');
const fs = require('fs');
const readline = require('readline');

// Настройки
const TOKEN = 'y0_AgAAAABj_wh0AAzVdwAAAAEZyKBqAABni95F1jVKsrv-lN9YTc1FHRBsiw';
const BASE_URL = 'https://cloud-api.yandex.net/v1/disk';

axios.defaults.headers.common['Authorization'] = `OAuth ${TOKEN}`;

// Функция ожидания нажатия Enter
function waitForEnter() {
    return new Promise((resolve) => {
        const rl = readline.createInterface({
            input: process.stdin,
            output: process.stdout,
        });
        rl.question('Нажмите Enter для продолжения...\n', () => {
            rl.close();
            resolve();
        });
    });
}

// Создание папки
async function createFolder(folderName) {
    try {
        const url = `${BASE_URL}/resources?path=${encodeURIComponent(folderName)}`;
        await axios.put(url);
        console.log(`Папка '${folderName}' создана.`);
    } catch (error) {
        console.error(`Ошибка при создании папки: ${error.response ? error.response.data : error.message}`);
    }
}

// Выгрузка файла
async function uploadFile(localFilePath, remotePath) {
    try {
        const url = `${BASE_URL}/resources/upload?path=${encodeURIComponent(remotePath)}&overwrite=true`;
        const { data } = await axios.get(url); // Получаем URL для загрузки
        const fileData = fs.createReadStream(localFilePath);
        await axios.put(data.href, fileData);
        console.log(`Файл '${localFilePath}' загружен в '${remotePath}'.`);
    } catch (error) {
        console.error(`Ошибка при загрузке файла: ${error.response ? error.response.data : error.message}`);
    }
}

// Загрузка файла
async function downloadFile(remotePath, localFilePath) {
    try {
        const url = `${BASE_URL}/resources/download?path=${encodeURIComponent(remotePath)}`;
        const { data } = await axios.get(url); // Получаем URL для скачивания
        const response = await axios.get(data.href, { responseType: 'stream' });
        const writer = fs.createWriteStream(localFilePath);
        response.data.pipe(writer);
        writer.on('finish', () => console.log(`Файл '${remotePath}' загружен локально как '${localFilePath}'.`));
        writer.on('error', (error) => console.error(`Ошибка при записи файла: ${error.message}`));
    } catch (error) {
        console.error(`Ошибка при загрузке файла: ${error.response ? error.response.data : error.message}`);
    }
}

// Копирование файла
async function copyFile(sourcePath, destinationPath) {
    try {
        const url = `${BASE_URL}/resources/copy?from=${encodeURIComponent(sourcePath)}&path=${encodeURIComponent(destinationPath)}&overwrite=true`;
        await axios.post(url);
        console.log(`Файл '${sourcePath}' скопирован в '${destinationPath}'.`);
    } catch (error) {
        console.error(`Ошибка при копировании файла: ${error.response ? error.response.data : error.message}`);
    }
}

// Удаление файла или папки
async function deleteResource(remotePath) {
    try {
        const url = `${BASE_URL}/resources?path=${encodeURIComponent(remotePath)}&permanently=true`;
        await axios.delete(url);
        console.log(`Ресурс '${remotePath}' удалён.`);
    } catch (error) {
        console.error(`Ошибка при удалении ресурса: ${error.response ? error.response.data : error.message}`);
    }
}

// Пример использования
(async () => {
    console.log('Создание папки "test"...');
    await createFolder('test');
    await waitForEnter();

    console.log('Загрузка файла "local.txt" в "test/remote.txt"...');
    await uploadFile('local.txt', 'test/remote.txt');
    await waitForEnter();

    console.log('Загрузка файла "test/remote.txt" локально как "downloaded.txt"...');
    await downloadFile('test/remote.txt', 'downloaded.txt');
    await waitForEnter();

    console.log('Копирование файла "test/remote.txt" в "test/copy.txt"...');
    await copyFile('test/remote.txt', 'test/copy.txt');
    await waitForEnter();

    console.log('Удаление файла "test/remote.txt"...');
    await deleteResource('test/remote.txt');
    await waitForEnter();

    console.log('Удаление файла "test/copy.txt"...');
    await deleteResource('test/copy.txt');
    await waitForEnter();

    console.log('Удаление папки "test"...');
    await deleteResource('test');
    await waitForEnter();

    console.log('Все операции выполнены.');
})();