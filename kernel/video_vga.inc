; VGA text mode and BIOS keyboard I/O helpers.
; Real-mode VGA text driver.
; Uses fixed attribute 0x07 for stable rendering in QEMU curses/GTK.

VGA_SAFE_ATTR equ 0x07

set_text_mode:
    pusha
    mov ax, 0x0003
    int 0x10
    popa
    ret

set_cursor_home:
    pusha
    mov byte [cursor_x], TEXT_LEFT_MARGIN
    mov byte [cursor_y], 0
    call update_cursor
    popa
    ret

clear_screen:
    pusha
    push es

    mov ax, 0xB800
    mov es, ax
    xor di, di

    mov al, ' '
    mov ah, VGA_SAFE_ATTR
    mov cx, SCREEN_WIDTH * SCREEN_HEIGHT

.clear_loop:
    stosw
    loop .clear_loop

    mov byte [cursor_x], TEXT_LEFT_MARGIN
    mov byte [cursor_y], 0
    call update_cursor

    pop es
    popa
    ret

scroll_up:
    pusha
    push ds
    push es

    mov ax, 0xB800
    mov ds, ax
    mov es, ax

    cld

    ; copy rows 1..24 to rows 0..23
    mov si, SCREEN_WIDTH * 2
    xor di, di
    mov cx, SCREEN_WIDTH * (SCREEN_HEIGHT - 1)
    rep movsw

    ; clear last row
    mov di, SCREEN_WIDTH * (SCREEN_HEIGHT - 1) * 2
    mov al, ' '
    mov ah, VGA_SAFE_ATTR
    mov cx, SCREEN_WIDTH
    rep stosw

    pop es
    pop ds
    popa
    ret

print_char:
    mov [print_char_value], al

    pusha
    push es

    mov al, [print_char_value]

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

    ; offset = ((cursor_y * SCREEN_WIDTH) + cursor_x) * 2
    xor ax, ax
    mov al, [cursor_y]
    mov cx, SCREEN_WIDTH
    mul cx

    xor cx, cx
    mov cl, [cursor_x]
    add ax, cx
    shl ax, 1

    mov di, ax
    mov ax, 0xB800
    mov es, ax

    mov al, [print_char_value]
    mov ah, VGA_SAFE_ATTR
    mov [es:di], ax

    inc byte [cursor_x]
    cmp byte [cursor_x], SCREEN_WIDTH
    jb .refresh

.newline:
    mov byte [cursor_x], TEXT_LEFT_MARGIN
    inc byte [cursor_y]
    cmp byte [cursor_y], SCREEN_HEIGHT
    jb .refresh

    call scroll_up
    mov byte [cursor_y], SCREEN_HEIGHT - 1
    mov byte [cursor_x], TEXT_LEFT_MARGIN
    jmp .refresh

.backspace:
    cmp byte [cursor_x], TEXT_LEFT_MARGIN
    jbe .refresh

    dec byte [cursor_x]

    xor ax, ax
    mov al, [cursor_y]
    mov cx, SCREEN_WIDTH
    mul cx

    xor cx, cx
    mov cl, [cursor_x]
    add ax, cx
    shl ax, 1

    mov di, ax
    mov ax, 0xB800
    mov es, ax

    mov al, ' '
    mov ah, VGA_SAFE_ATTR
    mov [es:di], ax

.refresh:
    call update_cursor

.done:
    pop es
    popa
    ret

print_string:
    pusha

.next:
    lodsb
    cmp al, 0
    je .done
    call print_char
    jmp .next

.done:
    popa
    ret

newline:
    pusha
    mov al, 13
    call print_char
    popa
    ret

read_line:
    pusha
    xor cx, cx

.read_key:
    xor ah, ah
    int 0x16

    cmp al, 0
    je .read_key

    cmp al, 0xE0
    je .read_key

    cmp al, 13
    je .enter

    cmp al, 8
    je .backspace

    cmp al, 32
    jb .read_key

    cmp al, 126
    ja .read_key

    cmp cx, CMD_BUFFER_SIZE - 1
    jae .read_key

    stosb
    inc cx
    call print_char
    jmp .read_key

.backspace:
    cmp cx, 0
    je .read_key

    dec di
    dec cx

    mov al, 8
    call print_char

    jmp .read_key

.enter:
    mov al, 0
    stosb
    popa
    ret

update_cursor:
    pusha

    xor ax, ax
    mov al, [cursor_y]
    mov cx, SCREEN_WIDTH
    mul cx

    xor bx, bx
    mov bl, [cursor_x]
    add ax, bx

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

    popa
    ret