; Ghost OS v0.5.3 kernel
; Pure NASM 16-bit x86 real mode CLI/TUI OS.

[BITS 16]
[ORG 0x1000]

%include "kernel/config.inc"

kernel_start:
    cli
    cld

    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, STACK_TOP

    sti

    call set_text_mode
    call clear_screen
    call set_cursor_home

    mov si, banner
    call print_string

    call shell_loop

halt_cpu:
    cli
.hang:
    hlt
    jmp .hang

%include "kernel/video_vga.inc"
%include "kernel/string.inc"
%include "kernel/math.inc"
%include "kernel/browser.inc"
%include "kernel/commands.inc"
%include "kernel/shell.inc"
%include "kernel/data.inc"

times KERNEL_SIZE-($-$$) db 0
