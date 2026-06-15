; RTL8139 raw Ethernet driver - v1.5.5-alpha

RTL8139_IDR0      equ 0x00
RTL8139_TSD0      equ 0x10
RTL8139_TSAD0     equ 0x20
RTL8139_RBSTART   equ 0x30
RTL8139_CR        equ 0x37
RTL8139_IMR       equ 0x3C
RTL8139_ISR       equ 0x3E
RTL8139_TCR       equ 0x40
RTL8139_RCR       equ 0x44
RTL8139_MPC       equ 0x4C
RTL8139_9346CR    equ 0x50
RTL8139_CONFIG0   equ 0x51
RTL8139_CONFIG1   equ 0x52
RTL8139_CONFIG3   equ 0x59
RTL8139_CONFIG4   equ 0x5A
RTL8139_MULINT    equ 0x5C
RTL8139_CONFIG5   equ 0xD8
RTL8139_CR_BUFE   equ 00000001b
RTL8139_CR_TE     equ 00000100b
RTL8139_CR_RE     equ 00001000b
RTL8139_CR_RST    equ 00010000b
RTL8139_RCR_AAP   equ 00000001h
RTL8139_RCR_APM   equ 00000002h
RTL8139_RCR_AM    equ 00000004h
RTL8139_RCR_AB    equ 00000008h
RTL8139_RCR_WRAP  equ 00000080h
RTL8139_RCR_MXDMA_1024 equ 00000600h
RTL8139_RCR_MXDMA_UNLIMITED equ 00000700h
RTL8139_RCR_RBLEN_8K equ 00000000h
RTL8139_RCR_RXFTH_16 equ 00000000h
RTL8139_RCR_RXFTH_NONE equ 0000E000h
RTL8139_RCR_QEMU equ RTL8139_RCR_AAP | RTL8139_RCR_APM | RTL8139_RCR_AM | RTL8139_RCR_AB | RTL8139_RCR_RBLEN_8K | RTL8139_RCR_MXDMA_1024 | RTL8139_RCR_RXFTH_16
RTL8139_RCR_PERMISSIVE equ RTL8139_RCR_QEMU | RTL8139_RCR_WRAP | RTL8139_RCR_MXDMA_UNLIMITED
RTL8139_RCR_MINIMAL equ RTL8139_RCR_APM | RTL8139_RCR_AB | RTL8139_RCR_RBLEN_8K | RTL8139_RCR_MXDMA_1024 | RTL8139_RCR_RXFTH_16
RTL8139_TCR_IFG96 equ 00000000h
RTL8139_TCR_MXDMA_1024 equ 00000600h
RTL8139_TCR_MXDMA_UNLIMITED equ 00000700h
RTL8139_TCR_QEMU equ RTL8139_TCR_IFG96 | RTL8139_TCR_MXDMA_1024
RTL8139_TCR_PERMISSIVE equ RTL8139_TCR_IFG96 | RTL8139_TCR_MXDMA_UNLIMITED
RTL8139_TCR_MINIMAL equ RTL8139_TCR_QEMU
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
TCP_STATE_CLOSED equ 0
TCP_STATE_SYN_SENT equ 1
TCP_STATE_ESTABLISHED equ 2

rtl8139_initialized db 0
rtl8139_tx_count dd 0
rtl8139_rx_count dd 0
rtl8139_error_count dd 0
rtl8139_tx_len dd 0
rtl8139_tx_slot db 0
rtl8139_last_tx_slot db 0
rtl8139_last_tsd_before dd 0
rtl8139_last_tsd_after dd 0
rtl8139_rxtest_first_before dd 0
rtl8139_rxtest_first_after dd 0
rtl8139_rxtest_cbr_before dw 0
rtl8139_rxtest_cbr_after dw 0
rtl8139_rxtest_isr_before dw 0
rtl8139_rxtest_isr_after dw 0
rtl8139_rxtest_tsd0 dd 0
rtl8139_cr_before_rcr db 0
rtl8139_rcr_after_write dd 0
rtl8139_io_base dw 0C000h
rtl8139_bar_raw dd 0
rtl8139_bar_index db 0
rtl8139_pci_bus db 0
rtl8139_pci_dev db 0
rtl8139_pci_fn db 0
rtl8139_pci_bdf_valid db 0
rtl8139_pci_cmd_before dw 0
rtl8139_pci_cmd_after dw 0
rtl8139_rx_offset dd 0
rtl8139_last_cbr dw 0
rtl8139_capr_snapshot dw 0
rtl8139_cbr_snapshot dw 0
rtl8139_rx_first_dword dd 0
rtl8139_rx_rbstart_override dd 0
rtl8139_rx_capr_mode db 1
rtl8139_rcr_value dd RTL8139_RCR_QEMU
rtl8139_tcr_value dd RTL8139_TCR_QEMU
rtl8139_rxmode_name dd rtl8139_rxmode_qemu_name
rtl8139_gateway_mac_known db 0
rtl8139_gateway_mac:
times 6 db 0
rtl8139_arp_reply_count dd 0
rtl8139_icmp_reply_count dd 0
rtl8139_udp_tx_count dd 0
rtl8139_udp_rx_count dd 0
rtl8139_ping_seq dw 1
rtl8139_last_icmp_seq dw 0
rtl8139_last_icmp_ttl db 0
rtl8139_last_icmp_src_ip db 0,0,0,0
rtl8139_udp_src_port dw 49152
rtl8139_udp_dst_port dw 1234
rtl8139_last_udp_src_ip db 0,0,0,0
rtl8139_last_udp_src_port dw 0
rtl8139_last_udp_dst_port dw 0
rtl8139_last_udp_len dw 0
rtl8139_tcp_state db TCP_STATE_CLOSED
rtl8139_tcp_tx_count dd 0
rtl8139_tcp_rx_count dd 0
rtl8139_tcp_synack_count dd 0
rtl8139_tcp_ack_tx_count dd 0
rtl8139_tcp_data_tx_count dd 0
rtl8139_tcp_data_rx_count dd 0
rtl8139_tcp_data_ack_tx_count dd 0
rtl8139_tcp_fin_rx_count dd 0
rtl8139_tcp_fin_ack_tx_count dd 0
rtl8139_tcp_last_payload_len dw 0
rtl8139_tcp_last_payload_wire_len dw 0
rtl8139_tcp_src_port dw 49153
rtl8139_tcp_dst_port dw 80
rtl8139_tcp_seq dd 01020304h
rtl8139_tcp_last_src_ip db 0,0,0,0
rtl8139_tcp_last_src_port dw 0
rtl8139_tcp_last_dst_port dw 0
rtl8139_tcp_last_flags db 0
rtl8139_tcp_last_seq dd 0
rtl8139_tcp_last_ack dd 0
rtl8139_pingtest_tsd dd 0
rtl8139_pingtest_isr dw 0
rtl8139_arp_wait_first dd 0
rtl8139_arp_wait_cbr dw 0
rtl8139_local_ip db 10,0,2,15
rtl8139_gateway_ip db 10,0,2,2
rtl8139_ipv4_tmp_protocol db 0

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
    call rtl8139_print_pci_cmd_debug
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
    mov byte [rtl8139_tx_slot], 0
    mov dword [rtl8139_last_tsd_before], 0
    mov dword [rtl8139_last_tsd_after], 0
    mov word [rtl8139_last_cbr], 0
    mov dword [rtl8139_arp_reply_count], 0
    mov dword [rtl8139_icmp_reply_count], 0
    mov dword [rtl8139_udp_tx_count], 0
    mov dword [rtl8139_udp_rx_count], 0
    mov dword [rtl8139_last_udp_src_ip], 0
    mov word [rtl8139_last_udp_src_port], 0
    mov word [rtl8139_last_udp_dst_port], 0
    mov word [rtl8139_last_udp_len], 0
    mov byte [rtl8139_tcp_state], TCP_STATE_CLOSED
    mov dword [rtl8139_tcp_tx_count], 0
    mov dword [rtl8139_tcp_rx_count], 0
    mov dword [rtl8139_tcp_synack_count], 0
    mov dword [rtl8139_tcp_ack_tx_count], 0
    mov dword [rtl8139_tcp_data_tx_count], 0
    mov dword [rtl8139_tcp_data_rx_count], 0
    mov dword [rtl8139_tcp_data_ack_tx_count], 0
    mov dword [rtl8139_tcp_fin_rx_count], 0
    mov dword [rtl8139_tcp_fin_ack_tx_count], 0
    mov word [rtl8139_tcp_last_payload_len], 0
    mov word [rtl8139_tcp_last_payload_wire_len], 0
    mov dword [rtl8139_tcp_last_src_ip], 0
    mov word [rtl8139_tcp_last_src_port], 0
    mov word [rtl8139_tcp_last_dst_port], 0
    mov byte [rtl8139_tcp_last_flags], 0
    mov dword [rtl8139_tcp_last_seq], 0
    mov dword [rtl8139_tcp_last_ack], 0

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
    mov dword [rtl8139_udp_tx_count], 0
    mov dword [rtl8139_udp_rx_count], 0
    mov dword [rtl8139_last_udp_src_ip], 0
    mov word [rtl8139_last_udp_src_port], 0
    mov word [rtl8139_last_udp_dst_port], 0
    mov word [rtl8139_last_udp_len], 0
    mov byte [rtl8139_tcp_state], TCP_STATE_CLOSED
    mov dword [rtl8139_tcp_tx_count], 0
    mov dword [rtl8139_tcp_rx_count], 0
    mov dword [rtl8139_tcp_synack_count], 0
    mov dword [rtl8139_tcp_ack_tx_count], 0
    mov dword [rtl8139_tcp_data_tx_count], 0
    mov dword [rtl8139_tcp_data_rx_count], 0
    mov dword [rtl8139_tcp_data_ack_tx_count], 0
    mov dword [rtl8139_tcp_fin_rx_count], 0
    mov dword [rtl8139_tcp_fin_ack_tx_count], 0
    mov word [rtl8139_tcp_last_payload_len], 0
    mov word [rtl8139_tcp_last_payload_wire_len], 0
    mov dword [rtl8139_tcp_last_src_ip], 0
    mov word [rtl8139_tcp_last_src_port], 0
    mov word [rtl8139_tcp_last_dst_port], 0
    mov byte [rtl8139_tcp_last_flags], 0
    mov dword [rtl8139_tcp_last_seq], 0
    mov dword [rtl8139_tcp_last_ack], 0

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

rtl8139_tcp_checksum_buffer:
times 64 db 0

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

rtl8139_ping_cmd:
    mov esi, pm_cmd_buffer
    call pm_skip_first_word
    call pm_skip_spaces
    cmp byte [esi], 0
    je rtl8139_pinggw

    mov edi, rtl8139_gateway_ip_text
    call pm_starts_with_word
    jc rtl8139_pinggw

    mov esi, rtl8139_ping_usage_msg
    call pm_print_string
    ret

rtl8139_pinggw:
    cmp byte [rtl8139_initialized], 1
    jne .not_ready

    call rtl8139_ensure_gateway_mac
    cmp byte [rtl8139_gateway_mac_known], 1
    jne .no_arp

    mov dword [rtl8139_icmp_reply_count], 0
    call rtl8139_build_icmp_echo_request
    call rtl8139_tx_submit_current_quiet

    mov esi, rtl8139_ping_sent_msg
    call pm_print_string
    mov ecx, 20
.poll:
    push ecx
    call rtl8139_wait_for_rx_activity
    call rtl8139_rx_process_packet_quiet
    pop ecx
    cmp dword [rtl8139_icmp_reply_count], 0
    jne .reply
    loop .poll
    ret

.reply:
    mov esi, rtl8139_icmp_reply_msg
    call pm_print_string
    ret

.no_arp:
    inc dword [rtl8139_error_count]
    mov esi, rtl8139_ping_no_arp_msg
    call pm_print_string
    ret

.not_ready:
    inc dword [rtl8139_error_count]
    mov esi, rtl8139_not_ready_msg
    call pm_print_string
    ret

rtl8139_udpsend_cmd:
    mov esi, pm_cmd_buffer
    call pm_skip_first_word
    call pm_skip_spaces

    mov edi, rtl8139_gateway_ip_text
    call pm_starts_with_word
    jnc .usage

    call pm_skip_first_word
    call pm_skip_spaces
    mov edi, rtl8139_udp_dst_port_text
    call pm_starts_with_word
    jnc .usage

    mov word [rtl8139_udp_dst_port], 1234

    cmp byte [rtl8139_initialized], 1
    je .ready

    call rtl8139_rxtest_quiet_netup
    cmp byte [rtl8139_initialized], 1
    jne .not_ready

.ready:
    call rtl8139_enable_pci_bus_master
    call rtl8139_ensure_gateway_mac
    cmp byte [rtl8139_gateway_mac_known], 1
    jne .no_arp

    call rtl8139_build_udp_packet
    call rtl8139_tx_submit_current_quiet
    inc dword [rtl8139_udp_tx_count]

    mov esi, rtl8139_udp_sent_msg
    call pm_print_string
    ret

.usage:
    mov esi, rtl8139_udpsend_usage_msg
    call pm_print_string
    ret

.no_arp:
    inc dword [rtl8139_error_count]
    mov esi, rtl8139_udp_no_arp_msg
    call pm_print_string
    ret

.not_ready:
    inc dword [rtl8139_error_count]
    mov esi, rtl8139_not_ready_msg
    call pm_print_string
    ret

rtl8139_udpstat:
    mov esi, rtl8139_udpstat_msg
    call pm_print_string

    mov esi, rtl8139_udp_tx_msg
    call pm_print_string
    mov eax, [rtl8139_udp_tx_count]
    call pm_print_uint32
    call pm_newline

    mov esi, rtl8139_udp_rx_msg
    call pm_print_string
    mov eax, [rtl8139_udp_rx_count]
    call pm_print_uint32
    call pm_newline

    mov esi, rtl8139_udp_src_ip_msg
    call pm_print_string
    mov esi, rtl8139_last_udp_src_ip
    call rtl8139_dump_ip
    call pm_newline

    mov esi, rtl8139_udp_src_port_msg
    call pm_print_string
    movzx eax, word [rtl8139_last_udp_src_port]
    call pm_print_uint32
    call pm_newline

    mov esi, rtl8139_udp_dst_port_msg
    call pm_print_string
    movzx eax, word [rtl8139_last_udp_dst_port]
    call pm_print_uint32
    call pm_newline

    mov esi, rtl8139_udp_len_msg
    call pm_print_string
    movzx eax, word [rtl8139_last_udp_len]
    call pm_print_uint32
    call pm_newline
    ret

rtl8139_tcpconnect_cmd:
    mov esi, pm_cmd_buffer
    call pm_skip_first_word
    call pm_skip_spaces

    mov edi, rtl8139_gateway_ip_text
    call pm_starts_with_word
    jnc .usage

    call pm_skip_first_word
    call pm_skip_spaces

    mov edi, rtl8139_tcp_dst_port_4433_text
    call pm_starts_with_word
    jc .port_4433

    mov edi, rtl8139_tcp_dst_port_8080_text
    call pm_starts_with_word
    jc .port_8080

    mov edi, rtl8139_tcp_dst_port_text
    call pm_starts_with_word
    jc .port_80

    jmp .usage

.port_4433:
    mov word [rtl8139_tcp_dst_port], 4433
    jmp .port_ok

.port_8080:
    mov word [rtl8139_tcp_dst_port], 8080
    jmp .port_ok

.port_80:
    mov word [rtl8139_tcp_dst_port], 80

.port_ok:
    cmp byte [rtl8139_initialized], 1
    je .ready

    call rtl8139_rxtest_quiet_netup
    cmp byte [rtl8139_initialized], 1
    jne .not_ready

.ready:
    call rtl8139_enable_pci_bus_master
    call rtl8139_ensure_gateway_mac
    cmp byte [rtl8139_gateway_mac_known], 1
    jne .no_arp

    mov byte [rtl8139_tcp_state], TCP_STATE_SYN_SENT
    mov dword [rtl8139_tcp_synack_count], 0
    call rtl8139_build_tcp_syn
    call rtl8139_tx_submit_current_quiet
    inc dword [rtl8139_tcp_tx_count]

    mov esi, rtl8139_tcp_syn_sent_msg
    call pm_print_string

    mov ecx, 20
.poll:
    push ecx
    call rtl8139_wait_for_rx_activity
    call rtl8139_rx_process_packet_quiet
    pop ecx
    cmp dword [rtl8139_tcp_synack_count], 0
    jne .synack
    loop .poll

    mov esi, rtl8139_tcp_syn_wait_msg
    call pm_print_string
    ret

