#include <wchar.h>
#include <iostream>
#include <Windows.h>
using namespace std;

#define FILE_PATH L"D:/BSTU/5 sem/ОС/Лабы/9/Windows/OS09-01.txt"
#define READ_BYTES 500

BOOL printFileText(LPWSTR fileName) {
	try {
		cout << "\nРЕЗУЛЬТАТ:\n\n";
		HANDLE hf = CreateFile(fileName, GENERIC_READ, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hf == INVALID_HANDLE_VALUE)
			throw "[ERROR] Ошибка при создании или открытии файла.";
		DWORD n = NULL;
		char buf[1024];
		ZeroMemory(buf, sizeof(buf));
		BOOL b = ReadFile(hf, &buf, READ_BYTES, &n, NULL);
		if (!b)
			throw "[ERROR] Функция ReadFile вызвала исключение.";
		cout << buf << endl;
		CloseHandle(hf);
		return true;
	}
	catch (const char* em){
		cout << "[ERROR] " << em << endl;
		return false;
	}
}

BOOL delRowFileTxt(LPWSTR fileName, DWORD row) {
	char filepath[20];
	size_t convertedChars = 0;
	wcstombs_s(&convertedChars, filepath, 20, fileName, _TRUNCATE);
	cout << "\n=========== Удаление строки: " << row << " ===========\n\n";
	try {
		HANDLE hf = CreateFile(fileName, GENERIC_READ, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hf == INVALID_HANDLE_VALUE) {
			CloseHandle(hf);
			throw "[ERROR] Ошибка при создании или открытии файла.";
		}
		DWORD n = NULL;
		char buf[1024];
		BOOL b;
		ZeroMemory(buf, sizeof(buf));
		b = ReadFile(hf, &buf, sizeof(buf), &n, NULL);
		if (!b) {
			CloseHandle(hf);
			throw ("[ERROR] Функция ReadFile вызвала исключение.");
		}
		cout << "ДО:\n\n";
		cout << buf << endl;
		CloseHandle(hf);
		HANDLE hAppend = CreateFile(fileName, GENERIC_WRITE, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		char editedBuf[1024];
		ZeroMemory(editedBuf, sizeof(editedBuf));
		int line = 1;
		int j = 0;
		bool rowFound = false;
		for (int i = 0; i < n; i++) {
			if (line == row)
				rowFound = true;
			else {
				editedBuf[j] = buf[i];
				j++;
			}
			if (buf[i] == '\n')
				line++;
		}
		if (!rowFound) {
			CloseHandle(hAppend);
			throw ("[ERROR] Строка не найдена.\n");
		}
		b = WriteFile(hAppend, editedBuf, n, &n, NULL);
		if (!b) {
			CloseHandle(hAppend);
			throw ("[ERROR] Функция WriteFile вызвала исключение.\n");
		}
		cout << "ПОСЛЕ:\n\n";
		cout << editedBuf << endl;
		CloseHandle(hAppend);
		cout << "\n==========================================\n";
		return true;
	}
	catch (const char* em) {
		cout << em << " \n";
		cout << "==========================================\n";
		return false;
	}
}

int main() {
	setlocale(LC_ALL, "Russian");
	LPWSTR file = (LPWSTR)FILE_PATH;
	delRowFileTxt(file, 1);
	delRowFileTxt(file, 3);
	delRowFileTxt(file, 8);
	delRowFileTxt(file, 10);
	printFileText(file);
	return 0;
}