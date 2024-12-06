#include <locale.h>
#include <wchar.h>
#include <Windows.h>
#include <iostream>
#include <fileapi.h>
using namespace std;

#define FILE_PATH L"D:/BSTU/5 sem/ОС/Лабы/9/Windows/OS09-01.txt"
#define READ_BYTES 1000

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
	catch (const char* em) {
		cout << "[ERROR] " << em << endl;
		return false;
	}
}

BOOL insRowFileTxt(LPWSTR fileName, LPWSTR str, DWORD row) {
	char filepath[20];
	size_t convertedChars = 0;
	wcstombs_s(&convertedChars, filepath, 20, fileName, _TRUNCATE);
	char stringToInsert[50];
	size_t convertedChars2 = 0;
	wcstombs_s(&convertedChars2, stringToInsert, 50, str, _TRUNCATE);
	cout << "\n======  Insert row: " << row << "\n\n";
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
		if (!b) {
			cout << "Ошибка при чтении файла.\n";
			CloseHandle(hf);
			return false;
		}
		cout << "ДО:\n\n";
		cout << buf << endl;
		CloseHandle(hf);
		HANDLE hAppend = CreateFile(fileName, GENERIC_WRITE, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		char editedBuf[1024];
		ZeroMemory(editedBuf, sizeof(editedBuf));
		int line = 1;
		int j = 0;
		int k = 0;
		for (int i = 0; i < n; i++) {
			if (line == row) {
				for (int k = 0; k < sizeof(stringToInsert); k++) {
					editedBuf[j] = str[k];
					j++;
					if (str[k + 1] == '\0') {
						editedBuf[j] = '\r';
						j++;
						editedBuf[j] = '\n';
						j++;
						row = 0;
						break;
					}
				}
				i--;
			}
			else {
				editedBuf[j] = buf[i];
				j++;
			}
			if (buf[i] == '\n')
				line++;
			if (buf[i + 1] == '\0' && row == -1) {
				for (int k = 0; k < sizeof(stringToInsert); k++) {
					editedBuf[j] = str[k];
					j++;
					if (str[k + 1] == '\0') {
						editedBuf[j] = '\r';
						j++;
						editedBuf[j] = '\n';
						j++;
						row = 0;
						break;
					}
				}
			}
		}
		b = WriteFile(hAppend, editedBuf, j, &n, NULL);
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
	catch (const char* em)
	{
		cout << em << " \n";
		cout << "==========================================\n";
		return false;
	}
}

int main() {
	setlocale(LC_ALL, "Russian");
	LPWSTR  file = (LPWSTR)FILE_PATH;
	char str[] = "000000000000000000000000000000";
	wchar_t wStr[50];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wStr, 50, str, _TRUNCATE);
	LPWSTR strToIns = wStr;
	insRowFileTxt(file, strToIns, -1);
	insRowFileTxt(file, strToIns, 1);
	insRowFileTxt(file, strToIns, 5);
	insRowFileTxt(file, strToIns, 7);
	printFileText(file);
}