.synack:
    call rtl8139_build_tcp_ack
    call rtl8139_tx_submit_current_quiet
    inc dword [rtl8139_tcp_ack_tx_count]
    mov byte [rtl8139_tcp_state], TCP_STATE_ESTABLISHED
    mov esi, rtl8139_tcp_established_msg
    call pm_print_string
    ret

.usage:
    mov esi, rtl8139_tcpconnect_usage_msg
    call pm_print_string
    ret

.no_arp:
    inc dword [rtl8139_error_count]
    mov esi, rtl8139_tcp_no_arp_msg
    call pm_print_string
    ret

.not_ready:
    inc dword [rtl8139_error_count]
    mov esi, rtl8139_not_ready_msg
    call pm_print_string
    ret

rtl8139_tcpconnect_start:
    jmp rtl8139_tcpconnect_cmd.port_ok

rtl8139_tcpget_cmd:
    cmp byte [rtl8139_tcp_state], TCP_STATE_ESTABLISHED
    je .established

    mov esi, rtl8139_tcp_not_established_msg
    call pm_print_string
    ret

.established:
    call rtl8139_build_tcp_http_get
    call rtl8139_tx_submit_current_quiet
    inc dword [rtl8139_tcp_tx_count]
    inc dword [rtl8139_tcp_data_tx_count]
    add dword [rtl8139_tcp_seq], rtl8139_tcp_http_get_len

    mov esi, rtl8139_tcp_data_sent_msg
    call pm_print_string
    ret

rtl8139_tlshello_cmd:
    cmp byte [rtl8139_tcp_state], TCP_STATE_ESTABLISHED
    je .established

    mov esi, rtl8139_tcp_not_established_msg
    call pm_print_string
    ret

.established:
    call tls_clienthello_prepare_keyshare
    call rtl8139_build_tcp_tls_clienthello
    call rtl8139_tx_submit_current_quiet
    inc dword [rtl8139_tcp_tx_count]
    inc dword [rtl8139_tcp_data_tx_count]
    add dword [rtl8139_tcp_seq], tls_clienthello_len

    mov esi, rtl8139_tls_clienthello_sent_msg
    call pm_print_string
    ret


rtl8139_tlstest_cmd:
    mov esi, rtl8139_tlstest_msg
    call pm_print_string

    call rtl8139_tlstest_reset_state

    call rtl8139_netup
    call rtl8139_tlstest_pause

    call rtl8139_pingtest
    call rtl8139_tlstest_pause

    mov word [rtl8139_tcp_dst_port], 4433
    call rtl8139_tcpconnect_start
    call rtl8139_tlstest_pause

    cmp byte [rtl8139_tcp_state], TCP_STATE_ESTABLISHED
    jne .done

    call rtl8139_tlshello_cmd
    call rtl8139_tlstest_pause

    call rtl8139_tlsrecv_cmd
    call rtl8139_tlstest_pause

    call rtl8139_tcpstat

.done:
    ret

rtl8139_tlstest_pause:
    push eax
    push esi
    mov esi, rtl8139_tlstest_press_key_msg
    call pm_print_string
    call pm_kbd_get_char
    call pm_newline
    pop esi
    pop eax
    ret

rtl8139_tlstest_reset_state:
    pushad

    mov byte [rtl8139_tcp_state], TCP_STATE_CLOSED
    mov dword [rtl8139_tcp_tx_count], 0
    mov dword [rtl8139_tcp_rx_count], 0
    mov dword [rtl8139_tcp_synack_count], 0
    mov dword [rtl8139_tcp_ack_tx_count], 0
    mov dword [rtl8139_tcp_data_tx_count], 0
    mov dword [rtl8139_tcp_data_rx_count], 0
    mov dword [rtl8139_tcp_data_ack_tx_count], 0
    mov dword [rtl8139_tcp_fin_rx_count], 0
    mov dword [rtl8139_tcp_fin_ack_tx_count], 0

    mov word [rtl8139_tcp_last_payload_len], 0
    mov word [rtl8139_tcp_last_payload_wire_len], 0
    mov dword [rtl8139_tcp_last_src_ip], 0
    mov word [rtl8139_tcp_last_src_port], 0
    mov byte [rtl8139_tcp_last_flags], 0

    mov word [rtl8139_tls_selected_version], 0
    mov word [rtl8139_tls_selected_group], 0
    mov dword [rtl8139_tls_server_key_share_len], 0

    mov edi, rtl8139_tls_server_pubkey
    mov ecx, 32
    xor eax, eax
    cld
    rep stosb

    popad
    ret

rtl8139_tcprecv_cmd:
    mov word [rtl8139_tcp_last_payload_len], 0
    mov word [rtl8139_tcp_last_payload_wire_len], 0

    mov ecx, 40
.poll:
    push ecx
    call rtl8139_wait_for_rx_activity
    call rtl8139_rx_process_packet_quiet
    pop ecx

    cmp word [rtl8139_tcp_last_payload_len], 0
    jne .found

    loop .poll

    mov esi, rtl8139_tcprecv_none_msg
    call pm_print_string
    ret

.found:
    mov esi, rtl8139_tcprecv_msg
    call pm_print_string

    mov esi, rtl8139_tcprecv_len_msg
    call pm_print_string
    movzx eax, word [rtl8139_tcp_last_payload_len]
    call pm_print_uint32
    call pm_newline

    call rtl8139_print_tcp_payload_ascii

    call rtl8139_build_tcp_data_ack
    call rtl8139_tx_submit_current_quiet
    inc dword [rtl8139_tcp_tx_count]
    inc dword [rtl8139_tcp_data_ack_tx_count]

    mov esi, rtl8139_tcp_data_ack_sent_msg
    call pm_print_string

    mov al, [rtl8139_tcp_last_flags]
    test al, 01h
    jz .no_fin_single

    inc dword [rtl8139_tcp_fin_ack_tx_count]
    mov byte [rtl8139_tcp_state], TCP_STATE_CLOSED
    mov esi, rtl8139_tcp_fin_ack_sent_msg
    call pm_print_string

.no_fin_single:
    ret

rtl8139_tcprecvall_cmd:
    mov byte [rtl8139_tcprecvall_remaining], 32
    mov dword [rtl8139_tcprecvall_segments], 0

.next_segment:
    cmp byte [rtl8139_tcprecvall_remaining], 0
    je .done_all

    dec byte [rtl8139_tcprecvall_remaining]
    mov word [rtl8139_tcp_last_payload_len], 0
    mov word [rtl8139_tcp_last_payload_wire_len], 0

    mov ecx, 40
.poll:
    push ecx
    call rtl8139_wait_for_rx_activity
    call rtl8139_rx_process_packet_quiet
    pop ecx

    cmp word [rtl8139_tcp_last_payload_len], 0
    jne .found

    loop .poll

    cmp dword [rtl8139_tcprecvall_segments], 0
    je .none

    mov esi, rtl8139_tcprecvall_done_msg
    call pm_print_string
    ret

.none:
    mov esi, rtl8139_tcprecv_none_msg
    call pm_print_string
    ret

.found:
    inc dword [rtl8139_tcprecvall_segments]

    mov esi, rtl8139_tcprecv_msg
    call pm_print_string

    mov esi, rtl8139_tcprecv_len_msg
    call pm_print_string
    movzx eax, word [rtl8139_tcp_last_payload_len]
    call pm_print_uint32
    call pm_newline

    call rtl8139_print_tcp_payload_ascii

    call rtl8139_build_tcp_data_ack
    call rtl8139_tx_submit_current_quiet
    inc dword [rtl8139_tcp_tx_count]
    inc dword [rtl8139_tcp_data_ack_tx_count]

    mov esi, rtl8139_tcp_data_ack_sent_msg
    call pm_print_string

    mov al, [rtl8139_tcp_last_flags]
    test al, 01h
    jz .next_segment

    inc dword [rtl8139_tcp_fin_ack_tx_count]
    mov byte [rtl8139_tcp_state], TCP_STATE_CLOSED
    mov esi, rtl8139_tcp_fin_ack_sent_msg
    call pm_print_string

    mov esi, rtl8139_tcp_fin_closed_msg
    call pm_print_string
    ret

.done_all:
    mov esi, rtl8139_tcprecvall_done_msg
    call pm_print_string
    ret

rtl8139_tlsrecv_cmd:
    mov byte [rtl8139_tcprecvall_remaining], 16
    mov dword [rtl8139_tcprecvall_segments], 0

.next_segment:
    cmp byte [rtl8139_tcprecvall_remaining], 0
    je .done_all

    dec byte [rtl8139_tcprecvall_remaining]
    mov word [rtl8139_tcp_last_payload_len], 0
    mov word [rtl8139_tcp_last_payload_wire_len], 0

    mov ecx, 40
.poll:
    push ecx
    call rtl8139_wait_for_rx_activity
    call rtl8139_rx_process_packet_quiet
    pop ecx

    cmp word [rtl8139_tcp_last_payload_len], 0
    jne .found

    loop .poll

    cmp dword [rtl8139_tcprecvall_segments], 0
    je .none

    mov esi, rtl8139_tlsrecv_done_msg
    call pm_print_string
    ret

.none:
    mov esi, rtl8139_tcprecv_none_msg
    call pm_print_string
    ret

.found:
    inc dword [rtl8139_tcprecvall_segments]

    mov esi, rtl8139_tcprecv_msg
    call pm_print_string

    mov esi, rtl8139_tcprecv_len_msg
    call pm_print_string
    movzx eax, word [rtl8139_tcp_last_payload_len]
    call pm_print_uint32
    call pm_newline

    mov esi, rtl8139_tcp_payload_wire_len_msg
    call pm_print_string
    movzx eax, word [rtl8139_tcp_last_payload_wire_len]
    call pm_print_uint32
    call pm_newline

    call rtl8139_print_tls_payload_info

    call rtl8139_build_tcp_data_ack
    call rtl8139_tx_submit_current_quiet
    inc dword [rtl8139_tcp_tx_count]
    inc dword [rtl8139_tcp_data_ack_tx_count]

    mov esi, rtl8139_tcp_data_ack_sent_msg
    call pm_print_string

    mov al, [rtl8139_tcp_last_flags]
    test al, 01h
    jz .next_segment

    inc dword [rtl8139_tcp_fin_ack_tx_count]
    mov byte [rtl8139_tcp_state], TCP_STATE_CLOSED
    mov esi, rtl8139_tcp_fin_ack_sent_msg
    call pm_print_string

    mov esi, rtl8139_tcp_fin_closed_msg
    call pm_print_string
    ret

.done_all:
    mov esi, rtl8139_tlsrecv_done_msg
    call pm_print_string
    ret

rtl8139_tcpstat:
    mov esi, rtl8139_tcpstat_msg
    call pm_print_string

    mov esi, rtl8139_tcp_state_msg
    call pm_print_string
    call rtl8139_print_tcp_state

    mov esi, rtl8139_tcp_tx_msg
    call pm_print_string
    mov eax, [rtl8139_tcp_tx_count]
    call pm_print_uint32
    call pm_newline

    mov esi, rtl8139_tcp_rx_msg
    call pm_print_string
    mov eax, [rtl8139_tcp_rx_count]
    call pm_print_uint32
    call pm_newline

    mov esi, rtl8139_tcp_synack_msg
    call pm_print_string
    mov eax, [rtl8139_tcp_synack_count]
    call pm_print_uint32
    call pm_newline

    mov esi, rtl8139_tcp_ack_tx_msg
    call pm_print_string
    mov eax, [rtl8139_tcp_ack_tx_count]
    call pm_print_uint32
    call pm_newline

    mov esi, rtl8139_tcp_data_tx_msg
    call pm_print_string
    mov eax, [rtl8139_tcp_data_tx_count]
    call pm_print_uint32
    call pm_newline

    mov esi, rtl8139_tcp_data_rx_msg
    call pm_print_string
    mov eax, [rtl8139_tcp_data_rx_count]
    call pm_print_uint32
    call pm_newline

    mov esi, rtl8139_tcp_payload_len_msg
    call pm_print_string
    movzx eax, word [rtl8139_tcp_last_payload_len]
    call pm_print_uint32
    call pm_newline

    mov esi, rtl8139_tcp_payload_wire_len_msg
    call pm_print_string
    movzx eax, word [rtl8139_tcp_last_payload_wire_len]
    call pm_print_uint32
    call pm_newline

    mov esi, rtl8139_tcp_data_ack_tx_msg
    call pm_print_string
    mov eax, [rtl8139_tcp_data_ack_tx_count]
    call pm_print_uint32
    call pm_newline

    mov esi, rtl8139_tcp_fin_rx_msg
    call pm_print_string
    mov eax, [rtl8139_tcp_fin_rx_count]
    call pm_print_uint32
    call pm_newline

    mov esi, rtl8139_tcp_fin_ack_tx_msg
    call pm_print_string
    mov eax, [rtl8139_tcp_fin_ack_tx_count]
    call pm_print_uint32
    call pm_newline

    mov esi, rtl8139_tcp_src_ip_msg
    call pm_print_string
    mov esi, rtl8139_tcp_last_src_ip
    call rtl8139_dump_ip
    call pm_newline

    mov esi, rtl8139_tcp_src_port_msg
    call pm_print_string
    movzx eax, word [rtl8139_tcp_last_src_port]
    call pm_print_uint32
    call pm_newline

    mov esi, rtl8139_tcp_flags_msg
    call pm_print_string
    movzx eax, byte [rtl8139_tcp_last_flags]
    call pm_print_hex32
    call pm_newline
    ret

rtl8139_print_tcp_state:
    cmp byte [rtl8139_tcp_state], TCP_STATE_SYN_SENT
    je .syn_sent
    cmp byte [rtl8139_tcp_state], TCP_STATE_ESTABLISHED
    je .established
    mov esi, rtl8139_tcp_state_closed_msg
    call pm_print_string
    ret
.syn_sent:
    mov esi, rtl8139_tcp_state_syn_sent_msg
    call pm_print_string
    ret
.established:
    mov esi, rtl8139_tcp_state_established_msg
    call pm_print_string
    ret

rtl8139_build_icmp_echo_request:
    pushad

    mov edi, rtl8139_tx_buffer
    mov ecx, 2048
    xor eax, eax
    rep stosb

    mov edi, rtl8139_tx_buffer

    ; Ethernet destination: learned gateway MAC.
    mov esi, rtl8139_gateway_mac
    mov ecx, 6

.copy_dst:
    lodsb
    stosb
    loop .copy_dst

    ; Ethernet source.
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

    mov ax, 20 + 8 + rtl8139_icmp_payload_len
    mov bl, 01h
    mov esi, rtl8139_gateway_ip
    call rtl8139_write_ipv4_header

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
    mov ax, [rtl8139_ping_seq]
    mov [rtl8139_last_icmp_seq], ax
    mov bl, ah
    mov bh, al
    mov al, bl
    stosb
    mov al, bh
    stosb

    mov esi, rtl8139_icmp_payload
    mov ecx, rtl8139_icmp_payload_len
.copy_payload:
    lodsb
    stosb
    loop .copy_payload

    mov esi, rtl8139_tx_buffer + 34
    mov ecx, 8 + rtl8139_icmp_payload_len
    call rtl8139_checksum
    mov [rtl8139_tx_buffer + 36], ah
    mov [rtl8139_tx_buffer + 37], al

    inc word [rtl8139_ping_seq]
    mov dword [rtl8139_tx_len], 60

    popad
    ret

rtl8139_build_udp_packet:
    pushad

    mov edi, rtl8139_tx_buffer
    mov ecx, 2048
    xor eax, eax
    rep stosb

    mov edi, rtl8139_tx_buffer

    mov esi, rtl8139_gateway_mac
    mov ecx, 6
.copy_dst:
    lodsb
    stosb
    loop .copy_dst

    mov esi, rtl8139_mac
    mov ecx, 6
.copy_src:
    lodsb
    stosb
    loop .copy_src

    mov al, 08h
    stosb
    mov al, 00h
    stosb

    mov ax, 20 + 8 + rtl8139_udp_payload_len
    mov bl, 11h
    mov esi, rtl8139_gateway_ip
    call rtl8139_write_ipv4_header

    mov ax, [rtl8139_udp_src_port]
    xchg al, ah
    stosw

    mov ax, [rtl8139_udp_dst_port]
    xchg al, ah
    stosw

    mov ax, 8 + rtl8139_udp_payload_len
    xchg al, ah
    stosw

    xor ax, ax
    stosw

    mov esi, rtl8139_udp_payload
    mov ecx, rtl8139_udp_payload_len
