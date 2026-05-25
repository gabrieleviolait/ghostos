; Minimal protected-mode heap with LIFO-safe kfree.

[BITS 32]

HEAP_HEADER_SIZE equ 8
HEAP_MAGIC equ 0x48454150
HEAP_GUARD equ 0x50414548

pm_heap_init:
    mov dword [pm_heap_current], PM_HEAP_START
    mov dword [pm_heap_end], PM_HEAP_START+PM_HEAP_SIZE
    mov dword [pm_heap_used], 0
    mov dword [pm_heap_peak], 0
    mov dword [pm_heap_alloc_count], 0
    mov dword [pm_heap_free_count], 0
    mov dword [pm_heap_bad_free_count], 0
    mov dword [pm_heap_corrupt_count], 0
    ret

kmalloc:
    push ebx
    push edx

    cmp ecx, 0
    je .fail
    add ecx, 15
    jc .fail
    and ecx, 0xFFFFFFF0
    mov eax, [pm_heap_current]
    mov ebx, eax
    add ebx, HEAP_HEADER_SIZE
    jc .fail
    add ebx, ecx
    jc .fail
    add ebx, 4
    jc .fail
    cmp ebx, [pm_heap_end]
    ja .fail

    mov [eax], ecx
    mov dword [eax+4], HEAP_MAGIC
    add eax, HEAP_HEADER_SIZE
    mov edx, eax
    add edx, ecx
    mov dword [edx], HEAP_GUARD
    mov [pm_heap_current], ebx
    add [pm_heap_used], ecx
    inc dword [pm_heap_alloc_count]

    mov ebx, [pm_heap_used]
    cmp ebx, [pm_heap_peak]
    jbe .ok
    mov [pm_heap_peak], ebx

.ok:
    pop edx
    pop ebx
    clc
    ret

.fail:
    xor eax, eax
    pop edx
    pop ebx
    stc
    ret

kfree:
    push ebx
    push edx

    cmp eax, 0
    je .done
    mov ebx, eax
    sub ebx, HEAP_HEADER_SIZE
    cmp ebx, PM_HEAP_START
    jb .bad_free
    cmp ebx, [pm_heap_current]
    jae .bad_free
    cmp dword [ebx+4], HEAP_MAGIC
    jne .bad_free

    mov ecx, [ebx]
    cmp ecx, 0
    je .corrupt
    mov edx, eax
    add edx, ecx
    jc .corrupt
    cmp edx, [pm_heap_current]
    jae .corrupt
    cmp dword [edx], HEAP_GUARD
    jne .corrupt
    inc dword [pm_heap_free_count]
    add edx, 4
    cmp edx, [pm_heap_current]
    jne .mark_free

    mov [pm_heap_current], ebx
    cmp [pm_heap_used], ecx
    jb .used_corrupt
    sub [pm_heap_used], ecx
    jmp .mark_free

.used_corrupt:
    mov dword [pm_heap_used], 0
    inc dword [pm_heap_corrupt_count]

.mark_free:
    mov dword [ebx+4], 0
    jmp .done

.bad_free:
    inc dword [pm_heap_bad_free_count]
    jmp .done

.corrupt:
    inc dword [pm_heap_corrupt_count]

.done:
    pop edx
    pop ebx
    ret

pm_heap_sanity_check:
    mov eax, [pm_heap_current]
    cmp eax, PM_HEAP_START
    jb .bad
    cmp eax, [pm_heap_end]
    ja .bad
    mov eax, [pm_heap_used]
    cmp eax, PM_HEAP_SIZE
    ja .bad
    ret

.bad:
    inc dword [pm_heap_corrupt_count]
    ret

pm_print_heap_info:
    call pm_heap_sanity_check

    mov esi, pm_heap_total_msg
    call pm_print_string
    mov eax, PM_HEAP_SIZE
    call pm_print_uint32
    call pm_newline

    mov esi, pm_heap_used_msg
    call pm_print_string
    mov eax, [pm_heap_used]
    call pm_print_uint32
    call pm_newline

    mov esi, pm_heap_free_msg
    call pm_print_string
    mov eax, [pm_heap_end]
    sub eax, [pm_heap_current]
    call pm_print_uint32
    call pm_newline

    mov esi, pm_heap_peak_msg
    call pm_print_string
    mov eax, [pm_heap_peak]
    call pm_print_uint32
    call pm_newline

    mov esi, pm_heap_allocs_msg
    call pm_print_string
    mov eax, [pm_heap_alloc_count]
    call pm_print_uint32
    mov al, '/'
    call pm_print_char
    mov eax, [pm_heap_free_count]
    call pm_print_uint32
    call pm_newline

    mov esi, pm_heap_bad_msg
    call pm_print_string
    mov eax, [pm_heap_bad_free_count]
    call pm_print_uint32
    mov al, '/'
    call pm_print_char
    mov eax, [pm_heap_corrupt_count]
    call pm_print_uint32
    call pm_newline
    ret

pm_heap_current dd 0
pm_heap_end dd 0
pm_heap_used dd 0
pm_heap_peak dd 0
pm_heap_alloc_count dd 0
pm_heap_free_count dd 0
pm_heap_bad_free_count dd 0
pm_heap_corrupt_count dd 0
pm_heap_total_msg db 'Heap total bytes: ',0
pm_heap_used_msg db 'Heap used bytes: ',0
pm_heap_free_msg db 'Heap free bytes: ',0
pm_heap_peak_msg db 'Heap peak bytes: ',0
pm_heap_allocs_msg db 'Heap alloc/free: ',0
pm_heap_bad_msg db 'Heap bad/corrupt: ',0
