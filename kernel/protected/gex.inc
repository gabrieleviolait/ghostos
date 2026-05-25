; Minimal protected-mode Ghost Executable loader.

[BITS 32]

GEX_MAGIC equ 0x31584547
GEX_HEADER_SIZE equ 32
GEX_LOAD_ADDR equ 0x00180000
GEX_LOAD_LIMIT equ 0x00190000
GEX_MAX_FILE_SIZE equ PAGE_BUFFER_SIZE-1

GEX_SYSCALL_PRINT equ 0
GEX_SYSCALL_READ equ 1
GEX_SYSCALL_EXIT equ 2

pm_gex_run:
    pushad

    mov edi, page_buffer
    mov ecx, GEX_MAX_FILE_SIZE
    call pm_fat12_read_file
    jc pm_gex_fat_error

    mov [pm_gex_file_size], eax
    cmp eax, GEX_HEADER_SIZE
    jb pm_gex_bad_magic

    mov esi, page_buffer
    cmp dword [esi], GEX_MAGIC
    jne pm_gex_bad_magic

    mov eax, [esi+16]
    cmp eax, 0
    jne pm_gex_bad_flags

    mov eax, [esi+8]
    cmp eax, GEX_LOAD_ADDR
    jne pm_gex_bad_load

    mov eax, [esi+12]
    cmp eax, 0
    je pm_gex_too_large
    mov [pm_gex_image_size], eax
    mov ebx, [pm_gex_file_size]
    sub ebx, GEX_HEADER_SIZE
    cmp eax, ebx
    ja pm_gex_too_large

    mov ebx, GEX_LOAD_ADDR
    add ebx, eax
    jc pm_gex_bad_load
    cmp ebx, GEX_LOAD_LIMIT
    ja pm_gex_too_large

    mov ebx, [esi+4]
    cmp ebx, eax
    jae pm_gex_bad_entry
    add ebx, GEX_LOAD_ADDR
    mov [pm_gex_entry], ebx

    push esi
    mov esi, page_buffer+GEX_HEADER_SIZE
    mov edi, GEX_LOAD_ADDR
    mov ecx, [pm_gex_image_size]
    cld
    rep movsb
    pop esi

    mov dword [pm_gex_exit_status], 0
    mov [pm_gex_saved_esp], esp
    mov edx, pm_gex_syscall
    call dword [pm_gex_entry]

pm_gex_after_call:
    popad
    clc
    ret

pm_gex_fat_error:
    call pm_print_fat12_error
    jmp pm_gex_fail

pm_gex_bad_magic:
    mov esi, pm_gex_bad_magic_msg
    jmp pm_gex_print_fail

pm_gex_bad_flags:
    mov esi, pm_gex_bad_flags_msg
    jmp pm_gex_print_fail

pm_gex_bad_load:
    mov esi, pm_gex_bad_load_msg
    jmp pm_gex_print_fail

pm_gex_too_large:
    mov esi, pm_gex_too_large_msg
    jmp pm_gex_print_fail

pm_gex_bad_entry:
    mov esi, pm_gex_bad_entry_msg

pm_gex_print_fail:
    call pm_print_string

pm_gex_fail:
    popad
    stc
    ret

pm_gex_syscall:
    cmp eax, GEX_SYSCALL_PRINT
    je .print
    cmp eax, GEX_SYSCALL_READ
    je .read
    cmp eax, GEX_SYSCALL_EXIT
    je .exit
    ret

.print:
    call pm_print_string
    ret

.read:
    call pm_gex_read_line
    ret

.exit:
    mov [pm_gex_exit_status], ebx
    mov esp, [pm_gex_saved_esp]
    jmp pm_gex_after_call

pm_gex_read_line:
    push ebx
    push ecx
    push edi

    cmp ecx, 2
    jb .empty
    dec ecx
    xor ebx, ebx

.loop:
    call pm_kbd_get_char
    cmp al, 13
    je .done
    cmp al, 8
    je .backspace
    cmp al, 32
    jb .loop
    cmp al, 126
    ja .loop
    cmp ebx, ecx
    jae .loop
    stosb
    inc ebx
    call pm_print_char
    jmp .loop

.backspace:
    cmp ebx, 0
    je .loop
    dec edi
    dec ebx
    mov al, 8
    call pm_print_char
    jmp .loop

.done:
    mov byte [edi], 0
    call pm_newline
    mov eax, ebx
    jmp .return

.empty:
    xor eax, eax

.return:
    pop edi
    pop ecx
    pop ebx
    ret

pm_gex_file_size dd 0
pm_gex_image_size dd 0
pm_gex_entry dd 0
pm_gex_saved_esp dd 0
pm_gex_exit_status dd 0
pm_gex_bad_magic_msg db 'GEX: invalid magic.',13,0
pm_gex_bad_flags_msg db 'GEX: unsupported flags.',13,0
pm_gex_bad_load_msg db 'GEX: invalid load address.',13,0
pm_gex_too_large_msg db 'GEX: image too large.',13,0
pm_gex_bad_entry_msg db 'GEX: entry outside image.',13,0
