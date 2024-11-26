#include <iostream>
#include "Winsock2.h"
#include <string.h>
#include <ws2tcpip.h>
#include <string>
#include <vector>
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)
#include <tchar.h>
#include <chrono>
using namespace std;
#pragma comment(lib, "WS2_32.lib")


void SendMessageToServers(char* serverName, char* message) {
    HANDLE cH = CreateFileA(serverName, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (cH == INVALID_HANDLE_VALUE) {
        cerr << "Failed to create file. Error: " << GetLastError() << endl;
    }
    else
    {
        cout << "File created successfully" << endl;
    }

    DWORD bytesSended;
    if (!WriteFile(cH, message, strlen(message), &bytesSended, NULL)) {
        cerr << "Failed to write to Mailslot. Error: " << GetLastError() << endl;
    }
    else {
        cout << "Message sent successfully. Bytes sent: " << bytesSended << endl;
    }

    CloseHandle(cH);
}

int main() {

    char nameServer3[40] = "\\\\DESKTOP-G64NRT0\\mailslot\\Box";
    char message3[18] = "Hello from client";
    SendMessageToServers(nameServer3, message3);

    char nameServer2[40] = "\\\\.\\mailslot\\Box";
    char message2[18] = "Hello from client";
    SendMessageToServers(nameServer2, message2);


    return 0;
}