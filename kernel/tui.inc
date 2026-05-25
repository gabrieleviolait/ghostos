; Text UI drawing primitives for VGA text mode.
; Common calling convention: DL=x, DH=y, AH=attr, AL=char, SI=string.
; Width and height use CL and CH for box/line routines.

draw_char_at:
    mov [tui_char_value], al
    mov [tui_attr_value], ah

    push ax
    push bx
    push dx
    push di
    push es

    cmp dl, SCREEN_WIDTH
    jae .done
    cmp dh, SCREEN_HEIGHT
    jae .done

    xor ax, ax
    mov al, dh
    mov bl, SCREEN_WIDTH
    mul bl

    xor bx, bx
    mov bl, dl
    add ax, bx
    shl ax, 1

    mov di, ax
    mov ax, 0xB800
    mov es, ax
    mov al, [tui_char_value]
    mov ah, [tui_attr_value]
    stosw

.done:
    pop es
    pop di
    pop dx
    pop bx
    pop ax
    ret

print_at:
    push ax
    push dx
    push si

.next:
    lodsb
    cmp al, 0
    je .done
    cmp dl, SCREEN_WIDTH
    jae .done
    call draw_char_at
    inc dl
    jmp .next

.done:
    pop si
    pop dx
    pop ax
    ret

draw_hline:
    push cx
    push dx

    xor ch, ch
    cmp cl, 0
    je .done

.loop:
    call draw_char_at
    inc dl
    loop .loop

.done:
    pop dx
    pop cx
    ret

draw_vline:
    push cx
    push dx

    xor ch, ch
    cmp cl, 0
    je .done

.loop:
    call draw_char_at
    inc dh
    loop .loop

.done:
    pop dx
    pop cx
    ret

clear_region:
    mov [tui_box_x], dl
    mov [tui_box_y], dh
    mov [tui_box_w], cl
    mov [tui_box_h], ch
    mov [tui_attr_value], ah

    push ax
    push bx
    push cx
    push dx

    mov bh, [tui_box_h]
    cmp bh, 0
    je .done

.row:
    mov dl, [tui_box_x]
    mov dh, [tui_box_y]
    mov cl, [tui_box_w]
    mov al, ' '
    mov ah, [tui_attr_value]
    call draw_hline
    inc byte [tui_box_y]
    dec bh
    jnz .row

.done:
    pop dx
    pop cx
    pop bx
    pop ax
    ret

draw_box:
    mov [tui_box_x], dl
    mov [tui_box_y], dh
    mov [tui_box_w], cl
    mov [tui_box_h], ch
    mov [tui_attr_value], ah

    push ax
    push cx
    push dx

    mov al, '-'
    mov ah, [tui_attr_value]
    mov dl, [tui_box_x]
    mov dh, [tui_box_y]
    mov cl, [tui_box_w]
    call draw_hline

    mov al, '-'
    mov ah, [tui_attr_value]
    mov dl, [tui_box_x]
    mov dh, [tui_box_y]
    add dh, [tui_box_h]
    dec dh
    mov cl, [tui_box_w]
    call draw_hline

    mov al, '|'
    mov ah, [tui_attr_value]
    mov dl, [tui_box_x]
    mov dh, [tui_box_y]
    mov cl, [tui_box_h]
    call draw_vline

    mov al, '|'
    mov ah, [tui_attr_value]
    mov dl, [tui_box_x]
    add dl, [tui_box_w]
    dec dl
    mov dh, [tui_box_y]
    mov cl, [tui_box_h]
    call draw_vline

    mov al, '+'
    mov ah, [tui_attr_value]
    mov dl, [tui_box_x]
    mov dh, [tui_box_y]
    call draw_char_at

    mov dl, [tui_box_x]
    add dl, [tui_box_w]
    dec dl
    mov dh, [tui_box_y]
    call draw_char_at

    mov dl, [tui_box_x]
    mov dh, [tui_box_y]
    add dh, [tui_box_h]
    dec dh
    call draw_char_at

    mov dl, [tui_box_x]
    add dl, [tui_box_w]
    dec dl
    mov dh, [tui_box_y]
    add dh, [tui_box_h]
    dec dh
    call draw_char_at

    pop dx
    pop cx
    pop ax
    ret

