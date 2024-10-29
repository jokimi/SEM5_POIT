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
        case WSAEINTR:                 msgText = "Работа функции прервана";                           break;
        case WSAEACCES:                msgText = "Разрешение отвергнуто";                             break;
        case WSAEFAULT:                msgText = "Ошибочный адрес";                                   break;
        case WSAEINVAL:                msgText = "Ошибка в аргументе ";                               break;
        case WSAEMFILE:                msgText = "Слишком много файлов открыто";                      break;
        case WSAEWOULDBLOCK:           msgText = "Ресурс временно недоступен";                        break;
        case WSAEINPROGRESS:           msgText = "Операция в процессе развития";                      break;
        case WSAEALREADY:              msgText = "Операция уже выполняется ";                         break;
        case WSAENOTSOCK:              msgText = "Сокет задан неправильно   ";                        break;
        case WSAEDESTADDRREQ:          msgText = "Требуется адрес расположения ";                     break;
        case WSAEMSGSIZE:              msgText = "Сообщение слишком длинное ";                        break;
        case WSAEPROTOTYPE:            msgText = "Неправильный тип протокола для сокета ";            break;
        case WSAENOPROTOOPT:           msgText = "Ошибка в опции протокола";                          break;
        case WSAEPROTONOSUPPORT:       msgText = "Протокол не поддерживается ";                       break;
        case WSAESOCKTNOSUPPORT:       msgText = "Тип сокета не поддерживается ";                     break;
        case WSAEOPNOTSUPP:            msgText = "Операция не поддерживается ";                       break;
        case WSAEPFNOSUPPORT:          msgText = "Тип протоколов не поддерживается ";                 break;
        case WSAEAFNOSUPPORT:          msgText = "Тип адресов не поддерживается протоколом";          break;
        case WSAEADDRINUSE:            msgText = "Адрес уже используется ";                           break;
        case WSAEADDRNOTAVAIL:         msgText = "Запрошенный адрес не может быть использован";       break;
        case WSAENETDOWN:              msgText = "Сеть отключена ";                                   break;
        case WSAENETUNREACH:           msgText = "Сеть не достижима";                                 break;
        case WSAENETRESET:             msgText = "Сеть разорвала соединение";                         break;
        case WSAECONNABORTED:          msgText = "Программный отказ связи ";                          break;
        case WSAECONNRESET:            msgText = "Связь восстановлена ";                              break;
        case WSAENOBUFS:               msgText = "Не хватает памяти для буферов";                     break;
        case WSAEISCONN:               msgText = "Сокет уже подключен";                               break;
        case WSAENOTCONN:              msgText = "Сокет не подключен";                                break;
        case WSAESHUTDOWN:             msgText = "Нельзя выполнить send: сокет завершил работу";      break;
        case WSAETIMEDOUT:             msgText = "Закончился отведенный интервал  времени";           break;
        case WSAECONNREFUSED:          msgText = "Соединение отклонено  ";                            break;
        case WSAEHOSTDOWN:             msgText = "Хост в неработоспособном состоянии";                break;
        case WSAEHOSTUNREACH:          msgText = "Нет маршрута для хоста ";                           break;
        case WSAEPROCLIM:              msgText = "Слишком много процессов ";                          break;
        case WSASYSNOTREADY:           msgText = "Сеть не доступна ";                                 break;
        case WSAVERNOTSUPPORTED:       msgText = "Данная версия недоступна ";                         break;
        case WSANOTINITIALISED:        msgText = "Не выполнена инициализация WS2_32.DLL";             break;
        case WSAEDISCON:               msgText = "Выполняется отключение";                            break;
        case WSATYPE_NOT_FOUND:        msgText = "Класс не найден ";                                  break;
        case WSAHOST_NOT_FOUND:        msgText = "Хост не найден";                                    break;
        case WSATRY_AGAIN:             msgText = "Неавторизированный хост не найден";                 break;
        case WSANO_RECOVERY:           msgText = "Неопределенная  ошибка";                            break;
        case WSANO_DATA:               msgText = "Нет записи запрошенного типа";                      break;
        case WSA_INVALID_HANDLE:       msgText = "Указанный дескриптор события  с ошибкой";           break;
        case WSA_INVALID_PARAMETER:    msgText = "Один или более параметров с ошибкой";               break;
        case WSA_IO_INCOMPLETE:        msgText = "Объект ввода-вывода не в сигнальном состоянии";     break;
        case WSA_IO_PENDING:           msgText = "Операция завершится позже  ";                       break;
        case WSA_NOT_ENOUGH_MEMORY:    msgText = "Не достаточно памяти ";                             break;
        case WSA_OPERATION_ABORTED:    msgText = "Операция отвергнута ";                              break;
        case WSASYSCALLFAILURE:        msgText = "Аварийное завершение системного вызова ";           break;
        default:                       msgText = "***ERROR***";                                       break;
    }
    return msgText;
}

