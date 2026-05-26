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
RTL8139_ISR_ROK   equ 0001h
RTL8139_ISR_RXER  equ 0002h
RTL8139_ISR_TOK   equ 0004h
RTL8139_ISR_TXER  equ 0008h
RTL8139_ISR_NETMASK equ RTL8139_ISR_ROK | RTL8139_ISR_RXER | RTL8139_ISR_TOK | RTL8139_ISR_TXER
RTL8139_CAPR equ 0x38
RTL8139_CBR  equ 0x3A
RTL8139_RX_RING_MASK equ 00001FFFh
RTL8139_TSD_OWN   equ 00002000h
RTL8139_TSD_TUN   equ 00004000h
RTL8139_TSD_TOK   equ 00008000h
RTL8139_TSD_TABT  equ 40000000h

rtl8139_initialized db 0
rtl8139_tx_count dd 0
rtl8139_rx_count dd 0
rtl8139_error_count dd 0
rtl8139_tx_len dd 0
rtl8139_io_base dw 0C000h
rtl8139_bar_raw dd 0
rtl8139_bar_index db 0
rtl8139_rx_offset dd 0
rtl8139_last_cbr dw 0
rtl8139_capr_snapshot dw 0
rtl8139_cbr_snapshot dw 0
rtl8139_gateway_mac_known db 0
rtl8139_gateway_mac:
times 6 db 0
rtl8139_arp_reply_count dd 0
rtl8139_icmp_reply_count dd 0
rtl8139_local_ip db 10,0,2,15
rtl8139_gateway_ip db 10,0,2,2

rtl8139_mac:
db 52h,54h,00h,12h,34h,56h
rtl8139_random_seed dd 12345678h

rtl8139_netbar:
    call rtl8139_bind_pci_io
    cmp byte [rtl8139_bar_index], 1
    je .bar1

    mov esi, rtl8139_bar0_raw_msg
    jmp .print_raw

.bar1:
    mov esi, rtl8139_bar1_raw_msg

.print_raw:
    call pm_print_string
    mov eax, [rtl8139_bar_raw]
    call pm_print_hex32
    call pm_newline

    mov esi, rtl8139_netbar_msg
    call pm_print_string
    movzx eax, word [rtl8139_io_base]
    call pm_print_hex32
    call pm_newline
    ret

rtl8139_bar0_raw_msg db 'BAR0 raw: ',0
rtl8139_bar1_raw_msg db 'BAR1 raw: ',0
rtl8139_netbar_msg db 'RTL8139 IO base: ',0

rtl8139_netup:
    call rtl8139_bind_pci_io
    mov esi, rtl8139_netup_msg
    call pm_print_string
    mov byte [rtl8139_initialized], 0
    call rtl8139_rx_init
    jc .failed
    mov byte [rtl8139_initialized], 1
    call rtl8139_rxstatus
    ret

.failed:
    inc dword [rtl8139_error_count]
    mov esi, rtl8139_netup_failed_msg
    call pm_print_string
    ret

rtl8139_netup_msg db 'RTL8139 netup',13,0
rtl8139_netup_failed_msg db 'RTL8139 netup failed',13,0


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

    mov esi, rtl8139_rxready_msg
    call pm_print_string
    movzx eax, byte [rtl8139_rx_ready]
    call pm_print_uint32
    call pm_newline

    ret

rtl8139_status_msg db 'RTL8139 status:',13,0
rtl8139_init_msg   db 'initialized=',0
rtl8139_tx_msg     db 'tx_count=',0
rtl8139_rx_msg     db 'rx_count=',0
rtl8139_err_msg    db 'errors=',0
rtl8139_rxready_msg db 'rx_ready=',0

rtl8139_txdemo:
    cmp byte [rtl8139_initialized], 1
    jne .not_ready

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
    mov byte [rtl8139_rx_ready], 0
    mov byte [rtl8139_gateway_mac_known], 0
    mov dword [rtl8139_tx_count], 0
    mov dword [rtl8139_rx_count], 0
    mov dword [rtl8139_error_count], 0
    mov dword [rtl8139_rx_offset], 0
    mov word [rtl8139_last_cbr], 0
    mov dword [rtl8139_arp_reply_count], 0
    mov dword [rtl8139_icmp_reply_count], 0

    mov esi, rtl8139_reset_msg
    call pm_print_string
    ret

