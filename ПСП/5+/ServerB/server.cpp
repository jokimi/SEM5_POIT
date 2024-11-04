#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Winsock2.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <tuple>
#pragma comment(lib, "WS2_32.lib")

int countServers = 0;

std::string GetErrorMsgText(int code) {
    std::string msgText;
    switch (code) {
        case WSAEINTR: msgText = "Interrupted function call";
            break;
        case WSAEACCES: msgText = "Permission denied";
            break;
        case WSAEFAULT: msgText = "Bad address";
            break;
        case WSAEINVAL: msgText = "Invalid argument";
            break;
        case WSAEMFILE: msgText = "Too many open files";
            break;
        case WSAEWOULDBLOCK: msgText = "Resource temporarily unavailable";
            break;
        case WSAEINPROGRESS: msgText = "Operation now in progress";
            break;
        case WSAEALREADY: msgText = "Operation already in progress";
            break;
        case WSAENOTSOCK: msgText = "Socket operation on non-socket";
            break;
        case WSAEDESTADDRREQ: msgText = "Destination address required";
            break;
        case WSAEMSGSIZE: msgText = "Message too long";
            break;
        case WSAEPROTOTYPE: msgText = "Protocol wrong type for socket";
            break;
        case WSAENOPROTOOPT: msgText = "Bad protocol option";
            break;
        case WSAEPROTONOSUPPORT: msgText = "Protocol not supported";
            break;
        case WSAESOCKTNOSUPPORT: msgText = "Socket type not supported";
            break;
        case WSAEOPNOTSUPP: msgText = "Operation not supported";
            break;
        case WSAEPFNOSUPPORT: msgText = "Protocol family not supported";
            break;
        case WSAEAFNOSUPPORT: msgText = "Address family not supported by protocol family";
            break;
        case WSAEADDRINUSE: msgText = "Address already in use";
            break;
        case WSAEADDRNOTAVAIL: msgText = "Cannot assign requested address";
            break;
        case WSAENETDOWN: msgText = "Network is down";
            break;
        case WSAENETUNREACH: msgText = "Network is unreachable";
            break;
        case WSAENETRESET: msgText = "Network dropped connection on reset";
            break;
        case WSAECONNABORTED: msgText = "Software caused connection abort";
            break;
        case WSAECONNRESET: msgText = "Connection reset by peer";
            break;
        case WSAENOBUFS: msgText = "No buffer space available";
            break;
        case WSAEISCONN: msgText = "Socket is already connected";
            break;
        case WSAENOTCONN: msgText = "Socket is not connected";
            break;
        case WSAESHUTDOWN: msgText = "Cannot send after socket shutdown";
            break;
        case WSAETIMEDOUT: msgText = "Connection timed out";
            break;
        case WSAECONNREFUSED: msgText = "Connection refused";
            break;
        case WSAEHOSTDOWN: msgText = "Host is down";
            break;
        case WSAEHOSTUNREACH: msgText = "No route to host";
            break;
        case WSAEPROCLIM: msgText = "Too many processes";
            break;
        case WSASYSNOTREADY: msgText = "Network subsystem is unavailable";
            break;
        case WSAVERNOTSUPPORTED: msgText = "Winsock.dll version out of range";
            break;
        case WSANOTINITIALISED: msgText = "Successful WSAStartup not yet performed";
            break;
        case WSAEDISCON: msgText = "Graceful shutdown in progress";
            break;
        case WSATYPE_NOT_FOUND: msgText = "Class type not found";
            break;
        case WSAHOST_NOT_FOUND: msgText = "Host not found";
            break;
        case WSATRY_AGAIN: msgText = "Non-authoritative host not found";
            break;
        case WSANO_RECOVERY: msgText = "Non-recoverable error";
            break;
        case WSANO_DATA: msgText = "Valid name, no data record of requested type";
            break;
        case WSA_INVALID_HANDLE: msgText = "Specified event object handle is invalid";
            break;
        case WSA_INVALID_PARAMETER: msgText = "One or more parameters are invalid";
            break;
        case WSA_IO_INCOMPLETE: msgText = "Overlapped I/O event object not in signaled state";
            break;
        case WSA_IO_PENDING: msgText = "Overlapped operations will complete later";
            break;
        case WSA_NOT_ENOUGH_MEMORY: msgText = "Not enough memory available";
            break;
        case WSA_OPERATION_ABORTED: msgText = "Overlapped operation aborted";
            break;
        case WSASYSCALLFAILURE: msgText = "System call failure";
            break;
        default: break;
    }
    return msgText;
};

std::tuple<std::string, int> SetErrorMsgText(std::string msgText, int code) {
    return {msgText + GetErrorMsgText(code), code};
};

bool GetRequestFromClient(char *name, short port, struct sockaddr *from, int *flen) {
    char buf[50];
    int check;
    char nameBuf[50];
    std::cout << "\nWait message..." << std::endl;
    while (true) {
        recvfrom(port, nameBuf, sizeof(nameBuf), NULL, from, flen);
        std::cout << "Client-HostName: " << nameBuf << std::endl;
        check = recvfrom(port, buf, sizeof(buf), NULL, from, flen);
        if (check == SOCKET_ERROR) {
            std::cout << "\nError" << std::endl;
            if (WSAGetLastError() == WSAETIMEDOUT)
                return false;
            else
                throw SetErrorMsgText("Recvfrom: ", WSAGetLastError());
        }
        if (strcmp(buf, name) == 0 &&
            strcmp(inet_ntoa(((SOCKADDR_IN *) from)->sin_addr), "192.168.82.189") != 0) {
            std::cout << buf << "\n";
            return true;
        }
        else
            return false;
    }
}

