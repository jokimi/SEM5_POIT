import nodemailer from 'nodemailer';

const EMAIL_ADDRESS = 'lizakozeka@gmail.com';
const APP_PASSWORD = 'svrfbtvljzisqgom';

const transporter = nodemailer.createTransport({
    service: 'gmail',
    auth: {
        user: EMAIL_ADDRESS,
        pass: APP_PASSWORD,
    },
});

export const send = (message) => {
    const mailOptions = {
        from: EMAIL_ADDRESS,
        to: EMAIL_ADDRESS,
        subject: 'task 3',
        text: message,
    };
    return transporter.sendMail(mailOptions);
};