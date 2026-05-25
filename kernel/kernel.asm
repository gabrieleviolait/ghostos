; Ghost OS v0.6 kernel
; Real-mode shell with VGA TUI, FAT12 support and optional protected mode demo.

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
    mov [boot_drive], dl

    call set_text_mode
    call clear_screen
    call set_cursor_home

    mov si, banner
    call print_string

    call shell_loop

    jmp halt_cpu

halt_cpu:
    cli
.hang:
    hlt
    jmp .hang

%include "kernel/video_vga.inc"
%include "kernel/tui.inc"
%include "kernel/string.inc"
%include "kernel/math.inc"
%include "kernel/fat12.inc"
%include "kernel/browser.inc"
%include "kernel/protected_mode.inc"
%include "kernel/commands.inc"
%include "kernel/shell.inc"
%include "kernel/data.inc"

times KERNEL_SIZE-($-$$) db 0
