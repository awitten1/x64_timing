#include "utils.h"
#include <iostream>

extern "C" float add_latency();

int main() {
    add_latency(); add_latency();
    std::cout << add_latency() << std::endl;
    return 0;
}