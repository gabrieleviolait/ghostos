; Minimal protected-mode Ghost Executable loader.

[BITS 32]

GEX_MAGIC equ 0x31584547
GEX_HEADER_SIZE equ 32
GEX_LOAD_ADDR equ 0x00180000
GEX_LOAD_LIMIT equ 0x00190000
GEX_MAX_FILE_SIZE equ PAGE_BUFFER_SIZE-1
GEX_MAX_ARGC equ 8

pm_gex_run:
    pushad
    call pm_gex_parse_args
    mov esi, [pm_gex_filename_ptr]
    cmp esi, 0
    je pm_gex_bad_args

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
    mov eax, [pm_gex_argc]
    mov [pm_gex_metadata+GEX_META_ARGC_OFF], eax
    mov dword [pm_gex_metadata+GEX_META_ARGV_OFF], pm_gex_argv
    mov dword [pm_gex_metadata+GEX_META_SYSCALL_OFF], pm_syscall_dispatch
    mov dword [pm_gex_metadata+GEX_META_VERSION_OFF], pm_sys_version_string
    mov [pm_gex_saved_esp], esp
    mov byte [pm_gex_running], 1
    mov eax, [pm_gex_argc]
    mov ebx, pm_gex_argv
    mov ecx, pm_gex_metadata
    mov ebp, pm_gex_metadata
    mov edx, pm_gex_legacy_syscall
    call dword [pm_gex_entry]

pm_gex_after_call:
    mov byte [pm_gex_running], 0
    call pm_gex_print_exit_status
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
    jmp pm_gex_print_fail

pm_gex_bad_args:
    mov esi, pm_gex_bad_args_msg

pm_gex_print_fail:
    call pm_print_string

pm_gex_fail:
    popad
    stc
    ret

pm_gex_parse_args:
    pushad

    mov dword [pm_gex_argc], 0
    mov dword [pm_gex_filename_ptr], 0
    mov edi, pm_gex_arg_buffer
    mov ecx, CMD_BUFFER_SIZE-1

.copy_loop:
    lodsb
    cmp al, 0
    je .copy_done
    stosb
    dec ecx
    jnz .copy_loop

.copy_done:
    mov byte [edi], 0
    mov esi, pm_gex_arg_buffer
    xor eax, eax

.next_arg:
    cmp byte [esi], ' '
    jne .check_arg
    inc esi
    jmp .next_arg

.check_arg:
    cmp byte [esi], 0
    je .done
    cmp eax, GEX_MAX_ARGC
    jae .done
    mov [pm_gex_argv+eax*4], esi
    cmp eax, 0
    jne .scan_arg
    mov [pm_gex_filename_ptr], esi

.scan_arg:
    inc eax

.scan_loop:
    cmp byte [esi], 0
    je .done
    cmp byte [esi], ' '
    je .terminate
    inc esi
    jmp .scan_loop

.terminate:
    mov byte [esi], 0
    inc esi
    jmp .next_arg

.done:
    mov [pm_gex_argc], eax
    popad
    ret

pm_gex_print_exit_status:
    mov esi, pm_gex_exit_msg
    call pm_print_string
    mov eax, [pm_gex_exit_status]
    call pm_print_uint32
    call pm_newline
    ret

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
pm_gex_running db 0
pm_gex_argc dd 0
pm_gex_filename_ptr dd 0
pm_gex_argv times GEX_MAX_ARGC dd 0
pm_gex_metadata:
    dd GEX_META_MAGIC
    dd GEX_ABI_VERSION
    dd 0
    dd pm_gex_argv
    dd pm_syscall_dispatch
    dd pm_sys_version_string
pm_gex_arg_buffer times CMD_BUFFER_SIZE db 0
pm_gex_bad_magic_msg db 'GEX: invalid magic.',13,0
pm_gex_bad_flags_msg db 'GEX: unsupported flags.',13,0
pm_gex_bad_load_msg db 'GEX: invalid load address.',13,0
pm_gex_too_large_msg db 'GEX: image too large.',13,0
pm_gex_bad_entry_msg db 'GEX: entry outside image.',13,0
pm_gex_bad_args_msg db 'GEX: missing program name.',13,0
pm_gex_exit_msg db 'GEX exit code: ',0
