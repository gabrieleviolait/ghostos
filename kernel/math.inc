; Number formatting and tiny numeric helpers.

parse_uint16:
    push bx
    push cx
    push dx

    xor ax, ax
    xor cx, cx

.next_digit:
    mov bl, [si]
    cmp bl, '0'
    jb .done
    cmp bl, '9'
    ja .done

    sub bl, '0'

    cmp ax, 6553
    ja .overflow
    jne .multiply
    cmp bl, 5
    ja .overflow

.multiply:
    mov dx, 10
    mul dx
    xor bh, bh
    add ax, bx

    inc si
    inc cx
    jmp .next_digit

.done:
    cmp cx, 0
    je .bad
    clc
    jmp .return

.overflow:
.bad:
    stc

.return:
    pop dx
    pop cx
    pop bx
    ret

hex_to_nibble:
    cmp al, '0'
    jb .bad
    cmp al, '9'
    jbe .digit

    cmp al, 'A'
    jb .lower
    cmp al, 'F'
    jbe .upper

.lower:
    cmp al, 'a'
    jb .bad
    cmp al, 'f'
    ja .bad
    sub al, 'a'
    add al, 10
    clc
    ret

.upper:
    sub al, 'A'
    add al, 10
    clc
    ret

.digit:
    sub al, '0'
    clc
    ret

.bad:
    stc
    ret

print_uint16:
    pusha
    cmp ax, 0
    jne .convert
    mov al, '0'
    call print_char
    jmp .done

.convert:
    xor cx, cx
    mov bx, 10

.div_loop:
    xor dx, dx
    div bx
    push dx
    inc cx
    cmp ax, 0
    jne .div_loop

.print_loop:
    pop dx
    add dl, '0'
    mov al, dl
    call print_char
    loop .print_loop

.done:
    popa
    ret