.copy_payload:
    lodsb
    stosb
    loop .copy_payload

    mov dword [rtl8139_tx_len], 60

    popad
    ret

rtl8139_build_tcp_syn:
    pushad

    mov edi, rtl8139_tx_buffer
    mov ecx, 2048
    xor eax, eax
    rep stosb

    mov edi, rtl8139_tx_buffer
    call rtl8139_write_ipv4_tcp_frame_prefix

    mov ax, [rtl8139_tcp_src_port]
    xchg al, ah
    stosw

    mov ax, [rtl8139_tcp_dst_port]
    xchg al, ah
    stosw

    mov eax, [rtl8139_tcp_seq]
    call rtl8139_stos_be32

    xor eax, eax
    call rtl8139_stos_be32

    mov al, 50h
    stosb
    mov al, 02h
    stosb
    mov al, 40h
    stosb
    xor al, al
    stosb
    xor ax, ax
    stosw
    stosw

    call rtl8139_finish_tcp_checksum
    mov dword [rtl8139_tx_len], 60

    popad
    ret

rtl8139_build_tcp_ack:
    pushad

    mov edi, rtl8139_tx_buffer
    mov ecx, 2048
    xor eax, eax
    rep stosb

    mov edi, rtl8139_tx_buffer
    call rtl8139_write_ipv4_tcp_frame_prefix

    mov ax, [rtl8139_tcp_src_port]
    xchg al, ah
    stosw

    mov ax, [rtl8139_tcp_dst_port]
    xchg al, ah
    stosw

    mov eax, [rtl8139_tcp_seq]
    inc eax
    call rtl8139_stos_be32

    mov eax, [rtl8139_tcp_last_seq]
    inc eax
    call rtl8139_stos_be32

    mov al, 50h
    stosb
    mov al, 10h
    stosb
    mov al, 40h
    stosb
    xor al, al
    stosb
    xor ax, ax
    stosw
    stosw

    call rtl8139_finish_tcp_checksum
    mov dword [rtl8139_tx_len], 60

    popad
    ret

rtl8139_build_tcp_data_ack:
    pushad

    mov edi, rtl8139_tx_buffer
    mov ecx, 2048
    xor eax, eax
    rep stosb

    mov edi, rtl8139_tx_buffer
    call rtl8139_write_ipv4_tcp_frame_prefix

    mov ax, [rtl8139_tcp_src_port]
    xchg al, ah
    stosw

    mov ax, [rtl8139_tcp_dst_port]
    xchg al, ah
    stosw

    ; Sequence number: use ACK received from peer.
    ; After our GET, peer ACK tells us the next seq it expects from us.
    mov eax, [rtl8139_tcp_last_ack]
    call rtl8139_stos_be32

    ; ACK number: peer sequence + real TCP payload length, plus 1 if FIN is set.
    mov eax, [rtl8139_tcp_last_seq]
    movzx edx, word [rtl8139_tcp_last_payload_wire_len]
    add eax, edx

    mov dl, [rtl8139_tcp_last_flags]
    test dl, 01h
    jz .no_fin_ack_seq
    inc eax

.no_fin_ack_seq:
    call rtl8139_stos_be32

    mov al, 50h
    stosb
    mov al, 10h
    stosb
    mov al, 40h
    stosb
    xor al, al
    stosb
    xor ax, ax
    stosw
    stosw

    call rtl8139_finish_tcp_checksum
    mov dword [rtl8139_tx_len], 60

    popad
    ret

rtl8139_build_tcp_http_get:
    pushad

    mov edi, rtl8139_tx_buffer
    mov ecx, 2048
    xor eax, eax
    rep stosb

    mov edi, rtl8139_tx_buffer

    ; Ethernet destination MAC
    mov esi, rtl8139_gateway_mac
    mov ecx, 6
.copy_dst:
    lodsb
    stosb
    loop .copy_dst

    ; Ethernet source MAC
    mov esi, rtl8139_mac
    mov ecx, 6
.copy_src:
    lodsb
    stosb
    loop .copy_src

    ; Ethertype IPv4
    mov al, 08h
    stosb
    mov al, 00h
    stosb

    ; IPv4 header with TCP + payload length
    mov ax, 20 + 20 + rtl8139_tcp_http_get_len
    mov bl, 06h
    mov esi, rtl8139_gateway_ip
    call rtl8139_write_ipv4_header

    ; TCP source port
    mov ax, [rtl8139_tcp_src_port]
    xchg al, ah
    stosw

    ; TCP destination port
    mov ax, [rtl8139_tcp_dst_port]
    xchg al, ah
    stosw

    ; TCP sequence number: initial seq + SYN byte
    mov eax, [rtl8139_tcp_seq]
    inc eax
    call rtl8139_stos_be32

    ; TCP ACK number: peer SYN seq + 1
    mov eax, [rtl8139_tcp_last_seq]
    inc eax
    call rtl8139_stos_be32

    ; TCP header length
    mov al, 50h
    stosb

    ; Flags: PSH + ACK
    mov al, 18h
    stosb

    ; Window
    mov al, 40h
    stosb
    xor al, al
    stosb

    ; Checksum + urgent pointer initially zero
    xor ax, ax
    stosw
    stosw

    ; Payload
    mov esi, rtl8139_tcp_http_get_payload
    mov ecx, rtl8139_tcp_http_get_len
.copy_payload:
    lodsb
    stosb
    loop .copy_payload

    call rtl8139_finish_tcp_checksum_payload

    mov dword [rtl8139_tx_len], 14 + 20 + 20 + rtl8139_tcp_http_get_len

    popad
    ret

rtl8139_build_tcp_tls_clienthello:
    pushad

    mov edi, rtl8139_tx_buffer
    mov ecx, 2048
    xor eax, eax
    rep stosb

    mov edi, rtl8139_tx_buffer

    ; Ethernet destination MAC
    mov esi, rtl8139_gateway_mac
    mov ecx, 6
.copy_dst:
    lodsb
    stosb
    loop .copy_dst

    ; Ethernet source MAC
    mov esi, rtl8139_mac
    mov ecx, 6
.copy_src:
    lodsb
    stosb
    loop .copy_src

    ; Ethertype IPv4
    mov al, 08h
    stosb
    mov al, 00h
    stosb

    ; IPv4 header with TCP + payload length
    mov ax, 20 + 20 + tls_clienthello_len
    mov bl, 06h
    mov esi, rtl8139_gateway_ip
    call rtl8139_write_ipv4_header

    ; TCP source port
    mov ax, [rtl8139_tcp_src_port]
    xchg al, ah
    stosw

    ; TCP destination port
    mov ax, [rtl8139_tcp_dst_port]
    xchg al, ah
    stosw

    ; TCP sequence number: initial seq + SYN byte
    mov eax, [rtl8139_tcp_seq]
    inc eax
    call rtl8139_stos_be32

    ; TCP ACK number: peer SYN seq + 1
    mov eax, [rtl8139_tcp_last_seq]
    inc eax
    call rtl8139_stos_be32

    ; TCP header length
    mov al, 50h
    stosb

    ; Flags: PSH + ACK
    mov al, 18h
    stosb

    ; Window
    mov al, 40h
    stosb
    xor al, al
    stosb

    ; Checksum + urgent pointer initially zero
    xor ax, ax
    stosw
    stosw

    ; Payload
    mov esi, tls_clienthello_payload
    mov ecx, tls_clienthello_len
.copy_payload:
    lodsb
    stosb
    loop .copy_payload

    call rtl8139_finish_tcp_checksum_tls_clienthello

    mov dword [rtl8139_tx_len], 14 + 20 + 20 + tls_clienthello_len

    popad
    ret

rtl8139_write_ipv4_tcp_frame_prefix:
    mov esi, rtl8139_gateway_mac
    mov ecx, 6
.copy_dst:
    lodsb
    stosb
    loop .copy_dst

    mov esi, rtl8139_mac
    mov ecx, 6
.copy_src:
    lodsb
    stosb
    loop .copy_src

    mov al, 08h
    stosb
    mov al, 00h
    stosb

    mov ax, 20 + 20
    mov bl, 06h
    mov esi, rtl8139_gateway_ip
    call rtl8139_write_ipv4_header
    ret

rtl8139_stos_be32:
    push eax
    shr eax, 24
    stosb
    pop eax
    push eax
    shr eax, 16
    stosb
    pop eax
    push eax
    shr eax, 8
    stosb
    pop eax
    stosb
    ret

rtl8139_finish_tcp_checksum:
    pushad

    mov edi, rtl8139_tcp_checksum_buffer
    mov ecx, 64
    xor eax, eax
    rep stosb

    mov edi, rtl8139_tcp_checksum_buffer
    mov esi, rtl8139_local_ip
    mov ecx, 4
.copy_src_ip:
    lodsb
    stosb
    loop .copy_src_ip

    mov esi, rtl8139_gateway_ip
    mov ecx, 4
.copy_dst_ip:
    lodsb
    stosb
    loop .copy_dst_ip

    xor al, al
    stosb
    mov al, 06h
    stosb
    xor al, al
    stosb
    mov al, 20
    stosb

    mov esi, rtl8139_tx_buffer + 34
    mov ecx, 20
.copy_tcp:
    lodsb
    stosb
    loop .copy_tcp

    mov esi, rtl8139_tcp_checksum_buffer
    mov ecx, 12 + 20
    call rtl8139_checksum
    mov [rtl8139_tx_buffer + 50], ah
    mov [rtl8139_tx_buffer + 51], al

    popad
    ret

rtl8139_finish_tcp_checksum_payload:
    pushad

    mov edi, rtl8139_tcp_checksum_buffer
    mov ecx, 64
    xor eax, eax
    rep stosb

    mov edi, rtl8139_tcp_checksum_buffer

    mov esi, rtl8139_local_ip
    mov ecx, 4
.copy_src_ip:
    lodsb
    stosb
    loop .copy_src_ip

    mov esi, rtl8139_gateway_ip
    mov ecx, 4
.copy_dst_ip:
    lodsb
    stosb
    loop .copy_dst_ip

    xor al, al
    stosb
    mov al, 06h
    stosb

    ; TCP length = 20-byte header + payload
    xor al, al
    stosb
    mov al, 20 + rtl8139_tcp_http_get_len
    stosb

    mov esi, rtl8139_tx_buffer + 34
    mov ecx, 20 + rtl8139_tcp_http_get_len
.copy_tcp:
    lodsb
    stosb
    loop .copy_tcp

    mov esi, rtl8139_tcp_checksum_buffer
    mov ecx, 12 + 20 + rtl8139_tcp_http_get_len
    call rtl8139_checksum

    mov [rtl8139_tx_buffer + 50], ah
    mov [rtl8139_tx_buffer + 51], al

    popad
    ret

; EDI=IPv4 header destination, AX=IPv4 total length, BL=protocol, ESI=dst IP.
; Returns EDI advanced past the 20-byte header.
rtl8139_finish_tcp_checksum_tls_clienthello:
    pushad

    mov edi, rtl8139_tcp_checksum_buffer
    mov ecx, 64
    xor eax, eax
    rep stosb

    mov edi, rtl8139_tcp_checksum_buffer

    mov esi, rtl8139_local_ip
    mov ecx, 4
.copy_src_ip:
    lodsb
    stosb
    loop .copy_src_ip

    mov esi, rtl8139_gateway_ip
    mov ecx, 4
.copy_dst_ip:
    lodsb
    stosb
    loop .copy_dst_ip

    xor al, al
    stosb
    mov al, 06h
    stosb

    ; TCP length = 20-byte header + payload
    xor al, al
    stosb
    mov al, 20 + tls_clienthello_len
    stosb

    mov esi, rtl8139_tx_buffer + 34
    mov ecx, 20 + tls_clienthello_len
.copy_tcp:
    lodsb
    stosb
    loop .copy_tcp

    mov esi, rtl8139_tcp_checksum_buffer
    mov ecx, 12 + 20 + tls_clienthello_len
    call rtl8139_checksum

    mov [rtl8139_tx_buffer + 50], ah
    mov [rtl8139_tx_buffer + 51], al

    popad
    ret

; EDI=IPv4 header destination, AX=IPv4 total length, BL=protocol, ESI=dst IP.
; Returns EDI advanced past the 20-byte header.
rtl8139_write_ipv4_header:
    push eax
    push ebx
    push ecx
    push edx
    push esi

    mov edx, edi
    mov [rtl8139_ipv4_tmp_protocol], bl
    mov bx, ax

    mov al, 45h
    stosb
    xor al, al
    stosb
    mov al, bh
    stosb
    mov al, bl
    stosb
    mov al, 00h
    stosb
    mov al, 01h
    stosb
    xor al, al
    stosb
    stosb
    mov al, 40h
    stosb
    mov al, [rtl8139_ipv4_tmp_protocol]
    stosb
    xor al, al
    stosb
    stosb

    mov esi, rtl8139_local_ip
    mov ecx, 4
.copy_src_ip:
    lodsb
    stosb
    loop .copy_src_ip

    mov esi, [esp]
    mov ecx, 4
.copy_dst_ip:
    lodsb
    stosb
    loop .copy_dst_ip

    push edx
    mov esi, edx
    mov ecx, 20
    call rtl8139_checksum
    pop edx
    mov [edx + 10], ah
    mov [edx + 11], al

    pop esi
    pop edx
    pop ecx
    pop ebx
    pop eax
    ret

rtl8139_ensure_gateway_mac:
    cmp byte [rtl8139_gateway_mac_known], 1
    je .done

    call rtl8139_build_arp_request
    call rtl8139_tx_submit_current_quiet

    mov ecx, 4
.process:
    push ecx
    call rtl8139_wait_for_rx_activity
    call rtl8139_rx_process_packet_quiet
    pop ecx
    cmp byte [rtl8139_gateway_mac_known], 1
    je .done
    loop .process
.done:
    ret

rtl8139_wait_for_rx_activity:
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_CR
    in al, dx
    test al, RTL8139_CR_BUFE
    jz .done

    mov eax, [rtl8139_rx_buffer]
    mov [rtl8139_arp_wait_first], eax

    mov dx, [rtl8139_io_base]
    add dx, RTL8139_CBR
    in ax, dx
    mov [rtl8139_arp_wait_cbr], ax

    mov ecx, 30000
.loop:
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_ISR
    in ax, dx
    test ax, RTL8139_ISR_ROK
    jnz .done

    mov dx, [rtl8139_io_base]
    add dx, RTL8139_CBR
    in ax, dx
    cmp ax, [rtl8139_arp_wait_cbr]
    jne .done

    mov eax, [rtl8139_rx_buffer]
    cmp eax, [rtl8139_arp_wait_first]
    jne .done
    loop .loop

.done:
    ret

rtl8139_pingtest:
    cmp byte [rtl8139_initialized], 1
    je .ready

    call rtl8139_rxtest_quiet_netup
    cmp byte [rtl8139_initialized], 1
    jne .not_ready

.ready:
    call rtl8139_enable_pci_bus_master
    mov dword [rtl8139_pingtest_tsd], 0
    mov word [rtl8139_pingtest_isr], 0
    mov dword [rtl8139_icmp_reply_count], 0
    mov word [rtl8139_last_icmp_seq], 0
    mov byte [rtl8139_last_icmp_ttl], 0
    mov dword [rtl8139_last_icmp_src_ip], 0

    call rtl8139_ensure_gateway_mac
    cmp byte [rtl8139_gateway_mac_known], 1
    jne .arp_failed

    mov esi, rtl8139_pingtest_arp_msg
    call pm_print_string
    mov esi, rtl8139_known_msg
    call pm_print_string

    call rtl8139_build_icmp_echo_request
    call rtl8139_tx_submit_current_quiet

    mov eax, [rtl8139_last_tsd_after]
    mov [rtl8139_pingtest_tsd], eax

    mov ecx, 80
.poll:
    push ecx
    call rtl8139_wait_for_rx_activity
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_ISR
    in ax, dx
    or [rtl8139_pingtest_isr], ax
    call rtl8139_rx_process_packet_quiet
    pop ecx
    cmp dword [rtl8139_icmp_reply_count], 0
    jne .summary
    loop .poll

.summary:
    call rtl8139_pingtest_summary
    ret

.arp_failed:
    mov esi, rtl8139_pingtest_arp_failed_msg
    call pm_print_string
    jmp .summary

.not_ready:
    inc dword [rtl8139_error_count]
    mov esi, rtl8139_not_ready_msg
    call pm_print_string
    ret

