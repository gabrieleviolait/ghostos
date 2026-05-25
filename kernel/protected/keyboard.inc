; Protected-mode keyboard IRQ buffer and shell line input.

[BITS 32]

KBD_BUFFER_MASK equ 31
KEY_LEFT equ 0x80
KEY_RIGHT equ 0x81
KEY_UP equ 0x82
KEY_DOWN equ 0x83
KEY_DELETE equ 0x84
KEY_HOME equ 0x85
KEY_END equ 0x86
PM_EDIT_MAX equ 40
PM_HISTORY_COUNT equ 2
PM_HISTORY_MASK equ 1

pm_kbd_reset:
    push eax
    push ecx

    mov byte [pm_kbd_head], 0
    mov byte [pm_kbd_tail], 0
    mov byte [pm_kbd_e0], 0
    mov ecx, 32

.flush:
    in al, 0x64
    test al, 1
    jz .done
    in al, 0x60
    loop .flush

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
    mov byte [pm_line_len], 0
    mov byte [pm_line_pos], 0
    mov byte [pm_history_view], 0
    mov byte [pm_cmd_buffer], 0
    mov al, [pm_cursor_x]
    mov [pm_line_x], al
    mov al, [pm_cursor_y]
    mov [pm_line_y], al

.read_key:
    call pm_kbd_get_char
    cmp al, 13
    je .enter
    cmp al, 9
    je .tab
    cmp al, 8
    je .backspace
    cmp al, KEY_LEFT
    je .left
    cmp al, KEY_RIGHT
    je .right
    cmp al, KEY_UP
    je .up
    cmp al, KEY_DOWN
    je .down
    cmp al, KEY_DELETE
    je .delete
    cmp al, KEY_HOME
    je .home
    cmp al, KEY_END
    je .end
    cmp al, 32
    jb .read_key
    cmp al, 126
    ja .read_key
    cmp byte [pm_line_len], PM_EDIT_MAX-1
    jae .read_key
    call pm_line_insert_char
    call pm_line_redraw
    jmp .read_key

.backspace:
    cmp byte [pm_line_pos], 0
    je .read_key
    call pm_line_backspace
    call pm_line_redraw
    jmp .read_key

.delete:
    mov al, [pm_line_pos]
    cmp al, [pm_line_len]
    jae .read_key
    call pm_line_delete
    call pm_line_redraw
    jmp .read_key

.left:
    cmp byte [pm_line_pos], 0
    je .read_key
    dec byte [pm_line_pos]
    call pm_line_set_cursor
    jmp .read_key

.right:
    mov al, [pm_line_pos]
    cmp al, [pm_line_len]
    jae .read_key
    inc byte [pm_line_pos]
    call pm_line_set_cursor
    jmp .read_key

.home:
    mov byte [pm_line_pos], 0
    call pm_line_set_cursor
    jmp .read_key

.end:
    mov al, [pm_line_len]
    mov [pm_line_pos], al
    call pm_line_set_cursor
    jmp .read_key

.up:
    call pm_history_prev
    jmp .read_key

.down:
    call pm_history_down
    jmp .read_key

.tab:
    call pm_line_complete
    jmp .read_key

.enter:
    call pm_history_store
    call pm_newline
    popad
    ret

pm_line_insert_char:
    pushad
    mov [pm_key_value], al
    movzx edx, byte [pm_line_pos]
    movzx ecx, byte [pm_line_len]
    sub ecx, edx
    inc ecx
    lea esi, [pm_cmd_buffer+edx+ecx-1]
    lea edi, [esi+1]
    std
    rep movsb
    cld
    mov al, [pm_key_value]
    mov [pm_cmd_buffer+edx], al
    inc byte [pm_line_len]
    inc byte [pm_line_pos]
    popad
    ret

pm_line_backspace:
    dec byte [pm_line_pos]

pm_line_delete:
    pushad
    movzx edx, byte [pm_line_pos]
    movzx ecx, byte [pm_line_len]
    sub ecx, edx
    lea esi, [pm_cmd_buffer+edx+1]
    lea edi, [pm_cmd_buffer+edx]
    cld
    rep movsb
    dec byte [pm_line_len]
    popad
    ret

pm_line_set_cursor:
    push eax
    mov al, [pm_line_x]
    add al, [pm_line_pos]
    mov [pm_cursor_x], al
    mov al, [pm_line_y]
    mov [pm_cursor_y], al
    call pm_update_cursor
    pop eax
    ret

pm_line_redraw:
    pushad
    mov al, [pm_line_x]
    mov [pm_cursor_x], al
    mov al, [pm_line_y]
    mov [pm_cursor_y], al
    call pm_update_cursor
    mov ecx, PM_EDIT_MAX
    mov al, ' '
.clear:
    call pm_print_char
    loop .clear
    mov al, [pm_line_x]
    mov [pm_cursor_x], al
    call pm_update_cursor
    mov esi, pm_cmd_buffer
    call pm_print_string
    popad
    call pm_line_set_cursor
    ret

pm_line_load:
    pushad
    xor ecx, ecx
    mov edi, pm_cmd_buffer
.copy:
    lodsb
    stosb
    cmp al, 0
    je .done
    inc ecx
    cmp ecx, PM_EDIT_MAX-1
    jb .copy
    mov byte [edi], 0
.done:
    mov [pm_line_len], cl
    mov [pm_line_pos], cl
    popad
    call pm_line_redraw
    ret

pm_history_slot:
    and eax, PM_HISTORY_MASK
    imul eax, eax, PM_EDIT_MAX
    add eax, pm_history
    ret

