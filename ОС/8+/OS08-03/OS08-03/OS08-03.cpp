#include <iostream>
#include <thread>
#include <chrono>
#include <unistd.h>

int global_var = 0;
int uninit_global_var;

void long_loop() {
    for (long long i = 0; i < 1e10; ++i) {
        if (i % 100000000 == 0) {
            global_var++;
            std::cout << "Iteration: " << i << ", global_var: " << global_var << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}

int main() {
    std::cout << "OS08_03: Длинный цикл. PID = " << getpid() << "\n";
    long_loop();
    return 0;
}