rtl8139_pingtest_summary:
    mov esi, rtl8139_pingtest_summary_msg
    call pm_print_string

    mov esi, rtl8139_rxtest_tok_msg
    call pm_print_string
    mov ebx, [rtl8139_pingtest_tsd]
    test ebx, RTL8139_TSD_TOK
    jz .tok_no
    call rtl8139_print_yes
    jmp .rok
.tok_no:
    call rtl8139_print_no

.rok:
    mov esi, rtl8139_rxtest_rok_msg
    call pm_print_string
    mov bx, [rtl8139_pingtest_isr]
    test bx, RTL8139_ISR_ROK
    jz .rok_no
    call rtl8139_print_yes
    jmp .arp
.rok_no:
    call rtl8139_print_no

.arp:
    mov esi, rtl8139_pingtest_arp_msg
    call pm_print_string
    cmp byte [rtl8139_gateway_mac_known], 1
    jne .arp_unknown
    mov esi, rtl8139_known_msg
    call pm_print_string
    jmp .icmp
.arp_unknown:
    mov esi, rtl8139_unknown_word_msg
    call pm_print_string

.icmp:
    mov esi, rtl8139_pingtest_icmp_msg
    call pm_print_string
    cmp dword [rtl8139_icmp_reply_count], 0
    je .icmp_no
    call rtl8139_print_yes
    jmp .src
.icmp_no:
    call rtl8139_print_no

.src:
    mov esi, rtl8139_pingtest_src_msg
    call pm_print_string
    mov esi, rtl8139_last_icmp_src_ip
    call rtl8139_dump_ip
    call pm_newline

    mov esi, rtl8139_pingtest_seq_msg
    call pm_print_string
    movzx eax, word [rtl8139_last_icmp_seq]
    call pm_print_uint32
    call pm_newline
    ret

rtl8139_pingdebug:
    cmp byte [rtl8139_initialized], 1
    je .ready

    call rtl8139_rxtest_quiet_netup
    cmp byte [rtl8139_initialized], 1
    jne .not_ready

.ready:
    call rtl8139_enable_pci_bus_master
    call rtl8139_ensure_gateway_mac
    cmp byte [rtl8139_gateway_mac_known], 1
    jne .arp_failed

    call rtl8139_build_icmp_echo_request
    call rtl8139_dump_icmp_tx_packet
    ret

.arp_failed:
    mov esi, rtl8139_pingtest_arp_failed_msg
    call pm_print_string
    ret

.not_ready:
    inc dword [rtl8139_error_count]
    mov esi, rtl8139_not_ready_msg
    call pm_print_string
    ret

rtl8139_dump_icmp_tx_packet:
    pushad

    mov esi, rtl8139_pingdebug_msg
    call pm_print_string

    mov esi, rtl8139_pingdebug_ethertype_msg
    call pm_print_string
    movzx eax, byte [rtl8139_tx_buffer + 12]
    shl eax, 8
    movzx ebx, byte [rtl8139_tx_buffer + 13]
    or eax, ebx
    call pm_print_hex32
    call pm_newline

    mov esi, rtl8139_pingdebug_ipv4_len_msg
    call pm_print_string
    movzx eax, byte [rtl8139_tx_buffer + 16]
    shl eax, 8
    movzx ebx, byte [rtl8139_tx_buffer + 17]
    or eax, ebx
    call pm_print_uint32
    call pm_newline

    mov esi, rtl8139_pingdebug_ipv4_id_msg
    call pm_print_string
    movzx eax, byte [rtl8139_tx_buffer + 18]
    shl eax, 8
    movzx ebx, byte [rtl8139_tx_buffer + 19]
    or eax, ebx
    call pm_print_hex32
    call pm_newline

    mov esi, rtl8139_pingdebug_ipv4_frag_msg
    call pm_print_string
    movzx eax, byte [rtl8139_tx_buffer + 20]
    shl eax, 8
    movzx ebx, byte [rtl8139_tx_buffer + 21]
    or eax, ebx
    call pm_print_hex32
    call pm_newline

    mov esi, rtl8139_pingdebug_ipv4_ttl_msg
    call pm_print_string
    movzx eax, byte [rtl8139_tx_buffer + 22]
    call pm_print_uint32
    call pm_newline

    mov esi, rtl8139_pingdebug_ipv4_proto_msg
    call pm_print_string
    movzx eax, byte [rtl8139_tx_buffer + 23]
    call pm_print_uint32
    call pm_newline

    mov esi, rtl8139_pingdebug_ipv4_checksum_msg
    call pm_print_string
    movzx eax, byte [rtl8139_tx_buffer + 24]
    shl eax, 8
    movzx ebx, byte [rtl8139_tx_buffer + 25]
    or eax, ebx
    call pm_print_hex32
    call pm_newline

    mov esi, rtl8139_pingdebug_ipv4_raw_sum_msg
    call pm_print_string
    mov esi, rtl8139_tx_buffer + 14
    mov ecx, 20
    call rtl8139_checksum_raw_sum
    movzx eax, ax
    call pm_print_hex32
    call pm_newline

    mov esi, rtl8139_pingdebug_ipv4_header_msg
    call pm_print_string
    mov esi, rtl8139_tx_buffer + 14
    mov ecx, 20
    call rtl8139_dump_bytes

    mov esi, rtl8139_pingdebug_icmp_checksum_msg
    call pm_print_string
    movzx eax, byte [rtl8139_tx_buffer + 36]
    shl eax, 8
    movzx ebx, byte [rtl8139_tx_buffer + 37]
    or eax, ebx
    call pm_print_hex32
    call pm_newline

    mov esi, rtl8139_pingdebug_icmp_raw_sum_msg
    call pm_print_string
    mov esi, rtl8139_tx_buffer + 34
    mov ecx, 8 + rtl8139_icmp_payload_len
    call rtl8139_checksum_raw_sum
    movzx eax, ax
    call pm_print_hex32
    call pm_newline

    mov esi, rtl8139_pingdebug_src_msg
    call pm_print_string
    mov esi, rtl8139_tx_buffer + 26
    call rtl8139_dump_ip
    call pm_newline

    mov esi, rtl8139_pingdebug_dst_msg
    call pm_print_string
    mov esi, rtl8139_tx_buffer + 30
    call rtl8139_dump_ip
    call pm_newline

    mov esi, rtl8139_pingdebug_icmp_type_msg
    call pm_print_string
    movzx eax, byte [rtl8139_tx_buffer + 34]
    call pm_print_uint32
    call pm_newline

    mov esi, rtl8139_pingdebug_icmp_seq_msg
    call pm_print_string
    movzx eax, byte [rtl8139_tx_buffer + 40]
    shl eax, 8
    movzx ebx, byte [rtl8139_tx_buffer + 41]
    or eax, ebx
    call pm_print_uint32
    call pm_newline

    mov esi, rtl8139_pingdebug_icmp_total_len_msg
    call pm_print_string
    mov eax, 8 + rtl8139_icmp_payload_len
    call pm_print_uint32
    call pm_newline

    mov esi, rtl8139_pingdebug_payload_len_msg
    call pm_print_string
    mov eax, rtl8139_icmp_payload_len
    call pm_print_uint32
    call pm_newline

    mov esi, rtl8139_pingdebug_payload_bytes_msg
    call pm_print_string
    mov esi, rtl8139_tx_buffer + 42
    mov ecx, rtl8139_icmp_payload_len
    call rtl8139_dump_bytes

    popad
    ret

rtl8139_gateway_ip_text db '10.0.2.2',0
rtl8139_icmp_payload db 'GHOSTOS!'
rtl8139_icmp_payload_len equ $ - rtl8139_icmp_payload
rtl8139_udp_payload db 'GHOSTOS UDP'
rtl8139_udp_payload_len equ $ - rtl8139_udp_payload
rtl8139_udp_dst_port_text db '1234',0
rtl8139_ping_usage_msg db 'Usage: ping 10.0.2.2',13,0
rtl8139_ping_sent_msg db 'ICMP echo request sent',13,0
rtl8139_ping_no_arp_msg db 'Ping failed: gateway MAC unknown',13,0
rtl8139_pingtest_summary_msg db 'PINGTEST SUMMARY',13,0
rtl8139_pingtest_arp_msg db 'ARP: ',0
rtl8139_pingtest_arp_failed_msg db 'ARP failed',13,0
rtl8139_pingtest_icmp_msg db 'ICMP reply: ',0
rtl8139_pingtest_src_msg db 'src IP: ',0
rtl8139_pingtest_seq_msg db 'seq: ',0
rtl8139_udpsend_usage_msg db 'Usage: udpsend 10.0.2.2 1234',13,0
rtl8139_udp_sent_msg db 'UDP sent',13,0
rtl8139_udp_no_arp_msg db 'UDP failed: gateway MAC unknown',13,0
rtl8139_udpstat_msg db 'UDP state:',13,0
rtl8139_udp_tx_msg db 'udp_tx=',0
rtl8139_udp_rx_msg db 'udp_rx=',0
rtl8139_udp_packet_msg db 'UDP packet',13,0
rtl8139_udp_src_ip_msg db 'src IP: ',0
rtl8139_udp_src_port_msg db 'src port: ',0
rtl8139_udp_dst_port_msg db 'dst port: ',0
rtl8139_udp_len_msg db 'length: ',0
rtl8139_tcp_dst_port_text db '80',0
rtl8139_tcpconnect_usage_msg db 'Usage: tcpconnect 10.0.2.2 80|8080|4433',13,0
rtl8139_tcp_syn_sent_msg db 'TCP SYN sent',13,0
rtl8139_tcp_syn_wait_msg db 'TCP state: SYN_SENT',13,0
rtl8139_tcp_established_msg db 'TCP state: ESTABLISHED',13,0
rtl8139_tcp_no_arp_msg db 'TCP failed: gateway MAC unknown',13,0
rtl8139_tcpstat_msg db 'TCP state:',13,0
rtl8139_tcp_state_msg db 'state=',0
rtl8139_tcp_state_closed_msg db 'CLOSED',13,0
rtl8139_tcp_state_syn_sent_msg db 'SYN_SENT',13,0
rtl8139_tcp_state_established_msg db 'ESTABLISHED',13,0
rtl8139_tcp_tx_msg db 'tcp_tx=',0
rtl8139_tcp_rx_msg db 'tcp_rx=',0
rtl8139_tcp_synack_msg db 'synack_rx=',0
rtl8139_tcp_ack_tx_msg db 'ack_tx=',0
rtl8139_tcp_packet_msg db 'TCP packet',13,0
rtl8139_tcp_src_ip_msg db 'src IP: ',0
rtl8139_tcp_src_port_msg db 'src port: ',0
rtl8139_tcp_flags_msg db 'flags: ',0
rtl8139_pingdebug_msg db 'PINGDEBUG TX IPv4/ICMP',13,0
rtl8139_pingdebug_ethertype_msg db 'ethertype: ',0
rtl8139_pingdebug_ipv4_len_msg db 'IPv4 len: ',0
rtl8139_pingdebug_ipv4_id_msg db 'IPv4 id: ',0
rtl8139_pingdebug_ipv4_frag_msg db 'IPv4 flags/frag: ',0
rtl8139_pingdebug_ipv4_ttl_msg db 'IPv4 ttl: ',0
rtl8139_pingdebug_ipv4_proto_msg db 'IPv4 protocol: ',0
rtl8139_pingdebug_ipv4_checksum_msg db 'IPv4 checksum: ',0
rtl8139_pingdebug_ipv4_raw_sum_msg db 'IPv4 raw sum: ',0
rtl8139_pingdebug_ipv4_header_msg db 'IPv4 header bytes:',13,0
rtl8139_pingdebug_icmp_checksum_msg db 'ICMP checksum: ',0
rtl8139_pingdebug_icmp_raw_sum_msg db 'ICMP raw sum: ',0
rtl8139_pingdebug_src_msg db 'src IP: ',0
rtl8139_pingdebug_dst_msg db 'dst IP: ',0
rtl8139_pingdebug_icmp_type_msg db 'ICMP type: ',0
rtl8139_pingdebug_icmp_seq_msg db 'ICMP seq: ',0
rtl8139_pingdebug_icmp_total_len_msg db 'ICMP total len: ',0
rtl8139_pingdebug_payload_len_msg db 'payload len: ',0
rtl8139_pingdebug_payload_bytes_msg db 'ICMP payload bytes:',13,0

rtl8139_checksum:
    call rtl8139_checksum_raw_sum
    not ax
    ret

rtl8139_checksum_raw_sum:
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
.fold_loop:
    mov eax, edx
    shr eax, 16
    and edx, 0FFFFh
    add edx, eax
    mov ax, dx
    cmp edx, 0FFFFh
    ja .fold_loop

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

rtl8139_arp:
    cmp byte [rtl8139_initialized], 1
    jne .not_ready

    call rtl8139_build_arp_request
    call rtl8139_tx_submit_current

    mov esi, rtl8139_arp_sent_msg
    call pm_print_string

    call rtl8139_rxpoll
    call rtl8139_rxpacket
    ret

.not_ready:
    inc dword [rtl8139_error_count]
    mov esi, rtl8139_not_ready_msg
    call pm_print_string
    ret

rtl8139_build_arp_request:
    pushad

    mov edi, rtl8139_tx_buffer
    mov ecx, 2048
    xor eax, eax
    rep stosb

    mov edi, rtl8139_tx_buffer

    ; Ethernet broadcast destination.
    mov ecx, 6
    mov al, 0FFh
    rep stosb

    ; Ethernet source MAC.
    mov esi, rtl8139_mac
    mov ecx, 6
.copy_eth_src:
    lodsb
    stosb
    loop .copy_eth_src

    ; EtherType ARP = 0x0806.
    mov al, 08h
    stosb
    mov al, 06h
    stosb

    ; HTYPE Ethernet.
    mov al, 00h
    stosb
    mov al, 01h
    stosb

    ; PTYPE IPv4.
    mov al, 08h
    stosb
    mov al, 00h
    stosb

    ; HLEN, PLEN.
    mov al, 06h
    stosb
    mov al, 04h
    stosb

    ; OPER request.
    mov al, 00h
    stosb
    mov al, 01h
    stosb

    ; Sender MAC.
    mov esi, rtl8139_mac
    mov ecx, 6
.copy_sender_mac:
    lodsb
    stosb
    loop .copy_sender_mac

    ; Sender IP.
    mov esi, rtl8139_local_ip
    mov ecx, 4
.copy_sender_ip:
    lodsb
    stosb
    loop .copy_sender_ip

    ; Target MAC unknown.
    xor eax, eax
    mov ecx, 6
    rep stosb

    ; Target IP 10.0.2.2.
    mov esi, rtl8139_gateway_ip
    mov ecx, 4
.copy_target_ip:
    lodsb
    stosb
    loop .copy_target_ip

    mov dword [rtl8139_tx_len], 60

    popad
    ret

rtl8139_arp_sent_msg db 'ARP request sent',13,0

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
    call rtl8139_tx_submit_current
    mov esi, rtl8139_txreal_sent_msg
    call pm_print_string
    ret

.not_ready:
    inc dword [rtl8139_error_count]
    mov esi, rtl8139_not_ready_msg
    call pm_print_string
    ret

rtl8139_tx_submit_current:
    call rtl8139_tx_submit_current_quiet

    mov esi, rtl8139_tx_slot_msg
    call pm_print_string
    movzx eax, byte [rtl8139_last_tx_slot]
    call pm_print_uint32
    call pm_newline

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
    mov eax, [rtl8139_last_tsd_before]
    call pm_print_hex32
    call pm_newline

    mov esi, rtl8139_tsd_after_msg
    call pm_print_string
    mov eax, [rtl8139_last_tsd_after]
    call pm_print_hex32
    call pm_newline
    mov ebx, [rtl8139_last_tsd_after]
    call rtl8139_print_tsd_flags
    ret

rtl8139_tx_submit_current_quiet:
    mov al, [rtl8139_tx_slot]
    and al, 03h
    mov [rtl8139_last_tx_slot], al

    movzx bx, byte [rtl8139_last_tx_slot]
    shl bx, 2
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_TSD0
    add dx, bx
    in eax, dx
    mov [rtl8139_last_tsd_before], eax

    ; TSADn = physical address of TX buffer
    movzx bx, byte [rtl8139_last_tx_slot]
    shl bx, 2
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_TSAD0
    add dx, bx
    mov eax, rtl8139_tx_buffer
    out dx, eax

    ; TSDn = packet length
    movzx bx, byte [rtl8139_last_tx_slot]
    shl bx, 2
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_TSD0
    add dx, bx
    mov eax, [rtl8139_tx_len]
    out dx, eax

    movzx bx, byte [rtl8139_last_tx_slot]
    shl bx, 2
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_TSD0
    add dx, bx
    mov ecx, 100000

