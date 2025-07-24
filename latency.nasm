
section .note.GNU-stack noalloc noexec nowrite progbits

section .bss

section .text
    global add_latency
    global mul_latency
    global div_latency
    global call_add_latency
    global getpid_impl
    global getpid_latency
    global do_getpids

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
    push rbx
    push r9
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
    pop r9
    pop rbx
    ret

mul_latency:
    push rbx
    push r9
    mov ebx, 75
serialize
    rdtsc
    mov r9, rax
%rep iters
    imul ebx, ebx, 87
%endrep
serialize
    rdtsc
    sub rax, r9
    mov r8, iters
    cvtsi2ss xmm1, r8
    cvtsi2ss xmm0, rax
    divss xmm0, xmm1
    pop r9
    pop rbx
    ret


do_getpids:
    push rbx
serialize
%rep iters
    mov rax, 0x27
    syscall
%endrep
serialize
    pop rbx
    ret

call_add_latency:
    push rbx
    push r9
    mov ebx, 0
serialize
    rdtsc
    mov r9, rax
    mov rdi, 18
    mov rsi, -84
%rep iters
    call call_add_latency_helper
    mov rdi, rax ; to introduce data dependency.
%endrep
serialize
    rdtsc
    sub rax, r9
    mov r8, iters
    cvtsi2ss xmm1, r8
    cvtsi2ss xmm0, rax
    divss xmm0, xmm1
    pop r9
    pop rbx
    ret


call_add_latency_helper:
    add rdi, rsi
    mov rax, rdi
    ret


div_latency:
    push rbx
    push r9
    push r8
    mov ebx, 75
serialize
    rdtsc
    mov r9, rax
    mov rax, 0
    mov rdx, 0
    mov r8, $89
%rep iters
    div r8
%endrep
serialize
    rdtsc
    sub rax, r9
    mov r8, iters
    cvtsi2ss xmm1, r8
    cvtsi2ss xmm0, rax
    divss xmm0, xmm1
    pop r8
    pop r9
    pop rbx
    ret

getpid_latency:
    push rbx
    push r9
    push r8
    mov ebx, 75
serialize
    rdtsc
    mov r9, rax
%rep iters
    mov rax, 0x27
    syscall
%endrep
serialize
    rdtsc
    sub rax, r9
    mov r8, iters
    cvtsi2ss xmm1, r8
    cvtsi2ss xmm0, rax
    divss xmm0, xmm1
    pop r8
    pop r9
    pop rbx
    ret




getpid_impl:
    mov rax, 0x27
    syscall
    ret


