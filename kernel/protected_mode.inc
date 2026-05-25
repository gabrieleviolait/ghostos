; v0.9 protected mode runtime: GDT, IDT, PIC, IRQ1 keyboard, VGA shell.

CODE_SEL equ gdt_code - gdt_start
DATA_SEL equ gdt_data - gdt_start
IDT_PRESENT_INT_GATE equ 10001110b
KBD_BUFFER_MASK equ 31
VGA_BASE equ 0xB8000
PM_STACK_TOP equ 0x90000

enter_protected_mode_demo:
enter_protected_mode_shell:
    cli
    lgdt [gdt_descriptor]

    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp dword CODE_SEL:protected_mode_start

[BITS 32]

protected_mode_start:
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

    sti

    call pm_clear_screen
    mov esi, pm_banner
    call pm_print_string
    call shell32_loop

pm_halt:
    cli
.hang:
    hlt
    jmp .hang

pm_io_wait:
    push eax
    xor al, al
    out 0x80, al
    pop eax
    ret

pm_pic_remap:
    mov al, 0x11
    out 0x20, al
    call pm_io_wait
    out 0xA0, al
    call pm_io_wait

    mov al, 0x20
    out 0x21, al
    call pm_io_wait
    mov al, 0x28
    out 0xA1, al
    call pm_io_wait

    mov al, 0x04
    out 0x21, al
    call pm_io_wait
    mov al, 0x02
    out 0xA1, al
    call pm_io_wait

    mov al, 0x01
    out 0x21, al
    call pm_io_wait
    out 0xA1, al
    call pm_io_wait

    mov al, 11111101b
    out 0x21, al
    call pm_io_wait
    mov al, 11111111b
    out 0xA1, al
    call pm_io_wait
    ret

pm_idt_init:
    pushad
    xor ebx, ebx

.loop:
    mov eax, pm_default_interrupt
    call pm_set_idt_gate
    inc ebx
    cmp ebx, 256
    jb .loop

    mov ebx, 0x21
    mov eax, pm_irq1_keyboard
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

pm_irq1_keyboard:
    pushad

    in al, 0x60
    test al, 0x80
    jnz .ack

    movzx ebx, al
    cmp ebx, 128
    jae .ack
    mov al, [pm_scancode_ascii+ebx]
    cmp al, 0
    je .ack
    call pm_kbd_buffer_put

.ack:
    mov al, 0x20
    out 0x20, al

    popad
    iretd

pm_kbd_buffer_put:
    push ebx
    push edx

    movzx ebx, byte [pm_kbd_head]
    mov dl, bl
    inc dl
    and dl, KBD_BUFFER_MASK
    cmp dl, [pm_kbd_tail]
    je .full

    mov [pm_kbd_buffer+ebx], al
    mov [pm_kbd_head], dl

.full:
    pop edx
    pop ebx
    ret

pm_kbd_get_char:
    push ebx

.wait:
    cli
    mov al, [pm_kbd_head]
    cmp al, [pm_kbd_tail]
    jne .has_char
    sti
    hlt
    jmp .wait

.has_char:
    movzx ebx, byte [pm_kbd_tail]
    mov al, [pm_kbd_buffer+ebx]
    inc bl
    and bl, KBD_BUFFER_MASK
    mov [pm_kbd_tail], bl
    sti

    pop ebx
    ret

pm_update_cursor:
    pushad

    movzx eax, byte [pm_cursor_y]
    mov ebx, SCREEN_WIDTH
    mul ebx
    movzx ebx, byte [pm_cursor_x]
    add eax, ebx
    mov bx, ax

    mov dx, 0x3D4
    mov al, 0x0F
    out dx, al
    inc dx
    mov al, bl
    out dx, al

    dec dx
    mov al, 0x0E
    out dx, al
    inc dx
    mov al, bh
    out dx, al

    popad
    ret

pm_clear_screen:
    pushad

    mov edi, VGA_BASE
    mov ah, [pm_text_attr]
    mov al, ' '
    mov ecx, SCREEN_WIDTH*SCREEN_HEIGHT
    cld
    rep stosw

    mov byte [pm_cursor_x], 0
    mov byte [pm_cursor_y], 0
    call pm_update_cursor

    popad
    ret

pm_scroll_up:
    pushad

    mov esi, VGA_BASE + SCREEN_WIDTH*2
    mov edi, VGA_BASE
    mov ecx, (SCREEN_WIDTH*(SCREEN_HEIGHT-1)*2)/4
    cld
    rep movsd

    mov edi, VGA_BASE + SCREEN_WIDTH*(SCREEN_HEIGHT-1)*2
    mov ah, [pm_text_attr]
    mov al, ' '
    mov ecx, SCREEN_WIDTH
    rep stosw

    popad
    ret

pm_print_char:
    mov [pm_char_value], al
    pushad

    mov al, [pm_char_value]
    cmp al, 0
    je .done
    cmp al, 13
    je .newline
    cmp al, 10
    je .newline
    cmp al, 8
    je .backspace
    cmp al, 32
    jb .done

    movzx eax, byte [pm_cursor_y]
    mov ebx, SCREEN_WIDTH
    mul ebx
    movzx ebx, byte [pm_cursor_x]
    add eax, ebx
    shl eax, 1

    mov edi, VGA_BASE
    add edi, eax
    mov al, [pm_char_value]
    mov ah, [pm_text_attr]
    mov [edi], ax

    inc byte [pm_cursor_x]
    cmp byte [pm_cursor_x], SCREEN_WIDTH
    jb .refresh