.wait_tx:
    in eax, dx
    test eax, RTL8139_TSD_TOK | RTL8139_TSD_TABT | RTL8139_TSD_TUN
    jnz .tx_done
    loop .wait_tx

.tx_done:
    mov [rtl8139_last_tsd_after], eax

    inc byte [rtl8139_tx_slot]
    and byte [rtl8139_tx_slot], 03h
    inc dword [rtl8139_tx_count]
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
rtl8139_tx_slot_msg db 'TX slot: ',0
rtl8139_tx_len_msg db 'TX len: ',0
rtl8139_tx_buffer_msg db 'TX buffer: ',0
rtl8139_tsd_before_msg db 'TSD before: ',0
rtl8139_tsd_after_msg db 'TSD after: ',0
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
    mov dword [rtl8139_rx_first_dword], 0
    mov byte [rtl8139_rx_capr_mode], 1
    mov byte [rtl8139_tx_slot], 0

    mov dx, [rtl8139_io_base]
    add dx, RTL8139_CR
    mov al, RTL8139_CR_RST
    out dx, al

    mov ecx, 100000
.reset_wait:
    in al, dx
    test al, RTL8139_CR_RST
    jz .reset_done
    loop .reset_wait

    mov esi, rtl8139_reset_timeout_msg
    call pm_print_string
    stc
    ret

.reset_done:
    call rtl8139_configure_rx_path

    call rtl8139_netup_debug

    mov byte [rtl8139_rx_ready], 1

    mov esi, rtl8139_rx_ready_msg
    call pm_print_string
    clc
    ret

rtl8139_rx_ready_msg db 'RTL8139 RX buffer ready',13,0
rtl8139_reset_timeout_msg db 'RTL8139 reset timeout',13,0

rtl8139_wake:
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_9346CR
    mov al, 0C0h
    out dx, al

    mov dx, [rtl8139_io_base]
    add dx, RTL8139_CONFIG1
    xor al, al
    out dx, al
    ret

rtl8139_get_rx_base:
    mov eax, [rtl8139_rx_rbstart_override]
    cmp eax, 0
    jne .done
    mov eax, rtl8139_rx_buffer
.done:
    ret

rtl8139_write_initial_capr:
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_CAPR
    cmp byte [rtl8139_rx_capr_mode], 0
    jne .caprf0
    xor ax, ax
    out dx, ax
    ret

.caprf0:
    mov ax, 0FFF0h
    out dx, ax
    ret

rtl8139_save_pci_bdf:
    mov al, [edi+PCI_REC_BUS]
    mov [rtl8139_pci_bus], al
    mov al, [edi+PCI_REC_DEV]
    mov [rtl8139_pci_dev], al
    mov al, [edi+PCI_REC_FN]
    mov [rtl8139_pci_fn], al
    mov byte [rtl8139_pci_bdf_valid], 1
    ret

rtl8139_enable_pci_bus_master:
    cmp byte [rtl8139_pci_bdf_valid], 1
    jne .done
    mov bl, [rtl8139_pci_bus]
    mov bh, [rtl8139_pci_dev]
    mov ch, [rtl8139_pci_fn]
    call pci_enable_bus_master
    mov ax, [pci_command_before]
    mov [rtl8139_pci_cmd_before], ax
    mov ax, [pci_command_after]
    mov [rtl8139_pci_cmd_after], ax
.done:
    ret

rtl8139_print_pci_cmd_debug:
    cmp byte [rtl8139_pci_bdf_valid], 1
    jne .unknown
    mov ax, [rtl8139_pci_cmd_before]
    mov [pci_command_before], ax
    mov ax, [rtl8139_pci_cmd_after]
    mov [pci_command_after], ax
    call pm_pci_print_command_debug
    ret
.unknown:
    mov esi, rtl8139_pci_cmd_unknown_msg
    call pm_print_string
    ret

rtl8139_configure_rx_path:
    call rtl8139_wake

    ; RX ring base.
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_RBSTART
    call rtl8139_get_rx_base
    out dx, eax

    ; Current Address of Packet Read: runtime selectable.
    call rtl8139_write_initial_capr

    ; CBR is read-only; after reset QEMU/RTL8139 should report zero.
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_CBR
    in ax, dx
    mov [rtl8139_last_cbr], ax

    ; Linux/Minix program RCR/TCR after RX/TX are enabled.
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_CR
    mov al, RTL8139_CR_RE | RTL8139_CR_TE
    out dx, al
    in al, dx
    mov [rtl8139_cr_before_rcr], al

    ; Accept all relevant Ethernet traffic.
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_RCR
    mov eax, [rtl8139_rcr_value]
    out dx, eax
    in eax, dx
    mov [rtl8139_rcr_after_write], eax

    ; Conservative TX config: IFG normal, DMA burst enabled.
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_TCR
    mov eax, [rtl8139_tcr_value]
    out dx, eax

    ; Enable the events we poll from ISR.
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_IMR
    mov ax, RTL8139_ISR_NETMASK
    out dx, ax

    ; Clear pending interrupt status after RX registers are programmed.
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_ISR
    mov ax, 0FFFFh
    out dx, ax
    ret

rtl8139_netup_debug:
    call rtl8139_print_rxmode

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
    call rtl8139_print_rcr_bits

    mov esi, rtl8139_rxbuf_config_msg
    call pm_print_string

    mov esi, rtl8139_imr_final_msg
    call pm_print_string
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_IMR
    in ax, dx
    movzx eax, ax
    call pm_print_hex32
    call pm_newline

    call rtl8139_dump_extra_regs
    ret

rtl8139_print_rxmode:
    mov esi, rtl8139_rxmode_msg
    call pm_print_string
    mov esi, [rtl8139_rxmode_name]
    call pm_print_string
    call pm_newline

    mov esi, rtl8139_rxmode_rcr_msg
    call pm_print_string
    mov eax, [rtl8139_rcr_value]
    call pm_print_hex32
    call pm_newline

    mov esi, rtl8139_rxmode_tcr_msg
    call pm_print_string
    mov eax, [rtl8139_tcr_value]
    call pm_print_hex32
    call pm_newline
    ret

rtl8139_print_rcr_bits:
    pushad
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_RCR
    in eax, dx
    mov ebx, eax

    test ebx, RTL8139_RCR_AAP
    jz .apm
    mov esi, rtl8139_rcr_aap_msg
    call pm_print_string
.apm:
    test ebx, RTL8139_RCR_APM
    jz .am
    mov esi, rtl8139_rcr_apm_msg
    call pm_print_string
.am:
    test ebx, RTL8139_RCR_AM
    jz .ab
    mov esi, rtl8139_rcr_am_msg
    call pm_print_string
.ab:
    test ebx, RTL8139_RCR_AB
    jz .wrap
    mov esi, rtl8139_rcr_ab_msg
    call pm_print_string
.wrap:
    test ebx, RTL8139_RCR_WRAP
    jz .done
    mov esi, rtl8139_rcr_wrap_msg
    call pm_print_string
.done:
    popad
    ret

rtl8139_rxbuf_phys_msg db 'RX buffer phys: ',0
rtl8139_rbstart_msg db 'RBSTART: ',0
rtl8139_cr_after_enable_msg db 'CR after enable: ',0
rtl8139_rcr_final_msg db 'RCR final: ',0
rtl8139_rxbuf_config_msg db 'RXBUF config: 8K + 16 + slack, RBLEN=00',13,0
rtl8139_imr_final_msg db 'IMR final: ',0
rtl8139_rxmode_msg db 'RX mode: ',0
rtl8139_rxmode_rcr_msg db 'RCR preset: ',0
rtl8139_rxmode_tcr_msg db 'TCR preset: ',0
rtl8139_rxmode_qemu_name db 'qemu',0
rtl8139_rxmode_permissive_name db 'permissive',0
rtl8139_rxmode_minimal_name db 'minimal',0
rtl8139_rcr_aap_msg db 'RCR bit AAP',13,0
rtl8139_rcr_apm_msg db 'RCR bit APM',13,0
rtl8139_rcr_am_msg db 'RCR bit AM',13,0
rtl8139_rcr_ab_msg db 'RCR bit AB',13,0
rtl8139_rcr_wrap_msg db 'RCR bit WRAP',13,0

rtl8139_dump_extra_regs:
    mov esi, rtl8139_rbstart_msg
    call pm_print_string
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_RBSTART
    in eax, dx
    call pm_print_hex32
    call pm_newline

    mov esi, rtl8139_tsr_msg
    call pm_print_string
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_TSD0
    in eax, dx
    call pm_print_hex32
    call pm_newline

    mov esi, rtl8139_tcr_msg
    call pm_print_string
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_TCR
    in eax, dx
    call pm_print_hex32
    call pm_newline

    mov esi, rtl8139_9346cr_msg
    call pm_print_string
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_9346CR
    in al, dx
    movzx eax, al
    call pm_print_hex32
    call pm_newline

    mov esi, rtl8139_config0_msg
    call pm_print_string
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_CONFIG0
    in al, dx
    movzx eax, al
    call pm_print_hex32
    call pm_newline

    mov esi, rtl8139_config1_msg
    call pm_print_string
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_CONFIG1
    in al, dx
    movzx eax, al
    call pm_print_hex32
    call pm_newline

    mov esi, rtl8139_config3_msg
    call pm_print_string
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_CONFIG3
    in al, dx
    movzx eax, al
    call pm_print_hex32
    call pm_newline

    mov esi, rtl8139_config4_msg
    call pm_print_string
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_CONFIG4
    in al, dx
    movzx eax, al
    call pm_print_hex32
    call pm_newline

    mov esi, rtl8139_config5_msg
    call pm_print_string
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_CONFIG5
    in al, dx
    movzx eax, al
    call pm_print_hex32
    call pm_newline

    mov esi, rtl8139_mpc_msg
    call pm_print_string
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_MPC
    in eax, dx
    call pm_print_hex32
    call pm_newline

    mov esi, rtl8139_mulint_msg
    call pm_print_string
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_MULINT
    in ax, dx
    movzx eax, ax
    call pm_print_hex32
    call pm_newline

    mov esi, rtl8139_command_msg
    call pm_print_string
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_CR
    in al, dx
    movzx eax, al
    call pm_print_hex32
    call pm_newline
    ret

rtl8139_tsr_msg db 'TSR/TSD0: ',0
rtl8139_tcr_msg db 'TCR: ',0
rtl8139_9346cr_msg db '9346CR: ',0
rtl8139_config0_msg db 'CONFIG0: ',0
rtl8139_config1_msg db 'CONFIG1: ',0
rtl8139_config3_msg db 'CONFIG3: ',0
rtl8139_config4_msg db 'CONFIG4: ',0
rtl8139_config5_msg db 'CONFIG5: ',0
rtl8139_mpc_msg db 'MPC: ',0
rtl8139_mulint_msg db 'MULINT: ',0
rtl8139_command_msg db 'COMMAND: ',0

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

    call rtl8139_print_pci_cmd_debug

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
    mov ax, 0FFF0h
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

rtl8139_rxreset:
    mov esi, pm_cmd_buffer
    call pm_skip_first_word
    call pm_skip_spaces

    cmp byte [esi], 0
    je .apply

    mov edi, rtl8139_arg_capr0
    call pm_starts_with_word
    jc .set_capr0

    mov edi, rtl8139_arg_caprf0
    call pm_starts_with_word
    jc .set_caprf0

    mov edi, rtl8139_arg_8000
    call pm_starts_with_word
    jc .experimental

    mov edi, rtl8139_arg_10000
    call pm_starts_with_word
    jc .experimental

    mov edi, rtl8139_arg_20000
    call pm_starts_with_word
    jc .experimental

    mov edi, rtl8139_arg_real
    call pm_starts_with_word
    jc .set_real

    mov esi, rtl8139_rxreset_usage_msg
    call pm_print_string
    ret

.set_capr0:
    mov byte [rtl8139_rx_capr_mode], 0
    jmp .apply

.set_caprf0:
    mov byte [rtl8139_rx_capr_mode], 1
    jmp .apply

.experimental:
    mov esi, rtl8139_rxreset_experimental_msg
    call pm_print_string
    ret

.set_real:
    mov dword [rtl8139_rx_rbstart_override], 0

.apply:
    mov byte [rtl8139_rx_ready], 0
    mov dword [rtl8139_rx_offset], 0
    call rtl8139_configure_rx_path
    mov byte [rtl8139_rx_ready], 1

    mov esi, rtl8139_rxreset_msg
    call pm_print_string
    call rtl8139_rxdebug
    ret

rtl8139_arg_capr0 db 'capr0',0
rtl8139_arg_caprf0 db 'caprf0',0
rtl8139_arg_8000 db '8000',0
rtl8139_arg_10000 db '10000',0
rtl8139_arg_20000 db '20000',0
rtl8139_arg_real db 'real',0
rtl8139_rxreset_msg db 'RTL8139 RX reset',13,0
rtl8139_rxreset_usage_msg db 'Usage: rxreset [capr0|caprf0|real] (8000/10000/20000 experimental disabled)',13,0
rtl8139_rxreset_experimental_msg db 'Experimental RBSTART override disabled; use rxreset real.',13,0

rtl8139_rxmode:
    mov esi, pm_cmd_buffer
    call pm_skip_first_word
    call pm_skip_spaces

    cmp byte [esi], 0
    je .show

    mov edi, rtl8139_rxmode_qemu_name
    call pm_starts_with_word
    jc .qemu

    mov edi, rtl8139_rxmode_permissive_name
    call pm_starts_with_word
    jc .permissive

    mov edi, rtl8139_rxmode_minimal_name
    call pm_starts_with_word
    jc .minimal

    mov esi, rtl8139_rxmode_usage_msg
    call pm_print_string
    ret

.qemu:
    mov dword [rtl8139_rcr_value], RTL8139_RCR_QEMU
    mov dword [rtl8139_tcr_value], RTL8139_TCR_QEMU
    mov dword [rtl8139_rxmode_name], rtl8139_rxmode_qemu_name
    jmp .apply

.permissive:
    mov dword [rtl8139_rcr_value], RTL8139_RCR_PERMISSIVE
    mov dword [rtl8139_tcr_value], RTL8139_TCR_PERMISSIVE
    mov dword [rtl8139_rxmode_name], rtl8139_rxmode_permissive_name
    jmp .apply

.minimal:
    mov dword [rtl8139_rcr_value], RTL8139_RCR_MINIMAL
    mov dword [rtl8139_tcr_value], RTL8139_TCR_MINIMAL
    mov dword [rtl8139_rxmode_name], rtl8139_rxmode_minimal_name

.apply:
    cmp byte [rtl8139_initialized], 1
    jne .show
    call rtl8139_configure_rx_path

.show:
    call rtl8139_print_rxmode
    ret

rtl8139_rxmode_usage_msg db 'Usage: rxmode [qemu|permissive|minimal]',13,0

rtl8139_rxtest:
    mov esi, pm_cmd_buffer
    call pm_skip_first_word
    call pm_skip_spaces

    mov edi, rtl8139_arg_verbose
    call pm_starts_with_word
    jc rtl8139_rxtest_verbose

    mov dword [rtl8139_rcr_value], RTL8139_RCR_QEMU
    mov dword [rtl8139_tcr_value], RTL8139_TCR_QEMU
    mov dword [rtl8139_rxmode_name], rtl8139_rxmode_qemu_name
    mov dword [rtl8139_rx_rbstart_override], 0
    mov byte [rtl8139_rx_capr_mode], 1
    mov byte [rtl8139_gateway_mac_known], 0
    mov dword [rtl8139_rxtest_first_before], 0
    mov dword [rtl8139_rxtest_first_after], 0
    mov word [rtl8139_rxtest_cbr_before], 0
    mov word [rtl8139_rxtest_cbr_after], 0
    mov word [rtl8139_rxtest_isr_before], 0
    mov word [rtl8139_rxtest_isr_after], 0
    mov dword [rtl8139_rxtest_tsd0], 0
    mov byte [rtl8139_cr_before_rcr], 0
    mov dword [rtl8139_rcr_after_write], 0

    call rtl8139_rxtest_quiet_netup
    cmp byte [rtl8139_initialized], 1
    jne .done

    mov eax, [rtl8139_rx_buffer]
    mov [rtl8139_rxtest_first_before], eax

    mov dx, [rtl8139_io_base]
    add dx, RTL8139_CBR
    in ax, dx
    mov [rtl8139_rxtest_cbr_before], ax

    mov dx, [rtl8139_io_base]
    add dx, RTL8139_ISR
    in ax, dx
    mov [rtl8139_rxtest_isr_before], ax

    call rtl8139_build_arp_request
    call rtl8139_rxtest_tx_submit_silent

    mov dx, [rtl8139_io_base]
    add dx, RTL8139_TSD0
    in eax, dx
    mov [rtl8139_rxtest_tsd0], eax

    mov ecx, 30000
