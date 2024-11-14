#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <locale.h>
#pragma warning(disable : 4996)

#define BUFFER_SIZE 10

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Russian");

    char pipe_name[80] = "\\\\.\\pipe\\MyPipe";
    char semaphore_name[80] = "MySemaphore";

    if (!GetEnvironmentVariableA("PIPE", pipe_name, sizeof(pipe_name))) {
        _cprintf("Using default pipe name: %s\n", pipe_name);
    }

    if (!GetEnvironmentVariableA("SEMAPHORE", semaphore_name, sizeof(semaphore_name))) {
        _cprintf("Using default semaphore name: %s\n", semaphore_name);
    }

    HANDLE semaphore = CreateSemaphoreA(NULL, 1, 1, semaphore_name);
    if (semaphore == NULL) {
        printf("CreateSemaphore failed: %d\n", GetLastError());
        return 1;
    }

    HANDLE hNamedPipe;
    DWORD dwBytesRead;
    DWORD dwBytesWrite;
    char pchMessage[BUFFER_SIZE];
    int nMessageLength;

    _cprintf("Creating named pipe with name: %s\n", pipe_name);

    hNamedPipe = CreateNamedPipeA(
        pipe_name,
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_MESSAGE | PIPE_WAIT,
        1,
        BUFFER_SIZE,
        BUFFER_SIZE,
        INFINITE,
        NULL
    );

    if (hNamedPipe == INVALID_HANDLE_VALUE) {
        _cprintf("Create named pipe failed.\n");
        _cprintf("The last error code: %d.\n", GetLastError());
        return 0;
    }

    for (;;) {
        _cprintf("The server is waiting for connection with a client...\n");
        if (!ConnectNamedPipe(hNamedPipe, NULL)) {
            _cprintf("Connect named pipe failed.\n");
            _cprintf("The last error code: %d.\n", GetLastError());
            CloseHandle(hNamedPipe);
            return 0;
        }

        char fullMessage[4096] = { 0 };
        int totalBytesRead = 0;

        do {
            if (!ReadFile(hNamedPipe, pchMessage, sizeof(pchMessage), &dwBytesRead, NULL)) {
                _cprintf("Data reading from the named pipe failed.\n");
                _cprintf("The last error code: %d.\n", GetLastError());
                CloseHandle(hNamedPipe);
                _cprintf("Press any key to exit.\n");
                _getch();
                return 0;
            }

            strncat(fullMessage, pchMessage, dwBytesRead);
            totalBytesRead += dwBytesRead;

        } while (dwBytesRead == BUFFER_SIZE);

        _cprintf("The server received the message from a client: \n\t%s\n", fullMessage);

        for (int i = 0; fullMessage[i] != '\0'; i++) {
            fullMessage[i] = toupper(fullMessage[i]);
        }

        _cprintf("Formatted message: \n\t%s\n", fullMessage);

        nMessageLength = strlen(fullMessage);
        int totalBytesSent = 0;

        while (totalBytesSent < nMessageLength) {
            int chunkSize = (nMessageLength - totalBytesSent > BUFFER_SIZE) ? BUFFER_SIZE : nMessageLength - totalBytesSent;
            if (!WriteFile(hNamedPipe, fullMessage + totalBytesSent, chunkSize, &dwBytesWrite, NULL)) {
                _cprintf("Write file failed.\n");
                _cprintf("The last error code: %d.\n", GetLastError());
                CloseHandle(hNamedPipe);
                _cprintf("Press any key to exit.\n");
                _getch();
                return 0;
            }

            totalBytesSent += chunkSize;
        }

        DisconnectNamedPipe(hNamedPipe);
    }

    CloseHandle(hNamedPipe);
    CloseHandle(semaphore);

    return 0;
}