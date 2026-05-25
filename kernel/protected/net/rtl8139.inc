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
rtl8139_tx_len dd 0
rtl8139_io_base dw 0C000h


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
    mov ecx, [rtl8139_tx_len]
    cmp ecx, 0
    jne .have_len
    mov ecx, 42
    .have_len:
    xor ebx, ebx

.dump_loop:
    lodsb
    call pm_print_hex8

    mov al, ' '
    call pm_print_char

    inc ebx
    cmp ebx, 16
    jne .continue

    call pm_newline
    xor ebx, ebx

.continue:
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

rtl8139_pingfake:
    cmp byte [rtl8139_initialized], 1
    jne .not_ready

    mov edi, rtl8139_tx_buffer

    ; Ethernet DEST broadcast
    mov ecx, 6
    mov al, 0FFh
    rep stosb

    ; Ethernet SRC rtl8139_mac
    mov esi, rtl8139_mac
    mov ecx, 6
    
.copy_mac:
    lodsb
    stosb
    loop .copy_mac

    ; EtherType IPv4 0x0800
    mov al, 08h
    stosb
    mov al, 00h
    stosb

    ; Minimal fake IPv4 header
    mov al, 45h        ; version/IHL
    stosb
    mov al, 00h        ; DSCP
    stosb
    mov al, 00h        ; total length high
    stosb
    mov al, 1Ch        ; total length low = 28
    stosb
    mov al, 00h        ; ID
    stosb
    mov al, 01h
    stosb
    mov al, 00h        ; flags/fragment
    stosb
    mov al, 00h
    stosb
    mov al, 40h        ; TTL
    stosb
    mov al, 01h        ; ICMP
    stosb
    mov al, 00h        ; checksum placeholder
    stosb
    mov al, 00h
    stosb

    ; SRC IP 10.0.2.15
    mov al, 0Ah
    stosb
    mov al, 00h
    stosb
    mov al, 02h
    stosb
    mov al, 0Fh
    stosb

    ; DST IP 10.0.2.2
    mov al, 0Ah
    stosb
    mov al, 00h
    stosb
    mov al, 02h
    stosb
    mov al, 02h
    stosb

    ; ICMP echo request
    mov al, 08h        ; type echo request
    stosb
    mov al, 00h        ; code
    stosb
    mov al, 00h        ; checksum placeholder
    stosb
    mov al, 00h
    stosb
    mov al, 12h        ; identifier
    stosb
    mov al, 34h
    stosb
    mov al, 00h        ; sequence
    stosb
    mov al, 01h
    stosb

    mov dword [rtl8139_tx_len], 42

    inc dword [rtl8139_tx_count]

    mov esi, rtl8139_pingfake_msg
    call pm_print_string
    ret

.not_ready:
    inc dword [rtl8139_error_count]
    mov esi, rtl8139_not_ready_msg
    call pm_print_string
    ret

rtl8139_pingfake_msg db 'Fake ICMP echo packet built',13,0

rtl8139_pktinfo:
    mov esi, rtl8139_pktinfo_msg
    call pm_print_string
    ret

rtl8139_pktinfo_msg db 'Packet:',13
                    db 'Ethernet -> ARP Request',13,0

pktdump_eth_hdr db 'Ethernet Header',13,0
pktdump_dst db '  DST MAC: ',0
pktdump_src db '  SRC MAC: ',0
pktdump_type db '  EtherType: ',0


rtl8139_arpfake:
    cmp byte [rtl8139_initialized], 1
    jne .not_ready

    mov edi, rtl8139_tx_buffer

    ; Ethernet DEST broadcast
    mov ecx, 6
    mov al, 0FFh
    rep stosb

    ; Ethernet SRC MAC
    mov esi, rtl8139_mac
    mov ecx, 6

.copy_mac:
    lodsb
    stosb
    loop .copy_mac

    ; EtherType ARP = 0x0806
    mov al, 08h
    stosb
    mov al, 06h
    stosb

    ; Hardware type Ethernet
    mov al, 00h
    stosb
    mov al, 01h
    stosb

    ; Protocol IPv4
    mov al, 08h
    stosb
    mov al, 00h
    stosb

    ; Hardware size
    mov al, 06h
    stosb

    ; Protocol size
    mov al, 04h
    stosb

    ; Opcode request
    mov al, 00h
    stosb
    mov al, 01h
    stosb

    ; Sender MAC
    mov esi, rtl8139_mac
    mov ecx, 6

