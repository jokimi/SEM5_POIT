const fs = require('fs');
const path = require('path');

const mimeTypes = {
    html: 'text/html',
    css: 'text/css',
    js: 'text/javascript',
    png: 'image/png',
    docx: 'application/msword',
    json: 'application/json',
    xml: 'application/xml',
    mp4: 'video/mp4',
};

module.exports = (staticDir) => {
    return class StaticServer {
        constructor() {
            this.staticDir = staticDir;
        }
        handleRequest(req, res) {
            const { method, url } = req;
            if (method !== 'GET') {
                res.writeHead(405, { 'Content-Type': 'text/plain' });
                res.end('Method Not Allowed');
                return;
            }
            const filePath = path.join(this.staticDir, url.slice(1));
            fs.stat(filePath, (err, stats) => {
                if (err || !stats.isFile()) {
                    res.writeHead(404, { 'Content-Type': 'text/plain' });
                    res.end('File Not Found');
                    return;
                }
                const ext = path.extname(filePath).slice(1);
                const mimeType = mimeTypes[ext] || 'application/octet-stream';
                res.writeHead(200, { 'Content-Type': mimeType });
                const readStream = fs.createReadStream(filePath);
                readStream.pipe(res);
            });
        }
    };
};