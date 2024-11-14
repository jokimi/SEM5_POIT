#include <windows.h>
#include <conio.h>
#include <string.h>
#include <stdio.h>
#include <locale.h>
#pragma warning(disable : 4996)

#pragma comment(lib, "user32.lib")

#define BUFFER_SIZE 10

int
main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Russian");

    char machineName[80];
    HANDLE hNamedPipe;
    DWORD dwBytesWrite; // для количества записанных байтов
    DWORD dwBytesRead; // для количества прочитанных байтов
    char pchMessage[BUFFER_SIZE]; // для сообщения
    int nMessageLength;  // длина сообщения
    char pipe_name[80];
    char sem_name[80];
    char fullMessage[4096] = { 0 };

    if (argc < 1) {
        fprintf(stderr, "Usage: %s <string> [pipe name] [semaphore name]\n", argv[0]);
        return 1;
    }

    strncpy(fullMessage, argv[1], sizeof(fullMessage) - 1);

    if (argc > 2) {
        strncpy(pipe_name, argv[2], sizeof(pipe_name) - 1);
        pipe_name[sizeof(pipe_name) - 1] = '\0';
    }
    else
        if (!GetEnvironmentVariableA("PIPE", pipe_name, sizeof(pipe_name))) {
            _cprintf("env error");
            return 1;
        }
    if (argc > 3) {
        strncpy(sem_name, argv[3], sizeof(sem_name) - 1);
        sem_name[sizeof(sem_name) - 1] = '\0';
    }
    else {
        if (!GetEnvironmentVariableA("SEMAPHORE", sem_name, sizeof(sem_name))) {
            _cprintf("env error");
            return 1;
        }
        _cprintf("sem name: %s\n", sem_name);
    }

    HANDLE semaphore = OpenSemaphoreA(
        SYNCHRONIZE | SEMAPHORE_MODIFY_STATE, FALSE, sem_name
    );
    if (semaphore == NULL) {
        _cprintf("OpenSemaphore failed: %d\n", GetLastError());
        return 1;
    }
    _cprintf("waiting for semaphore...\n");
    // Ожидаем семафор
    WaitForSingleObject(semaphore, INFINITE);

    // связываемся с именованным каналом
    hNamedPipe =CreateFileA(
            pipe_name,                     // имя канала
            GENERIC_READ | GENERIC_WRITE, // читаем и записываем в канал
            FILE_SHARE_READ | FILE_SHARE_WRITE, // разрешаем чтение и запись
            NULL, // безопасность по умолчанию
            OPEN_EXISTING, // открываем существующий канал
            FILE_ATTRIBUTE_NORMAL, // атрибуты по умолчанию
            NULL); // дополнительных атрибутов нет

    // проверяем связь с каналом
    if (hNamedPipe == INVALID_HANDLE_VALUE) {
        _cprintf("Connection with the named pipe failed.\n");
        _cprintf("The last error code: %d.\n", GetLastError());
        _cprintf("Press any key to exit.\n");
        _getch();

        return 0;
    }

    nMessageLength = strlen(fullMessage);
    int totalBytesSent = 0;

    while (totalBytesSent < nMessageLength) {
        int chunkSize = (nMessageLength - totalBytesSent > BUFFER_SIZE) ? BUFFER_SIZE : nMessageLength - totalBytesSent;
        if (!WriteFile(hNamedPipe, // дескриптор канала
            fullMessage + totalBytesSent, // адрес буфера для вывода данных
            chunkSize, // количество записываемых байтов
            &dwBytesWrite, // количество записанных байтов
            NULL // передача данных синхронная
        )) {
            _cprintf("Write file failed.\n");
            _cprintf("The last error code: %d.\n", GetLastError());
            CloseHandle(hNamedPipe);
            CloseHandle(semaphore);
            _cprintf("Press any key to exit.\n");
            _getch();

            return 0;
        }

        totalBytesSent += chunkSize;
    }
    _cprintf("The client sent the message to a server: \n\t%s\n", fullMessage);

    int totalBytesRead = 0;
    memset(fullMessage, 0, sizeof(fullMessage));

    do {
        // читаем сообщение
        if (!ReadFile(hNamedPipe, // дескриптор канала
            pchMessage, // адрес буфера для ввода данных
            sizeof(pchMessage), // количество читаемых байтов
            &dwBytesRead, // количество прочитанных байтов
            NULL)) // передача данных синхронная
        {
            _cprintf("Data reading from the named pipe failed.\n");
            _cprintf("The last error code: %d.\n", GetLastError());
            CloseHandle(hNamedPipe);
            CloseHandle(semaphore);
            _cprintf("Press any key to exit.\n");
            _getch();

            return 0;
        }

        // Копируем прочитанную часть в итоговое сообщение
        strncat(fullMessage, pchMessage, dwBytesRead);
        totalBytesRead += dwBytesRead;

    } while (dwBytesRead == BUFFER_SIZE);
    _cprintf("The client received the message from a server: \n\t%s\n",
        fullMessage);

    _cprintf("Press any key to exit.\n");
    _getch();

    CloseHandle(hNamedPipe);
    ReleaseSemaphore(semaphore, 1, NULL);
    CloseHandle(semaphore);

    return 0;
}