rtl8139_reset_msg db 'RTL8139 driver state reset',13,0


rtl8139_netclear:
    mov dword [rtl8139_tx_count], 0
    mov dword [rtl8139_rx_count], 0
    mov dword [rtl8139_error_count], 0
    mov dword [rtl8139_arp_reply_count], 0
    mov dword [rtl8139_icmp_reply_count], 0

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

align 16, db 0
rtl8139_tx_buffer:
times 2048 db 0

align 256, db 0
rtl8139_rx_buffer:
times 8192+16+1500 db 0

rtl8139_rx_ready db 0

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
    call rtl8139_pinggw
    ret

rtl8139_pinggw:
    cmp byte [rtl8139_initialized], 1
    jne .not_ready

    mov edi, rtl8139_tx_buffer

    ; Ethernet DEST: learned gateway MAC, or broadcast until ARP succeeds.
    cmp byte [rtl8139_gateway_mac_known], 1
    jne .broadcast_dst

    mov esi, rtl8139_gateway_mac
    mov ecx, 6

.copy_dst:
    lodsb
    stosb
    loop .copy_dst
    jmp .dst_done

.broadcast_dst:
    mov ecx, 6
    mov al, 0FFh
    rep stosb

.dst_done:

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
    mov esi, rtl8139_local_ip
    mov ecx, 4
.copy_src_ip:
    lodsb
    stosb
    loop .copy_src_ip

    ; DST IP 10.0.2.2
    mov esi, rtl8139_gateway_ip
    mov ecx, 4
.copy_dst_ip:
    lodsb
    stosb
    loop .copy_dst_ip

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

    mov esi, rtl8139_tx_buffer + 14
    mov ecx, 20
    call rtl8139_checksum
    mov [rtl8139_tx_buffer + 24], ah
    mov [rtl8139_tx_buffer + 25], al

    mov esi, rtl8139_tx_buffer + 34
    mov ecx, 8
    call rtl8139_checksum
    mov [rtl8139_tx_buffer + 36], ah
    mov [rtl8139_tx_buffer + 37], al

    mov dword [rtl8139_tx_len], 42

    mov esi, rtl8139_pingfake_msg
    call pm_print_string
    ret

.not_ready:
    inc dword [rtl8139_error_count]
    mov esi, rtl8139_not_ready_msg
    call pm_print_string
    ret

rtl8139_pingfake_msg db 'ICMP echo request for gateway built',13,0

rtl8139_checksum:
    push ebx
    push ecx
    push edx
    push esi

    xor edx, edx

.loop:
    cmp ecx, 2
    jb .maybe_odd

    xor eax, eax
    lodsb
    shl eax, 8
    movzx ebx, byte [esi]
    inc esi
    or eax, ebx
    add edx, eax
    sub ecx, 2
    jmp .loop

.maybe_odd:
    cmp ecx, 0
    je .fold
    xor eax, eax
    lodsb
    shl eax, 8
    add edx, eax

.fold:
    mov eax, edx
    shr eax, 16
    and edx, 0FFFFh
    add edx, eax
    mov eax, edx
    shr eax, 16
    add dx, ax
    not dx
    mov ax, dx

    pop esi
    pop edx
    pop ecx
    pop ebx
    ret

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

rtl8139_dump_mac_colon:
    mov ecx, 6

.loop:
    lodsb
    call pm_print_hex8

    cmp ecx, 1
    je .done

    mov al, ':'
    call pm_print_char

    loop .loop

.done:
    ret

rtl8139_txreal:
    cmp byte [rtl8139_initialized], 1
    jne .not_ready

    call rtl8139_build_tx_min_frame

    mov esi, rtl8139_tx_len_msg
    call pm_print_string
    mov eax, [rtl8139_tx_len]
    call pm_print_uint32
    call pm_newline

    mov esi, rtl8139_tx_buffer_msg
    call pm_print_string
    mov eax, rtl8139_tx_buffer
    call pm_print_hex32
    call pm_newline

    mov esi, rtl8139_tsd_before_msg
    call pm_print_string
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_TSD0
    in eax, dx
    call pm_print_hex32
    call pm_newline

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

    mov dx, [rtl8139_io_base]
    add dx, RTL8139_TSD0
    mov ecx, 100000