string SetErrorMsgText(string msgText, int code) {
    return msgText + GetErrorMsgText(code);
}

/*GetServer Function: Attempts to find a server by sending a broadcast message.
Socket Creation: Creates a UDP socket. If it fails, it throws an error.*/
bool GetServer(char* call, short port, struct sockaddr* from, int* flen) {
    SOCKET sS;
    if ((sS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
        throw SetErrorMsgText("socket:", WSAGetLastError());

    // Enable broadcasting
    int optval = 1;
    if (setsockopt(sS, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(optval)) == SOCKET_ERROR) {
        throw SetErrorMsgText("setsockopt:", WSAGetLastError());
    }

    SOCKADDR_IN all;
    memset(&all, 0, sizeof(all));
    all.sin_family = AF_INET;
    all.sin_port = htons(port);
    all.sin_addr.s_addr = INADDR_BROADCAST;

    if (sendto(sS, call, strlen(call) + 1, NULL, (sockaddr*)&all, sizeof(all)) == SOCKET_ERROR)
        throw SetErrorMsgText("sendto:", WSAGetLastError());
    cout << "Message sent: " << call << endl;

    char ibuf[50];
    memset(ibuf, 0, sizeof(ibuf));
    SOCKADDR_IN serv;
    memset(&serv, 0, sizeof(serv));
    int lserv = sizeof(serv);

    // Timeout for receiving responses
    int timeout = 2000;
    if (setsockopt(sS, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout)) == SOCKET_ERROR)
        throw SetErrorMsgText("setsockopt (timeout):", WSAGetLastError());

    while (true) {
        int libuf = recvfrom(sS, ibuf, sizeof(ibuf), NULL, (sockaddr*)&serv, &lserv);
        if (libuf == SOCKET_ERROR) {
            if (WSAGetLastError() == WSAETIMEDOUT) {
                continue; // Continue listening after a timeout
            } else {
                throw SetErrorMsgText("recvfrom:", WSAGetLastError());
            }
        }

        ibuf[libuf] = '\0';
        cout << "Received message: " << ibuf << endl;
        if (strcmp(ibuf, call) == 0) {
            memcpy(from, &serv, sizeof(serv));
            cout << "Matching message received. Server found!" << endl;
        }
    }
    closesocket(sS);
    return false;
}

int main() {
    setlocale(LC_ALL, "Russian");
    try {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
            throw SetErrorMsgText("Startup: ", WSAGetLastError());

        char name[20] = "SERVER1";
        short port = 2000;
        SOCKADDR_IN serv;
        memset(&serv, 0, sizeof(serv));
        int lserv = sizeof(serv);

        if (GetServer(name, port, (struct sockaddr*)&serv, &lserv))
            cout << "Server found at: " << inet_ntoa(serv.sin_addr) << ":" << ntohs(serv.sin_port) << endl;
        else
            cout << "No server found." << endl;
    }
    catch (string errorMsgText) {
        cout << endl << "WSAGetLastError: " << errorMsgText;
    }
    return 0;
}