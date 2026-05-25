; v0.8 protected mode demo. This intentionally does not return to the shell.

CODE_SEL equ gdt_code - gdt_start
DATA_SEL equ gdt_data - gdt_start

enter_protected_mode_demo:
    cli
    lgdt [gdt_descriptor]

    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp dword CODE_SEL:protected_mode_32

[BITS 32]
protected_mode_32:
    mov ax, DATA_SEL
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000

    mov edi, 0xB8000
    mov esi, protected_mode_msg
    mov ah, 0x0A

.print:
    lodsb
    cmp al, 0
    je .halt
    mov [edi], ax
    add edi, 2
    jmp .print

.halt:
    hlt
    jmp .halt

[BITS 16]

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

protected_mode_msg db 'Protected mode entered',0
