; Protected-mode GDT, mode switch, and 32-bit entry point.

[BITS 16]

CODE_SEL equ gdt_code - gdt_start
DATA_SEL equ gdt_data - gdt_start
PM_STACK_TOP equ 0x90000

enter_protected_mode_demo:
enter_protected_mode_shell:
    cli
    lgdt [gdt_descriptor]

    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp dword CODE_SEL:protected_mode_start

align 8
gdt_start:
gdt_null:
    dq 0

gdt_code:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10011010b
    db 11001111b
    db 0x00

gdt_data:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10010010b
    db 11001111b
    db 0x00

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

[BITS 32]

protected_mode_start:
    cld
    mov ax, DATA_SEL
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, PM_STACK_TOP

    call pm_idt_init
    lidt [idt_descriptor]
    call pm_pic_remap
    call pm_kbd_reset
    call pm_memory_init
    call pm_tasks_init

    sti

    call pm_clear_screen
    mov esi, pm_banner
    call pm_print_string
    call pm_print_system_status
    call shell32_loop

pm_halt:
    cli
.hang:
    hlt
    jmp .hang