.poll_loop:
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_ISR
    in ax, dx
    mov [rtl8139_rxtest_isr_after], ax
    test ax, RTL8139_ISR_ROK
    jnz .poll_done

    mov dx, [rtl8139_io_base]
    add dx, RTL8139_CBR
    in ax, dx
    cmp ax, [rtl8139_last_cbr]
    jne .poll_done

    mov eax, [rtl8139_rx_buffer]
    cmp eax, [rtl8139_rxtest_first_before]
    jne .poll_done
    loop .poll_loop

.poll_done:
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_CBR
    in ax, dx
    mov [rtl8139_rxtest_cbr_after], ax

    mov dx, [rtl8139_io_base]
    add dx, RTL8139_ISR
    in ax, dx
    mov [rtl8139_rxtest_isr_after], ax

    mov eax, [rtl8139_rx_buffer]
    mov [rtl8139_rxtest_first_after], eax

    call rtl8139_rxtest_parse_silent
    call rtl8139_rxtest_summary

.done:
    ret

rtl8139_rxtest_verbose:
    mov esi, rtl8139_rxtest_verbose_msg
    call pm_print_string
    call rtl8139_netup
    cmp byte [rtl8139_initialized], 1
    jne .done
    mov eax, [rtl8139_rx_buffer]
    mov [rtl8139_rxtest_first_before], eax
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_CBR
    in ax, dx
    mov [rtl8139_rxtest_cbr_before], ax
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_ISR
    in ax, dx
    mov [rtl8139_rxtest_isr_before], ax
    call rtl8139_build_arp_request
    call rtl8139_tx_submit_current
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_TSD0
    in eax, dx
    mov [rtl8139_rxtest_tsd0], eax
    call rtl8139_rxdebug
    call rtl8139_rxpacket
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_CBR
    in ax, dx
    mov [rtl8139_rxtest_cbr_after], ax
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_ISR
    in ax, dx
    mov [rtl8139_rxtest_isr_after], ax
    mov eax, [rtl8139_rx_buffer]
    mov [rtl8139_rxtest_first_after], eax
    call rtl8139_rxtest_summary
.done:
    ret

rtl8139_rxtest_quiet_netup:
    call rtl8139_bind_pci_io_quiet
    mov byte [rtl8139_initialized], 0
    mov byte [rtl8139_rx_ready], 0
    mov dword [rtl8139_rx_offset], 0
    mov dword [rtl8139_rx_first_dword], 0

    mov dx, [rtl8139_io_base]
    add dx, RTL8139_CR
    mov al, RTL8139_CR_RST
    out dx, al

    mov ecx, 100000
.reset_wait:
    in al, dx
    test al, RTL8139_CR_RST
    jz .reset_done
    loop .reset_wait
    stc
    ret

.reset_done:
    call rtl8139_configure_rx_path
    mov byte [rtl8139_rx_ready], 1
    mov byte [rtl8139_initialized], 1
    clc
    ret

rtl8139_bind_pci_io_quiet:
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
    call rtl8139_save_pci_bdf
    call rtl8139_enable_pci_bus_master
    mov byte [pm_pci_nic_type], PCI_NIC_RTL8139
    ret

.fallback:
    mov word [rtl8139_io_base], 0C000h
    mov dword [rtl8139_bar_raw], 0
    mov byte [rtl8139_bar_index], 0
    mov byte [rtl8139_pci_bdf_valid], 0
    ret

rtl8139_rxtest_tx_submit_silent:
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_TSAD0
    mov eax, rtl8139_tx_buffer
    out dx, eax

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
    jnz .done
    loop .wait_tx
.done:
    mov [rtl8139_rxtest_tsd0], eax
    ret

rtl8139_rxtest_parse_silent:
    pushad
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_CR
    in al, dx
    test al, RTL8139_CR_BUFE
    jnz .done

    mov ebx, [rtl8139_rx_offset]
    and ebx, RTL8139_RX_RING_MASK
    movzx eax, word [rtl8139_rx_buffer + ebx + 2]
    cmp eax, 42
    jb .done
    cmp eax, 1600
    ja .done

    mov esi, rtl8139_rx_buffer
    add esi, ebx
    add esi, 4
    cmp byte [esi + 12], 08h
    jne .advance
    cmp byte [esi + 13], 06h
    jne .advance
    cmp byte [esi + 20], 00h
    jne .advance
    cmp byte [esi + 21], 02h
    jne .advance
    cmp byte [esi + 28], 10
    jne .advance
    cmp byte [esi + 29], 0
    jne .advance
    cmp byte [esi + 30], 2
    jne .advance
    cmp byte [esi + 31], 2
    jne .advance

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

.advance:
    movzx eax, word [rtl8139_rx_buffer + ebx + 2]
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
.done:
    popad
    ret

rtl8139_rxtest_summary:
    mov esi, rtl8139_rxtest_summary_msg
    call pm_print_string

    mov esi, rtl8139_rxtest_mode_msg
    call pm_print_string
    mov esi, [rtl8139_rxmode_name]
    call pm_print_string
    call pm_newline

    mov esi, rtl8139_rbstart_msg
    call pm_print_string
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_RBSTART
    in eax, dx
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

    mov esi, rtl8139_rxtest_cbr_before_msg
    call pm_print_string
    movzx eax, word [rtl8139_rxtest_cbr_before]
    call pm_print_hex32
    call pm_newline

    mov esi, rtl8139_rxtest_cbr_after_msg
    call pm_print_string
    movzx eax, word [rtl8139_rxtest_cbr_after]
    call pm_print_hex32
    call pm_newline

    mov esi, rtl8139_rxtest_isr_before_msg
    call pm_print_string
    movzx eax, word [rtl8139_rxtest_isr_before]
    call pm_print_hex32
    call pm_newline

    mov esi, rtl8139_rxtest_isr_after_msg
    call pm_print_string
    movzx eax, word [rtl8139_rxtest_isr_after]
    call pm_print_hex32
    call pm_newline

    mov esi, rtl8139_rxtest_cr_before_rcr_msg
    call pm_print_string
    movzx eax, byte [rtl8139_cr_before_rcr]
    call pm_print_hex32
    call pm_newline

    mov esi, rtl8139_rxtest_rcr_after_write_msg
    call pm_print_string
    mov eax, [rtl8139_rcr_after_write]
    call pm_print_hex32
    call pm_newline

    call rtl8139_print_pci_cmd_debug

    mov esi, rtl8139_tcr_msg
    call pm_print_string
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_TCR
    in eax, dx
    call pm_print_hex32
    call pm_newline

    mov esi, rtl8139_rxstatus_cr_msg
    call pm_print_string
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_CR
    in al, dx
    movzx eax, al
    call pm_print_hex32
    call pm_newline

    mov esi, rtl8139_rxtest_tok_msg
    call pm_print_string
    mov ebx, [rtl8139_rxtest_tsd0]
    test ebx, RTL8139_TSD_TOK
    jz .tok_no
    call rtl8139_print_yes
    jmp .rok
.tok_no:
    call rtl8139_print_no

.rok:
    mov esi, rtl8139_rxtest_rok_msg
    call pm_print_string
    mov bx, [rtl8139_rxtest_isr_after]
    test bx, RTL8139_ISR_ROK
    jz .rok_no
    call rtl8139_print_yes
    jmp .ring
.rok_no:
    call rtl8139_print_no

.ring:
    mov esi, rtl8139_rxtest_mem_changed_msg
    call pm_print_string
    mov eax, [rtl8139_rxtest_first_after]
    cmp eax, [rtl8139_rxtest_first_before]
    jne .mem_yes
    call rtl8139_print_no
    jmp .first
.mem_yes:
    call rtl8139_print_yes

.first:
    mov esi, rtl8139_rxdebug_first_msg
    call pm_print_string
    mov eax, [rtl8139_rxtest_first_after]
    call pm_print_hex32
    call pm_newline

.arp:
    mov esi, rtl8139_rxtest_arp_state_msg
    call pm_print_string
    cmp byte [rtl8139_gateway_mac_known], 1
    jne .arp_unknown
    mov esi, rtl8139_known_msg
    call pm_print_string
    jmp .gateway
.arp_unknown:
    mov esi, rtl8139_unknown_word_msg
    call pm_print_string

.gateway:
    mov esi, rtl8139_rxtest_gateway_mac_msg
    call pm_print_string
    cmp byte [rtl8139_gateway_mac_known], 1
    jne .gateway_unknown
    mov esi, rtl8139_gateway_mac
    call rtl8139_dump_mac
    call pm_newline
    ret
.gateway_unknown:
    mov esi, rtl8139_unknown_line_msg
    call pm_print_string
    ret

rtl8139_print_yes:
    mov esi, rtl8139_yes_msg
    call pm_print_string
    ret

rtl8139_print_no:
    mov esi, rtl8139_no_msg
    call pm_print_string
    ret

rtl8139_arg_verbose db 'verbose',0
rtl8139_rxtest_verbose_msg db 'RTL8139 RX test verbose',13,0
rtl8139_rxtest_summary_msg db 'RXTEST SUMMARY',13,0
rtl8139_rxtest_mode_msg db 'mode: ',0
rtl8139_rxtest_cbr_before_msg db 'CBR before: ',0
rtl8139_rxtest_cbr_after_msg db 'CBR after: ',0
rtl8139_rxtest_isr_before_msg db 'ISR before: ',0
rtl8139_rxtest_isr_after_msg db 'ISR after: ',0
rtl8139_rxtest_cr_before_rcr_msg db 'CR before RCR: ',0
rtl8139_rxtest_rcr_after_write_msg db 'RCR after write: ',0
rtl8139_rxtest_mem_changed_msg db 'RX ring changed: ',0
rtl8139_rxtest_tok_msg db 'TOK: ',0
rtl8139_rxtest_rok_msg db 'ROK: ',0
rtl8139_rxtest_arp_state_msg db 'ARP state: ',0
rtl8139_rxtest_gateway_mac_msg db 'gateway_mac: ',0
rtl8139_known_msg db 'known',13,0
rtl8139_unknown_word_msg db 'unknown',13,0
rtl8139_unknown_line_msg db 'unknown',13,0
rtl8139_yes_msg db 'yes',13,0
rtl8139_no_msg db 'no',13,0

rtl8139_rxdebug:
    cmp byte [rtl8139_rx_ready], 1
    jne .not_ready

    mov esi, rtl8139_rxdebug_msg
    call pm_print_string

    call rtl8139_rxstatus
    call rtl8139_dump_extra_regs

    mov esi, rtl8139_rxdebug_first_msg
    call pm_print_string
    mov eax, [rtl8139_rx_buffer]
    call pm_print_hex32
    call pm_newline

    mov eax, [rtl8139_rx_buffer]
    cmp eax, [rtl8139_rx_first_dword]
    je .same_mem
    mov [rtl8139_rx_first_dword], eax
    mov esi, rtl8139_rxdebug_mem_changed_msg
    call pm_print_string
    jmp .dump_ring

.same_mem:
    mov esi, rtl8139_rxdebug_mem_same_msg
    call pm_print_string

.dump_ring:
    mov esi, rtl8139_rxpeek_msg
    call pm_print_string
    mov esi, rtl8139_rx_buffer
    mov ecx, 64
    call rtl8139_dump_bytes

    mov esi, rtl8139_rxdebug_poll_msg
    call pm_print_string
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_CBR
    in ax, dx
    mov bx, ax
    mov ecx, 100

.poll_loop:
    mov dx, [rtl8139_io_base]
    add dx, RTL8139_CBR
    in ax, dx
    cmp ax, bx
    jne .changed
    loop .poll_loop

    mov esi, rtl8139_rxdebug_cbr_same_msg
    call pm_print_string
    movzx eax, bx
    call pm_print_hex32
    call pm_newline
    ret

.changed:
    mov esi, rtl8139_rxdebug_cbr_changed_msg
    call pm_print_string
    movzx eax, ax
    call pm_print_hex32
    call pm_newline
    ret

.not_ready:
    mov esi, rtl8139_rxpeek_notready_msg
    call pm_print_string
    ret

rtl8139_rxdebug_msg db 'RTL8139 RX debug',13,0
rtl8139_rxdebug_first_msg db 'RX first dword: ',0
rtl8139_rxdebug_mem_changed_msg db 'RX memory changed',13,0
rtl8139_rxdebug_mem_same_msg db 'RX memory unchanged',13,0
rtl8139_rxdebug_poll_msg db 'CBR poll: ',0
rtl8139_rxdebug_cbr_same_msg db 'unchanged ',0
rtl8139_rxdebug_cbr_changed_msg db 'changed ',0

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

    call rtl8139_rx_dump_l3_fields

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

rtl8139_rx_dump_l3_fields:
    pushad

    mov esi, rtl8139_rx_ethertype_msg
    call pm_print_string
    movzx eax, byte [edi + 12]
    shl eax, 8
    movzx ebx, byte [edi + 13]
    or eax, ebx
    call pm_print_hex32
    call pm_newline

    cmp byte [edi + 12], 08h
    jne .not_ipv4
    cmp byte [edi + 13], 00h
    jne .not_ipv4
    cmp byte [edi + 14], 45h
    jne .not_ipv4

    mov esi, rtl8139_rx_ipv4_protocol_msg
    call pm_print_string
    movzx eax, byte [edi + 23]
    call pm_print_uint32
    call pm_newline

    mov esi, rtl8139_rx_src_ip_msg
    call pm_print_string
    lea esi, [edi + 26]
    call rtl8139_dump_ip
    call pm_newline

    mov esi, rtl8139_rx_dst_ip_msg
    call pm_print_string
    lea esi, [edi + 30]
    call rtl8139_dump_ip
    call pm_newline

    mov esi, rtl8139_rx_icmp_type_msg
    call pm_print_string
    cmp byte [edi + 23], 01h
    jne .icmp_na
    movzx eax, byte [edi + 34]
    call pm_print_uint32
    call pm_newline
    jmp .done

.icmp_na:
    mov esi, rtl8139_na_msg
    call pm_print_string
    jmp .done

.not_ipv4:
    mov esi, rtl8139_rx_ipv4_protocol_msg
    call pm_print_string
    mov esi, rtl8139_na_msg
    call pm_print_string
    mov esi, rtl8139_rx_icmp_type_msg
    call pm_print_string
    mov esi, rtl8139_na_msg
    call pm_print_string
    mov esi, rtl8139_rx_src_ip_msg
    call pm_print_string
    mov esi, rtl8139_na_msg
    call pm_print_string
    mov esi, rtl8139_rx_dst_ip_msg
    call pm_print_string
    mov esi, rtl8139_na_msg
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

rtl8139_rxipv4dump:
    cmp byte [rtl8139_rx_ready], 1
    jne .not_ready

    mov esi, rtl8139_rxipv4dump_msg
    call pm_print_string

    mov ecx, 16
.loop:
    push ecx
    call rtl8139_wait_for_rx_activity

    mov dx, [rtl8139_io_base]
    add dx, RTL8139_CR
    in al, dx
    test al, RTL8139_CR_BUFE
    jnz .empty

    call rtl8139_rx_packet
    pop ecx
    loop .loop
    ret

.empty:
    pop ecx
    ret

.not_ready:
    mov esi, rtl8139_rxpeek_notready_msg
    call pm_print_string
    ret