.wait_tx:
    in eax, dx
    test eax, RTL8139_TSD_TOK | RTL8139_TSD_TABT | RTL8139_TSD_TUN
    jnz .tx_done
    loop .wait_tx

.tx_done:
    mov ebx, eax
    mov esi, rtl8139_tsd_after_msg
    call pm_print_string
    mov eax, ebx
    call pm_print_hex32
    call pm_newline
    call rtl8139_print_tsd_flags

    inc dword [rtl8139_tx_count]

    mov esi, rtl8139_txreal_sent_msg
    call pm_print_string
    ret

.not_ready:
    inc dword [rtl8139_error_count]
    mov esi, rtl8139_not_ready_msg
    call pm_print_string
    ret

rtl8139_build_tx_min_frame:
    pushad

    mov edi, rtl8139_tx_buffer
    mov ecx, 2048
    xor eax, eax
    rep stosb

    mov edi, rtl8139_tx_buffer

    ; Destination MAC: broadcast.
    mov ecx, 6
    mov al, 0FFh
    rep stosb

    ; Source MAC.
    mov esi, rtl8139_mac
    mov ecx, 6
.copy_src:
    lodsb
    stosb
    loop .copy_src

    ; EtherType 0x88B5, GhostOS local test frame.
    mov al, 88h
    stosb
    mov al, 0B5h
    stosb

    mov esi, rtl8139_tx_payload
    mov ecx, rtl8139_tx_payload_len
.copy_payload:
    lodsb
    stosb
    loop .copy_payload

    mov dword [rtl8139_tx_len], 60

    popad
    ret

rtl8139_print_tsd_flags:
    pushad

    test ebx, RTL8139_TSD_TOK
    jz .tabt
    mov esi, rtl8139_tsd_tok_msg
    call pm_print_string

.tabt:
    test ebx, RTL8139_TSD_TABT
    jz .tun
    mov esi, rtl8139_tsd_tabt_msg
    call pm_print_string

.tun:
    test ebx, RTL8139_TSD_TUN
    jz .own
    mov esi, rtl8139_tsd_tun_msg
    call pm_print_string

.own:
    test ebx, RTL8139_TSD_OWN
    jz .done
    mov esi, rtl8139_tsd_own_msg
    call pm_print_string

.done:
    popad
    ret

rtl8139_txreal_sent_msg db 'RTL8139 TX submitted to hardware',13,0
rtl8139_tx_len_msg db 'TX len: ',0
rtl8139_tx_buffer_msg db 'TX buffer: ',0
rtl8139_tsd_before_msg db 'TSD0 before: ',0
rtl8139_tsd_after_msg db 'TSD0 after: ',0
rtl8139_tsd_tok_msg db 'TOK',13,0
rtl8139_tsd_tabt_msg db 'TABT',13,0
rtl8139_tsd_tun_msg db 'TUN',13,0
rtl8139_tsd_own_msg db 'OWN',13,0
rtl8139_tx_payload db 'GHOSTOS TX'
rtl8139_tx_payload_len equ $ - rtl8139_tx_payload

rtl8139_txstatus:
    mov esi, rtl8139_txstatus_msg
    call pm_print_string

    mov dx, [rtl8139_io_base]
    add dx, RTL8139_TSD0
    in eax, dx

    mov ebx, eax
    call pm_print_hex32
    call pm_newline
    call rtl8139_print_tsd_flags
    ret

rtl8139_txstatus_msg db 'RTL8139 TSD0: ',0

rtl8139_rx_init:
    mov byte [rtl8139_rx_ready], 0
    mov dword [rtl8139_rx_offset], 0

    ; Software reset with a bounded wait so netup cannot hang forever.
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_CR
    mov al, 10h
    out dx, al

    mov ecx, 100000
.reset_wait:
    in al, dx
    test al, 10h
    jz .reset_done
    loop .reset_wait

    mov esi, rtl8139_reset_timeout_msg
    call pm_print_string
    stc
    ret

