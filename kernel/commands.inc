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
    call render_desktop
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

do_stress:
    mov si, stress_header
    call print_string

    xor bx, bx

.loop:
    inc bx
    mov si, stress_line_prefix
    call print_string
    mov ax, bx
    call print_uint16
    mov si, stress_line_suffix
    call print_string

    cmp bx, 40
    jb .loop

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

    call parse_uint16
    jc .usage
    push ax

    call skip_spaces

    mov dl, [si]
    cmp dl, '+'
    je .operator_ok
    cmp dl, '-'
    je .operator_ok
    cmp dl, '*'
    je .operator_ok
    cmp dl, '/'
    je .operator_ok
    jmp .usage_drop_left

.operator_ok:
    inc si
    call skip_spaces
    call parse_uint16
    jc .usage_drop_left

    mov cx, ax
    call skip_spaces
    cmp byte [si], 0
    jne .usage_drop_left

    pop ax

    cmp dl, '+'
    je .plus
    cmp dl, '-'
    je .minus
    cmp dl, '*'
    je .multiply
    cmp dl, '/'
    je .divide
    jmp .usage

.plus:
    add ax, cx
    jc .overflow
    jmp .print_result

.minus:
    cmp ax, cx
    jb .negative
    sub ax, cx
    jmp .print_result

.multiply:
    mul cx
    cmp dx, 0
    jne .overflow
    jmp .print_result

.divide:
    cmp cx, 0
    je .divide_by_zero
    xor dx, dx
    div cx
    jmp .print_result

.print_result:
    mov si, result_msg
    call print_string
    call print_uint16
    call newline
    jmp shell_loop

.negative:
    mov si, negative_msg
    call print_string
    jmp shell_loop

.divide_by_zero:
    mov si, div_zero_msg
    call print_string
    jmp shell_loop

.overflow:
    mov si, overflow_msg
    call print_string
    jmp shell_loop

.usage_drop_left:
    pop ax

.usage:
    mov si, calc_usage
    call print_string
    jmp shell_loop
