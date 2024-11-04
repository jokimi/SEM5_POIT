#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <string>
#include <algorithm>
#include <iostream>
#include "Winsock2.h"
#pragma comment(lib, "WS2_32.lib")

std::string GetErrorMsgText(int code) {
	std::string msgText;
	switch (code) {
		case WSAEINTR: msgText = "Interrupted function call"; break;
		case WSAEACCES: msgText = "Permission denied"; break;
		case WSAEFAULT: msgText = "Bad address"; break;
		case WSAEINVAL: msgText = "Invalid argument"; break;
		case WSAEMFILE: msgText = "Too many open files"; break;
		case WSAEWOULDBLOCK: msgText = "Resource temporarily unavailable"; break;
		case WSAEINPROGRESS: msgText = "Operation now in progress"; break;
		case WSAEALREADY: msgText = "Operation already in progress"; break;
		case WSAENOTSOCK: msgText = "Socket operation on non-socket"; break;
		case WSAEDESTADDRREQ: msgText = "Destination address required"; break;
		case WSAEMSGSIZE: msgText = "Message too long"; break;
		case WSAEPROTOTYPE: msgText = "Protocol wrong type for socket"; break;
		case WSAENOPROTOOPT: msgText = "Bad protocol option"; break;
		case WSAEPROTONOSUPPORT: msgText = "Protocol not supported"; break;
		case WSAESOCKTNOSUPPORT: msgText = "Socket type not supported"; break;
		case WSAEOPNOTSUPP: msgText = "Operation not supported"; break;
		case WSAEPFNOSUPPORT: msgText = "Protocol family not supported"; break;
		case WSAEAFNOSUPPORT: msgText = "Address family not supported by protocol family"; break;
		case WSAEADDRINUSE: msgText = "Address already in use"; break;
		case WSAEADDRNOTAVAIL: msgText = "Cannot assign requested address"; break;
		case WSAENETDOWN: msgText = "Network is down"; break;
		case WSAENETUNREACH: msgText = "Network is unreachable"; break;
		case WSAENETRESET: msgText = "Network dropped connection on reset"; break;
		case WSAECONNABORTED: msgText = "Software caused connection abort"; break;
		case WSAECONNRESET: msgText = "Connection reset by peer"; break;
		case WSAENOBUFS: msgText = "No buffer space available"; break;
		case WSAEISCONN: msgText = "Socket is already connected"; break;
		case WSAENOTCONN: msgText = "Socket is not connected"; break;
		case WSAESHUTDOWN: msgText = "Cannot send after socket shutdown"; break;
		case WSAETIMEDOUT: msgText = "Connection timed out"; break;
		case WSAECONNREFUSED: msgText = "Connection refused"; break;
		case WSAEHOSTDOWN: msgText = "Host is down"; break;
		case WSAEHOSTUNREACH: msgText = "No route to host"; break;
		case WSAEPROCLIM: msgText = "Too many processes"; break;
		case WSASYSNOTREADY: msgText = "Network subsystem is unavailable"; break;
		case WSAVERNOTSUPPORTED: msgText = "Winsock.dll version out of range"; break;
		case WSANOTINITIALISED: msgText = "Successful WSAStartup not yet performed"; break;
		case WSAEDISCON: msgText = "Graceful shutdown in progress"; break;
		case WSATYPE_NOT_FOUND: msgText = "Class type not found"; break;
		case WSAHOST_NOT_FOUND: msgText = "Host not found"; break;
		case WSATRY_AGAIN: msgText = "Non-authoritative host not found"; break;
		case WSANO_RECOVERY: msgText = "Non-recoverable error"; break;
		case WSANO_DATA: msgText = "Valid name, no data record of requested type"; break;
		case WSA_INVALID_HANDLE: msgText = "Specified event object handle is invalid"; break;
		case WSA_INVALID_PARAMETER: msgText = "One or more parameters are invalid"; break;
		case WSA_IO_INCOMPLETE: msgText = "Overlapped I/O event object not in signaled state"; break;
		case WSA_IO_PENDING: msgText = "Overlapped operations will complete later"; break;
		case WSA_NOT_ENOUGH_MEMORY: msgText = "Not enough memory available"; break;
		case WSA_OPERATION_ABORTED: msgText = "Overlapped operation aborted"; break;
		case WSASYSCALLFAILURE: msgText = "System call failure"; break;
		default: break;
	}
	return msgText;
};

std::string SetErrorMsgText(std::string msgText, int code) {
	return msgText + GetErrorMsgText(code);
};

bool GetServerByName(const char* name, const char* call, struct sockaddr* from, int* flen) {
	SOCKET sS = socket(AF_INET, SOCK_DGRAM, NULL);
	hostent* host = gethostbyname(name);
	if (!host)
		throw SetErrorMsgText("gethostbyname: ", WSAGetLastError());
	char* ip_addr = inet_ntoa(*(in_addr*)(host->h_addr));
	std::cout << "Host Name: " << host->h_name << std::endl;
	std::cout << "Server IP: " << ip_addr << std::endl;
	SOCKADDR_IN server;
	server.sin_family = AF_INET;
	server.sin_port = htons(2000);
	server.sin_addr.s_addr = inet_addr(ip_addr);
	char message[10];
	char hostname[256];
	if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR) {
		std::cerr << "Ошибка получения имени хоста" << std::endl;
		WSACleanup();
		return 1;
	}
	if (sendto(sS, hostname, strlen(hostname)+1, NULL, (const sockaddr*)&server, *flen) == SOCKET_ERROR)
		throw SetErrorMsgText("sendto:", WSAGetLastError());
	if (sendto(sS, call, strlen(call)+1, NULL, (const sockaddr*)&server, *flen) == SOCKET_ERROR)
		throw SetErrorMsgText("sendto:", WSAGetLastError());
	int buf = 0;
	if ((buf = recvfrom(sS, message, sizeof(message), NULL, (sockaddr*)&server, flen)) == SOCKET_ERROR)
		throw SetErrorMsgText("recvfrom:", WSAGetLastError());
	message[buf] = 0x00;
	std::cout << "\nMessage from server: " << message << "\n";
	*from = *((sockaddr*)&server);
	closesocket(sS);
	return true;
}

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	SOCKET cS;
	WSADATA wsaData;
	try {
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
			throw SetErrorMsgText("Startup:", WSAGetLastError());
		if ((cS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
			throw SetErrorMsgText("socket:", WSAGetLastError());
		sockaddr from;
		int length = sizeof(from);
		GetServerByName("annPuzu", "Hello", &from, &length);
		//GetServerByName("DESKTOP-3SMK9QM", "Hello", &from, &length);
		if (closesocket(cS) == SOCKET_ERROR)
			throw SetErrorMsgText("closesocket:", WSAGetLastError());
		if (WSACleanup() == SOCKET_ERROR)
			throw SetErrorMsgText("Cleanup:", WSAGetLastError());
	}
	catch (std::string errorMsgText) {
		std::cout << std::endl << "WSAGetLastError: " << errorMsgText;
	}
	return 0;
}