.reset_done:
    ; RX ring base.
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_RBSTART
    mov eax, rtl8139_rx_buffer
    out dx, eax

    ; Current Address of Packet Read starts at zero.
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_CAPR
    xor ax, ax
    out dx, ax

    ; Accept physical/multicast/broadcast/all, WRAP, unlimited DMA burst.
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_RCR
    mov eax, 0000078Fh
    out dx, eax

    ; Enable the events we poll from ISR.
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_IMR
    mov ax, RTL8139_ISR_NETMASK
    out dx, ax

    ; Clear pending interrupt status before enabling RX/TX.
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_ISR
    mov ax, 0FFFFh
    out dx, ax

    ; Enable RX + TX.
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_CR
    mov al, 0Ch
    out dx, al

    mov dx, [rtl8139_io_base]
    add dx, RTL8139_CBR
    in ax, dx
    mov [rtl8139_last_cbr], ax

    call rtl8139_netup_debug

    mov byte [rtl8139_rx_ready], 1

    mov esi, rtl8139_rx_ready_msg
    call pm_print_string
    clc
    ret

rtl8139_rx_ready_msg db 'RTL8139 RX buffer ready',13,0
rtl8139_reset_timeout_msg db 'RTL8139 reset timeout',13,0

rtl8139_netup_debug:
    mov esi, rtl8139_rxbuf_phys_msg
    call pm_print_string
    mov eax, rtl8139_rx_buffer
    call pm_print_hex32
    call pm_newline

    mov esi, rtl8139_rbstart_msg
    call pm_print_string
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_RBSTART
    in eax, dx
    call pm_print_hex32
    call pm_newline

    mov esi, rtl8139_cr_after_enable_msg
    call pm_print_string
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_CR
    in al, dx
    movzx eax, al
    call pm_print_hex32
    call pm_newline

    mov esi, rtl8139_rcr_final_msg
    call pm_print_string
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_RCR
    in eax, dx
    call pm_print_hex32
    call pm_newline

    mov esi, rtl8139_imr_final_msg
    call pm_print_string
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_IMR
    in ax, dx
    movzx eax, ax
    call pm_print_hex32
    call pm_newline
    ret

rtl8139_rxbuf_phys_msg db 'RX buffer phys: ',0
rtl8139_rbstart_msg db 'RBSTART: ',0
rtl8139_cr_after_enable_msg db 'CR after enable: ',0
rtl8139_rcr_final_msg db 'RCR final: ',0
rtl8139_imr_final_msg db 'IMR final: ',0

rtl8139_rxpoll:
    cmp byte [rtl8139_initialized], 1
    jne .not_ready

    mov esi, rtl8139_rxpoll_msg
    call pm_print_string

    mov dx, [rtl8139_io_base]
    add dx, RTL8139_ISR
    in ax, dx
    mov bx, ax

    movzx eax, ax
    call pm_print_hex32
    call pm_newline

    cmp bx, 0
    jne .have_isr

    call rtl8139_rx_check_cbr_change
    jc .done

    mov esi, rtl8139_rxpoll_none_msg
    call pm_print_string
    ret

.have_isr:
    call rtl8139_print_isr_activity

    test bx, RTL8139_ISR_ROK
    jz .check_tok

    inc dword [rtl8139_rx_count]
    push ebx
    call rtl8139_rx_process_packet
    pop ebx

.check_tok:
    test bx, RTL8139_ISR_TOK
    jz .check_errors

    inc dword [rtl8139_tx_count]

.check_errors:
    test bx, RTL8139_ISR_RXER
    jz .check_txer

    inc dword [rtl8139_error_count]

.check_txer:
    test bx, RTL8139_ISR_TXER
    jz .ack

    inc dword [rtl8139_error_count]

.ack:
    test bx, RTL8139_ISR_ROK
    jnz .write_ack
    push ebx
    call rtl8139_rx_check_cbr_change
    pop ebx

.write_ack:
    ; Acknowledge exactly the ISR bits that were read.
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_ISR
    mov ax, bx
    out dx, ax
.done:
    ret

.not_ready:
    inc dword [rtl8139_error_count]
    mov esi, rtl8139_not_ready_msg
    call pm_print_string
    ret

rtl8139_rxpoll_msg db 'RTL8139 ISR: ',0
rtl8139_rxpoll_none_msg db 'No IRQ events',13,0
rtl8139_rxpoll_rok_msg db 'RX OK',13,0
rtl8139_rxpoll_tok_msg db 'TOK',13,0
rtl8139_rxpoll_rxer_msg db 'RX ERR',13,0
rtl8139_rxpoll_txer_msg db 'TER',13,0

