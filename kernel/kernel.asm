; Ghost OS v0.5.2 kernel
; Pure NASM 16-bit x86 real mode CLI/TUI OS.
; Commands: help, clear/cls, echo, about, version, calc, mem, ascii, color, gui, reboot, halt.

[BITS 16]
[ORG 0x1000]

CMD_BUFFER_SIZE equ 128

kernel_start:
    cli
    cld

    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x9000

    sti

    call set_text_mode
    call clear_screen
    call set_cursor_home

    mov si, banner
    call print_string

shell_loop:
    mov si, prompt
    call print_string

    mov di, cmd_buffer
    call read_line
    call newline

    mov si, cmd_buffer
    call skip_spaces
    cmp byte [si], 0
    je shell_loop

    mov di, cmd_help
    call starts_with_word
    jc do_help

    mov di, cmd_clear
    call starts_with_word
    jc do_clear

    mov di, cmd_cls
    call starts_with_word
    jc do_clear

    mov di, cmd_echo
    call starts_with_word
    jc do_echo

    mov di, cmd_about
    call starts_with_word
    jc do_about

    mov di, cmd_version
    call starts_with_word
    jc do_version

    mov di, cmd_gui
    call starts_with_word
    jc do_gui

    mov di, cmd_browse
    call starts_with_word
    jc do_browse

    mov di, cmd_reboot
    call starts_with_word
    jc do_reboot

    mov di, cmd_halt
    call starts_with_word
    jc do_halt

    mov di, cmd_mem
    call starts_with_word
    jc do_mem

    mov di, cmd_ascii
    call starts_with_word
    jc do_ascii

    mov di, cmd_color
    call starts_with_word
    jc do_color

    mov di, cmd_calc
    call starts_with_word
    jc do_calc

    mov si, unknown_msg
    call print_string
    jmp shell_loop

; ---------------- commands ----------------

do_help:
    mov si, help_msg
    call print_string
    jmp shell_loop

do_clear:
    call clear_screen
    call set_cursor_home
    jmp shell_loop

do_gui:
    call clear_screen
    call set_cursor_home
    mov si, gui_msg
    call print_string
    jmp shell_loop

do_about:
    mov si, about_msg
    call print_string
    jmp shell_loop

do_version:
    mov si, version_msg
    call print_string
    jmp shell_loop

do_echo:
    mov si, cmd_buffer
    call skip_first_word
    call skip_spaces
    call print_string
    call newline
    jmp shell_loop

do_reboot:
    mov si, reboot_msg
    call print_string
    int 0x19
    jmp halt_cpu

do_halt:
    mov si, halt_msg
    call print_string
    jmp halt_cpu

do_mem:
    int 0x12
    mov si, mem_msg
    call print_string
    call print_uint16
    mov si, kb_msg
    call print_string
    jmp shell_loop

do_ascii:
    mov si, ascii_msg
    call print_string
    jmp shell_loop

do_color:
    mov si, cmd_buffer
    call skip_first_word
    call skip_spaces
    mov al, [si]
    call hex_to_nibble
    jc .usage
    mov [text_attr], al
    mov si, color_ok
    call print_string
    jmp shell_loop
.usage:
    mov si, color_usage
    call print_string
    jmp shell_loop

do_calc:
    mov si, cmd_buffer
    call skip_first_word
    call skip_spaces

    mov al, [si]
    cmp al, '0'
    jb .usage
    cmp al, '9'
    ja .usage
    sub al, '0'
    mov bl, al

    inc si
    mov al, [si]
    cmp al, '+'
    je .plus
    cmp al, '-'
    je .minus
    jmp .usage


.plus:
    inc si
    mov al, [si]
    cmp al, '0'
    jb .usage
    cmp al, '9'
    ja .usage
    sub al, '0'
    add bl, al
    xor ax, ax
    mov al, bl
    mov si, result_msg
    call print_string
    call print_uint16
    call newline
    jmp shell_loop

.minus:
    inc si
    mov al, [si]
    cmp al, '0'
    jb .usage
    cmp al, '9'
    ja .usage
    sub al, '0'
    cmp bl, al
    jb .negative
    sub bl, al
    xor ax, ax
    mov al, bl
    mov si, result_msg
    call print_string
    call print_uint16
    call newline
    jmp shell_loop

.negative:
    mov si, negative_msg
    call print_string
    jmp shell_loop

.usage:
    mov si, calc_usage
    call print_string
    jmp shell_loop

do_browse:
    call clear_screen
    call set_cursor_home
    mov si, browser_page
    call print_string
    jmp shell_loop

; ---------------- video / IO ----------------

set_text_mode:
    pusha
    mov ax, 0x0003
    int 0x10
    popa
    ret

set_cursor_home:
    pusha
    mov ah, 0x02
    mov bh, 0x00
    mov dh, 0x00
    mov dl, 0x02        ; evita taglio prima lettera a sinistra
    int 0x10
    popa
    ret

clear_screen:
    pusha
    push es

    mov ax, 0xB800
    mov es, ax
    xor di, di
    mov ah, [text_attr]
    mov al, ' '
    mov cx, 80*25

.clear_loop:
    stosw
    loop .clear_loop

    mov byte [cursor_x], 2
    mov byte [cursor_y], 0
    call update_cursor

    pop es
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

    xor ax, ax
    mov al, [cursor_y]
    mov bl, 80
    mul bl

    xor bx, bx
    mov bl, [cursor_x]
    add ax, bx
    shl ax, 1

    mov di, ax
    mov ax, 0xB800
    mov es, ax

    mov al, [print_char_value]
    mov ah, [text_attr]
    stosw

    inc byte [cursor_x]
    cmp byte [cursor_x], 80
    jb .refresh

