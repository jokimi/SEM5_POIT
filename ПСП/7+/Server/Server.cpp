#include <iostream>
#include "Winsock2.h"
#include <string.h>
#include <ws2tcpip.h>
#include <string>
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)
#include <tchar.h>
using namespace std;
#pragma comment(lib, "WS2_32.lib")

string SetErrorMsgText(string msg, int errorNumber) {
    string result = "";
    switch (errorNumber)
    {
    case WSAEINTR:
        result = msg + "Работа функции прервана WSAITR";
    case WSAEACCES:
        result = msg + "Разрешение отвергнутоWSAEACCES";
    case WSAEFAULT:
        result = msg + "Ошибочный адрес  WSAEFAULT";
    case WSAEINVAL:
        result = msg + "Ошибка в аргументеWSAEINVAL";
    case WSAEMFILE:
        result = msg + "Слишком много файлов открытоWSAEMFILE";
    case WSAEWOULDBLOCK:
        result = msg + "Ресурс временно недоступенWSAEWOULDBLOCK";
    case WSAEINPROGRESS:
        result = msg + "Операция в процессе развитияWSAEINPROGRESS";
    case WSAEALREADY:
        result = msg + "Операция уже выполняется WSAEALREADY";
    case WSAENOTSOCK:
        result = msg + "Сокет задан неправильно  WSAENOTSOCK ";
    case WSAEDESTADDRREQ:
        result = msg + "Требуется адрес расположения WSAEDESTADDRREQ ";
    case WSAEMSGSIZE:
        result = msg + "Сообщение слишком длинное ";
    case WSAEPROTOTYPE:
        result = msg + "Неправильный тип протокола для сокета ";
    case WSAENOPROTOOPT:
        result = msg + "Ошибка в опции протокола";
    case WSAEPROTONOSUPPORT:
        result = msg + "Протокол не поддерживается";
    case WSAESOCKTNOSUPPORT:
        result = msg + "Тип сокета не поддерживается ";
    case WSAEOPNOTSUPP:
        result = msg + "Операция не поддерживается";
    case WSAEPFNOSUPPORT:
        result = msg + "Тип протоколов не поддерживается ";
    case WSAEAFNOSUPPORT:
        result = msg + "Тип адресов не поддерживается протоколом";
    case WSAEADDRINUSE:
        result = msg + "Адрес уже используется ";
    case WSAEADDRNOTAVAIL:
        result = msg + "Запрошенный адрес не может быть использован";
    case WSAENETDOWN:
        result = msg + "Сеть отключена ";
    case WSAENETUNREACH:
        result = msg + "Сеть не достижима";
    case WSAENETRESET:
        result = msg + "Сеть разорвала соединение";
    case WSAECONNABORTED:
        result = msg + "Программный отказ связи";
    case WSAECONNRESET:
        result = msg + "Связь восстановлена ";
    case WSAENOBUFS:
        result = msg + "Не хватает памяти для буферов";
    case WSAEISCONN:
        result = msg + "Сокет уже подключен";
    case WSAENOTCONN:
        result = msg + "Сокет не подключен";
    case WSAESHUTDOWN:
        result = msg + "Нельзя выполнить send: сокет завершил работу";
    case WSAETIMEDOUT:
        result = msg + "Закончился отведенный интервал  времени";
    case WSAECONNREFUSED:
        result = msg + "Соединение отклонено";
    case WSAEHOSTDOWN:
        result = msg + "Хост в неработоспособном состоянии";
    case WSAEHOSTUNREACH:
        result = msg + "Нет маршрута для хоста ";
    case WSAEPROCLIM:
        result = msg + "Слишком много процессов ";
    case WSASYSNOTREADY:
        result = msg + "Сеть не доступна";
    case WSAVERNOTSUPPORTED:
        result = msg + "Данная версия недоступна ";
    case WSANOTINITIALISED:
        result = msg + "Не выполнена инициализация WS2_32.DLL";
    case WSAEDISCON:
        result = msg + "Выполняется отключение";
    case WSATYPE_NOT_FOUND:
        result = msg + "Класс не найден ";
    case WSAHOST_NOT_FOUND:
        result = msg + "Хост не найден";
    case WSATRY_AGAIN:
        result = msg + "Неавторизированный хост не найден ";
    case WSANO_RECOVERY:
        result = msg + "Неопределенная  ошибка";
    case WSANO_DATA:
        result = msg + "Нет записи запрошенного типа ";
    case WSA_INVALID_HANDLE:
        result = msg + "Указанный дескриптор события  с ошибкой";
    case WSA_INVALID_PARAMETER:
        result = msg + "Один или более параметров с ошибкой";
    case WSA_IO_INCOMPLETE:
        result = msg + "Объект ввода-вывода не в сигнальном состоянии";
    case WSA_IO_PENDING:
        result = msg + "Операция завершится позже";
    case WSA_NOT_ENOUGH_MEMORY:
        result = msg + "Не достаточно памяти ";
    case WSA_OPERATION_ABORTED:
        result = msg + "Операция отвергнута";
    case WSASYSCALLFAILURE:
        result = msg + "Аварийное завершение системного вызова";
    default:
        break;
    }
    return result;
}

int main() {
    HANDLE sH = CreateMailslot(TEXT("\\\\.\\mailslot\\Box"), 500, 180000, NULL);
    cout << "server listening..." << endl;
    if (sH == INVALID_HANDLE_VALUE) {
        throw SetErrorMsgText("create: ", GetLastError());
    }
    char rbuf[18];
    DWORD bytesRead;
    if (!ReadFile(sH, rbuf, sizeof(rbuf) - 1, &bytesRead, NULL)) {
        DWORD error = GetLastError();
        if (error == ERROR_SEM_TIMEOUT) {
            cout << "Timeout expired while reading from Mailslot." << endl;
        }
        else {
            throw SetErrorMsgText("read: ", GetLastError());
        }
    }
    else {
        cout << bytesRead << endl;
        cout << "Received message: " << rbuf << endl;
    }
    return 0;
}