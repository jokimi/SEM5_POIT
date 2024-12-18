function handleButtonClick() {
    alert('Кнопка нажата!');
}

async function loadJsonData() {
    try {
        const response = await fetch('json/data.json');
        if (!response.ok) {
            throw new Error('Сеть отвечает с ошибкой');
        }
        const data = await response.json();
        console.log(data);
        const output = document.getElementById('jsonOutput');
        output.textContent = JSON.stringify(data, null, 2);
    }
    catch (error) {
        console.error('Ошибка при загрузке JSON:', error);
    }
}

async function loadXmlData() {
    try {
        const response = await fetch('xml/data.xml');
        if (!response.ok) {
            throw new Error('Сеть отвечает с ошибкой');
        }
        const text = await response.text();
        console.log(text);
        const output = document.getElementById('xmlOutput');
        output.textContent = text;
    }
    catch (error) {
        console.error('Ошибка при загрузке XML:', error);
    }
}

document.addEventListener('DOMContentLoaded', () => {
    const button = document.getElementById('myButton');
    button.addEventListener('click', handleButtonClick);
    loadJsonData();
    loadXmlData();
});