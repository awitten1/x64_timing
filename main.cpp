#include "utils.h"
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <sched.h>
#include <err.h>
#include <fcntl.h>

extern "C" float add_latency();
extern "C" float mul_latency();
extern "C" float div_latency();
extern "C" float call_add_latency();
extern "C" int getpid_impl();
extern "C" float getpid_latency();
extern "C" void do_getpids();

#ifndef ITERS
#define ITERS 1000
#endif

void pin_thread(int cpu_id) {
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(cpu_id, &set);
    if (sched_setaffinity(getpid(), sizeof(set), &set) == -1)
        err(EXIT_FAILURE, "sched_setaffinity");
}


void warmup_cpu() {
    auto& now = std::chrono::steady_clock::now;
    auto t1 = now();
    while (now() - t1 < std::chrono::seconds(1));
}

struct FDWrapper {
    FDWrapper(int fd) : fd(fd) {}
    ~FDWrapper() {
        close(fd);
    }

    operator int() {
        return fd;
    }
    int fd;
};

uint64_t rdmsr0(uint64_t r) {
    static FDWrapper fd = open("/dev/cpu/0/msr", O_RDONLY);
    uint64_t res;
    int ret = pread(fd, &res, 8, r);
    if (ret == -1) {
        err(EXIT_FAILURE, "failed reading msr");
    }
    std::cout << res << std::endl;
    return res;
}

int main() {
    pin_thread(0);
    warmup_cpu();

    do_getpids();
    uint64_t c1 = rdmsr0(0xC00000E8);
    do_getpids();
    uint64_t c2 = rdmsr0(0xC00000E8);
    std::cout << ((c2 - c1) / (double)(ITERS*10)) << std::endl;;

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