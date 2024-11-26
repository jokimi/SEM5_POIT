#include <windows.h>
#include <iostream>
#include <vector>
#include <string>

std::string GetMailslotErrorMsgText(int code) {
    std::string msgText;
    switch (code) {
        case ERROR_FILE_NOT_FOUND:
            msgText = "Mailslot not found";
        break;
        case ERROR_ACCESS_DENIED:
            msgText = "Access to Mailslot denied";
        break;
        case ERROR_INVALID_HANDLE:
            msgText = "Invalid handle for Mailslot";
        break;
        case ERROR_ALREADY_EXISTS:
            msgText = "Mailslot already exists";
        break;
        case ERROR_INVALID_PARAMETER:
            msgText = "Invalid parameter";
        break;
        case ERROR_IO_PENDING:
            msgText = "I/O operation is in progress";
        break;
        case ERROR_INSUFFICIENT_BUFFER:
            msgText = "Insufficient buffer for Mailslot message";
        break;
        case ERROR_MORE_DATA:
            msgText = "More data available for Mailslot message";
        break;
        case ERROR_BAD_NETPATH:
            msgText = "Bad network path for Mailslot";
        break;
        case ERROR_NETNAME_DELETED:
            msgText = "Network name for Mailslot deleted";
        break;
        case ERROR_SEM_TIMEOUT:
            msgText = "Mailslot timed out waiting for response";
        break;
        case ERROR_OPERATION_ABORTED:
            msgText = "Mailslot operation aborted";
        break;
        case ERROR_NO_SYSTEM_RESOURCES:
            msgText = "No system resources available for Mailslot";
        break;
        case ERROR_NOT_CONNECTED:
            msgText = "Mailslot server is not connected";
        break;
        case ERROR_DISK_FULL:
            msgText = "Disk is full, cannot write to Mailslot";
        break;
        default:
            msgText = "Unknown Mailslot error";
        break;
    }
    return msgText;
}

std::string SetMailSlotError(std::string msgText, int code) {
    return msgText + ": " + GetMailslotErrorMsgText(code);
}

int main() {

    std::vector<std::string> serverNames = {
        "\\\\DESKTOP-4TISIR1\\mailslot\\Box",
        "\\\\.\\mailslot\\Box2",
        "\\\\DESKTOP-G64NRT0\\mailslot\\Box3"
    };

    DWORD writeTimeout = 5000;

    std::string message = "Hello from Maislot-client (for 3 servers)";

    try {
        for (const auto& serverName : serverNames) {
            LPCSTR slotName = serverName.c_str();

            HANDLE hMailslot = CreateFile(slotName, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
            if (hMailslot == INVALID_HANDLE_VALUE) {
                std::cerr << "Mailslot " << serverName << " is not available. Error: " << GetLastError() << std::endl;
                continue;
            }

            OVERLAPPED overlapped = { 0 };
            DWORD bytesWritten;
            BOOL success = WriteFile(hMailslot, message.c_str(), message.size() + 1, &bytesWritten, NULL);

            if (!success && GetLastError() == ERROR_IO_PENDING) {
                DWORD waitResult = WaitForSingleObject(overlapped.hEvent, writeTimeout);
                if (waitResult == WAIT_TIMEOUT) {
                    std::cerr << "WriteFile timed out on " << serverName << std::endl;
                    CancelIo(hMailslot);
                } else if (waitResult == WAIT_OBJECT_0) {
                    std::cout << "Message sent to " << serverName << std::endl;
                } else {
                    std::cerr << "Error in WriteFile operation on " << serverName << ": " << GetLastError() << std::endl;
                }
            } else if (success) {
                std::cout << "Message sent to " << serverName << std::endl;
            } else {
                std::cerr << "Error sending message to " << serverName << ": " << GetLastError() << std::endl;
            }
            CloseHandle(hMailslot);
        }
    }
    catch (std::string ErrorMailSlotText)
    {
        std::cout << std::endl << ErrorMailSlotText;
    }

    return 0;
}
