; Protected-mode IDT setup and fatal exception screen.

[BITS 32]

IDT_PRESENT_INT_GATE equ 10001110b

pm_idt_init:
    pushad
    xor ebx, ebx

.default_loop:
    mov eax, pm_default_interrupt
    call pm_set_idt_gate
    inc ebx
    cmp ebx, 256
    jb .default_loop

    xor ebx, ebx
    mov esi, pm_exception_stubs

.exception_loop:
    lodsd
    call pm_set_idt_gate
    inc ebx
    cmp ebx, 32
    jb .exception_loop

    mov ebx, 0x21
    mov eax, pm_irq1_keyboard
    call pm_set_idt_gate

    mov ebx, 0x20
    mov eax, pm_irq0_timer
    call pm_set_idt_gate

    mov ebx, 0x26
    mov eax, pm_irq6_floppy
    call pm_set_idt_gate

    popad
    ret

pm_set_idt_gate:
    push eax
    push ebx
    push edx

    mov edx, idt_start
    shl ebx, 3
    add edx, ebx

    mov word [edx], ax
    mov word [edx+2], CODE_SEL
    mov byte [edx+4], 0
    mov byte [edx+5], IDT_PRESENT_INT_GATE
    shr eax, 16
    mov word [edx+6], ax

    pop edx
    pop ebx
    pop eax
    ret

pm_default_interrupt:
    iretd

pm_exception_0:
    push dword 0
    push dword 0
    jmp pm_exception_common
pm_exception_1:
    push dword 0
    push dword 1
    jmp pm_exception_common
pm_exception_2:
    push dword 0
    push dword 2
    jmp pm_exception_common
pm_exception_3:
    push dword 0
    push dword 3
    jmp pm_exception_common
pm_exception_4:
    push dword 0
    push dword 4
    jmp pm_exception_common
pm_exception_5:
    push dword 0
    push dword 5
    jmp pm_exception_common
pm_exception_6:
    push dword 0
    push dword 6
    jmp pm_exception_common
pm_exception_7:
    push dword 0
    push dword 7
    jmp pm_exception_common
pm_exception_8:
    push dword 8
    jmp pm_exception_common
pm_exception_9:
    push dword 0
    push dword 9
    jmp pm_exception_common
pm_exception_10:
    push dword 10
    jmp pm_exception_common
pm_exception_11:
    push dword 11
    jmp pm_exception_common
pm_exception_12:
    push dword 12
    jmp pm_exception_common
pm_exception_13:
    push dword 13
    jmp pm_exception_common
pm_exception_14:
    push dword 14
    jmp pm_exception_common
pm_exception_15:
    push dword 0
    push dword 15
    jmp pm_exception_common
pm_exception_16:
    push dword 0
    push dword 16
    jmp pm_exception_common
pm_exception_17:
    push dword 17
    jmp pm_exception_common
pm_exception_18:
    push dword 0
    push dword 18
    jmp pm_exception_common
pm_exception_19:
    push dword 0
    push dword 19
    jmp pm_exception_common
pm_exception_20:
    push dword 0
    push dword 20
    jmp pm_exception_common
pm_exception_21:
    push dword 0
    push dword 21
    jmp pm_exception_common
pm_exception_22:
    push dword 0
    push dword 22
    jmp pm_exception_common
pm_exception_23:
    push dword 0
    push dword 23
    jmp pm_exception_common
pm_exception_24:
    push dword 0
    push dword 24
    jmp pm_exception_common
pm_exception_25:
    push dword 0
    push dword 25
    jmp pm_exception_common
pm_exception_26:
    push dword 0
    push dword 26
    jmp pm_exception_common
pm_exception_27:
    push dword 0
    push dword 27
    jmp pm_exception_common
pm_exception_28:
    push dword 0
    push dword 28
    jmp pm_exception_common
pm_exception_29:
    push dword 0
    push dword 29
    jmp pm_exception_common
pm_exception_30:
    push dword 0
    push dword 30
    jmp pm_exception_common
pm_exception_31:
    push dword 0
    push dword 31
    jmp pm_exception_common

pm_exception_common:
    cli
    mov eax, [esp]
    mov [pm_panic_vector], eax
    mov eax, [esp+4]
    mov [pm_panic_error], eax
    call pm_panic_exception
    jmp pm_halt

pm_panic:
    cli
    push esi
    call pm_clear_screen
    mov esi, pm_panic_header
    call pm_print_string
    pop esi
    call pm_print_string
    call pm_newline
    mov esi, pm_panic_halt_msg
    call pm_print_string
    jmp pm_halt

pm_panic_exception:
    call pm_clear_screen
    mov esi, pm_panic_header
    call pm_print_string
    mov esi, pm_exception_msg
    call pm_print_string

    mov esi, pm_exception_vector_msg
    call pm_print_string
    mov eax, [pm_panic_vector]
    call pm_print_uint32
    mov esi, pm_space_open_paren
    call pm_print_string
    mov eax, [pm_panic_vector]
    call pm_print_hex32
    mov esi, pm_close_paren_newline
    call pm_print_string

    mov esi, pm_exception_error_msg
    call pm_print_string
    mov eax, [pm_panic_error]
    call pm_print_hex32
    call pm_newline

    mov esi, pm_panic_halt_msg
    call pm_print_string
    jmp pm_halt

pm_exception_stubs:
    dd pm_exception_0, pm_exception_1, pm_exception_2, pm_exception_3
    dd pm_exception_4, pm_exception_5, pm_exception_6, pm_exception_7
    dd pm_exception_8, pm_exception_9, pm_exception_10, pm_exception_11
    dd pm_exception_12, pm_exception_13, pm_exception_14, pm_exception_15
    dd pm_exception_16, pm_exception_17, pm_exception_18, pm_exception_19
    dd pm_exception_20, pm_exception_21, pm_exception_22, pm_exception_23
    dd pm_exception_24, pm_exception_25, pm_exception_26, pm_exception_27
    dd pm_exception_28, pm_exception_29, pm_exception_30, pm_exception_31

idt_descriptor:
    dw idt_end - idt_start - 1
    dd idt_start

align 8
idt_start:
    times 256 dq 0
idt_end:

pm_panic_vector dd 0
pm_panic_error dd 0
pm_panic_header db 'Ghost OS protected-mode panic',13,0
pm_exception_msg db 'Fatal CPU exception.',13,0
pm_exception_vector_msg db 'Vector: ',0
pm_exception_error_msg db 'Error code: ',0
pm_space_open_paren db ' (',0
pm_close_paren_newline db ')',13,0
pm_panic_halt_msg db 'CPU halted. Reboot to return to ghost>.',13,0
