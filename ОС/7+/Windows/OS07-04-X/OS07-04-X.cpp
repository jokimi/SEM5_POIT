#include <iostream>
#include <ctime>
#include <Windows.h>
#include <cmath>

using namespace std;

volatile bool active = true;

DWORD WINAPI stopCycle(HANDLE htimer) {
    WaitForSingleObject(htimer, INFINITE);
    active = false;
    return 0;
}

bool isPrime(int n) {
    if (n <= 1)
        return false;
    if (n == 2)
        return true;
    if (n % 2 == 0)
        return false;
    for (int i = 3; i <= sqrt(n); i += 2)
        if (n % i == 0)
            return false;
    return true;
}

int main(int argc, char* argv[]) {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    HANDLE htimer = NULL;
    int parm = 1;
    int val = 1;
    if (argc > 1)
        parm = atoi(argv[1]);
    cout << "Дочерний процесс " << parm << " запущен" << endl;
    if (parm == 1)
        htimer = OpenWaitableTimer(TIMER_ALL_ACCESS, FALSE, L"os07-04-1");
    else
        htimer = OpenWaitableTimer(TIMER_ALL_ACCESS, FALSE, L"os07-04-2");
    HANDLE hChild = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)stopCycle, htimer, NULL, NULL);
    while (active) {
        if (isPrime(val))
            cout << "Простое число: " << val << endl;
        val++;
    }
    WaitForSingleObject(hChild, INFINITE);
    cout << "Процесс завершен через " << (parm == 1 ? "1 минуту" : "2 минуты") << endl;
    return 0;
}