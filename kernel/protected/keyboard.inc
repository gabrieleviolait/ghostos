; Protected-mode keyboard IRQ buffer and shell line input.

[BITS 32]

KBD_BUFFER_MASK equ 31

pm_kbd_reset:
    push eax
    push ecx

    mov byte [pm_kbd_head], 0
    mov byte [pm_kbd_tail], 0
    mov ecx, 32

.flush:
    in al, 0x64
    test al, 1
    jz .done
    in al, 0x60
    loop .flush
    jmp .done

.done:
    pop ecx
    pop eax
    ret

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

pm_read_line:
    pushad
    cld
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
