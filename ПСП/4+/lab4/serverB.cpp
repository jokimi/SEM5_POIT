#include <iostream>
#include "Winsock2.h"
#pragma comment(lib, "WS2_32.lib")
#pragma warning(disable : 4996)
using namespace std;

#define INADDR_ANY (u_long)0x00000000           // любой адрес
#define INADDR_LOOPBACK 0x7f000001              // внутренняя петля
#define INADDR_BROADCAST (u_long)0xffffffff     // широковещание
#define INADDR_NONE 0xffffffff                  // нет адреса
#define ADDR_ANY INADDR_ANY                     // любой адрес

typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr_in* PSOCKADDR_IN;
typedef struct sockaddr_in FAR* LPSOCKADDR_IN;

string GetErrorMsgText(int code) {
    string msgText;
    switch (code) {
        case WSAEINTR:  msgText = "Работа функции прервана"; break;
        case WSAEACCES: msgText = "Разрешение отвергнуто"; break;
        case WSAEFAULT: msgText = "Ошибочный адрес"; break;
        case WSAEINVAL: msgText = "Ошибка в аргументе "; break;
        case WSAEMFILE: msgText = "Слишком много файлов открыто"; break;
        case WSAEWOULDBLOCK: msgText = "Ресурс временно недоступен"; break;
        case WSAEINPROGRESS: msgText = "Операция в процессе развития"; break;
        case WSAEALREADY: msgText = "Операция уже выполняется "; break;
        case WSAENOTSOCK: msgText = "Сокет задан неправильно "; break;
        case WSAEDESTADDRREQ: msgText = "Требуется адрес расположения "; break;
        case WSAEMSGSIZE: msgText = "Сообщение слишком длинное "; break;
        case WSAEPROTOTYPE: msgText = "Неправильный тип протокола для сокета "; break;
        case WSAENOPROTOOPT: msgText = "Ошибка в опции протокола"; break;
        case WSAEPROTONOSUPPORT: msgText = "Протокол не поддерживается "; break;
        case WSAESOCKTNOSUPPORT: msgText = "Тип сокета не поддерживается "; break;
        case WSAEOPNOTSUPP: msgText = "Операция не поддерживается "; break;
        case WSAEPFNOSUPPORT: msgText = "Тип протоколов не поддерживается "; break;
        case WSAEAFNOSUPPORT: msgText = "Тип адресов не поддерживается протоколом"; break;
        case WSAEADDRINUSE: msgText = "Address already in use"; break;
        case WSAEADDRNOTAVAIL: msgText = "Запрошенный адрес не может быть использован"; break;
        case WSAENETDOWN: msgText = "Сеть отключена "; break;
        case WSAENETUNREACH: msgText = "Сеть не достижима"; break;
        case WSAENETRESET: msgText = "Сеть разорвала соединение"; break;
        case WSAECONNABORTED: msgText = "Программный отказ связи "; break;
        case WSAECONNRESET: msgText = "Связь восстановлена "; break;
        case WSAENOBUFS: msgText = "Не хватает памяти для буферов"; break;
        case WSAEISCONN: msgText = "Сокет уже подключен"; break;
        case WSAENOTCONN: msgText = "Сокет не подключен"; break;
        case WSAESHUTDOWN: msgText = "Нельзя выполнить send: сокет завершил работу"; break;
        case WSAETIMEDOUT: msgText = "Закончился отведенный интервал времени"; break;
        case WSAECONNREFUSED: msgText = "Соединение отклонено "; break;
        case WSAEHOSTDOWN: msgText = "Хост в неработоспособном состоянии"; break;
        case WSAEHOSTUNREACH: msgText = "Нет маршрута для хоста "; break;
        case WSAEPROCLIM: msgText = "Слишком много процессов "; break;
        case WSASYSNOTREADY: msgText = "Сеть не доступна "; break;
        case WSAVERNOTSUPPORTED: msgText = "Данная версия недоступна "; break;
        case WSANOTINITIALISED: msgText = "Не выполнена инициализация WS2_32.DLL"; break;
        case WSAEDISCON: msgText = "Выполняется отключение"; break;
        case WSATYPE_NOT_FOUND: msgText = "Класс не найден "; break;
        case WSAHOST_NOT_FOUND: msgText = "Хост не найден"; break;
        case WSATRY_AGAIN: msgText = "Неавторизированный хост не найден"; break;
        case WSANO_RECOVERY: msgText = "Неопределенная ошибка"; break;
        case WSANO_DATA: msgText = "Нет записи запрошенного типа"; break;
        case WSA_INVALID_HANDLE: msgText = "Указанный дескриптор события с ошибкой"; break;
        case WSA_INVALID_PARAMETER: msgText = "Один или более параметров с ошибкой"; break;
        case WSA_IO_INCOMPLETE: msgText = "Объект ввода-вывода не в сигнальном состоянии"; break;
        case WSA_IO_PENDING: msgText = "Операция завершится позже "; break;
        case WSA_NOT_ENOUGH_MEMORY: msgText = "Не достаточно памяти "; break;
        case WSA_OPERATION_ABORTED: msgText = "Операция отвергнута "; break;
        case WSASYSCALLFAILURE: msgText = "Аварийное завершение системного вызова "; break;
        default: msgText = "***ERROR***"; break;
    }
    return msgText;
}

