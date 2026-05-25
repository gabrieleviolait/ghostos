; Ghost OS v1.0 kernel
; Enters protected mode and runs a minimal 32-bit shell.

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
    jmp enter_protected_mode_shell

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