rtl8139_print_isr_activity:
    test bx, RTL8139_ISR_ROK
    jz .rxer
    mov esi, rtl8139_rxpoll_rok_msg
    call pm_print_string

.rxer:
    test bx, RTL8139_ISR_RXER
    jz .tok
    mov esi, rtl8139_rxpoll_rxer_msg
    call pm_print_string

.tok:
    test bx, RTL8139_ISR_TOK
    jz .ter
    mov esi, rtl8139_rxpoll_tok_msg
    call pm_print_string

.ter:
    test bx, RTL8139_ISR_TXER
    jz .done
    mov esi, rtl8139_rxpoll_txer_msg
    call pm_print_string

.done:
    ret


rtl8139_rxpeek:
    cmp byte [rtl8139_rx_ready], 1
    jne .not_ready

    mov esi, rtl8139_rxpeek_msg
    call pm_print_string

    mov esi, rtl8139_rx_buffer
    mov ecx, 64
    call rtl8139_dump_bytes
    ret

.not_ready:
    mov esi, rtl8139_rxpeek_notready_msg
    call pm_print_string
    ret

rtl8139_rxpeek_msg db 'RTL8139 RX buffer peek:',13,0
rtl8139_rxpeek_notready_msg db 'RX buffer not initialized',13,0

rtl8139_rxpacket:
    cmp byte [rtl8139_rx_ready], 1
    jne .not_ready

    call rtl8139_rx_packet
    ret

.not_ready:
    mov esi, rtl8139_rxpeek_notready_msg
    call pm_print_string
    ret

rtl8139_rxstatus:
    mov esi, rtl8139_rxstatus_cr_msg
    call pm_print_string
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_CR
    in al, dx
    movzx eax, al
    call pm_print_hex32
    call pm_newline

    mov esi, rtl8139_rxstatus_rcr_msg
    call pm_print_string
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_RCR
    in eax, dx
    call pm_print_hex32
    call pm_newline

    mov esi, rtl8139_rxstatus_isr_msg
    call pm_print_string
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_ISR
    in ax, dx
    movzx eax, ax
    call pm_print_hex32
    call pm_newline

    mov esi, rtl8139_rxstatus_imr_msg
    call pm_print_string
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_IMR
    in ax, dx
    movzx eax, ax
    call pm_print_hex32
    call pm_newline

    mov esi, rtl8139_rxstatus_capr_msg
    call pm_print_string
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_CAPR
    in ax, dx
    movzx eax, ax
    call pm_print_hex32
    call pm_newline

    mov esi, rtl8139_rxstatus_cbr_msg
    call pm_print_string
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_CBR
    in ax, dx
    movzx eax, ax
    call pm_print_hex32
    call pm_newline

    ret

rtl8139_rxstatus_cr_msg db 'CR: ',0
rtl8139_rxstatus_rcr_msg db 'RCR: ',0
rtl8139_rxstatus_isr_msg db 'ISR: ',0
rtl8139_rxstatus_imr_msg db 'IMR: ',0
rtl8139_rxstatus_capr_msg db 'CAPR: ',0
rtl8139_rxstatus_cbr_msg db 'CBR: ',0

rtl8139_rxkick:
    cmp byte [rtl8139_initialized], 1
    jne .not_ready

    mov dword [rtl8139_rx_offset], 0

    mov dx, [rtl8139_io_base]
    add dx, RTL8139_CAPR
    xor ax, ax
    out dx, ax

    mov dx, [rtl8139_io_base]
    add dx, RTL8139_ISR
    mov ax, 0FFFFh
    out dx, ax

    mov esi, rtl8139_rxkick_msg
    call pm_print_string

    call rtl8139_rxstatus

    mov esi, rtl8139_rxpoll_msg
    call pm_print_string
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_ISR
    in ax, dx
    mov bx, ax
    movzx eax, ax
    call pm_print_hex32
    call pm_newline

    cmp bx, 0
    jne .activity
    mov esi, rtl8139_rxpoll_none_msg
    call pm_print_string
    ret

.activity:
    call rtl8139_print_isr_activity
    ret

.not_ready:
    inc dword [rtl8139_error_count]
    mov esi, rtl8139_not_ready_msg
    call pm_print_string
    ret

rtl8139_rxkick_msg db 'RTL8139 RX kick',13,0