string SetErrorMsgText(string msgText, int code) {
    return msgText + GetErrorMsgText(code);
}

int CountServers(char* call, short port) {
    SOCKET sS;
    if ((sS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
        throw SetErrorMsgText("socket:", WSAGetLastError());
    int timeout = 2000;
    if (setsockopt(sS, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout)) == SOCKET_ERROR)
        throw SetErrorMsgText("setsockopt:", WSAGetLastError());
    int optval = 1;
    if (setsockopt(sS, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(int)) == SOCKET_ERROR)
        throw SetErrorMsgText("setsockopt:", WSAGetLastError());
    SOCKADDR_IN all;
    all.sin_family = AF_INET;
    all.sin_port = htons(port);
    all.sin_addr.s_addr = INADDR_BROADCAST;
    if (sendto(sS, call, strlen(call) + 1, NULL, (sockaddr*)&all, sizeof(all)) == SOCKET_ERROR)
        throw SetErrorMsgText("send:", WSAGetLastError());
    char ibuf[20];
    SOCKADDR_IN serv;
    memset(&serv, 0, sizeof(serv));
    int lserv = sizeof(serv);
    int counter = 0;
    for (;;) {
        int libuf = recvfrom(sS, ibuf, sizeof(ibuf), NULL, (sockaddr*)&serv, &lserv);
        if (libuf == SOCKET_ERROR) {
            if (WSAGetLastError() == WSAETIMEDOUT)
                break;
            else
                throw SetErrorMsgText("recv:", WSAGetLastError());
        }
        if (strcmp(ibuf, call) == 0) {
            counter++;
            cout << "Found server at: " << inet_ntoa(serv.sin_addr) << ":" << ntohs(serv.sin_port) << endl;
        }
        memset(ibuf, 0, sizeof(ibuf));
    }
    closesocket(sS);
    return counter;
}

bool GetRequestFromClient(char* name, short port, struct sockaddr* from, int* flen) {
    SOCKET sS;
    if ((sS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
        throw SetErrorMsgText("socket:", WSAGetLastError());
    SOCKADDR_IN serv;
    serv.sin_family = AF_INET;
    serv.sin_port = htons(port);
    serv.sin_addr.s_addr = INADDR_ANY;
    if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
        throw SetErrorMsgText("bind: ", WSAGetLastError());
    char ibuf[50];
    int libuf = 0;
    for (;;) {
        if (libuf = recvfrom(sS, ibuf, sizeof(ibuf), NULL, from, flen) == SOCKET_ERROR) {
            if (WSAGetLastError() == WSAETIMEDOUT)
                return false;
            else throw SetErrorMsgText("recv: ", WSAGetLastError());
        }
        if (strcmp(ibuf, name) == 0) {
            if (closesocket(sS) == SOCKET_ERROR)
                throw SetErrorMsgText("closesocket: ", WSAGetLastError());
            return true;
        }
    }
}

bool PutAnswerToClient(char* name, struct sockaddr* to, int* lto) {
    SOCKET sS;
    if ((sS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
        throw SetErrorMsgText("socket:", WSAGetLastError());
    int libuf = 0;
    if ((libuf = sendto(sS, name, strlen(name) + 1, NULL, to, *lto)) == SOCKET_ERROR)
        throw SetErrorMsgText("send: ", WSAGetLastError());
    if (closesocket(sS) == SOCKET_ERROR)
        throw SetErrorMsgText("closesocket: ", WSAGetLastError());
    return true;
}

int main() {
    try {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
            throw SetErrorMsgText("Startup:", WSAGetLastError());
        char name[20] = "SERVER1";
        short port = 2000;
        int existingServers = CountServers(name, port);
        cout << "Number of servers named '" << name << "': " << existingServers << endl;
        SOCKADDR_IN clnt;
        memset(&clnt, 0, sizeof(clnt));
        int lclnt = sizeof(clnt);
        for (;;) {
            if (GetRequestFromClient(name, port, (struct sockaddr*)&clnt, &lclnt))
                cout << "Received message from client" << endl;
            cout << inet_ntoa(clnt.sin_addr) << ":" << ntohs(clnt.sin_port) << endl;
            if (PutAnswerToClient(name, (struct sockaddr*)&clnt, &lclnt))
                cout << "Responded to client" << endl;
        }
        if (WSACleanup() == SOCKET_ERROR)
            throw SetErrorMsgText("Cleanup:", WSAGetLastError());
    }
    catch (string errorMsgText) {
        cout << endl << "WSAGetLastError: " << errorMsgText;
    }
    return 0;
}