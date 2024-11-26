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
        result = msg + "������ ������� �������� WSAITR";
    case WSAEACCES:
        result = msg + "���������� ����������WSAEACCES";
    case WSAEFAULT:
        result = msg + "��������� �����  WSAEFAULT";
    case WSAEINVAL:
        result = msg + "������ � ���������WSAEINVAL";
    case WSAEMFILE:
        result = msg + "������� ����� ������ �������WSAEMFILE";
    case WSAEWOULDBLOCK:
        result = msg + "������ �������� ����������WSAEWOULDBLOCK";
    case WSAEINPROGRESS:
        result = msg + "�������� � �������� ��������WSAEINPROGRESS";
    case WSAEALREADY:
        result = msg + "�������� ��� ����������� WSAEALREADY";
    case WSAENOTSOCK:
        result = msg + "����� ����� �����������  WSAENOTSOCK ";
    case WSAEDESTADDRREQ:
        result = msg + "��������� ����� ������������ WSAEDESTADDRREQ ";
    case WSAEMSGSIZE:
        result = msg + "��������� ������� ������� ";
    case WSAEPROTOTYPE:
        result = msg + "������������ ��� ��������� ��� ������ ";
    case WSAENOPROTOOPT:
        result = msg + "������ � ����� ���������";
    case WSAEPROTONOSUPPORT:
        result = msg + "�������� �� ��������������";
    case WSAESOCKTNOSUPPORT:
        result = msg + "��� ������ �� �������������� ";
    case WSAEOPNOTSUPP:
        result = msg + "�������� �� ��������������";
    case WSAEPFNOSUPPORT:
        result = msg + "��� ���������� �� �������������� ";
    case WSAEAFNOSUPPORT:
        result = msg + "��� ������� �� �������������� ����������";
    case WSAEADDRINUSE:
        result = msg + "����� ��� ������������ ";
    case WSAEADDRNOTAVAIL:
        result = msg + "����������� ����� �� ����� ���� �����������";
    case WSAENETDOWN:
        result = msg + "���� ��������� ";
    case WSAENETUNREACH:
        result = msg + "���� �� ���������";
    case WSAENETRESET:
        result = msg + "���� ��������� ����������";
    case WSAECONNABORTED:
        result = msg + "����������� ����� �����";
    case WSAECONNRESET:
        result = msg + "����� ������������� ";
    case WSAENOBUFS:
        result = msg + "�� ������� ������ ��� �������";
    case WSAEISCONN:
        result = msg + "����� ��� ���������";
    case WSAENOTCONN:
        result = msg + "����� �� ���������";
    case WSAESHUTDOWN:
        result = msg + "������ ��������� send: ����� �������� ������";
    case WSAETIMEDOUT:
        result = msg + "���������� ���������� ��������  �������";
    case WSAECONNREFUSED:
        result = msg + "���������� ���������";
    case WSAEHOSTDOWN:
        result = msg + "���� � ����������������� ���������";
    case WSAEHOSTUNREACH:
        result = msg + "��� �������� ��� ����� ";
    case WSAEPROCLIM:
        result = msg + "������� ����� ��������� ";
    case WSASYSNOTREADY:
        result = msg + "���� �� ��������";
    case WSAVERNOTSUPPORTED:
        result = msg + "������ ������ ���������� ";
    case WSANOTINITIALISED:
        result = msg + "�� ��������� ������������� WS2_32.DLL";
    case WSAEDISCON:
        result = msg + "����������� ����������";
    case WSATYPE_NOT_FOUND:
        result = msg + "����� �� ������ ";
    case WSAHOST_NOT_FOUND:
        result = msg + "���� �� ������";
    case WSATRY_AGAIN:
        result = msg + "������������������ ���� �� ������ ";
    case WSANO_RECOVERY:
        result = msg + "��������������  ������";
    case WSANO_DATA:
        result = msg + "��� ������ ������������ ���� ";
    case WSA_INVALID_HANDLE:
        result = msg + "��������� ���������� �������  � �������";
    case WSA_INVALID_PARAMETER:
        result = msg + "���� ��� ����� ���������� � �������";
    case WSA_IO_INCOMPLETE:
        result = msg + "������ �����-������ �� � ���������� ���������";
    case WSA_IO_PENDING:
        result = msg + "�������� ���������� �����";
    case WSA_NOT_ENOUGH_MEMORY:
        result = msg + "�� ���������� ������ ";
    case WSA_OPERATION_ABORTED:
        result = msg + "�������� ����������";
    case WSASYSCALLFAILURE:
        result = msg + "��������� ���������� ���������� ������";
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