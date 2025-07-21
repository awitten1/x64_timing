#include "utils.h"
#include <iostream>

extern "C" float add_latency();
extern "C" float mul_latency();
extern "C" float div_latency();

void warmup_cpu() {
    char c;
    volatile char* x = &c;
    for (int i = 0; i < 0x2710; ++i)
        *x = 0x63;
}

int main() {
    warmup_cpu();
    add_latency(); add_latency();
    std::cout << "add latency " << add_latency() << std::endl;
    mul_latency(); mul_latency();
    std::cout << "mul latency " << mul_latency() << std::endl;
    div_latency(); div_latency();
    std::cout << "div latency " << div_latency() << std::endl;

    return 0;
}