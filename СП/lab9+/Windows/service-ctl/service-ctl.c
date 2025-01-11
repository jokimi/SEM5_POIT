#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void MyCreateService(const char* serviceName, const char* executablePath) {
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
    if (!hSCManager) {
        printf("Failed to open service manager. Error: %lu\n", GetLastError());
        return;
    }

    SC_HANDLE hService = CreateServiceA(
        hSCManager,
        serviceName,
        serviceName,
        SERVICE_ALL_ACCESS,
        SERVICE_WIN32_OWN_PROCESS,
        SERVICE_AUTO_START,
        SERVICE_ERROR_NORMAL,
        executablePath,
        NULL, NULL, NULL, NULL, NULL);

    if (!hService) {
        printf("Failed to create service. Error: %lu\n", GetLastError());
        CloseServiceHandle(hSCManager);
        return;
    }

    printf("Service '%s' created successfully.\n", serviceName);

    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);
}

void MyDeleteService(const char* serviceName) {
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (!hSCManager) {
        printf("Failed to open service manager. Error: %lu\n", GetLastError());
        return;
    }

    SC_HANDLE hService = OpenServiceA(hSCManager, serviceName, DELETE);
    if (!hService) {
        printf("Failed to open service. Error: %lu\n", GetLastError());
        CloseServiceHandle(hSCManager);
        return;
    }

    if (!DeleteService(hService)) {
        printf("Failed to delete service. Error: %lu\n", GetLastError());
    }
    else {
        printf("Service '%s' deleted successfully.\n", serviceName);
    }

    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);
}

void MyStartService(const char* serviceName, const char* logDir, const char* watchDir) {
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (!hSCManager) {
        printf("Failed to open service manager. Error: %lu\n", GetLastError());
        return;
    }

    SC_HANDLE hService = OpenServiceA(hSCManager, serviceName, SERVICE_START | SERVICE_QUERY_STATUS);
    if (!hService) {
        printf("Failed to open service. Error: %lu\n", GetLastError());
        CloseServiceHandle(hSCManager);
        return;
    }

    const char* args[] = { logDir, watchDir };
    if (!StartServiceA(hService, 2, args)) {
        printf("Failed to start service. Error: %lu\n", GetLastError());
    }
    else {
        printf("Service '%s' started successfully.\n", serviceName);
    }

    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);
}

void MyStopService(const char* serviceName) {
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (!hSCManager) {
        printf("Failed to open service manager. Error: %lu\n", GetLastError());
        return;
    }

    SC_HANDLE hService = OpenServiceA(hSCManager, serviceName, SERVICE_STOP | SERVICE_QUERY_STATUS);
    if (!hService) {
        printf("Failed to open service. Error: %lu\n", GetLastError());
        CloseServiceHandle(hSCManager);
        return;
    }

    SERVICE_STATUS serviceStatus;
    if (!ControlService(hService, SERVICE_CONTROL_STOP, &serviceStatus)) {
        printf("Failed to stop service. Error: %lu\n", GetLastError());
    }
    else {
        printf("Service '%s' stopped successfully.\n", serviceName);
    }

    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);
}

void MyPauseService(const char* serviceName) {
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (!hSCManager) {
        printf("Failed to open service manager. Error: %lu\n", GetLastError());
        return;
    }

    SC_HANDLE hService = OpenServiceA(hSCManager, serviceName, SERVICE_PAUSE_CONTINUE | SERVICE_QUERY_STATUS);
    if (!hService) {
        printf("Failed to open service. Error: %lu\n", GetLastError());
        CloseServiceHandle(hSCManager);
        return;
    }

    SERVICE_STATUS serviceStatus;
    if (!ControlService(hService, SERVICE_CONTROL_PAUSE, &serviceStatus)) {
        printf("Failed to pause service. Error: %lu\n", GetLastError());
    }
    else {
        printf("Service '%s' paused successfully.\n", serviceName);
    }

    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);
}

void MyContinueService(const char* serviceName) {
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (!hSCManager) {
        printf("Failed to open service manager. Error: %lu\n", GetLastError());
        return;
    }

    SC_HANDLE hService = OpenServiceA(hSCManager, serviceName, SERVICE_PAUSE_CONTINUE | SERVICE_QUERY_STATUS);
    if (!hService) {
        printf("Failed to open service. Error: %lu\n", GetLastError());
        CloseServiceHandle(hSCManager);
        return;
    }

    SERVICE_STATUS serviceStatus;
    if (!ControlService(hService, SERVICE_CONTROL_CONTINUE, &serviceStatus)) {
        printf("Failed to continue service. Error: %lu\n", GetLastError());
    }
    else {
        printf("Service '%s' resumed successfully.\n", serviceName);
    }

    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);
}

void MyServiceInfo(const char* serviceName) {
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (!hSCManager) {
        printf("Failed to open service manager. Error: %lu\n", GetLastError());
        return;
    }

    SC_HANDLE hService = OpenServiceA(hSCManager, serviceName, SERVICE_QUERY_STATUS);
    if (!hService) {
        printf("Failed to open service. Error: %lu\n", GetLastError());
        CloseServiceHandle(hSCManager);
        return;
    }

    SERVICE_STATUS_PROCESS status;
    DWORD bytesNeeded;
    if (!QueryServiceStatusEx(hService, SC_STATUS_PROCESS_INFO, (LPBYTE)&status, sizeof(status), &bytesNeeded)) {
        printf("Failed to query service status. Error: %lu\n", GetLastError());
    }
    else {
        printf("Service '%s' status:\n", serviceName);
        printf("  State: %lu\n", status.dwCurrentState);
        printf("  Process ID: %lu\n", status.dwProcessId);
    }

    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: service-ctl <service name> <operation> [parameters]\n");
        return 1;
    }

    const char* serviceName = argv[1];
    const char* operation = argv[2];

    if (strcmp(operation, "create") == 0 && argc == 4) {
        MyCreateService(serviceName, argv[3]);
    }
    else if (strcmp(operation, "start") == 0 && argc == 5) {
        MyStartService(serviceName, argv[3], argv[4]);
    }
    else if (strcmp(operation, "stop") == 0) {
        MyStopService(serviceName);
    }
    else if (strcmp(operation, "delete") == 0) {
        MyDeleteService(serviceName);
    }
    else if (strcmp(operation, "info") == 0) {
        MyServiceInfo(serviceName);
    }
    else if (strcmp(operation, "pause") == 0) {
        MyPauseService(serviceName);
    }
    else if (strcmp(operation, "continue") == 0) {
        MyContinueService(serviceName);
    }
    else {
        printf("Invalid operation or missing parameters.\n");
        printf("Supported operations: Create, Start, Stop, Delete, Info, Pause, Continue\n");
    }

    return 0;
}