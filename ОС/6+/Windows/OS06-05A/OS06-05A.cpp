﻿#include <iostream>
#include <Windows.h>

int main() {
	int pid = GetCurrentProcessId();
	HANDLE event = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"OS06-05");
	WaitForSingleObject(event, INFINITE);
	SetEvent(event);
	for (int i = 1; i <= 90; i++) {
		printf("[OS06-05A]\t %d.  PID = %d\n", i, pid);
		Sleep(100);
	}
	CloseHandle(event);
}