; Shell command implementations.

do_help:
    mov si, help_msg
    call print_string
    jmp shell_loop

do_clear:
    call clear_screen
    call set_cursor_home
    jmp shell_loop

do_gui:
    call clear_screen
    call set_cursor_home
    mov si, gui_msg
    call print_string
    jmp shell_loop

do_about:
    mov si, about_msg
    call print_string
    jmp shell_loop

do_version:
    mov si, version_msg
    call print_string
    jmp shell_loop

do_echo:
    mov si, cmd_buffer
    call skip_first_word
    call skip_spaces
    call print_string
    call newline
    jmp shell_loop

do_reboot:
    mov si, reboot_msg
    call print_string
    int 0x19
    jmp halt_cpu

do_halt:
    mov si, halt_msg
    call print_string
    jmp halt_cpu

do_mem:
    int 0x12
    mov si, mem_msg
    call print_string
    call print_uint16
    mov si, kb_msg
    call print_string
    jmp shell_loop

do_ascii:
    mov si, ascii_msg
    call print_string
    jmp shell_loop

do_color:
    mov si, cmd_buffer
    call skip_first_word
    call skip_spaces
    mov al, [si]
    call hex_to_nibble
    jc .usage
    mov [text_attr], al
    mov si, color_ok
    call print_string
    jmp shell_loop
.usage:
    mov si, color_usage
    call print_string
    jmp shell_loop

do_calc:
    mov si, cmd_buffer
    call skip_first_word
    call skip_spaces

    mov al, [si]
    cmp al, '0'
    jb .usage
    cmp al, '9'
    ja .usage
    sub al, '0'
    mov bl, al

    inc si
    mov al, [si]
    cmp al, '+'
    je .plus
    cmp al, '-'
    je .minus
    jmp .usage

.plus:
    inc si
    mov al, [si]
    cmp al, '0'
    jb .usage
    cmp al, '9'
    ja .usage
    sub al, '0'
    add bl, al
    xor ax, ax
    mov al, bl
    mov si, result_msg
    call print_string
    call print_uint16
    call newline
    jmp shell_loop

.minus:
    inc si
    mov al, [si]
    cmp al, '0'
    jb .usage
    cmp al, '9'
    ja .usage
    sub al, '0'
    cmp bl, al
    jb .negative
    sub bl, al
    xor ax, ax
    mov al, bl
    mov si, result_msg
    call print_string
    call print_uint16
    call newline
    jmp shell_loop

.negative:
    mov si, negative_msg
    call print_string
    jmp shell_loop

.usage:
    mov si, calc_usage
    call print_string
    jmp shell_loop
