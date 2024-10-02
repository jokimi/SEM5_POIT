#include <iostream>
#include <thread>
#include <chrono>
#include <windows.h>
using namespace std;

int main() {
    setlocale(LC_ALL, "ru");
    DWORD processID = GetCurrentProcessId();
    cout << "Идентификатор процесса: " << processID << endl;

    while (true) {
        cout << "Процесс работает...\nИдентификатор: " << processID << endl;
        this_thread::sleep_for(chrono::seconds(5));
    }

    return 0;
}