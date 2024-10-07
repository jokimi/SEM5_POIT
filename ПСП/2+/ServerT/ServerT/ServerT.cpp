#include <iostream>
#include "Winsock2.h"
#include <ws2tcpip.h> // Для inet_ntop
#pragma comment(lib, "WS2_32.lib")
#pragma warning(suppress : 4996)
#pragma warning(disable : 4996)
using namespace std;

string GetErrorMsgText(int code);
string SetErrorMsgText(string msgText, int code);

int main() {
    setlocale(LC_ALL, "rus");
    int i, t;
    try {
        WSADATA wsaData;
        SOCKET serverSocket;
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
            throw SetErrorMsgText("Startup:", WSAGetLastError());
        if ((serverSocket = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
            throw SetErrorMsgText("socket:", WSAGetLastError());
        SOCKADDR_IN serv;
        serv.sin_family = AF_INET;
        serv.sin_port = htons(2000);
        serv.sin_addr.s_addr = INADDR_ANY;
        if (bind(serverSocket, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
            throw SetErrorMsgText("bind:", WSAGetLastError());
        if (SOCKET_ERROR == listen(serverSocket, 2))
            cout << "Listen: " << WSAGetLastError() << endl;
        cout << "Сервер начал прослушку!" << endl;
        SOCKET clientSocket;
        SOCKADDR_IN clnt;
        memset(&clnt, 0, sizeof(clnt));
        int lclnt = sizeof(clnt);
        while (true) {
            i = 1;
            if ((clientSocket = accept(serverSocket, (sockaddr*)&clnt, &lclnt)) == INVALID_SOCKET)
                throw SetErrorMsgText("accept:", WSAGetLastError());
            cout << "Соединение с клиентом успешно!" << endl;
            char ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &clnt.sin_addr, ip, sizeof(ip));
            cout << "Адрес клиента: " << ip << ":" << ntohs(clnt.sin_port) << "\n\n\n\n\n";
            char ibuf[50], obuf[50] = "server: принято ";
            int libuf = 0, lobuf = 0;
            while (true) {
                t = clock();
                if ((libuf = recv(clientSocket, ibuf, sizeof(ibuf), NULL)) == SOCKET_ERROR) {
                    cout << "Recv: " << WSAGetLastError() << endl;
                    break;
                }
                _itoa_s(lobuf, obuf + sizeof("server: принято ") - 1, 50 - sizeof("server: принято ") + 1, 10);
                cout << "Принято сообщение " << i << ": " << ibuf << endl;
                i++;
                if ((lobuf = send(clientSocket, ibuf, strlen(ibuf) + 1, NULL)) == SOCKET_ERROR) {
                    cout << "Send: " << WSAGetLastError() << endl;
                    break;
                }
            }
            cout << "\n\nКлиент отключился" << endl;
            cout << "Затраченное время: " << ((float)t) / CLOCKS_PER_SEC << " секунд";
        }

        if (closesocket(serverSocket) == SOCKET_ERROR)
            throw SetErrorMsgText("closesocket:", WSAGetLastError());
        if (WSACleanup() == SOCKET_ERROR)
            throw SetErrorMsgText("Cleanup:", WSAGetLastError());
    }
    catch (string errorMsgText) {
        cout << endl << "WSAGetLastError: " << errorMsgText;
    }
    return 0;
}

string GetErrorMsgText(int code) {
    string msgText;
    switch (code) {
        case WSAEINTR:
            msgText = "Работа функции прервана";
            break;
        case WSAEACCES:
            msgText = "Разрешение отвергнуто";
            break;
        case WSAEFAULT:
            msgText = "Ошибочный адрес";
            break;
        case WSASYSCALLFAILURE:
            msgText = "Аварийное завершение системного вызова";
            break;
        default:
            msgText = "***ERROR***";
            break;
    }
    return msgText;
}

string SetErrorMsgText(string msgText, int code) {
    return msgText + GetErrorMsgText(code);
}