#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <locale.h>
#include <stdbool.h>

int main(int argc, char* argv[]) {
	char* locale = setlocale(LC_ALL, "Russian");
	char c;
	int rowNumber = 0;
	int in = open("/mnt/d/BSTU/5 sem/ОС/Лабы/9/Linux/OS09-05.txt", O_RDONLY);
	int out;
	int num;
	bool yaBool = 0;
	if (argc != 2) {
		printf("Введите целое число!\n");
		exit(1);
	}
	num = atoi(argv[1]);
	if (num % 2 != 0)
		out = open("./OS09-06-1.txt", O_CREAT | O_WRONLY, 0777);
	else {
		out = open("./OS09-06-2.txt", O_CREAT | O_WRONLY, 0777);
		yaBool = 1;
	}
	while (read(in, &c, 1) == 1) {
		if (!yaBool)
			write(out, &c, 1);
		if (c == '\n')
			yaBool = !yaBool;
	}
	printf("Данные записаны в файл.\n");
	close(out);
	close(in);
	return 0;
}