.newline:
    mov byte [pm_cursor_x], 0
    inc byte [pm_cursor_y]
    cmp byte [pm_cursor_y], SCREEN_HEIGHT
    jb .refresh
    call pm_scroll_up
    mov byte [pm_cursor_y], SCREEN_HEIGHT-1
    jmp .refresh

.backspace:
    cmp byte [pm_cursor_x], 0
    je .refresh
    dec byte [pm_cursor_x]

    movzx eax, byte [pm_cursor_y]
    mov ebx, SCREEN_WIDTH
    mul ebx
    movzx ebx, byte [pm_cursor_x]
    add eax, ebx
    shl eax, 1

    mov edi, VGA_BASE
    add edi, eax
    mov al, ' '
    mov ah, [pm_text_attr]
    mov [edi], ax

.refresh:
    call pm_update_cursor

.done:
    popad
    ret

pm_print_string:
    pushad

.next:
    lodsb
    cmp al, 0
    je .done
    call pm_print_char
    jmp .next

.done:
    popad
    ret

pm_newline:
    mov al, 13
    call pm_print_char
    ret

pm_read_line:
    pushad
    xor ecx, ecx

.read_key:
    call pm_kbd_get_char
    cmp al, 13
    je .enter
    cmp al, 8
    je .backspace
    cmp al, 32
    jb .read_key
    cmp al, 126
    ja .read_key
    cmp ecx, CMD_BUFFER_SIZE-1
    jae .read_key

    stosb
    inc ecx
    call pm_print_char
    jmp .read_key

.backspace:
    cmp ecx, 0
    je .read_key
    dec edi
    dec ecx
    mov al, 8
    call pm_print_char
    jmp .read_key

.enter:
    mov al, 0
    stosb
    call pm_newline
    popad
    ret

pm_skip_spaces:
    cmp byte [esi], ' '
    jne .done
    inc esi
    jmp pm_skip_spaces
.done:
    ret

pm_skip_first_word:
    call pm_skip_spaces
.loop:
    cmp byte [esi], 0
    je .done
    cmp byte [esi], ' '
    je .done
    inc esi
    jmp .loop
.done:
    ret

pm_starts_with_word:
    pushad
.loop:
    mov al, [edi]
    cmp al, 0
    je .word_end
    cmp al, [esi]
    jne .no
    inc esi
    inc edi
    jmp .loop

.word_end:
    mov al, [esi]
    cmp al, 0
    je .yes
    cmp al, ' '
    je .yes

.no:
    popad
    clc
    ret

.yes:
    popad
    stc
    ret

shell32_loop:
    mov esi, pm_prompt
    call pm_print_string

    mov edi, pm_cmd_buffer
    call pm_read_line

    mov esi, pm_cmd_buffer
    call pm_skip_spaces
    cmp byte [esi], 0
    je shell32_loop

    mov edi, pm_cmd_help
    call pm_starts_with_word
    jc pm_do_help

    mov edi, pm_cmd_clear
    call pm_starts_with_word
    jc pm_do_clear

    mov edi, pm_cmd_cls
    call pm_starts_with_word
    jc pm_do_clear

    mov edi, pm_cmd_echo
    call pm_starts_with_word
    jc pm_do_echo

    mov edi, pm_cmd_version
    call pm_starts_with_word
    jc pm_do_version

    mov edi, pm_cmd_halt
    call pm_starts_with_word
    jc pm_do_halt

    mov esi, pm_unknown_msg
    call pm_print_string
    jmp shell32_loop

pm_do_help:
    mov esi, pm_help_msg
    call pm_print_string
    jmp shell32_loop

pm_do_clear:
    call pm_clear_screen
    jmp shell32_loop

pm_do_echo:
    mov esi, pm_cmd_buffer
    call pm_skip_first_word
    call pm_skip_spaces
    call pm_print_string
    call pm_newline
    jmp shell32_loop

pm_do_version:
    mov esi, pm_version_msg
    call pm_print_string
    jmp shell32_loop

pm_do_halt:
    mov esi, pm_halt_msg
    call pm_print_string
    jmp pm_halt

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

idt_descriptor:
    dw idt_end - idt_start - 1
    dd idt_start

align 8
idt_start:
    times 256 dq 0
idt_end:

pm_text_attr db 0x07
pm_cursor_x db 0
pm_cursor_y db 0
pm_char_value db 0
pm_kbd_head db 0
pm_kbd_tail db 0
pm_kbd_buffer times 32 db 0
pm_cmd_buffer times CMD_BUFFER_SIZE db 0

pm_scancode_ascii:
    db 0,0,'1','2','3','4','5','6','7','8','9','0','-','=',8,0
    db 'q','w','e','r','t','y','u','i','o','p','[',']',13,0,'a','s'
    db 'd','f','g','h','j','k','l',';',39,'`',0,92,'z','x','c','v'
    db 'b','n','m',',','.','/',0,'*',0,' '
    times 128-($-pm_scancode_ascii) db 0

pm_banner db 'Ghost OS 32-bit shell',13
          db 'Protected mode, VGA memory, IRQ1 keyboard.',13,13,0
pm_prompt db 'ghost32> ',0
pm_help_msg db 'Commands:',13
            db '  help       show this help',13
            db '  clear/cls  clear screen',13
            db '  echo TEXT  print text',13
            db '  version    show version',13
            db '  halt       stop CPU',13,0
pm_version_msg db 'Ghost OS v0.9 / 32-bit protected mode shell',13,0
pm_unknown_msg db 'Unknown command. Type help.',13,0
pm_halt_msg db 'System halted.',13,0

pm_cmd_help db 'help',0
pm_cmd_clear db 'clear',0
pm_cmd_cls db 'cls',0
pm_cmd_echo db 'echo',0
pm_cmd_version db 'version',0
pm_cmd_halt db 'halt',0
