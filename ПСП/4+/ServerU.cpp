#include <iostream>
#include <clocale>
#include "Winsock2.h"
using namespace std;
#pragma comment(lib, "WS2_32.lib")  

int main() {
    setlocale(LC_ALL, "rus");
    WSADATA wsaData;
    SOCKET sS;
    SOCKADDR_IN serv;
    serv.sin_family = AF_INET;
    serv.sin_port = htons(2000); // Порт должен совпадать с клиентом
    serv.sin_addr.s_addr = INADDR_ANY;

    try {
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
            throw "WSAStartup failed";

        if ((sS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
            throw "Socket creation failed";

        if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
            throw "Bind failed";

        SOCKADDR_IN clientInfo;
        memset(&clientInfo, 0, sizeof(clientInfo));
        char ibuf[50];
        int lc = sizeof(clientInfo), lb = 0;

        while (true) {
            lb = recvfrom(sS, ibuf, sizeof(ibuf), NULL, (sockaddr*)&clientInfo, &lc);
            if (lb == SOCKET_ERROR)
                throw "recvfrom failed";

            ibuf[lb] = '\0'; // Завершите строку нулем
            cout << "Received from client: " << ibuf << endl;

            // Проверяем, совпадает ли сообщение с "SERVER1"
            if (strcmp(ibuf, "SERVER1") == 0) {
                // Отправляем ответ обратно клиенту
                sendto(sS, ibuf, lb, 0, (sockaddr*)&clientInfo, lc);
                cout << "Response sent to client." << endl;
            }
        }
    }
    catch (const char* error_msg_text) {
        cout << endl << error_msg_text;
    }
    WSACleanup(); // Очистка
    return 0;
}