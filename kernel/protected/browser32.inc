; Protected-mode disk-backed .GHT browser renderer.

[BITS 32]

pm_render_ght_buffer:
    pushad

.next:
    lodsb
    cmp al, 0
    je .done
    cmp al, '<'
    je .tag
    cmp al, 10
    je .newline
    cmp al, 13
    je .next
    call pm_print_char
    jmp .next

.newline:
    call pm_newline
    jmp .next

.tag:
    mov al, [esi]
    cmp al, 'h'
    je .maybe_hr
    cmp al, 'H'
    je .maybe_hr
    cmp al, 'p'
    je .tag_newline
    cmp al, 'P'
    je .tag_newline
    cmp al, '/'
    je .tag_newline
    jmp .skip_tag

.maybe_hr:
    mov al, [esi+1]
    cmp al, 'r'
    je .horizontal_rule
    cmp al, 'R'
    je .horizontal_rule
    jmp .skip_tag

.tag_newline:
    call pm_newline
    jmp .skip_tag

.horizontal_rule:
    call pm_newline
    push esi
    mov esi, ght_hr_line
    call pm_print_string
    pop esi
    call pm_newline
    jmp .skip_tag

.skip_tag:
    cmp byte [esi], 0
    je .done
    cmp byte [esi], '>'
    je .tag_done
    inc esi
    jmp .skip_tag

.tag_done:
    inc esi
    jmp .next

.done:
    popad
    ret

pm_browser_filename_ptr dd 0