rtl8139_rx_check_cbr_change:
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_CBR
    in ax, dx
    cmp ax, [rtl8139_last_cbr]
    jne .changed
    clc
    ret

.changed:
    mov [rtl8139_last_cbr], ax
    mov esi, rtl8139_cbr_changed_msg
    call pm_print_string
    movzx eax, ax
    call pm_print_hex32
    call pm_newline
    call rtl8139_rx_process_packet
    stc
    ret

rtl8139_rx_process_packet:
    pushad
    call rtl8139_rx_packet
    popad
    ret

rtl8139_rx_packet:
    pushad

    mov dx, [rtl8139_io_base]
    add dx, RTL8139_CAPR
    in ax, dx
    mov [rtl8139_capr_snapshot], ax

    mov dx, [rtl8139_io_base]
    add dx, RTL8139_CBR
    in ax, dx
    mov [rtl8139_cbr_snapshot], ax

    mov ax, [rtl8139_capr_snapshot]
    cmp ax, [rtl8139_cbr_snapshot]
    je .empty

    mov dx, [rtl8139_io_base]
    add dx, RTL8139_CR
    in al, dx
    test al, 01h
    jnz .empty

    movzx eax, word [rtl8139_cbr_snapshot]
    and eax, RTL8139_RX_RING_MASK
    cmp eax, [rtl8139_rx_offset]
    je .empty

    mov ebx, [rtl8139_rx_offset]
    and ebx, RTL8139_RX_RING_MASK
    mov [rtl8139_rx_offset], ebx

    movzx eax, word [rtl8139_rx_buffer + ebx]
    movzx edx, word [rtl8139_rx_buffer + ebx + 2]

    cmp edx, 14
    jb .discard
    cmp edx, 1600
    ja .discard

    mov esi, rtl8139_rxpacket_human_msg
    call pm_print_string

    mov esi, rtl8139_rxpacket_len_msg
    call pm_print_string
    mov eax, edx
    call pm_print_uint32
    call pm_newline

    mov edi, rtl8139_rx_buffer
    add edi, ebx
    add edi, 4

    mov esi, rtl8139_rxpacket_dst_msg
    call pm_print_string
    mov esi, edi
    call rtl8139_dump_mac_colon
    call pm_newline

    mov esi, rtl8139_rxpacket_src_msg
    call pm_print_string
    mov esi, edi
    add esi, 6
    call rtl8139_dump_mac_colon
    call pm_newline

    mov esi, rtl8139_rxpacket_type_msg
    call pm_print_string
    mov al, '0'
    call pm_print_char
    mov al, 'x'
    call pm_print_char
    mov al, [edi + 12]
    call pm_print_hex8
    mov al, [edi + 13]
    call pm_print_hex8
    call pm_newline

    cmp byte [edi + 12], 08h
    jne .advance
    cmp byte [edi + 13], 06h
    je .arp
    cmp byte [edi + 13], 00h
    je .ipv4
    jmp .advance

.arp:
    mov esi, rtl8139_rxpacket_arp_msg
    call pm_print_string
    call rtl8139_rx_parse_current
    jmp .advance

.ipv4:
    mov esi, rtl8139_rxpacket_ipv4_msg
    call pm_print_string
    call rtl8139_rx_parse_current
    jmp .advance

.discard:
    inc dword [rtl8139_error_count]
    mov esi, rtl8139_rxpacket_discard_msg
    call pm_print_string
    call rtl8139_rx_discard_to_cbr
    jmp .done

.advance:
    call rtl8139_rx_advance_current
    jmp .done

.empty:
    mov esi, rtl8139_rx_empty_msg
    call pm_print_string

.done:
    popad
    ret

rtl8139_rx_dump_current:
    pushad

    mov esi, rtl8139_rxpacket_msg
    call pm_print_string

    mov esi, rtl8139_rxpacket_offset_msg
    call pm_print_string
    mov eax, [rtl8139_rx_offset]
    call pm_print_hex32
    call pm_newline

    mov ebx, [rtl8139_rx_offset]

    mov esi, rtl8139_rxpacket_status_msg
    call pm_print_string
    movzx eax, word [rtl8139_rx_buffer + ebx]
    call pm_print_hex32
    call pm_newline

    mov esi, rtl8139_rxpacket_length_msg
    call pm_print_string
    movzx eax, word [rtl8139_rx_buffer + ebx + 2]
    call pm_print_uint32
    call pm_newline

    mov esi, rtl8139_rxpacket_frame_msg
    call pm_print_string
    mov esi, rtl8139_rx_buffer
    add esi, ebx
    add esi, 4
    mov ecx, 64
    call rtl8139_dump_bytes

    popad
    ret

