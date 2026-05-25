; Ghost OS v1.0-beta kernel
; Real-mode recovery shell plus protected-mode ghost32 runtime.

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

    call collect_e820_memory_map

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
%include "kernel/protected/gdt.inc"
%include "kernel/protected/idt.inc"
%include "kernel/protected/irq.inc"
%include "kernel/protected/paging.inc"
%include "kernel/protected/memory.inc"
%include "kernel/protected/heap.inc"
%include "kernel/protected/tasks.inc"
%include "kernel/protected/keyboard.inc"
%include "kernel/protected/vga32.inc"
%include "kernel/protected/shell32.inc"
[BITS 16]
%include "kernel/commands.inc"
%include "kernel/shell.inc"
%include "kernel/data.inc"

times KERNEL_SIZE-($-$$) db 0
