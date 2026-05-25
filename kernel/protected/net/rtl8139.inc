; RTL8139 raw Ethernet driver - v1.5.5-alpha

RTL8139_IDR0      equ 0x00
RTL8139_TSD0      equ 0x10
RTL8139_TSAD0     equ 0x20
RTL8139_RBSTART   equ 0x30
RTL8139_CR        equ 0x37
RTL8139_IMR       equ 0x3C
RTL8139_ISR       equ 0x3E
RTL8139_RCR       equ 0x44
RTL8139_CONFIG1   equ 0x52

rtl8139_initialized db 0
rtl8139_tx_count dd 0
rtl8139_rx_count dd 0
rtl8139_error_count dd 0

rtl8139_mac:
db 52h,54h,00h,12h,34h,56h
rtl8139_random_seed dd 12345678h


rtl8139_netup:
    mov esi, rtl8139_netup_msg
    call pm_print_string
    mov byte [rtl8139_initialized], 1
    ret

rtl8139_netup_msg db 'RTL8139 netup skeleton OK',13,0


rtl8139_netstat:
    mov esi, rtl8139_status_msg
    call pm_print_string

    mov esi, rtl8139_init_msg
    call pm_print_string
    movzx eax, byte [rtl8139_initialized]
    call pm_print_uint32
    call pm_newline

    mov esi, rtl8139_tx_msg
    call pm_print_string
    mov eax, [rtl8139_tx_count]
    call pm_print_uint32
    call pm_newline

    mov esi, rtl8139_rx_msg
    call pm_print_string
    mov eax, [rtl8139_rx_count]
    call pm_print_uint32
    call pm_newline

    mov esi, rtl8139_err_msg
    call pm_print_string
    mov eax, [rtl8139_error_count]
    call pm_print_uint32
    call pm_newline
    ret

rtl8139_status_msg db 'RTL8139 status:',13,0
rtl8139_init_msg   db 'initialized=',0
rtl8139_tx_msg     db 'tx_count=',0
rtl8139_rx_msg     db 'rx_count=',0
rtl8139_err_msg    db 'errors=',0

rtl8139_txdemo:
    cmp byte [rtl8139_initialized], 1
    jne .not_ready

    inc dword [rtl8139_tx_count]

    mov edi, rtl8139_tx_buffer

    ; DEST MAC FF:FF:FF:FF:FF:FF
    mov ecx, 6
    mov al, 0FFh
    rep stosb

    ; SRC MAC from rtl8139_mac
    mov esi, rtl8139_mac
    mov ecx, 6

.copy_mac:
    lodsb
    stosb
    loop .copy_mac

    ; EtherType 0x88B5
    mov al, 88h
    stosb
    mov al, 0B5h
    stosb

    ; Payload "GHOST"
    mov al, 'G'
    stosb
    mov al, 'H'
    stosb
    mov al, 'O'
    stosb
    mov al, 'S'
    stosb
    mov al, 'T'
    stosb

    mov esi, rtl8139_txdemo_msg
    call pm_print_string
    ret

.not_ready:
    inc dword [rtl8139_error_count]
    mov esi, rtl8139_not_ready_msg
    call pm_print_string
    ret

rtl8139_txdemo_msg db 'RTL8139 fake ethernet frame built',13,0
rtl8139_not_ready_msg db 'RTL8139 not initialized. Run netup first.',13,0

rtl8139_rxdump:
    cmp byte [rtl8139_initialized], 1
    jne .not_ready

    mov esi, rtl8139_rxdump_empty_msg
    call pm_print_string
    ret

.not_ready:
    inc dword [rtl8139_error_count]
    mov esi, rtl8139_not_ready_msg
    call pm_print_string
    ret

rtl8139_rxdump_empty_msg db 'RTL8139 rxdump skeleton: no packet',13,0

rtl8139_netreset:
    mov byte [rtl8139_initialized], 0
    mov dword [rtl8139_tx_count], 0
    mov dword [rtl8139_rx_count], 0
    mov dword [rtl8139_error_count], 0

    mov esi, rtl8139_reset_msg
    call pm_print_string
    ret

rtl8139_reset_msg db 'RTL8139 driver state reset',13,0


rtl8139_netclear:
    mov dword [rtl8139_tx_count], 0
    mov dword [rtl8139_rx_count], 0
    mov dword [rtl8139_error_count], 0

    mov esi, rtl8139_clear_msg
    call pm_print_string
    ret

rtl8139_clear_msg db 'RTL8139 counters cleared',13,0

rtl8139_txdump:
    mov esi, rtl8139_dump_msg
    call pm_print_string

    mov esi, rtl8139_tx_buffer
    mov ecx, 19

.dump_loop:
    lodsb
    call pm_print_hex32
    mov al, ' '
    call pm_print_char
    loop .dump_loop

    call pm_newline
    ret

rtl8139_dump_msg db 'TX frame dump:',13,0

rtl8139_tx_buffer:
times 64 db 0

rtl8139_print_mac:
    mov esi, rtl8139_mac_msg
    call pm_print_string

    mov esi, rtl8139_mac
    mov ecx, 6

.loop:
    lodsb
    movzx eax, al
    call pm_print_hex32

    cmp ecx, 1
    je .done

    mov al, ':'
    call pm_print_char

    loop .loop

.done:
    call pm_newline
    ret

rtl8139_mac_msg db 'RTL8139 MAC: ',0

rtl8139_setmac:
    mov esi, pm_cmd_buffer

    call pm_skip_first_word
    call pm_skip_spaces

    cmp byte [esi], 0
    je .usage

    mov edi, rtl8139_mac
    mov ecx, 6

.parse_byte:
    call pm_skip_spaces

    ; first hex char
    mov al, [esi]
    cmp al, 0
    je .usage
    call hex_char_to_nibble
    shl al, 4
    mov bl, al

    ; second hex char
    inc esi
    mov al, [esi]
    cmp al, 0
    je .usage
    call hex_char_to_nibble

    or al, bl
    stosb

    inc esi
    loop .parse_byte

    mov esi, rtl8139_setmac_ok
    call pm_print_string
    ret

.usage:
    mov esi, rtl8139_setmac_usage
    call pm_print_string
    ret


rtl8139_setmac_ok db 'MAC updated',13,0
rtl8139_setmac_usage db 'Usage: setmac AA BB CC DD EE FF',13,0

hex_char_to_nibble:
    cmp al, '0'
    jb .bad
    cmp al, '9'
    jbe .num

    cmp al, 'A'
    jb .lower
    cmp al, 'F'
    jbe .upper

.lower:
    cmp al, 'a'
    jb .bad
    cmp al, 'f'
    ja .bad

    sub al, 'a' - 10
    ret

.upper:
    sub al, 'A' - 10
    ret

.num:
    sub al, '0'
    ret

.bad:
    xor al, al
    ret

rtl8139_random32:
    mov eax, [rtl8139_random_seed]
    imul eax, eax, 1664525
    add eax, 1013904223
    mov [rtl8139_random_seed], eax
    ret

rtl8139_randommac:
    mov edi, rtl8139_mac
    mov ecx, 6

.fill:
    call rtl8139_random32
    stosb
    loop .fill

    ; locally administered
    or byte [rtl8139_mac], 00000010b

    ; unicast
    and byte [rtl8139_mac], 11111110b

    mov esi, rtl8139_randommac_msg
    call pm_print_string
    ret

rtl8139_randommac_msg db 'Random MAC generated',13,0