rtl8139_rx_parse_current:
    pushad

    mov ebx, [rtl8139_rx_offset]
    movzx eax, word [rtl8139_rx_buffer + ebx + 2]
    cmp eax, 42
    jb .done

    mov esi, rtl8139_rx_buffer
    add esi, ebx
    add esi, 4

    cmp byte [esi + 12], 08h
    jne .done

    cmp byte [esi + 13], 06h
    je .maybe_arp
    cmp byte [esi + 13], 00h
    je .maybe_ipv4
    jmp .done

.maybe_arp:
    cmp byte [esi + 20], 00h
    jne .done
    cmp byte [esi + 21], 02h
    jne .done
    cmp byte [esi + 28], 10
    jne .done
    cmp byte [esi + 29], 0
    jne .done
    cmp byte [esi + 30], 2
    jne .done
    cmp byte [esi + 31], 2
    jne .done

    push esi
    add esi, 22
    mov edi, rtl8139_gateway_mac
    mov ecx, 6
.copy_gateway_mac:
    lodsb
    stosb
    loop .copy_gateway_mac
    pop esi

    mov byte [rtl8139_gateway_mac_known], 1
    inc dword [rtl8139_arp_reply_count]
    mov esi, rtl8139_arp_learned_msg
    call pm_print_string
    mov esi, rtl8139_gateway_mac
    call rtl8139_dump_mac
    call pm_newline
    jmp .done

.maybe_ipv4:
    cmp byte [esi + 14], 45h
    jne .done
    cmp byte [esi + 23], 01h
    jne .done
    cmp byte [esi + 34], 00h
    jne .done
    cmp byte [esi + 35], 00h
    jne .done

    inc dword [rtl8139_icmp_reply_count]
    mov esi, rtl8139_icmp_reply_msg
    call pm_print_string

.done:
    popad
    ret

rtl8139_rx_advance_current:
    pushad

    mov ebx, [rtl8139_rx_offset]
    movzx eax, word [rtl8139_rx_buffer + ebx + 2]
    cmp eax, 4
    jb .done
    cmp eax, 1600
    ja .done

    add eax, ebx
    add eax, 4
    add eax, 3
    and eax, 0FFFFFFFCh
    and eax, RTL8139_RX_RING_MASK
    mov [rtl8139_rx_offset], eax

    mov dx, [rtl8139_io_base]
    add dx, RTL8139_CAPR
    mov ax, [rtl8139_rx_offset]
    sub ax, 16
    out dx, ax

    mov dx, [rtl8139_io_base]
    add dx, RTL8139_CBR
    in ax, dx
    mov [rtl8139_last_cbr], ax

.done:
    popad
    ret

rtl8139_rx_discard_to_cbr:
    pushad

    movzx eax, word [rtl8139_cbr_snapshot]
    and eax, RTL8139_RX_RING_MASK
    mov [rtl8139_rx_offset], eax

    mov dx, [rtl8139_io_base]
    add dx, RTL8139_CAPR
    mov ax, [rtl8139_rx_offset]
    sub ax, 16
    out dx, ax

    mov ax, [rtl8139_cbr_snapshot]
    mov [rtl8139_last_cbr], ax

    popad
    ret

rtl8139_dump_bytes:
    pushad
    xor ebx, ebx

.loop:
    cmp ecx, 0
    je .done

    lodsb
    call pm_print_hex8
    mov al, ' '
    call pm_print_char

    inc ebx
    dec ecx
    cmp ebx, 16
    jne .loop

    call pm_newline
    xor ebx, ebx
    jmp .loop

.done:
    cmp ebx, 0
    je .out
    call pm_newline

.out:
    popad
    ret

rtl8139_arpstat:
    mov esi, rtl8139_arpstat_msg
    call pm_print_string

    cmp byte [rtl8139_gateway_mac_known], 1
    jne .unknown

    mov esi, rtl8139_gateway_known_msg
    call pm_print_string
    mov esi, rtl8139_gateway_mac
    call rtl8139_dump_mac
    call pm_newline
    ret