draw_title:
    push ax
    push dx
    push si

    inc dl
    mov al, ' '
    call draw_char_at
    inc dl

.next:
    lodsb
    cmp al, 0
    je .tail
    call draw_char_at
    inc dl
    jmp .next

.tail:
    mov al, ' '
    call draw_char_at

    pop si
    pop dx
    pop ax
    ret

render_desktop:
    call clear_screen

    mov dl, 2
    mov dh, 0
    mov cl, 76
    mov ch, 3
    mov ah, 0x0F
    call draw_box
    mov dl, 35
    mov dh, 0
    mov si, desktop_title
    mov ah, 0x0F
    call draw_title
    mov dl, 17
    mov dh, 1
    mov si, desktop_header_line
    mov ah, 0x0B
    call print_at

    mov dl, 2
    mov dh, 4
    mov cl, 24
    mov ch, 7
    mov ah, 0x0F
    call draw_box
    mov dl, 4
    mov dh, 4
    mov si, desktop_status_title
    mov ah, 0x0E
    call draw_title
    mov dl, 4
    mov dh, 6
    mov si, desktop_status_1
    mov ah, 0x07
    call print_at
    mov dl, 4
    mov dh, 7
    mov si, desktop_status_2
    mov ah, 0x07
    call print_at
    mov dl, 4
    mov dh, 8
    mov si, desktop_status_3
    mov ah, 0x07
    call print_at
    mov dl, 4
    mov dh, 9
    mov si, desktop_status_4
    mov ah, 0x07
    call print_at

    mov dl, 28
    mov dh, 4
    mov cl, 50
    mov ch, 7
    mov ah, 0x0F
    call draw_box
    mov dl, 30
    mov dh, 4
    mov si, desktop_commands_title
    mov ah, 0x0E
    call draw_title
    mov dl, 30
    mov dh, 6
    mov si, desktop_commands_1
    mov ah, 0x07
    call print_at
    mov dl, 30
    mov dh, 7
    mov si, desktop_commands_2
    mov ah, 0x07
    call print_at
    mov dl, 30
    mov dh, 8
    mov si, desktop_commands_3
    mov ah, 0x07
    call print_at
    mov dl, 30
    mov dh, 9
    mov si, desktop_commands_4
    mov ah, 0x07
    call print_at

    mov dl, 2
    mov dh, 12
    mov cl, 76
    mov ch, 8
    mov ah, 0x0F
    call draw_box
    mov dl, 4
    mov dh, 12
    mov si, desktop_browser_title
    mov ah, 0x0B
    call draw_title
    mov dl, 4
    mov dh, 14
    mov si, desktop_browser_1
    mov ah, 0x0F
    call print_at
    mov dl, 4
    mov dh, 15
    mov si, desktop_browser_2
    mov ah, 0x07
    call print_at
    mov dl, 4
    mov dh, 16
    mov si, desktop_browser_3
    mov ah, 0x07
    call print_at
    mov dl, 4
    mov dh, 17
    mov si, desktop_browser_4
    mov ah, 0x07
    call print_at

    mov dl, 2
    mov dh, 21
    mov cl, 76
    mov ch, 3
    mov ah, 0x0F
    call draw_box
    mov dl, 4
    mov dh, 21
    mov si, desktop_footer_title
    mov ah, 0x0E
    call draw_title
    mov dl, 4
    mov dh, 22
    mov si, desktop_footer_line
    mov ah, 0x07
    call print_at

    mov byte [cursor_x], TEXT_LEFT_MARGIN
    mov byte [cursor_y], 24
    call update_cursor
    ret
