




inline void serialize() {
    asm volatile("xor %%eax, %%eax; cpuid "
        ::: "%eax","%ebx","%ecx","%edx" );
}


static inline int rdtsc() {
    int r;
    asm volatile("rdtsc" : "=a"(r) : : "%edx");
    return r;
}


