#include <iostream>
#include <thread>
#include <chrono>
#include <windows.h>
using namespace std;

int main() {
    setlocale(LC_ALL, "ru");
    DWORD processID = GetCurrentProcessId();
    wcout << L"Идентификатор процесса OS-03-02-1: " << processID << endl;

    for (int i = 0; i < 50; ++i) {
        wcout << L"OS-03-02-1: итерация " << i + 1 << endl;
        this_thread::sleep_for(chrono::seconds(1));
    }

    return 0;
}