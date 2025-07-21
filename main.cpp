#include "utils.h"
#include <chrono>
#include <cstdio>
#include <iostream>
#include <unistd.h>

extern "C" float add_latency();
extern "C" float mul_latency();
extern "C" float div_latency();
extern "C" float call_add_latency();
extern "C" int getpid_impl();
extern "C" float getpid_latency();


void warmup_cpu() {
    auto& now = std::chrono::steady_clock::now;
    auto t1 = now();
    while (now() - t1 < std::chrono::seconds(1));
}

int main() {
    warmup_cpu();
    add_latency(); add_latency();
    std::cout << "add latency " << add_latency() << std::endl;
    mul_latency(); mul_latency();
    std::cout << "mul latency " << mul_latency() << std::endl;
    div_latency(); div_latency();
    std::cout << "div latency " << div_latency() << std::endl;
    call_add_latency();
    std::cout << "call add latency " << call_add_latency() << std::endl;

    if (getpid_impl() != getpid()) {
        fprintf(stderr, "getpid_impl is wrong");
    }

    getpid_latency();
    std::cout << "get pid latency " << getpid_latency() << std::endl;

    return 0;
}