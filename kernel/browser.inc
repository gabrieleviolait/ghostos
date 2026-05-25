; Disk-backed text browser command and tiny .GHT renderer.

do_browse:
    call clear_screen
    call set_cursor_home

    mov si, cmd_buffer
    call skip_first_word
    call skip_spaces
    cmp byte [si], 0
    jne .have_name
    mov si, default_page_name

.have_name:
    mov [browser_filename_ptr], si
    mov di, page_buffer
    mov cx, PAGE_BUFFER_SIZE-1
    call fat12_read_file
    jc .load_error

    mov si, browser_disk_header
    call print_string
    mov si, [browser_filename_ptr]
    call print_string
    call newline
    mov si, browser_separator
    call print_string

    mov si, page_buffer
    call render_ght_buffer
    call newline
    jmp shell_loop

.load_error:
    mov si, browse_error_msg
    call print_string
    jmp shell_loop

render_ght_buffer:
    pusha

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
    call print_char
    jmp .next

.newline:
    call newline
    jmp .next

.tag:
    mov al, [si]
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
    mov al, [si+1]
    cmp al, 'r'
    je .horizontal_rule
    cmp al, 'R'
    je .horizontal_rule
    jmp .skip_tag

.tag_newline:
    call newline
    jmp .skip_tag

.horizontal_rule:
    call newline
    push si
    mov si, ght_hr_line
    call print_string
    pop si
    call newline
    jmp .skip_tag

.skip_tag:
    cmp byte [si], 0
    je .done
    cmp byte [si], '>'
    je .tag_done
    inc si
    jmp .skip_tag

.tag_done:
    inc si
    jmp .next

.done:
    popa
    ret