rtl8139_rx_process_packet_quiet:
    pushad

    mov dx, [rtl8139_io_base]
    add dx, RTL8139_CBR
    in ax, dx
    mov [rtl8139_cbr_snapshot], ax

    mov dx, [rtl8139_io_base]
    add dx, RTL8139_CR
    in al, dx
    test al, RTL8139_CR_BUFE
    jnz .done

    movzx eax, word [rtl8139_cbr_snapshot]
    and eax, RTL8139_RX_RING_MASK
    cmp eax, [rtl8139_rx_offset]
    je .done

    mov ebx, [rtl8139_rx_offset]
    and ebx, RTL8139_RX_RING_MASK
    mov [rtl8139_rx_offset], ebx

    movzx edx, word [rtl8139_rx_buffer + ebx + 2]
    cmp edx, 14
    jb .discard
    cmp edx, 1600
    ja .discard

    mov esi, rtl8139_rx_buffer
    add esi, ebx
    add esi, 4

    cmp byte [esi + 12], 08h
    jne .advance
    cmp byte [esi + 13], 06h
    je .arp
    cmp byte [esi + 13], 00h
    je .ipv4
    jmp .advance

.arp:
    cmp edx, 42
    jb .advance
    cmp byte [esi + 20], 00h
    jne .advance
    cmp byte [esi + 21], 02h
    jne .advance
    cmp byte [esi + 28], 10
    jne .advance
    cmp byte [esi + 29], 0
    jne .advance
    cmp byte [esi + 30], 2
    jne .advance
    cmp byte [esi + 31], 2
    jne .advance

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
    jmp .advance

.ipv4:
    cmp edx, 42
    jb .advance
    cmp byte [esi + 14], 45h
    jne .advance
    cmp byte [esi + 23], 01h
    je .icmp
    cmp byte [esi + 23], 11h
    je .udp
    cmp byte [esi + 23], 06h
    je .tcp
    jmp .advance

.icmp:
    cmp byte [esi + 34], 00h
    jne .advance
    cmp byte [esi + 35], 00h
    jne .advance

    mov al, [esi + 26]
    mov [rtl8139_last_icmp_src_ip], al
    mov al, [esi + 27]
    mov [rtl8139_last_icmp_src_ip + 1], al
    mov al, [esi + 28]
    mov [rtl8139_last_icmp_src_ip + 2], al
    mov al, [esi + 29]
    mov [rtl8139_last_icmp_src_ip + 3], al
    mov al, [esi + 22]
    mov [rtl8139_last_icmp_ttl], al
    movzx eax, byte [esi + 40]
    shl eax, 8
    movzx edx, byte [esi + 41]
    or eax, edx
    mov [rtl8139_last_icmp_seq], ax
    inc dword [rtl8139_icmp_reply_count]
    jmp .advance

.udp:
    mov al, [esi + 26]
    mov [rtl8139_last_udp_src_ip], al
    mov al, [esi + 27]
    mov [rtl8139_last_udp_src_ip + 1], al
    mov al, [esi + 28]
    mov [rtl8139_last_udp_src_ip + 2], al
    mov al, [esi + 29]
    mov [rtl8139_last_udp_src_ip + 3], al
    movzx eax, byte [esi + 34]
    shl eax, 8
    movzx edx, byte [esi + 35]
    or eax, edx
    mov [rtl8139_last_udp_src_port], ax
    movzx eax, byte [esi + 36]
    shl eax, 8
    movzx edx, byte [esi + 37]
    or eax, edx
    mov [rtl8139_last_udp_dst_port], ax
    movzx eax, byte [esi + 38]
    shl eax, 8
    movzx edx, byte [esi + 39]
    or eax, edx
    mov [rtl8139_last_udp_len], ax
    inc dword [rtl8139_udp_rx_count]
    jmp .advance

.tcp:
    call rtl8139_rx_record_tcp_current

.advance:
    call rtl8139_rx_advance_current
    jmp .done

.discard:
    call rtl8139_rx_discard_to_cbr

.done:
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
    cmp byte [esi + 21], 01h
    je .arp_request
    cmp byte [esi + 21], 02h
    je .arp_reply
    jmp .done

.arp_request:
    mov esi, rtl8139_arp_request_msg
    call pm_print_string
    jmp .done

.arp_reply:
    push esi
    mov esi, rtl8139_arp_reply_msg
    call pm_print_string
    pop esi

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
    je .icmp_ipv4
    cmp byte [esi + 23], 11h
    je .udp_ipv4
    cmp byte [esi + 23], 06h
    je .tcp_ipv4
    jmp .done

.icmp_ipv4:
    cmp byte [esi + 35], 00h
    jne .done
    cmp byte [esi + 34], 00h
    jne .done

    mov al, [esi + 26]
    mov [rtl8139_last_icmp_src_ip], al
    mov al, [esi + 27]
    mov [rtl8139_last_icmp_src_ip + 1], al
    mov al, [esi + 28]
    mov [rtl8139_last_icmp_src_ip + 2], al
    mov al, [esi + 29]
    mov [rtl8139_last_icmp_src_ip + 3], al
    mov al, [esi + 22]
    mov [rtl8139_last_icmp_ttl], al
    movzx eax, byte [esi + 40]
    shl eax, 8
    movzx edx, byte [esi + 41]
    or eax, edx
    mov [rtl8139_last_icmp_seq], ax

    inc dword [rtl8139_icmp_reply_count]
    mov esi, rtl8139_icmp_reply_msg
    call pm_print_string
    mov esi, rtl8139_icmp_src_msg
    call pm_print_string
    mov esi, rtl8139_last_icmp_src_ip
    call rtl8139_dump_ip
    call pm_newline
    mov esi, rtl8139_icmp_seq_msg
    call pm_print_string
    movzx eax, word [rtl8139_last_icmp_seq]
    call pm_print_uint32
    call pm_newline
    mov esi, rtl8139_icmp_ttl_msg
    call pm_print_string
    movzx eax, byte [rtl8139_last_icmp_ttl]
    call pm_print_uint32
    call pm_newline
    jmp .done

.udp_ipv4:
    mov al, [esi + 26]
    mov [rtl8139_last_udp_src_ip], al
    mov al, [esi + 27]
    mov [rtl8139_last_udp_src_ip + 1], al
    mov al, [esi + 28]
    mov [rtl8139_last_udp_src_ip + 2], al
    mov al, [esi + 29]
    mov [rtl8139_last_udp_src_ip + 3], al
    movzx eax, byte [esi + 34]
    shl eax, 8
    movzx edx, byte [esi + 35]
    or eax, edx
    mov [rtl8139_last_udp_src_port], ax
    movzx eax, byte [esi + 36]
    shl eax, 8
    movzx edx, byte [esi + 37]
    or eax, edx
    mov [rtl8139_last_udp_dst_port], ax
    movzx eax, byte [esi + 38]
    shl eax, 8
    movzx edx, byte [esi + 39]
    or eax, edx
    mov [rtl8139_last_udp_len], ax

    inc dword [rtl8139_udp_rx_count]
    mov esi, rtl8139_udp_packet_msg
    call pm_print_string
    mov esi, rtl8139_udp_src_ip_msg
    call pm_print_string
    mov esi, rtl8139_last_udp_src_ip
    call rtl8139_dump_ip
    call pm_newline
    mov esi, rtl8139_udp_src_port_msg
    call pm_print_string
    movzx eax, word [rtl8139_last_udp_src_port]
    call pm_print_uint32
    call pm_newline
    mov esi, rtl8139_udp_dst_port_msg
    call pm_print_string
    movzx eax, word [rtl8139_last_udp_dst_port]
    call pm_print_uint32
    call pm_newline
    mov esi, rtl8139_udp_len_msg
    call pm_print_string
    movzx eax, word [rtl8139_last_udp_len]
    call pm_print_uint32
    call pm_newline
    jmp .done

.tcp_ipv4:
    call rtl8139_rx_record_tcp_current
    mov esi, rtl8139_tcp_packet_msg
    call pm_print_string
    mov esi, rtl8139_tcp_src_ip_msg
    call pm_print_string
    mov esi, rtl8139_tcp_last_src_ip
    call rtl8139_dump_ip
    call pm_newline
    mov esi, rtl8139_tcp_src_port_msg
    call pm_print_string
    movzx eax, word [rtl8139_tcp_last_src_port]
    call pm_print_uint32
    call pm_newline
    mov esi, rtl8139_tcp_flags_msg
    call pm_print_string
    movzx eax, byte [rtl8139_tcp_last_flags]
    call pm_print_hex32
    call pm_newline
    jmp .done

.done:
    popad
    ret

rtl8139_rx_record_tcp_current:
    pushad

    cmp byte [esi + 23], 06h
    jne .done

    mov al, [esi + 26]
    mov [rtl8139_tcp_last_src_ip], al
    mov al, [esi + 27]
    mov [rtl8139_tcp_last_src_ip + 1], al
    mov al, [esi + 28]
    mov [rtl8139_tcp_last_src_ip + 2], al
    mov al, [esi + 29]
    mov [rtl8139_tcp_last_src_ip + 3], al

    movzx eax, byte [esi + 34]
    shl eax, 8
    movzx edx, byte [esi + 35]
    or eax, edx
    mov [rtl8139_tcp_last_src_port], ax

    movzx eax, byte [esi + 36]
    shl eax, 8
    movzx edx, byte [esi + 37]
    or eax, edx
    mov [rtl8139_tcp_last_dst_port], ax

    movzx eax, byte [esi + 38]
    shl eax, 8
    movzx edx, byte [esi + 39]
    or eax, edx
    shl eax, 8
    movzx edx, byte [esi + 40]
    or eax, edx
    shl eax, 8
    movzx edx, byte [esi + 41]
    or eax, edx
    mov [rtl8139_tcp_last_seq], eax

    movzx eax, byte [esi + 42]
    shl eax, 8
    movzx edx, byte [esi + 43]
    or eax, edx
    shl eax, 8
    movzx edx, byte [esi + 44]
    or eax, edx
    shl eax, 8
    movzx edx, byte [esi + 45]
    or eax, edx
    mov [rtl8139_tcp_last_ack], eax

    mov al, [esi + 47]
    mov [rtl8139_tcp_last_flags], al
    inc dword [rtl8139_tcp_rx_count]

    test al, 01h
    jz .no_fin_count
    inc dword [rtl8139_tcp_fin_rx_count]
.no_fin_count:

    mov ax, [rtl8139_tcp_last_src_port]
    cmp ax, [rtl8139_tcp_dst_port]
    jne .done
    mov ax, [rtl8139_tcp_last_dst_port]
    cmp ax, [rtl8139_tcp_src_port]
    jne .done

    ; If this packet belongs to the active TCP flow, compute payload length.
    ; ESI points to Ethernet frame start.
    ; IPv4 total length: [esi+16..17]
    movzx eax, byte [esi + 16]
    shl eax, 8
    movzx edx, byte [esi + 17]
    or eax, edx

    ; IPv4 header length = low nibble of [esi+14] * 4
    movzx ebx, byte [esi + 14]
    and ebx, 0Fh
    shl ebx, 2

    cmp eax, ebx
    jbe .check_synack
    sub eax, ebx

    ; TCP header length = high nibble of TCP data offset byte * 4
    ; TCP starts at Ethernet 14 + IPv4 header length.
    movzx ecx, byte [esi + 46]
    shr ecx, 4
    shl ecx, 2

    cmp eax, ecx
    jbe .check_synack
    sub eax, ecx

    cmp eax, 0
    je .check_synack

    ; Store real TCP payload length for ACK calculation.
    mov [rtl8139_tcp_last_payload_wire_len], ax

    ; Limit stored payload to 383 bytes and NUL terminate.
    cmp eax, 383
    jbe .payload_len_ok
    mov eax, 383

.payload_len_ok:
    mov [rtl8139_tcp_last_payload_len], ax
    inc dword [rtl8139_tcp_data_rx_count]

    ; Payload pointer = frame + 14 + ip_header_len + tcp_header_len
    mov edx, esi
    add edx, 14
    add edx, ebx
    add edx, ecx

    push esi
    mov esi, edx
    mov edi, rtl8139_tcp_payload_buffer
    mov ecx, eax

.copy_payload:
    cmp ecx, 0
    je .payload_done
    lodsb
    stosb
    dec ecx
    jmp .copy_payload

.payload_done:
    mov byte [edi], 0
    pop esi

.check_synack:
    mov al, [rtl8139_tcp_last_flags]
    and al, 12h
    cmp al, 12h
    jne .done

    inc dword [rtl8139_tcp_synack_count]

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

rtl8139_print_tls_type_name:
    pushad

    mov esi, rtl8139_tls_type_name_msg
    call pm_print_string

    cmp al, 14h
    je .ccs
    cmp al, 15h
    je .alert
    cmp al, 16h
    je .handshake
    cmp al, 17h
    je .appdata

    mov esi, rtl8139_tls_name_unknown_msg
    call pm_print_string
    jmp .done

.ccs:
    mov esi, rtl8139_tls_name_ccs_msg
    call pm_print_string
    jmp .done

.alert:
    mov esi, rtl8139_tls_name_alert_msg
    call pm_print_string
    jmp .done

.handshake:
    mov esi, rtl8139_tls_name_handshake_msg
    call pm_print_string
    jmp .done

.appdata:
    mov esi, rtl8139_tls_name_appdata_msg
    call pm_print_string

.done:
    popad
    ret

rtl8139_print_tls_handshake_name:
    pushad

    mov esi, rtl8139_tls_handshake_name_msg
    call pm_print_string

    cmp al, 01h
    je .client_hello
    cmp al, 02h
    je .server_hello
    cmp al, 08h
    je .encrypted_extensions
    cmp al, 0Bh
    je .certificate
    cmp al, 0Fh
    je .certificate_verify
    cmp al, 14h
    je .finished

    mov esi, rtl8139_tls_name_unknown_msg
    call pm_print_string
    jmp .done

.client_hello:
    mov esi, rtl8139_tls_hs_clienthello_msg
    call pm_print_string
    jmp .done

.server_hello:
    mov esi, rtl8139_tls_hs_serverhello_msg
    call pm_print_string
    jmp .done

.encrypted_extensions:
    mov esi, rtl8139_tls_hs_encrypted_extensions_msg
    call pm_print_string
    jmp .done

.certificate:
    mov esi, rtl8139_tls_hs_certificate_msg
    call pm_print_string
    jmp .done

.certificate_verify:
    mov esi, rtl8139_tls_hs_certificate_verify_msg
    call pm_print_string
    jmp .done

.finished:
    mov esi, rtl8139_tls_hs_finished_msg
    call pm_print_string

.done:
    popad
    ret

rtl8139_print_tls_serverhello_fields:
    pushad

    ; EDI = TLS record offset in rtl8139_tcp_payload_buffer.
    ; TLS record header = 5 bytes.
    ; Handshake header = 4 bytes.
    ; ServerHello body starts at EDI + 9.
    movzx ecx, word [rtl8139_tcp_last_payload_len]

    ; Need at least up to session_id_len at body + 34.
    mov eax, edi
    add eax, 44
    cmp eax, ecx
    ja .short

    mov esi, rtl8139_tls_sh_legacy_msg
    call pm_print_string
    movzx eax, byte [rtl8139_tcp_payload_buffer + edi + 9]
    shl eax, 8
    movzx edx, byte [rtl8139_tcp_payload_buffer + edi + 10]
    or eax, edx
    call pm_print_hex32
    call pm_newline

    ; session_id_len at body + 34 = EDI + 43
    movzx ebx, byte [rtl8139_tcp_payload_buffer + edi + 43]

    ; cipher_suite offset = body + 35 + session_id_len = EDI + 44 + sid_len
    mov edx, edi
    add edx, 44
    add edx, ebx

    ; Need cipher_suite 2 bytes + compression 1 + extensions_len 2
    mov eax, edx
    add eax, 5
    cmp eax, ecx
    ja .short

    mov esi, rtl8139_tls_sh_cipher_msg
    call pm_print_string
    movzx eax, byte [rtl8139_tcp_payload_buffer + edx]
    shl eax, 8
    movzx ebx, byte [rtl8139_tcp_payload_buffer + edx + 1]
    or eax, ebx
    call pm_print_hex32
    call pm_newline

    ; extensions length offset = cipher_offset + 3
    add edx, 3

    mov esi, rtl8139_tls_sh_extlen_msg
    call pm_print_string
    movzx eax, byte [rtl8139_tcp_payload_buffer + edx]
    shl eax, 8
    movzx ebx, byte [rtl8139_tcp_payload_buffer + edx + 1]
    or eax, ebx
    mov [rtl8139_tls_sh_ext_len], eax
    call pm_print_uint32
    call pm_newline

    ; extensions start after the 2-byte extensions length.
    add edx, 2
    mov [rtl8139_tls_sh_ext_start], edx
    mov eax, [rtl8139_tls_sh_ext_len]
    add eax, edx
    mov [rtl8139_tls_sh_ext_end], eax
    cmp eax, ecx
    ja .short

    call rtl8139_parse_tls_serverhello_extensions

    jmp .done

