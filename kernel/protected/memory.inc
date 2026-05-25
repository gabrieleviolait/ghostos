; BIOS E820 collection and simple physical frame bitmap.

[BITS 16]

collect_e820_memory_map:
    pusha
    push es

    mov word [e820_count], 0
    xor ax, ax
    mov es, ax
    xor ebx, ebx
    xor bp, bp
    mov di, e820_map

.next:
    mov eax, 0xE820
    mov edx, 0x534D4150
    mov ecx, E820_ENTRY_SIZE
    int 0x15
    jc .done
    cmp eax, 0x534D4150
    jne .done
    cmp ecx, 20
    jb .done

    inc bp
    add di, E820_ENTRY_SIZE
    cmp bp, E820_MAX_ENTRIES
    jae .done
    cmp ebx, 0
    jne .next

.done:
    mov [e820_count], bp
    pop es
    popa
    ret

[BITS 32]

pm_memory_init:
    call pm_detect_memory_mb
    call pm_page_alloc_init
    call pm_init_paging
    call pm_heap_init
    ret

pm_detect_memory_mb:
    pushad

    mov dword [pm_detected_memory_mb], PM_MAX_MEMORY_MB
    cmp word [e820_count], 0
    je .done

    xor edx, edx
    movzx ecx, word [e820_count]
    mov esi, e820_map

.loop:
    cmp dword [esi+16], 1
    jne .next
    cmp dword [esi+4], 0
    jne .next
    cmp dword [esi+12], 0
    jne .large

    mov eax, [esi]
    add eax, [esi+8]
    jc .large
    cmp eax, edx
    jbe .next
    mov edx, eax
    jmp .next

.large:
    mov edx, PM_MAX_MEMORY_MB*1024*1024

.next:
    add esi, E820_ENTRY_SIZE
    loop .loop

    cmp edx, 0
    je .done
    shr edx, 20
    cmp edx, PM_MAX_MEMORY_MB
    jbe .store
    mov edx, PM_MAX_MEMORY_MB

.store:
    mov [pm_detected_memory_mb], edx

.done:
    popad
    ret

pm_print_memmap:
    cmp word [e820_count], 0
    jne .have_map
    mov esi, pm_memmap_none_msg
    call pm_print_string
    ret

.have_map:
    pushad
    mov esi, pm_memmap_header
    call pm_print_string
    movzx ecx, word [e820_count]
    mov esi, e820_map

.loop:
    mov eax, [esi]
    call pm_print_hex32
    mov al, ' '
    call pm_print_char
    mov eax, [esi+8]
    call pm_print_hex32
    mov al, ' '
    call pm_print_char
    mov eax, [esi+16]
    call pm_print_uint32
    call pm_newline
    add esi, E820_ENTRY_SIZE
    loop .loop
    popad
    ret

pm_detected_memory_mb dd PM_MAX_MEMORY_MB
e820_count dw 0
e820_map times E820_MAX_ENTRIES*E820_ENTRY_SIZE db 0
pm_memmap_none_msg db 'No E820 memory map available.',13,0
pm_memmap_header db 'Base       Length     Type',13,0
