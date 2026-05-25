; Built-in text browser command.

do_browse:
    call clear_screen
    call set_cursor_home
    mov si, browser_page
    call print_string
    jmp shell_loop
