const { send } = require('lab06-m0603');

const message = "Это тестовое сообщение";

send(message)
    .then(() => {
        console.log('Письмо отправлено успешно!');
    })
    .catch((error) => {
        console.error('Ошибка при отправке письма:', error);
    });