#include <iostream>
#include <thread>
#include <sched.h>
#include <unistd.h>
#include <vector>

int main() {
    // Получение идентификатора текущего процесса
    pid_t process_id = getpid();

    // Получение идентификатора текущего потока
    std::thread::id thread_id = std::this_thread::get_id();

    // Получение приоритета (nice) текущего потока
    int nice_value = nice(0); // 0 - для получения текущего значения nice

    // Получение списка доступных процессоров
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    sched_getaffinity(0, sizeof(cpu_set_t), &cpuset);

    std::vector<int> available_cpus;
    for (int i = 0; i < CPU_SETSIZE; ++i) {
        if (CPU_ISSET(i, &cpuset)) {
            available_cpus.push_back(i);
        }
    }

    // Вывод информации
    std::cout << "Идентификатор текущего процесса: " << process_id << std::endl;
    std::cout << "Идентификатор текущего (main) потока: " << thread_id << std::endl;
    std::cout << "Приоритет (nice) текущего потока: " << nice_value << std::endl;
    std::cout << "Номера доступных процессоров: ";
    for (const int& cpu : available_cpus) {
        std::cout << cpu << " ";
    }
    std::cout << std::endl;

    return 0;
}