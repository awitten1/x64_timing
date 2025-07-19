
section .note.GNU-stack noalloc noexec nowrite progbits

section .bss

section .text
    global add_latency

%ifndef iters
    %define iters 100000
%endif

%macro serialize 0
       xor     eax, eax
       cpuid
%endmacro

;
; To measure latency, use a chain of instructions
; where each instruction depends on the previous instruction.
;
add_latency:
    mov ebx, 0
serialize
    rdtsc
    mov r9, rax
%rep iters
    add ebx, 5
%endrep
serialize
    rdtsc
    sub rax, r9
    mov r8, iters
    cvtsi2ss xmm1, r8
    cvtsi2ss xmm0, rax
    divss xmm0, xmm1