.unknown:
    mov esi, rtl8139_gateway_unknown_msg
    call pm_print_string
    ret

rtl8139_netdiag:
    call rtl8139_netstat
    call rtl8139_rxstatus
    call rtl8139_arpstat

    mov esi, rtl8139_arp_replies_msg
    call pm_print_string
    mov eax, [rtl8139_arp_reply_count]
    call pm_print_uint32
    call pm_newline

    mov esi, rtl8139_icmp_replies_msg
    call pm_print_string
    mov eax, [rtl8139_icmp_reply_count]
    call pm_print_uint32
    call pm_newline
    ret

rtl8139_cbr_changed_msg db 'CBR changed: ',0
rtl8139_rx_empty_msg db 'RX empty',13,0
rtl8139_rxpacket_human_msg db 'RX packet',13,0
rtl8139_rxpacket_len_msg db 'len: ',0
rtl8139_rxpacket_dst_msg db 'dst: ',0
rtl8139_rxpacket_src_msg db 'src: ',0
rtl8139_rxpacket_type_msg db 'type: ',0
rtl8139_rxpacket_arp_msg db 'ARP frame',13,0
rtl8139_rxpacket_ipv4_msg db 'IPv4 frame',13,0
rtl8139_rxpacket_discard_msg db 'RX packet discarded',13,0
rtl8139_rxpacket_msg db 'RTL8139 RX packet:',13,0
rtl8139_rxpacket_offset_msg db 'offset=',0
rtl8139_rxpacket_status_msg db 'status=',0
rtl8139_rxpacket_length_msg db 'length=',0
rtl8139_rxpacket_frame_msg db 'frame[0..63]:',13,0
rtl8139_arp_learned_msg db 'ARP reply: gateway MAC ',0
rtl8139_icmp_reply_msg db 'ICMP echo reply',13,0
rtl8139_arpstat_msg db 'ARP state:',13,0
rtl8139_gateway_known_msg db 'gateway_mac=',0
rtl8139_gateway_unknown_msg db 'gateway_mac=unknown',13,0
rtl8139_arp_replies_msg db 'arp_replies=',0
rtl8139_icmp_replies_msg db 'icmp_replies=',0

rtl8139_bind_pci_io:
    call pci_scan_bus
    xor ebx, ebx

.loop:
    cmp ebx, [pm_pci_count]
    jae .fallback

    imul edi, ebx, PCI_RECORD_SIZE
    add edi, pm_pci_records
    call pm_pci_get_nic_type
    jc .next
    cmp al, PCI_NIC_RTL8139
    jne .next

    mov eax, [edi+PCI_REC_BAR0]
    mov dword [rtl8139_bar_raw], eax
    mov byte [rtl8139_bar_index], 0
    call rtl8139_try_bind_io_bar
    jnc .bound

    mov eax, [edi+PCI_REC_BAR1]
    mov dword [rtl8139_bar_raw], eax
    mov byte [rtl8139_bar_index], 1
    call rtl8139_try_bind_io_bar
    jnc .bound

    jmp .fallback

.next:
    inc ebx
    jmp .loop

.bound:
    mov byte [pm_pci_nic_type], PCI_NIC_RTL8139
    mov esi, rtl8139_pci_bound_msg
    call pm_print_string
    ret

.fallback:
    mov word [rtl8139_io_base], 0C000h
    mov dword [rtl8139_bar_raw], 0
    mov byte [rtl8139_bar_index], 0
    mov esi, rtl8139_pci_fallback_msg
    call pm_print_string
    ret

rtl8139_try_bind_io_bar:
    cmp eax, 0
    je .bad
    cmp eax, 0FFFFFFFFh
    je .bad
    test eax, 1
    jz .bad

    and eax, 0FFFFFFFCh
    cmp eax, 0
    je .bad
    cmp eax, 10000h
    jae .bad

    mov [rtl8139_io_base], ax
    mov [pm_pci_io_base], ax
    clc
    ret

.bad:
    stc
    ret

rtl8139_pci_bound_msg db 'RTL8139 PCI IO bound',13,0
rtl8139_pci_fallback_msg db 'RTL8139 PCI bind failed, using fallback',13,0
