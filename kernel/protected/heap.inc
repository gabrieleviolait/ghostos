; Minimal bump heap for protected-mode kernel allocations.

[BITS 32]

pm_heap_init:
    mov dword [pm_heap_current], PM_HEAP_START
    mov dword [pm_heap_end], PM_HEAP_START+PM_HEAP_SIZE
    ret

kmalloc:
    push ebx

    add ecx, 15
    and ecx, 0xFFFFFFF0
    mov eax, [pm_heap_current]
    mov ebx, eax
    add ebx, ecx
    cmp ebx, [pm_heap_end]
    ja .fail
    mov [pm_heap_current], ebx
    pop ebx
    clc
    ret

.fail:
    xor eax, eax
    pop ebx
    stc
    ret

kfree:
    ret

pm_heap_current dd 0
pm_heap_end dd 0