.newline:
    mov byte [cursor_x], 2
    inc byte [cursor_y]
    cmp byte [cursor_y], 25
    jb .refresh
    mov byte [cursor_y], 24
    jmp .refresh

.backspace:
    cmp byte [cursor_x], 2
    jbe .refresh
    dec byte [cursor_x]

    xor ax, ax
    mov al, [cursor_y]
    mov bl, 80
    mul bl

    xor bx, bx
    mov bl, [cursor_x]
    add ax, bx
    shl ax, 1

    mov di, ax
    mov ax, 0xB800
    mov es, ax
    mov al, ' '
    mov ah, [text_attr]
    stosw

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

    cmp cx, CMD_BUFFER_SIZE-1
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
    mov bl, 80
    mul bl

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

; ---------------- helpers ----------------

skip_spaces:
    cmp byte [si], ' '
    jne .done
    inc si
    jmp skip_spaces
.done:
    ret

skip_first_word:
    call skip_spaces
.loop:
    cmp byte [si], 0
    je .done
    cmp byte [si], ' '
    je .done
    inc si
    jmp .loop
.done:
    ret

starts_with_word:
    pusha
.loop:
    mov al, [di]
    cmp al, 0
    je .word_end
    cmp al, [si]
    jne .no
    inc si
    inc di
    jmp .loop

.word_end:
    mov al, [si]
    cmp al, 0
    je .yes
    cmp al, ' '
    je .yes

.no:
    popa
    clc
    ret

.yes:
    popa
    stc
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

halt_cpu:
    cli
.hang:
    hlt
    jmp .hang

; ---------------- data ----------------

text_attr db 0x07

cursor_x db 2
cursor_y db 0
print_char_value db 0

cmd_buffer times CMD_BUFFER_SIZE db 0

cmd_help    db 'help',0
cmd_clear   db 'clear',0
cmd_cls     db 'cls',0
cmd_echo    db 'echo',0
cmd_about   db 'about',0
cmd_version db 'version',0
cmd_gui     db 'gui',0
cmd_reboot  db 'reboot',0
cmd_halt    db 'halt',0
cmd_mem     db 'mem',0
cmd_ascii   db 'ascii',0
cmd_color   db 'color',0
cmd_calc    db 'calc',0
cmd_browse db 'browse',0

banner db '================================================',13
       db '  Ghost OS v0.5.2',13
       db '  Minimal 16-bit Assembly OS / BIOS only',13
       db '================================================',13
       db '  Type help for commands. Type gui for dashboard.',13,13,0

prompt db 'ghost> ',0

help_msg db 'Commands:',13
         db '  help           show this help',13
         db '  clear / cls    clear screen',13
         db '  gui            show text dashboard',13
         db '  echo TEXT      print text',13
         db '  about          system info',13
         db '  version        show version',13
         db '  calc 2+3       one-digit add/sub calculator',13
         db '  mem            show conventional memory KB',13
         db '  ascii          show internal ASCII logo',13
         db '  color N        set text color 0-F',13
         db '  reboot         BIOS reboot',13
         db '  halt           stop CPU',13,0

gui_msg db '+------------------------------------------------+',13
        db '|                  GHOST OS GUI                  |',13
        db '+------------------------------------------------+',13
        db '| Mode      : 16-bit real mode                   |',13
        db '| Kernel    : NASM Assembly                      |',13
        db '| Video     : BIOS text mode 80x25               |',13
        db '| Shell     : internal CLI                       |',13
        db '| Filesystem: not yet implemented                |',13
        db '+------------------------------------------------+',13
        db '| Commands: help  cls  echo  calc  mem  color    |',13
        db '|           about version ascii reboot halt      |',13
        db '+------------------------------------------------+',13,13,0

browser_page db '+------------------------------------------------+',13
             db '|                GHOST BROWSER v0.1              |',13
             db '+------------------------------------------------+',13
             db '| URL: ghost://home                              |',13
             db '+------------------------------------------------+',13
             db 13
             db '# Welcome to GhostNet',13
             db 13
             db 'This is the first local text-mode page rendered',13
             db 'inside Ghost OS.',13
             db 13
             db 'Supported pseudo-tags:',13
             db '  <title>  <h1>  <p>  <a>  <pre>  <hr>',13
             db 13
             db 'Links:',13
             db '  [1] ghost://about',13
             db '  [2] ghost://help',13
             db 13
             db 'Real network/Tor support will come later, after',13
             db 'filesystem, protected mode and network stack.',13
             db 13
             db '+------------------------------------------------+',13,13,0

about_msg db 'Ghost OS v0.5.2',13
          db 'Tiny educational OS written from scratch in NASM.',13
          db 'No Linux, no DOS, no external kernel. BIOS only.',13
          db 'Goal: small CLI-first assembly operating system.',13,0

version_msg db 'Ghost OS v0.5.2 / x86 16-bit real mode / NASM',13,0

unknown_msg db 'Unknown command. Type help.',13,0
reboot_msg  db 'Rebooting...',13,0
halt_msg    db 'System halted.',13,0
mem_msg     db 'Conventional memory: ',0
kb_msg      db ' KB',13,0
result_msg  db 'Result: ',0

negative_msg db 'Result is negative. v0.5.2 supports unsigned display only.',13,0
calc_usage  db 'Usage: calc 2+3 or calc 8-5. One digit numbers only.',13,0
color_usage db 'Usage: color N where N is 0-F.',13,0
color_ok    db 'Color changed.',13,0

ascii_msg db '          __',13
          db '         / _)  Ghost OS',13
          db '  .-^^^-/ /',13
          db ' __/       /',13
          db '<__.|_|-|_|',13,0

times 16384-($-$$) db 0