.sender_mac:
    lodsb
    stosb
    loop .sender_mac

    ; Sender IP = 10.0.2.15
    mov al, 0Ah
    stosb
    mov al, 00h
    stosb
    mov al, 02h
    stosb
    mov al, 0Fh
    stosb

    ; Target MAC = 00:00:00:00:00:00
    xor eax, eax
    mov ecx, 6
    rep stosb

    ; Target IP = 10.0.2.2
    mov al, 0Ah
    stosb
    mov al, 00h
    stosb
    mov al, 02h
    stosb
    mov al, 02h
    stosb
    
    mov dword [rtl8139_tx_len], 42

    inc dword [rtl8139_tx_count]

    mov esi, rtl8139_arpfake_msg
    call pm_print_string
    ret

.not_ready:
    inc dword [rtl8139_error_count]
    mov esi, rtl8139_not_ready_msg
    call pm_print_string
    ret

rtl8139_arpfake_msg db 'Fake ARP request packet built',13,0

rtl8139_pktdump:
    mov esi, pktdump_eth_hdr
    call pm_print_string

    ; DST MAC
    mov esi, pktdump_dst
    call pm_print_string

    mov esi, rtl8139_tx_buffer
    call rtl8139_dump_mac
    call pm_newline

    ; SRC MAC
    mov esi, pktdump_src
    call pm_print_string

    mov esi, rtl8139_tx_buffer + 6
    call rtl8139_dump_mac
    call pm_newline

    ; EtherType
    mov esi, pktdump_type
    call pm_print_string

    mov esi, rtl8139_tx_buffer + 12

    lodsb
    call pm_print_hex8

    mov al, ' '
    call pm_print_char

    lodsb
    call pm_print_hex8

    call pm_newline

    mov esi, pktdump_arp_hdr
    call pm_print_string

    mov esi, pktdump_arp_opcode
    call pm_print_string
    mov esi, rtl8139_tx_buffer + 20
    lodsb
    call pm_print_hex8
    mov al, ' '
    call pm_print_char
    lodsb
    call pm_print_hex8
    call pm_newline

    mov esi, pktdump_sender_mac
    call pm_print_string
    mov esi, rtl8139_tx_buffer + 22
    call rtl8139_dump_mac
    call pm_newline

    mov esi, pktdump_sender_ip
    call pm_print_string
    mov esi, rtl8139_tx_buffer + 28
    call rtl8139_dump_ip
    call pm_newline

    mov esi, pktdump_target_mac
    call pm_print_string
    mov esi, rtl8139_tx_buffer + 32
    call rtl8139_dump_mac
    call pm_newline

    mov esi, pktdump_target_ip
    call pm_print_string
    mov esi, rtl8139_tx_buffer + 38
    call rtl8139_dump_ip
    call pm_newline

    ret

rtl8139_dump_ip:
    mov ecx, 4

.loop:
    lodsb
    movzx eax, al
    call pm_print_uint32

    cmp ecx, 1
    je .done

    mov al, '.'
    call pm_print_char

    loop .loop

.done:
    ret

pktdump_arp_hdr db 'ARP Header',13,0
pktdump_arp_opcode db '  Opcode: ',0
pktdump_sender_mac db '  Sender MAC: ',0
pktdump_sender_ip db '  Sender IP : ',0
pktdump_target_mac db '  Target MAC: ',0
pktdump_target_ip db '  Target IP : ',0

rtl8139_dump_mac:
    mov ecx, 6

.loop:
    lodsb
    call pm_print_hex8

    cmp ecx, 1
    je .done

    mov al, ' '
    call pm_print_char

    loop .loop

.done:
    ret

rtl8139_txreal:
    cmp byte [rtl8139_initialized], 1
    jne .not_ready

    cmp dword [rtl8139_tx_len], 0
    je .no_packet

    ; TSAD0 = physical address of TX buffer
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_TSAD0
    mov eax, rtl8139_tx_buffer
    out dx, eax

    ; TSD0 = packet length
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_TSD0
    mov eax, [rtl8139_tx_len]
    out dx, eax

    mov esi, rtl8139_txreal_sent_msg
    call pm_print_string
    ret

.no_packet:
    mov esi, rtl8139_txreal_no_packet_msg
    call pm_print_string
    ret

.not_ready:
    inc dword [rtl8139_error_count]
    mov esi, rtl8139_not_ready_msg
    call pm_print_string
    ret

rtl8139_txreal_sent_msg db 'RTL8139 TX submitted to hardware',13,0
rtl8139_txreal_no_packet_msg db 'RTL8139 TX failed: no packet built',13,0

rtl8139_txstatus:
    mov esi, rtl8139_txstatus_msg
    call pm_print_string

    mov dx, [rtl8139_io_base]
    add dx, RTL8139_TSD0
    in eax, dx

    call pm_print_hex32
    call pm_newline
    ret

rtl8139_txstatus_msg db 'RTL8139 TSD0: ',0