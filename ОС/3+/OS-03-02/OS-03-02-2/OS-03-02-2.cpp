#include <iostream>
#include <thread>
#include <chrono>
#include <windows.h>
using namespace std;

int main() {
    setlocale(LC_ALL, "ru");
    DWORD processID = GetCurrentProcessId();
    std::wcout << L"Идентификатор процесса OS-03-02-2: " << processID << std::endl;

    for (int i = 0; i < 125; ++i) {
        std::wcout << L"OS-03-02-2: итерация " << i + 1 << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}