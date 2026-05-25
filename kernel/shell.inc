; Command shell loop and dispatch.

shell_loop:
    mov si, prompt
    call print_string

    mov di, cmd_buffer
    call read_line
    call newline

    mov si, cmd_buffer
    call skip_spaces
    cmp byte [si], 0
    je shell_loop

    mov di, cmd_help
    call starts_with_word
    jc do_help

    mov di, cmd_clear
    call starts_with_word
    jc do_clear

    mov di, cmd_cls
    call starts_with_word
    jc do_clear

    mov di, cmd_echo
    call starts_with_word
    jc do_echo

    mov di, cmd_about
    call starts_with_word
    jc do_about

    mov di, cmd_version
    call starts_with_word
    jc do_version

    mov di, cmd_gui
    call starts_with_word
    jc do_gui

    mov di, cmd_browse
    call starts_with_word
    jc do_browse

    mov di, cmd_reboot
    call starts_with_word
    jc do_reboot

    mov di, cmd_halt
    call starts_with_word
    jc do_halt

    mov di, cmd_mem
    call starts_with_word
    jc do_mem

    mov di, cmd_ascii
    call starts_with_word
    jc do_ascii

    mov di, cmd_color
    call starts_with_word
    jc do_color

    mov di, cmd_calc
    call starts_with_word
    jc do_calc

    mov di, cmd_stress
    call starts_with_word
    jc do_stress

    mov di, cmd_flood
    call starts_with_word
    jc do_stress

    mov si, unknown_msg
    call print_string
    jmp shell_loop