.short:
    mov esi, rtl8139_tls_sh_short_msg
    call pm_print_string

.done:
    popad
    ret

rtl8139_parse_tls_serverhello_extensions:
    pushad

    mov esi, rtl8139_tls_sh_extensions_msg
    call pm_print_string

    mov eax, [rtl8139_tls_sh_ext_start]
    mov [rtl8139_tls_sh_ext_ptr], eax

.next_ext:
    mov edx, [rtl8139_tls_sh_ext_ptr]
    mov ebx, edx
    add ebx, 4
    cmp ebx, [rtl8139_tls_sh_ext_end]
    ja .done

    movzx eax, byte [rtl8139_tcp_payload_buffer + edx]
    shl eax, 8
    movzx ecx, byte [rtl8139_tcp_payload_buffer + edx + 1]
    or eax, ecx
    mov [rtl8139_tls_sh_ext_type], ax

    movzx eax, byte [rtl8139_tcp_payload_buffer + edx + 2]
    shl eax, 8
    movzx ecx, byte [rtl8139_tcp_payload_buffer + edx + 3]
    or eax, ecx
    mov [rtl8139_tls_sh_cur_ext_len], eax

    mov ebx, edx
    add ebx, 4
    add ebx, eax
    cmp ebx, [rtl8139_tls_sh_ext_end]
    ja .done

    mov esi, rtl8139_tls_sh_ext_msg
    call pm_print_string
    mov ax, [rtl8139_tls_sh_ext_type]
    call rtl8139_print_hex16

    cmp word [rtl8139_tls_sh_ext_type], 002Bh
    je .print_supported_versions_name
    cmp word [rtl8139_tls_sh_ext_type], 0033h
    je .print_key_share_name

    call pm_newline
    jmp .print_ext_len

.print_supported_versions_name:
    mov esi, rtl8139_tls_sh_supported_versions_name_msg
    call pm_print_string
    jmp .print_ext_len

.print_key_share_name:
    mov esi, rtl8139_tls_sh_key_share_name_msg
    call pm_print_string

.print_ext_len:
    mov esi, rtl8139_tls_sh_ext_len_msg
    call pm_print_string
    mov eax, [rtl8139_tls_sh_cur_ext_len]
    call pm_print_uint32
    call pm_newline

    cmp word [rtl8139_tls_sh_ext_type], 002Bh
    je .supported_versions
    cmp word [rtl8139_tls_sh_ext_type], 0033h
    je .key_share
    jmp .advance

.supported_versions:
    cmp dword [rtl8139_tls_sh_cur_ext_len], 2
    jb .advance

    mov edx, [rtl8139_tls_sh_ext_ptr]
    add edx, 4
    movzx eax, byte [rtl8139_tcp_payload_buffer + edx]
    shl eax, 8
    movzx ecx, byte [rtl8139_tcp_payload_buffer + edx + 1]
    or eax, ecx
    mov [rtl8139_tls_selected_version], ax

    mov esi, rtl8139_tls_sh_selected_version_msg
    call pm_print_string
    mov ax, [rtl8139_tls_selected_version]
    call rtl8139_print_hex16
    call pm_newline
    call pm_newline
    jmp .advance

.key_share:
    cmp dword [rtl8139_tls_sh_cur_ext_len], 4
    jb .advance

    mov edx, [rtl8139_tls_sh_ext_ptr]
    add edx, 4

    movzx eax, byte [rtl8139_tcp_payload_buffer + edx]
    shl eax, 8
    movzx ecx, byte [rtl8139_tcp_payload_buffer + edx + 1]
    or eax, ecx
    mov [rtl8139_tls_selected_group], ax

    movzx eax, byte [rtl8139_tcp_payload_buffer + edx + 2]
    shl eax, 8
    movzx ecx, byte [rtl8139_tcp_payload_buffer + edx + 3]
    or eax, ecx
    mov [rtl8139_tls_server_key_share_len], eax

    mov esi, rtl8139_tls_sh_group_msg
    call pm_print_string
    mov ax, [rtl8139_tls_selected_group]
    call rtl8139_print_hex16
    cmp word [rtl8139_tls_selected_group], 001Dh
    jne .group_newline
    mov esi, rtl8139_tls_sh_x25519_name_msg
    call pm_print_string
.group_newline:
    call pm_newline

    mov esi, rtl8139_tls_sh_key_len_msg
    call pm_print_string
    mov eax, [rtl8139_tls_server_key_share_len]
    call pm_print_uint32
    call pm_newline

    cmp word [rtl8139_tls_selected_group], 001Dh
    jne .advance
    cmp dword [rtl8139_tls_server_key_share_len], 32
    jne .advance
    cmp dword [rtl8139_tls_sh_cur_ext_len], 36
    jb .advance

    add edx, 4
    mov esi, rtl8139_tcp_payload_buffer
    add esi, edx
    mov edi, rtl8139_tls_server_pubkey
    mov ecx, 32
    cld
    rep movsb

    mov esi, rtl8139_tls_sh_pubkey_saved_msg
    call pm_print_string
    call pm_newline

.advance:
    mov eax, [rtl8139_tls_sh_ext_ptr]
    add eax, 4
    add eax, [rtl8139_tls_sh_cur_ext_len]
    mov [rtl8139_tls_sh_ext_ptr], eax
    jmp .next_ext

.done:
    popad
    ret

rtl8139_print_hex16:
    push eax
    push ebx

    mov bx, ax
    mov esi, pm_hex_prefix
    call pm_print_string
    mov al, bh
    call pm_print_hex8
    mov al, bl
    call pm_print_hex8

    pop ebx
    pop eax
    ret

rtl8139_print_tls_payload_info:
    pushad

    xor edi, edi                         ; offset inside payload buffer

.next_record:
    movzx ecx, word [rtl8139_tcp_last_payload_len]

    ; Need at least 5 bytes from current offset.
    mov eax, edi
    add eax, 5
    cmp eax, ecx
    ja .done

    mov esi, rtl8139_tls_record_msg
    call pm_print_string

    mov esi, rtl8139_tls_offset_msg
    call pm_print_string
    mov eax, edi
    call pm_print_uint32
    call pm_newline

    mov esi, rtl8139_tls_type_msg
    call pm_print_string
    movzx eax, byte [rtl8139_tcp_payload_buffer + edi]
    call pm_print_hex32
    call pm_newline

    mov al, [rtl8139_tcp_payload_buffer + edi]
    call rtl8139_print_tls_type_name

    mov esi, rtl8139_tls_version_msg
    call pm_print_string
    movzx eax, byte [rtl8139_tcp_payload_buffer + edi + 1]
    shl eax, 8
    movzx edx, byte [rtl8139_tcp_payload_buffer + edi + 2]
    or eax, edx
    call pm_print_hex32
    call pm_newline

    mov esi, rtl8139_tls_len_msg
    call pm_print_string
    movzx eax, byte [rtl8139_tcp_payload_buffer + edi + 3]
    shl eax, 8
    movzx edx, byte [rtl8139_tcp_payload_buffer + edi + 4]
    or eax, edx
    push eax
    call pm_print_uint32
    call pm_newline
    pop eax                              ; EAX = tls record len

    ; If record type is handshake and payload has byte 5, print handshake type.
    cmp byte [rtl8139_tcp_payload_buffer + edi], 16h
    jne .advance

    mov edx, edi
    add edx, 6
    movzx ecx, word [rtl8139_tcp_last_payload_len]
    cmp edx, ecx
    ja .advance

    mov esi, rtl8139_tls_handshake_msg
    call pm_print_string
    movzx edx, byte [rtl8139_tcp_payload_buffer + edi + 5]
    push eax
    mov eax, edx
    call pm_print_hex32
    call pm_newline

    mov al, dl
    call rtl8139_print_tls_handshake_name

    mov al, dl
    cmp al, 02h
    jne .no_serverhello_fields
    call rtl8139_print_tls_serverhello_fields

.no_serverhello_fields:
    pop eax

.advance:
    ; next offset += 5 + tls_len
    add eax, 5
    add edi, eax

    ; avoid runaway if malformed zero/huge length wraps weirdly
    cmp edi, 383
    jae .done

    jmp .next_record

.done:
    popad
    ret

rtl8139_print_tcp_payload_ascii:
    pushad

    mov esi, rtl8139_tcp_payload_buffer
    movzx ecx, word [rtl8139_tcp_last_payload_len]

    cmp ecx, 255
    jbe .loop
    mov ecx, 255

.loop:
    cmp ecx, 0
    je .done

    lodsb

    cmp al, 13
    je .newline
    cmp al, 10
    je .newline

    cmp al, 32
    jb .dot
    cmp al, 126
    ja .dot

    call pm_print_char
    jmp .next

.dot:
    mov al, '.'
    call pm_print_char
    jmp .next

.newline:
    call pm_newline

.next:
    dec ecx
    jmp .loop

.done:
    call pm_newline
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
rtl8139_rx_ethertype_msg db 'RX ethertype: ',0
rtl8139_rx_ipv4_protocol_msg db 'RX IPv4 protocol: ',0
rtl8139_rx_icmp_type_msg db 'RX ICMP type: ',0
rtl8139_rx_src_ip_msg db 'RX src IP: ',0
rtl8139_rx_dst_ip_msg db 'RX dst IP: ',0
rtl8139_rxipv4dump_msg db 'RX IPv4 dump',13,0
rtl8139_na_msg db 'n/a',13,0
rtl8139_rxpacket_arp_msg db 'ARP frame',13,0
rtl8139_rxpacket_ipv4_msg db 'IPv4 frame',13,0
rtl8139_rxpacket_discard_msg db 'RX packet discarded',13,0
rtl8139_rxpacket_msg db 'RTL8139 RX packet:',13,0
rtl8139_rxpacket_offset_msg db 'offset=',0
rtl8139_rxpacket_status_msg db 'status=',0
rtl8139_rxpacket_length_msg db 'length=',0
rtl8139_rxpacket_frame_msg db 'frame[0..63]:',13,0
rtl8139_arp_learned_msg db 'ARP reply: gateway MAC ',0
rtl8139_arp_request_msg db 'ARP request',13,0
rtl8139_arp_reply_msg db 'ARP reply',13,0
rtl8139_icmp_reply_msg db 'ICMP echo reply',13,0
rtl8139_icmp_src_msg db 'src IP: ',0
rtl8139_icmp_seq_msg db 'seq: ',0
rtl8139_icmp_ttl_msg db 'ttl: ',0
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
    call rtl8139_save_pci_bdf
    call rtl8139_enable_pci_bus_master
    mov byte [pm_pci_nic_type], PCI_NIC_RTL8139
    mov esi, rtl8139_pci_bound_msg
    call pm_print_string
    ret

.fallback:
    mov word [rtl8139_io_base], 0C000h
    mov dword [rtl8139_bar_raw], 0
    mov byte [rtl8139_bar_index], 0
    mov byte [rtl8139_pci_bdf_valid], 0
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
rtl8139_pci_cmd_unknown_msg db 'PCI CMD: unknown',13,0

; ============================================================
; TCP payload send test data/messages
; Added for GhostNet TCP v0.2a
; ============================================================

rtl8139_tcp_dst_port_8080_text db '8080',0
rtl8139_tcp_data_tx_msg db 'data_tx=',0
rtl8139_tcp_not_established_msg db 'TCP failed: not established',13,0
rtl8139_tcp_data_sent_msg db 'TCP data sent',13,0

rtl8139_tcp_http_get_payload:
    db 'GET / HTTP/1.0',13,10
    db 'Host: 10.0.2.2',13,10
    db 13,10
rtl8139_tcp_http_get_len equ $ - rtl8139_tcp_http_get_payload

; ============================================================
; TCP payload receive/dump data/messages
; Added for GhostNet TCP v0.2b
; ============================================================

rtl8139_tcp_data_rx_msg db 'data_rx=',0
rtl8139_tcp_payload_len_msg db 'payload_len=',0
rtl8139_tcprecv_msg db 'TCP RX data',13,0
rtl8139_tcprecv_len_msg db 'len=',0
rtl8139_tcprecv_none_msg db 'TCP RX data: none',13,0
rtl8139_tcp_payload_buffer times 384 db 0

; TCP data ACK messages
rtl8139_tcp_data_ack_tx_msg db 'data_ack_tx=',0
rtl8139_tcp_data_ack_sent_msg db 'TCP data ACK sent',13,0

; TCP receive-all state/messages
rtl8139_tcprecvall_remaining db 0
rtl8139_tcprecvall_segments dd 0
rtl8139_tcprecvall_done_msg db 'TCP RX all done',13,0

; TCP FIN/close messages
rtl8139_tcp_fin_rx_msg db 'fin_rx=',0
rtl8139_tcp_fin_ack_tx_msg db 'fin_ack_tx=',0
rtl8139_tcp_fin_ack_sent_msg db 'TCP FIN ACK sent',13,0
rtl8139_tcp_fin_closed_msg db 'TCP state: CLOSED',13,0

; GhostTLS TCP transport messages
rtl8139_tls_clienthello_sent_msg db 'TLS ClientHello sent',13,0
rtl8139_tlstest_msg db 'GhostTLS test',13,0
rtl8139_tlstest_press_key_msg db '-- press key --',13,0

rtl8139_tcp_dst_port_4433_text db '4433',0

; GhostTLS receive parser messages
rtl8139_tls_record_msg db 'TLS record',13,0
rtl8139_tls_type_msg db 'type=',0
rtl8139_tls_version_msg db 'version=',0
rtl8139_tls_len_msg db 'tls_len=',0
rtl8139_tls_handshake_msg db 'handshake=',0
rtl8139_tls_short_msg db 'TLS record: too short',13,0
rtl8139_tlsrecv_done_msg db 'TLS RX done',13,0

rtl8139_tcp_payload_wire_len_msg db 'payload_wire_len=',0

rtl8139_tls_offset_msg db 'offset=',0

; GhostTLS readable parser names
rtl8139_tls_type_name_msg db 'type_name=',0
rtl8139_tls_handshake_name_msg db 'handshake_name=',0
rtl8139_tls_name_ccs_msg db 'ChangeCipherSpec',13,0
rtl8139_tls_name_alert_msg db 'Alert',13,0
rtl8139_tls_name_handshake_msg db 'Handshake',13,0
rtl8139_tls_name_appdata_msg db 'ApplicationDataEncrypted',13,0
rtl8139_tls_name_unknown_msg db 'Unknown',13,0
rtl8139_tls_hs_clienthello_msg db 'ClientHello',13,0
rtl8139_tls_hs_serverhello_msg db 'ServerHello',13,0
rtl8139_tls_hs_encrypted_extensions_msg db 'EncryptedExtensions',13,0
rtl8139_tls_hs_certificate_msg db 'Certificate',13,0
rtl8139_tls_hs_certificate_verify_msg db 'CertificateVerify',13,0
rtl8139_tls_hs_finished_msg db 'Finished',13,0

; GhostTLS ServerHello field extraction
rtl8139_tls_sh_legacy_msg db 'sh_legacy=',0
rtl8139_tls_sh_cipher_msg db 'sh_cipher=',0
rtl8139_tls_sh_extlen_msg db 'sh_ext_len=',0
rtl8139_tls_sh_short_msg db 'ServerHello: short',13,0
rtl8139_tls_sh_extensions_msg db 'ServerHello extensions',13,0
rtl8139_tls_sh_ext_msg db 'ext=',0
rtl8139_tls_sh_ext_len_msg db 'ext_len=',0
rtl8139_tls_sh_supported_versions_name_msg db ' supported_versions',13,0
rtl8139_tls_sh_key_share_name_msg db ' key_share',13,0
rtl8139_tls_sh_selected_version_msg db 'selected_version=',0
rtl8139_tls_sh_group_msg db 'group=',0
rtl8139_tls_sh_x25519_name_msg db ' x25519',0
rtl8139_tls_sh_key_len_msg db 'key_len=',0
rtl8139_tls_sh_pubkey_saved_msg db 'server_pubkey saved',13,0

rtl8139_tls_sh_ext_start dd 0
rtl8139_tls_sh_ext_end dd 0
rtl8139_tls_sh_ext_ptr dd 0
rtl8139_tls_sh_ext_len dd 0
rtl8139_tls_sh_cur_ext_len dd 0
rtl8139_tls_sh_ext_type dw 0
rtl8139_tls_selected_version dw 0
rtl8139_tls_selected_group dw 0
rtl8139_tls_server_key_share_len dd 0
rtl8139_tls_server_pubkey times 32 db 0