bool PutAnswerToClient(char *name, short port, struct sockaddr *to, int *lto) {
    int lenght = strlen(name);
    return sendto(port, name, lenght + 1, NULL, to, *lto);
}

SOCKET sS;

bool GetServer(char *name, short port, sockaddr *from, int *flen) {
    ((SOCKADDR_IN *) from)->sin_family = AF_INET;
    ((SOCKADDR_IN *) from)->sin_port = htons(port);
    ((SOCKADDR_IN *) from)->sin_addr.s_addr = inet_addr("192.168.82.255");
    int lbuf;
    char ibuf[50];
    int optval = 1;
    try {
        if (setsockopt(sS, SOL_SOCKET, SO_BROADCAST, (char *) &optval, sizeof(optval)) == SOCKET_ERROR)
            throw SetErrorMsgText("opt: ", WSAGetLastError());
        for (int i = 0; i < 5; i++) {
            if (sendto(sS, name, strlen(name) + 1, NULL, from, *flen) == SOCKET_ERROR)
                throw SetErrorMsgText("sendto: ", WSAGetLastError());
            if (recvfrom(sS, ibuf, sizeof(ibuf), NULL, from, flen) == SOCKET_ERROR) {
                if (WSAGetLastError() == WSAETIMEDOUT)
                    return false;
                else
                    throw SetErrorMsgText("recvfrom: ", WSAGetLastError());
            }
            if (strcmp(name, ibuf) == 0 &&
                strcmp(inet_ntoa(((SOCKADDR_IN *) from)->sin_addr), "192.168.82.189") != 0) {
                countServers++;
                std::cout << "There's a server with the same callsign!" << std::endl;
                std::cout << "Count: " << countServers << std::endl;
                std::cout << "Hostname: " << gethostbyaddr((char*)&((SOCKADDR_IN*)from)->sin_addr, sizeof(((SOCKADDR_IN*)from)->sin_addr), AF_INET)->h_name << std::endl;
                std::cout << "IP: " << inet_ntoa(((SOCKADDR_IN *) from)->sin_addr) << std::endl;
                std::cout << "Port: " << ntohs(((struct sockaddr_in *) from)->sin_port) << std::endl;
                return true;
            }

            Sleep(1000);
        }
    }
    catch (std::tuple<std::string, int> &errorMsgText) {
        if (std::get<1>(errorMsgText) == WSAETIMEDOUT) {
            std::cout << "Number of servers with the same callsign: " << countServers << std::endl;
            if (closesocket(sS) == SOCKET_ERROR)
                throw SetErrorMsgText("closesocket: ", WSAGetLastError());
        } else throw SetErrorMsgText("GetServer:", WSAGetLastError());
    }
    return true;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    WSADATA wsaData;
    SOCKADDR_IN client;
    int clientSize = sizeof(client);
    char name[] = "Hello";
    char hostname[32];
    try {
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
            throw SetErrorMsgText("Startup:", WSAGetLastError());
        if ((sS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
            throw SetErrorMsgText("socket:", WSAGetLastError());
        SOCKADDR_IN serv;
        serv.sin_family = AF_INET;
        serv.sin_port = htons(2000);
        serv.sin_addr.s_addr = INADDR_ANY;
        if (bind(sS, (LPSOCKADDR) &serv, sizeof(serv)) == SOCKET_ERROR)
            throw SetErrorMsgText("bind:", WSAGetLastError());
        std::cout << "Checking for other servers..." << std::endl;
        GetServer(name, 2000, (sockaddr *) &client, &clientSize);
        int optval = 0;
        if (setsockopt(sS, SOL_SOCKET, SO_BROADCAST, (char *) &optval, sizeof(optval)) == SOCKET_ERROR)
            throw SetErrorMsgText("opt: ", WSAGetLastError());
        SOCKADDR_IN clientB;
        int clientSize = sizeof(clientB);
        if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR)
            throw SetErrorMsgText("gethostname:", WSAGetLastError());
        std::cout << "Server name: " << hostname << std::endl;
        while (true) {
            if (GetRequestFromClient(name, sS, (SOCKADDR *) &clientB, &clientSize)) {
                std::cout << std::endl;
                std::cout << "Client socket:" << std::endl;
                std::cout << "IP: " << inet_ntoa(clientB.sin_addr) << std::endl;
                std::cout << "Port: " << htons(clientB.sin_port) << std::endl;
                std::cout << std::endl;
                if (PutAnswerToClient(name, sS, (SOCKADDR *) &clientB, &clientSize)) {
                    std::cout << "Success!" << std::endl;
                }
            }
        }
        if (closesocket(sS) == SOCKET_ERROR)
            throw SetErrorMsgText("closesocket:", WSAGetLastError());
        if (WSACleanup() == SOCKET_ERROR)
            throw SetErrorMsgText("Cleanup:", WSAGetLastError());
    }
    catch (std::tuple<std::string, int> &errorMsgText) {
        std::cout << std::endl << "Error: " << std::get<0>(errorMsgText) << " Code: " << std::get<1>(errorMsgText);
        system("pause");
    }
    return 0;
}