; Protected-mode VGA text output and debug printing helpers.

[BITS 32]

VGA_BASE equ 0xB8000

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

    cmp byte [pm_cursor_x], SCREEN_WIDTH
    jb .row_ok
    mov byte [pm_cursor_x], 0
    inc byte [pm_cursor_y]

.row_ok:
    cmp byte [pm_cursor_y], SCREEN_HEIGHT
    jb .draw
    call pm_scroll_up
    mov byte [pm_cursor_y], SCREEN_HEIGHT-1

.draw:
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
    jne .backspace_same_row
    cmp byte [pm_cursor_y], 0
    je .refresh
    dec byte [pm_cursor_y]
    mov byte [pm_cursor_x], SCREEN_WIDTH-1
    jmp .erase

.backspace_same_row:
    dec byte [pm_cursor_x]

.erase:
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

pm_print_uint32:
    pushad

    cmp eax, 0
    jne .convert
    mov al, '0'
    call pm_print_char
    jmp .done

.convert:
    xor ecx, ecx
    mov ebx, 10

.div_loop:
    xor edx, edx
    div ebx
    push edx
    inc ecx
    cmp eax, 0
    jne .div_loop

.print_loop:
    pop eax
    add al, '0'
    call pm_print_char
    loop .print_loop

.done:
    popad
    ret

pm_print_hex32:
    pushad

    mov esi, pm_hex_prefix
    call pm_print_string
    mov edx, eax
    mov ecx, 8

.hex_loop:
    rol edx, 4
    mov ebx, edx
    and ebx, 0x0F
    mov al, [pm_hex_digits+ebx]
    call pm_print_char
    loop .hex_loop

    popad
    ret

pm_text_attr db 0x07
pm_cursor_x db 0
pm_cursor_y db 0
pm_char_value db 0
pm_hex_prefix db '0x',0
pm_hex_digits db '0123456789ABCDEF'