pm_history_store:
    cmp byte [pm_line_len], 0
    je .done
    pushad
    movzx eax, byte [pm_history_next]
    call pm_history_slot
    mov edi, eax
    mov esi, pm_cmd_buffer
    movzx ecx, byte [pm_line_len]
    inc ecx
    cld
    rep movsb
    inc byte [pm_history_next]
    and byte [pm_history_next], PM_HISTORY_MASK
    cmp byte [pm_history_count], PM_HISTORY_COUNT
    jae .stored
    inc byte [pm_history_count]
.stored:
    popad
.done:
    ret

pm_history_prev:
    cmp byte [pm_history_count], 0
    je .done
    mov al, [pm_history_view]
    cmp al, [pm_history_count]
    jae .load
    inc byte [pm_history_view]
.load:
    movzx eax, byte [pm_history_next]
    sub al, [pm_history_view]
    call pm_history_slot
    mov esi, eax
    call pm_line_load
.done:
    ret

pm_history_down:
    cmp byte [pm_history_view], 0
    je .done
    dec byte [pm_history_view]
    cmp byte [pm_history_view], 0
    jne .load
    mov byte [pm_line_len], 0
    mov byte [pm_line_pos], 0
    mov byte [pm_cmd_buffer], 0
    call pm_line_redraw
    ret
.load:
    movzx eax, byte [pm_history_next]
    sub al, [pm_history_view]
    call pm_history_slot
    mov esi, eax
    call pm_line_load
.done:
    ret

pm_line_complete:
    pushad
    movzx ecx, byte [pm_line_len]
    mov esi, pm_cmd_buffer
.space_check:
    cmp ecx, 0
    je .scan
    cmp byte [esi], ' '
    je .done
    inc esi
    dec ecx
    jmp .space_check
.scan:
    xor ebx, ebx
    mov dword [pm_completion_first], 0
    mov edi, pm_completion_table
.scan_loop:
    mov esi, [edi]
    cmp esi, 0
    je .scan_done
    call pm_completion_matches
    jnc .next
    inc ebx
    cmp dword [pm_completion_first], 0
    jne .next
    mov [pm_completion_first], esi
.next:
    add edi, 4
    jmp .scan_loop
.scan_done:
    cmp ebx, 1
    je .single
    ja .many
    jmp .done
.single:
    mov esi, [pm_completion_first]
    mov edi, pm_cmd_buffer
    xor ecx, ecx
.single_copy:
    lodsb
    cmp al, 0
    je .single_space
    stosb
    inc ecx
    jmp .single_copy
.single_space:
    mov al, ' '
    stosb
    inc ecx
    mov byte [edi], 0
    mov [pm_line_len], cl
    mov [pm_line_pos], cl
    call pm_line_redraw
    jmp .done
.many:
    call pm_newline
    mov edi, pm_completion_table
.many_loop:
    mov esi, [edi]
    cmp esi, 0
    je .many_done
    call pm_completion_matches
    jnc .many_next
    call pm_print_string
    mov al, ' '
    call pm_print_char
.many_next:
    add edi, 4
    jmp .many_loop
.many_done:
    call pm_newline
    mov esi, pm_prompt
    call pm_print_string
    mov al, [pm_cursor_x]
    mov [pm_line_x], al
    mov al, [pm_cursor_y]
    mov [pm_line_y], al
    call pm_line_redraw
.done:
    popad
    ret

pm_completion_matches:
    push ebx
    push ecx
    push edi
    push esi
    mov edi, pm_cmd_buffer
    movzx ecx, byte [pm_line_len]
.loop:
    cmp ecx, 0
    je .yes
    mov al, [edi]
    cmp al, [esi]
    jne .no
    inc edi
    inc esi
    dec ecx
    jmp .loop
.yes:
    pop esi
    pop edi
    pop ecx
    pop ebx
    stc
    ret
.no:
    pop esi
    pop edi
    pop ecx
    pop ebx
    clc
    ret

pm_kbd_head db 0
pm_kbd_tail db 0
pm_kbd_e0 db 0
pm_kbd_buffer times 32 db 0
pm_cmd_buffer times CMD_BUFFER_SIZE db 0
pm_key_value db 0
pm_line_len db 0
pm_line_pos db 0
pm_line_x db 0
pm_line_y db 0
pm_history_count db 0
pm_history_next db 0
pm_history_view db 0
pm_completion_first dd 0
pm_history times PM_HISTORY_COUNT*PM_EDIT_MAX db 0

pm_completion_table:
    dd pm_cmd_help, pm_cmd_clear, pm_cmd_cls, pm_cmd_echo
    dd pm_cmd_version, pm_cmd_mem, pm_cmd_heapinfo, pm_cmd_pages
    dd pm_cmd_lspci, pm_cmd_netdev, pm_cmd_regs, pm_cmd_stack
    dd pm_cmd_uptime, pm_cmd_panic
    dd pm_cmd_memmap, pm_cmd_tasks, pm_cmd_yield, pm_cmd_ls
    dd pm_cmd_cat, pm_cmd_browse, pm_cmd_run, pm_cmd_halt, 0

pm_scancode_ascii:
    db 0,0,'1','2','3','4','5','6','7','8','9','0','-','=',8,9
    db 'q','w','e','r','t','y','u','i','o','p','[',']',13,0,'a','s'
    db 'd','f','g','h','j','k','l',';',39,'`',0,92,'z','x','c','v'
    db 'b','n','m',',','.','/',0,'*',0,' '
    times 128-($-pm_scancode_ascii) db 0
