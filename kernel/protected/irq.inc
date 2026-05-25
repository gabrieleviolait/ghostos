; PIC remap and protected-mode IRQ handlers.

[BITS 32]

pm_io_wait:
    push eax
    xor al, al
    out 0x80, al
    pop eax
    ret

pm_pic_remap:
    push eax

    mov al, 0xFF
    out 0x21, al
    call pm_io_wait
    out 0xA1, al
    call pm_io_wait

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

    mov al, 10111101b
    out 0x21, al
    call pm_io_wait
    mov al, 11111111b
    out 0xA1, al
    call pm_io_wait

    pop eax
    ret

pm_irq1_keyboard:
    pushad

    in al, 0x60
    cmp al, 0xE0
    jne .not_e0
    mov byte [pm_kbd_e0], 1
    jmp .ack

.not_e0:
    cmp byte [pm_kbd_e0], 0
    je .normal
    mov byte [pm_kbd_e0], 0
    test al, 0x80
    jnz .ack
    cmp al, 0x4B
    je .left
    cmp al, 0x4D
    je .right
    cmp al, 0x48
    je .up
    cmp al, 0x50
    je .down
    cmp al, 0x53
    je .delete
    cmp al, 0x47
    je .home
    cmp al, 0x4F
    je .end
    jmp .ack

.left:
    mov al, KEY_LEFT
    jmp .put
.right:
    mov al, KEY_RIGHT
    jmp .put
.up:
    mov al, KEY_UP
    jmp .put
.down:
    mov al, KEY_DOWN
    jmp .put
.delete:
    mov al, KEY_DELETE
    jmp .put
.home:
    mov al, KEY_HOME
    jmp .put
.end:
    mov al, KEY_END
    jmp .put

.normal:
    test al, 0x80
    jnz .ack

    movzx ebx, al
    cmp ebx, 128
    jae .ack
    mov al, [pm_scancode_ascii+ebx]
    cmp al, 0
    je .ack

.put:
    call pm_kbd_buffer_put

.ack:
    mov al, 0x20
    out 0x20, al

    popad
    iretd

pm_irq6_floppy:
    pushad

    mov byte [pm_floppy_irq_fired], 1
    mov al, 0x20
    out 0x20, al

    popad
    iretd
