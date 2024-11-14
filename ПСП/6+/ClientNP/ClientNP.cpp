#pragma once
#include "Winsock2.h"
#pragma comment(lib, "WS2_32.lib") 
#include <iostream>
#include <string>
#include <tchar.h>
using namespace std;

#define PIPE_NAME "\\\\.\\pipe\\Tube"

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

string SetPipeError(string msgText, int code) {
	return msgText + GetErrorMsgText(code);
};
int main() {
	setlocale(LC_ALL, "Rus");
	cout << "ClientNP" << endl;
	HANDLE hPipe; 
	DWORD ps;
	try {
		if ((hPipe = CreateFile(
			PIPE_NAME,
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			NULL,
			NULL)) == INVALID_HANDLE_VALUE)
			throw SetPipeError("createfile:", GetLastError());
		int message_amount = 100;
		char obuf[50];
		for (int i = 1; i <= message_amount; i++) {
			string buf = "Hello from ClientNP " + to_string(i);
			if (!WriteFile(hPipe, buf.c_str(), sizeof(buf), &ps, NULL))
				throw SetPipeError("WriteFile: ", GetLastError());
			if (ReadFile(hPipe, obuf, sizeof(obuf), &ps, NULL))
				cout << obuf << endl;
			else
				throw SetPipeError("ReadFile: ", GetLastError());
		}
		CloseHandle(hPipe);
	}
	catch (string ErrorPipeText) {
		cout << endl << ErrorPipeText;
	}
}