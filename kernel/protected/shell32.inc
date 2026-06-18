; Protected-mode shell parser, commands, and status output.

[BITS 32]

pm_print_system_status:
    mov esi, pm_memory_prefix
    call pm_print_string
    mov eax, [pm_detected_memory_mb]
    call pm_print_uint32
    mov esi, pm_memory_suffix
    call pm_print_string
    mov esi, pm_heap_ready_msg
    call pm_print_string
    mov esi, pm_task1_msg
    call pm_print_string
    mov esi, pm_task2_msg
    call pm_print_string
    call pm_newline
    ret

pm_skip_spaces:
    cmp byte [esi], ' '
    jne .done
    inc esi
    jmp pm_skip_spaces
.done:
    ret

pm_skip_first_word:
    call pm_skip_spaces
.loop:
    cmp byte [esi], 0
    je .done
    cmp byte [esi], ' '
    je .done
    inc esi
    jmp .loop
.done:
    ret

pm_starts_with_word:
    pushad
.loop:
    mov al, [edi]
    cmp al, 0
    je .word_end
    cmp al, [esi]
    jne .no
    inc esi
    inc edi
    jmp .loop

.word_end:
    mov al, [esi]
    cmp al, 0
    je .yes
    cmp al, ' '
    je .yes

.no:
    popad
    clc
    ret

.yes:
    popad
    stc
    ret

shell32_loop:
    mov esi, pm_prompt
    call pm_print_string

    mov edi, pm_cmd_buffer
    call pm_read_line

    mov esi, pm_cmd_buffer
    call pm_skip_spaces
    cmp byte [esi], 0
    je shell32_loop

    mov edi, pm_cmd_help
    call pm_starts_with_word
    jc pm_do_help

    mov edi, pm_cmd_clear
    call pm_starts_with_word
    jc pm_do_clear

    mov edi, pm_cmd_cls
    call pm_starts_with_word
    jc pm_do_clear

    mov edi, pm_cmd_echo
    call pm_starts_with_word
    jc pm_do_echo

    mov edi, pm_cmd_ls
    call pm_starts_with_word
    jc pm_do_ls

    mov edi, pm_cmd_cat
    call pm_starts_with_word
    jc pm_do_cat

    mov edi, pm_cmd_browse
    call pm_starts_with_word
    jc pm_do_browse

    mov edi, pm_cmd_torrelays
    call pm_starts_with_word
    jc pm_do_torrelays

    mov edi, pm_cmd_torpick
    call pm_starts_with_word
    jc pm_do_torpick

    mov edi, pm_cmd_torstate
    call pm_starts_with_word
    jc pm_do_torstate

    mov edi, pm_cmd_torconn
    call pm_starts_with_word
    jc pm_do_torconn

    mov edi, pm_cmd_torversions
    call pm_starts_with_word
    jc pm_do_torversions

    mov edi, pm_cmd_run
    call pm_starts_with_word
    jc pm_do_run

    mov edi, pm_cmd_version
    call pm_starts_with_word
    jc pm_do_version

    mov edi, pm_cmd_mem
    call pm_starts_with_word
    jc pm_do_mem

    mov edi, pm_cmd_heapinfo
    call pm_starts_with_word
    jc pm_do_heapinfo

    mov edi, pm_cmd_lspci
    call pm_starts_with_word
    jc pm_do_lspci

    mov edi, pm_cmd_netdev
    call pm_starts_with_word
    jc pm_do_netdev

    mov edi, pm_cmd_sha256test
    call pm_starts_with_word
    jc pm_do_sha256test

    ; hmacsha256test
    mov esi, pm_cmd_buffer
    mov edi, cmd_hmacsha256test
    call pm_starts_with_word
    jc pm_do_hmacsha256test

    ; hkdfsha256test
    mov esi, pm_cmd_buffer
    mov edi, cmd_hkdfsha256test
    call pm_starts_with_word
    jc pm_do_hkdfsha256test

    ; chacha20test
    mov esi, pm_cmd_buffer
    mov edi, cmd_chacha20test
    call pm_starts_with_word
    jc pm_do_chacha20test

    ; poly1305test
    mov esi, pm_cmd_buffer
    mov edi, cmd_poly1305test
    call pm_starts_with_word
    jc pm_do_poly1305test

    ; aeadmactest
    mov esi, pm_cmd_buffer
    mov edi, cmd_aeadmactest
    call pm_starts_with_word
    jc pm_do_aeadmactest

    ; aeaddecrypttest
    mov esi, pm_cmd_buffer
    mov edi, cmd_aeaddecrypttest
    call pm_starts_with_word
    jc pm_do_aeaddecrypttest

    ; tlsscheduletest
    mov esi, pm_cmd_buffer
    mov edi, cmd_tlsscheduletest
    call pm_starts_with_word
    jc pm_do_tlsscheduletest

    mov edi, pm_cmd_rngtest
    call pm_starts_with_word
    jc pm_do_rngtest

    mov edi, pm_cmd_rfc7748test
    call pm_starts_with_word
    jc pm_do_rfc7748test

    mov edi, pm_cmd_netbar
    call pm_starts_with_word
    jc pm_do_netbar

    mov edi, pm_cmd_netup
    call pm_starts_with_word
    jc pm_do_netup

    mov edi, pm_cmd_netstat
    call pm_starts_with_word
    jc pm_do_netstat

    mov edi, pm_cmd_netdiag
    call pm_starts_with_word
    jc pm_do_netdiag

    mov edi, pm_cmd_mac
    call pm_starts_with_word
    jc pm_do_mac

    mov edi, pm_cmd_setmac
    call pm_starts_with_word
    jc pm_do_setmac

    mov edi, pm_cmd_randommac
    call pm_starts_with_word
    jc pm_do_randommac

    mov edi, pm_cmd_pinggw
    call pm_starts_with_word
    jc pm_do_pinggw

    mov edi, pm_cmd_ping
    call pm_starts_with_word
    jc pm_do_ping

    mov edi, pm_cmd_pingtest
    call pm_starts_with_word
    jc pm_do_pingtest

    mov edi, pm_cmd_pingdebug
    call pm_starts_with_word
    jc pm_do_pingdebug

    mov edi, pm_cmd_udpsend
    call pm_starts_with_word
    jc pm_do_udpsend

    mov edi, pm_cmd_udpstat
    call pm_starts_with_word
    jc pm_do_udpstat

    mov edi, pm_cmd_tcpconnect
    call pm_starts_with_word
    jc pm_do_tcpconnect

    mov edi, pm_cmd_tcpget
    call pm_starts_with_word
    jc pm_do_tcpget

    mov edi, pm_cmd_tlshello
    call pm_starts_with_word
    jc pm_do_tlshello

    mov edi, pm_cmd_tlsrecv
    call pm_starts_with_word
    jc pm_do_tlsrecv

    mov edi, pm_cmd_tlssecrets
    call pm_starts_with_word
    jc pm_do_tlssecrets

    mov edi, pm_cmd_tlstest
    call pm_starts_with_word
    jc pm_do_tlstest

    mov edi, pm_cmd_tcprecv
    call pm_starts_with_word
    jc pm_do_tcprecv

    mov edi, pm_cmd_tcprecvall
    call pm_starts_with_word
    jc pm_do_tcprecvall

    mov edi, pm_cmd_tcpstat
    call pm_starts_with_word
    jc pm_do_tcpstat

    mov edi, pm_cmd_pingfake
    call pm_starts_with_word
    jc pm_do_pingfake

    mov edi, pm_cmd_arpfake
    call pm_starts_with_word
    jc pm_do_arpfake

    mov edi, pm_cmd_arp
    call pm_starts_with_word
    jc pm_do_arp

    mov edi, pm_cmd_arpstat
    call pm_starts_with_word
    jc pm_do_arpstat

    mov edi, pm_cmd_txdemo
    call pm_starts_with_word
    jc pm_do_txdemo

    mov edi, pm_cmd_txdump
    call pm_starts_with_word
    jc pm_do_txdump

    mov edi, pm_cmd_txreal
    call pm_starts_with_word
    jc pm_do_txreal

    mov edi, pm_cmd_txstatus
    call pm_starts_with_word
    jc pm_do_txstatus

    mov edi, pm_cmd_pktinfo
    call pm_starts_with_word
    jc pm_do_pktinfo

    mov edi, pm_cmd_pktdump
    call pm_starts_with_word
    jc pm_do_pktdump

    mov edi, pm_cmd_rxdump
    call pm_starts_with_word
    jc pm_do_rxdump

    mov edi, pm_cmd_rxpoll
    call pm_starts_with_word
    jc pm_do_rxpoll

    mov edi, pm_cmd_rxpeek
    call pm_starts_with_word
    jc pm_do_rxpeek

    mov edi, pm_cmd_rxpacket
    call pm_starts_with_word
    jc pm_do_rxpacket

    mov edi, pm_cmd_rxipv4dump
    call pm_starts_with_word
    jc pm_do_rxipv4dump

    mov edi, pm_cmd_rxkick
    call pm_starts_with_word
    jc pm_do_rxkick

    mov edi, pm_cmd_rxdebug
    call pm_starts_with_word
    jc pm_do_rxdebug

    mov edi, pm_cmd_rxreset
    call pm_starts_with_word
    jc pm_do_rxreset

    mov edi, pm_cmd_rxmode
    call pm_starts_with_word
    jc pm_do_rxmode

    mov edi, pm_cmd_rxtest
    call pm_starts_with_word
    jc pm_do_rxtest

    mov edi, pm_cmd_rxstatus
    call pm_starts_with_word
    jc pm_do_rxstatus

    mov edi, pm_cmd_netreset
    call pm_starts_with_word
    jc pm_do_netreset

    mov edi, pm_cmd_netclear
    call pm_starts_with_word
    jc pm_do_netclear

    mov edi, pm_cmd_regs
    call pm_starts_with_word
    jc pm_do_regs

    mov edi, pm_cmd_stack
    call pm_starts_with_word
    jc pm_do_stack

    mov edi, pm_cmd_uptime
    call pm_starts_with_word
    jc pm_do_uptime

    mov edi, pm_cmd_panic
    call pm_starts_with_word
    jc pm_do_panic

    mov edi, pm_cmd_pages
    call pm_starts_with_word
    jc pm_do_pages

    mov edi, pm_cmd_memmap
    call pm_starts_with_word
    jc pm_do_memmap

    mov edi, pm_cmd_tasks
    call pm_starts_with_word
    jc pm_do_tasks

    mov edi, pm_cmd_yield
    call pm_starts_with_word
    jc pm_do_yield

    mov edi, pm_cmd_halt
    call pm_starts_with_word
    jc pm_do_halt

    mov esi, pm_unknown_msg
    call pm_print_string
    jmp shell32_loop

pm_do_help:
    mov esi, pm_help_msg
    call pm_print_string
    jmp shell32_loop

pm_do_clear:
    call pm_clear_screen
    jmp shell32_loop

pm_do_echo:
    mov esi, pm_cmd_buffer
    call pm_skip_first_word
    call pm_skip_spaces
    call pm_print_string
    call pm_newline
    jmp shell32_loop

pm_do_ls:
    call pm_fat12_list_root
    jmp shell32_loop

pm_do_cat:
    mov esi, pm_cmd_buffer
    call pm_skip_first_word
    call pm_skip_spaces
    cmp byte [esi], 0
    je .usage

    mov edi, page_buffer
    mov ecx, PAGE_BUFFER_SIZE-1
    call pm_fat12_read_file
    jc .error

    mov esi, page_buffer
    call pm_print_string
    call pm_newline
    jmp shell32_loop

.usage:
    mov esi, cat_usage
    call pm_print_string
    jmp shell32_loop

.error:
    call pm_print_fat12_error
    jmp shell32_loop

pm_do_browse:
    call pm_clear_screen

    mov esi, pm_cmd_buffer
    call pm_skip_first_word
    call pm_skip_spaces
    cmp byte [esi], 0
    jne .have_name
    mov esi, default_page_name

.have_name:
    mov [pm_browser_filename_ptr], esi
    mov edi, page_buffer
    mov ecx, PAGE_BUFFER_SIZE-1
    call pm_fat12_read_file
    jc .load_error

    mov esi, browser_disk_header
    call pm_print_string
    mov esi, [pm_browser_filename_ptr]
    call pm_print_string
    call pm_newline
    mov esi, browser_separator
    call pm_print_string

    mov esi, page_buffer
    call pm_render_ght_buffer
    call pm_newline
    jmp shell32_loop

.load_error:
    call pm_print_fat12_error
    jmp shell32_loop

pm_do_torrelays:
    mov esi, torrelays_file_name
    mov edi, page_buffer
    mov ecx, PAGE_BUFFER_SIZE-1
    call pm_fat12_read_file
    jc .load_error

    mov esi, torrelays_header_msg
    call pm_print_string
    mov esi, page_buffer
    call torrelays_parse_buffer
    mov esi, torrelays_count_msg
    call pm_print_string
    movzx eax, byte [torrelays_count]
    call pm_print_uint32
    call pm_newline
    jmp shell32_loop

.load_error:
    mov esi, torrelays_load_error_msg
    call pm_print_string
    call pm_print_fat12_error
    jmp shell32_loop

torrelays_parse_buffer:
    ; Minimal stage-1 parser: print Relay and ntor lines from TORRELAY.GHT.
    pushad
    mov byte [torrelays_count], 0

.next_line:
    cmp byte [esi], 0
    je .done

    cmp byte [esi], 'R'
    jne .check_ntor
    cmp byte [esi + 1], 'e'
    jne .skip_line
    cmp byte [esi + 2], 'l'
    jne .skip_line
    cmp byte [esi + 3], 'a'
    jne .skip_line
    cmp byte [esi + 4], 'y'
    jne .skip_line
    cmp byte [esi + 5], ' '
    jne .skip_line
    inc byte [torrelays_count]
    call torrelays_print_line
    jmp .skip_line

.check_ntor:
    cmp byte [esi], 'n'
    jne .skip_line
    cmp byte [esi + 1], 't'
    jne .skip_line
    cmp byte [esi + 2], 'o'
    jne .skip_line
    cmp byte [esi + 3], 'r'
    jne .skip_line
    cmp byte [esi + 4], '='
    jne .skip_line
    mov esi, torrelays_ntor_prefix_msg
    call pm_print_string
    mov esi, [torrelays_line_ptr]
    call torrelays_print_line
    jmp .skip_line

.skip_line:
    cmp byte [esi], 0
    je .done
    cmp byte [esi], 10
    je .line_done
    inc esi
    jmp .skip_line

.line_done:
    inc esi
    mov [torrelays_line_ptr], esi
    jmp .next_line

.done:
    popad
    ret

torrelays_print_line:
    mov [torrelays_line_ptr], esi

.next:
    mov al, [esi]
    cmp al, 0
    je .done
    cmp al, 13
    je .newline
    cmp al, 10
    je .newline
    cmp al, '<'
    je .newline
    call pm_print_char
    inc esi
    jmp .next

.newline:
    call pm_newline

.done:
    ret

pm_do_torpick:
    mov esi, pm_cmd_buffer
    call pm_skip_first_word
    call pm_skip_spaces
    mov al, [esi]
    cmp al, '1'
    jb .not_found
    cmp al, '9'
    ja .not_found
    cmp byte [esi + 1], 0
    je .index_ok
    cmp byte [esi + 1], ' '
    jne .not_found

.index_ok:
    mov [torpick_target], al

    mov esi, torrelays_file_name
    mov edi, page_buffer
    mov ecx, PAGE_BUFFER_SIZE-1
    call pm_fat12_read_file
    jc .load_error

    mov esi, page_buffer
    call torpick_find_relay
    jc .not_found
    jmp shell32_loop

.load_error:
    mov esi, torrelays_load_error_msg
    call pm_print_string
    call pm_print_fat12_error
    jmp shell32_loop

.not_found:
    mov esi, torpick_not_found_msg
    call pm_print_string
    jmp shell32_loop

pm_do_torstate:
    mov esi, torstate_header_msg
    call pm_print_string

    cmp byte [ghosttor_selected_ready], 1
    jne .not_selected

    mov esi, torstate_selected_yes_msg
    call pm_print_string
    call ghosttor_print_selected_fields
    jmp shell32_loop

.not_selected:
    mov esi, torstate_selected_no_msg
    call pm_print_string
    jmp shell32_loop

pm_do_torconn:
    cmp byte [ghosttor_selected_ready], 1
    jne .not_selected

    mov esi, torconn_header_msg
    call pm_print_string

    mov esi, torpick_nickname_msg
    mov edi, ghosttor_selected_nickname
    call ghosttor_print_string_field

    mov esi, torpick_ip_msg
    mov edi, ghosttor_selected_ip
    call ghosttor_print_string_field

    mov esi, torpick_orport_msg
    mov edi, ghosttor_selected_orport
    call ghosttor_print_string_field

    mov esi, ghosttor_selected_ip
    mov edi, rtl8139_tcp_dst_ip
    call ghosttor_parse_ipv4_to
    jc .bad_state

    mov esi, ghosttor_selected_orport
    call ghosttor_parse_uint16
    jc .bad_state

    mov [rtl8139_tcp_dst_port], ax
    call rtl8139_tcpconnect_start
    jmp shell32_loop

.not_selected:
    mov esi, torconn_not_selected_msg
    call pm_print_string
    jmp shell32_loop

.bad_state:
    mov esi, torconn_bad_state_msg
    call pm_print_string
    jmp shell32_loop

pm_do_torversions:
    call ghosttor_build_versions_cell

    mov esi, torversions_header_msg
    call pm_print_string

    mov esi, torversions_circ_id_msg
    call pm_print_string
    xor eax, eax
    call pm_print_uint32
    call pm_newline

    mov esi, torversions_cmd_msg
    call pm_print_string
    mov eax, 7
    call pm_print_uint32
    call pm_newline

    mov esi, torversions_len_msg
    call pm_print_string
    movzx eax, word [ghosttor_versions_payload_len]
    call pm_print_uint32
    call pm_newline

    mov esi, torversions_versions_msg
    call pm_print_string

    mov esi, torversions_cell_msg
    call pm_print_string
    mov esi, ghosttor_versions_cell
    movzx ecx, word [ghosttor_versions_cell_len]
    call rtl8139_dump_bytes
    jmp shell32_loop

ghosttor_build_versions_cell:
    mov byte [ghosttor_versions_cell], 0
    mov byte [ghosttor_versions_cell + 1], 0
    mov byte [ghosttor_versions_cell + 2], 7
    mov byte [ghosttor_versions_cell + 3], 0
    mov byte [ghosttor_versions_cell + 4], 6
    mov byte [ghosttor_versions_cell + 5], 0
    mov byte [ghosttor_versions_cell + 6], 3
    mov byte [ghosttor_versions_cell + 7], 0
    mov byte [ghosttor_versions_cell + 8], 4
    mov byte [ghosttor_versions_cell + 9], 0
    mov byte [ghosttor_versions_cell + 10], 5
    mov word [ghosttor_versions_payload_len], 6
    mov word [ghosttor_versions_cell_len], 11
    ret

torpick_find_relay:
    pushad

.next_line:
    cmp byte [esi], 0
    je .not_found

    cmp byte [esi], 'R'
    jne .skip_line
    cmp byte [esi + 1], 'e'
    jne .skip_line
    cmp byte [esi + 2], 'l'
    jne .skip_line
    cmp byte [esi + 3], 'a'
    jne .skip_line
    cmp byte [esi + 4], 'y'
    jne .skip_line
    cmp byte [esi + 5], ' '
    jne .skip_line
    mov al, [torpick_target]
    cmp [esi + 6], al
    jne .skip_line
    cmp byte [esi + 7], ':'
    jne .skip_line
    call torpick_print_selected
    popad
    clc
    ret

.skip_line:
    cmp byte [esi], 0
    je .not_found
    cmp byte [esi], 10
    je .line_done
    inc esi
    jmp .skip_line

.line_done:
    inc esi
    jmp .next_line

.not_found:
    popad
    stc
    ret

torpick_print_selected:
    mov byte [ghosttor_selected_ready], 0
    mov al, [torpick_target]
    mov [ghosttor_selected_index], al

    add esi, 9

    mov edi, ghosttor_selected_nickname
    mov ecx, 31
    mov byte [torpick_delim], ' '
    call torpick_copy_field

    call pm_skip_spaces

    mov edi, ghosttor_selected_ip
    mov ecx, 15
    mov byte [torpick_delim], ':'
    call torpick_copy_field
    inc esi

    mov edi, ghosttor_selected_orport
    mov ecx, 7
    mov byte [torpick_delim], ' '
    call torpick_copy_field

    call pm_skip_spaces
    add esi, 6
    mov edi, ghosttor_selected_flags
    mov ecx, 63
    mov byte [torpick_delim], 0
    call torpick_copy_field

.find_ntor:
    cmp byte [esi], 0
    je .done
    cmp byte [esi], 10
    je .ntor_line
    inc esi
    jmp .find_ntor

.ntor_line:
    inc esi
    add esi, 5
    mov edi, ghosttor_selected_ntor
    mov ecx, 79
    mov byte [torpick_delim], 0
    call torpick_copy_field

    mov byte [ghosttor_selected_ready], 1
    mov esi, torpick_selected_msg
    call pm_print_string
    call ghosttor_print_selected_fields

.done:
    ret

torpick_copy_field:
    cmp byte [esi], 0
    je .done
    cmp byte [esi], 13
    je .done
    cmp byte [esi], 10
    je .done
    cmp byte [torpick_delim], 0
    je .store_or_skip
    mov al, [torpick_delim]
    cmp [esi], al
    je .done

.store_or_skip:
    cmp ecx, 0
    je .skip_store
    mov al, [esi]
    mov [edi], al
    inc edi
    dec ecx

.skip_store:
    inc esi
    jmp torpick_copy_field

.done:
    mov byte [edi], 0
    ret

ghosttor_print_selected_fields:
    mov esi, torpick_index_msg
    call pm_print_string
    mov al, [ghosttor_selected_index]
    call pm_print_char
    call pm_newline

    mov esi, torpick_nickname_msg
    mov edi, ghosttor_selected_nickname
    call ghosttor_print_string_field

    mov esi, torpick_ip_msg
    mov edi, ghosttor_selected_ip
    call ghosttor_print_string_field

    mov esi, torpick_orport_msg
    mov edi, ghosttor_selected_orport
    call ghosttor_print_string_field

    mov esi, torpick_flags_msg
    mov edi, ghosttor_selected_flags
    call ghosttor_print_string_field

    mov esi, torpick_ntor_msg
    mov edi, ghosttor_selected_ntor
    call ghosttor_print_string_field
    ret

ghosttor_print_string_field:
    call pm_print_string
    mov esi, edi
    call pm_print_string
    call pm_newline
    ret

; ESI = dotted IPv4 string, EDI = 4-byte output.
; Carry clear on success, set on parse failure.
ghosttor_parse_ipv4_to:
    mov ecx, 4

.next_octet:
    call ghosttor_parse_u8
    jc .fail
    mov [edi], al
    inc edi
    dec ecx
    cmp ecx, 0
    je .done
    cmp byte [esi], '.'
    jne .fail
    inc esi
    jmp .next_octet

.done:
    cmp byte [esi], 0
    jne .fail
    clc
    ret

.fail:
    stc
    ret

ghosttor_parse_u8:
    push ebx
    push ecx
    push edx

    xor eax, eax
    xor ebx, ebx

.digit:
    movzx edx, byte [esi]
    cmp dl, '0'
    jb .done
    cmp dl, '9'
    ja .done
    sub edx, '0'
    mov ecx, eax
    shl eax, 3
    lea eax, [eax + ecx * 2]
    add eax, edx
    cmp eax, 255
    ja .fail
    inc esi
    inc bl
    jmp .digit

.done:
    cmp bl, 0
    je .fail
    clc
    pop edx
    pop ecx
    pop ebx
    ret

.fail:
    stc
    pop edx
    pop ecx
    pop ebx
    ret

ghosttor_parse_uint16:
    push ebx
    push ecx
    push edx

    xor eax, eax
    xor ebx, ebx

.digit:
    movzx edx, byte [esi]
    cmp dl, '0'
    jb .done
    cmp dl, '9'
    ja .done
    sub edx, '0'
    mov ecx, eax
    shl eax, 3
    lea eax, [eax + ecx * 2]
    add eax, edx
    cmp eax, 65535
    ja .fail
    inc esi
    inc bl
    jmp .digit

.done:
    cmp bl, 0
    je .fail
    cmp byte [esi], 0
    jne .fail
    clc
    pop edx
    pop ecx
    pop ebx
    ret

.fail:
    stc
    pop edx
    pop ecx
    pop ebx
    ret

pm_do_run:
    mov esi, pm_cmd_buffer
    call pm_skip_first_word
    call pm_skip_spaces
    cmp byte [esi], 0
    je .usage
    call pm_gex_run
    jmp shell32_loop

.usage:
    mov esi, pm_run_usage_msg
    call pm_print_string
    jmp shell32_loop

pm_do_version:
    mov esi, pm_version_msg
    call pm_print_string
    jmp shell32_loop

pm_do_mem:
    call pm_print_system_status
    jmp shell32_loop

pm_do_heapinfo:
    call pm_print_heap_info
    jmp shell32_loop

pm_do_lspci:
    call pm_pci_print_devices
    jmp shell32_loop

pm_do_netdev:
    call pm_pci_print_netdev
    jmp shell32_loop

pm_do_sha256test:
    call sha256test
    jmp shell32_loop

pm_do_hmacsha256test:
    call hmac_sha256test
    jmp shell32_loop

pm_do_hkdfsha256test:
    call hkdf_sha256test
    jmp shell32_loop

pm_do_chacha20test:
    call chacha20test
    jmp shell32_loop

pm_do_poly1305test:
    call poly1305test
    jmp shell32_loop

pm_do_aeadmactest:
    call aeadmactest
    jmp shell32_loop

pm_do_aeaddecrypttest:
    call aeaddecrypttest
    jmp shell32_loop

pm_do_tlsscheduletest:
    call tls_key_schedule_test
    jmp shell32_loop

pm_do_rngtest:
    call rngtest
    jmp shell32_loop

pm_do_rfc7748test:
    call x25519_rfc7748_validate_min
    jmp shell32_loop

pm_do_netbar:
    call rtl8139_netbar
    jmp shell32_loop

pm_do_netup:
    call rtl8139_netup
    jmp shell32_loop

pm_do_netstat:
    call rtl8139_netstat
    jmp shell32_loop

pm_do_netdiag:
    call rtl8139_netdiag
    jmp shell32_loop

pm_do_mac:
    call rtl8139_print_mac
    jmp shell32_loop
    
pm_do_setmac:
    call rtl8139_setmac
    jmp shell32_loop

pm_do_randommac:
    call rtl8139_randommac
    jmp shell32_loop

pm_do_pinggw:
    call rtl8139_pinggw
    jmp shell32_loop

pm_do_ping:
    call rtl8139_ping_cmd
    jmp shell32_loop

pm_do_pingtest:
    call rtl8139_pingtest
    jmp shell32_loop

pm_do_pingdebug:
    call rtl8139_pingdebug
    jmp shell32_loop

pm_do_udpsend:
    call rtl8139_udpsend_cmd
    jmp shell32_loop

pm_do_udpstat:
    call rtl8139_udpstat
    jmp shell32_loop

pm_do_tcpconnect:
    call rtl8139_tcpconnect_cmd
    jmp shell32_loop

pm_do_tcpget:
    call rtl8139_tcpget_cmd
    jmp shell32_loop

pm_do_tlshello:
    call rtl8139_tlshello_cmd
    jmp shell32_loop

pm_do_tlsrecv:
    call rtl8139_tlsrecv_cmd
    jmp shell32_loop

pm_do_tlssecrets:
    call tls_derive_shared_secret
    call tls_derive_handshake_secrets
    jmp shell32_loop

pm_do_tlstest:
    call rtl8139_tlstest_cmd
    jmp shell32_loop

pm_do_tcprecv:
    call rtl8139_tcprecv_cmd
    jmp shell32_loop

pm_do_tcprecvall:
    call rtl8139_tcprecvall_cmd
    jmp shell32_loop

pm_do_tcpstat:
    call rtl8139_tcpstat
    jmp shell32_loop

pm_do_pingfake:
    call rtl8139_pingfake
    jmp shell32_loop

pm_do_arpfake:
    call rtl8139_arpfake
    jmp shell32_loop

pm_do_arp:
    call rtl8139_arp
    jmp shell32_loop

pm_do_arpstat:
    call rtl8139_arpstat
    jmp shell32_loop

pm_do_txdemo:
    call rtl8139_txdemo
    jmp shell32_loop

pm_do_txdump:
    call rtl8139_txdump
    jmp shell32_loop

pm_do_txreal:
    call rtl8139_txreal
    jmp shell32_loop

pm_do_txstatus:
    call rtl8139_txstatus
    jmp shell32_loop

pm_do_pktinfo:
    call rtl8139_pktinfo
    jmp shell32_loop

pm_do_pktdump:
    call rtl8139_pktdump
    jmp shell32_loop

pm_do_rxdump:
    call rtl8139_rxdump
    jmp shell32_loop

pm_do_rxpoll:
    call rtl8139_rxpoll
    jmp shell32_loop

pm_do_rxpeek:
    call rtl8139_rxpeek
    jmp shell32_loop

pm_do_rxpacket:
    call rtl8139_rxpacket
    jmp shell32_loop

pm_do_rxipv4dump:
    call rtl8139_rxipv4dump
    jmp shell32_loop

pm_do_rxkick:
    call rtl8139_rxkick
    jmp shell32_loop

pm_do_rxdebug:
    call rtl8139_rxdebug
    jmp shell32_loop

pm_do_rxreset:
    call rtl8139_rxreset
    jmp shell32_loop

pm_do_rxmode:
    call rtl8139_rxmode
    jmp shell32_loop

pm_do_rxtest:
    call rtl8139_rxtest
    jmp shell32_loop

pm_do_rxstatus:
    call rtl8139_rxstatus
    jmp shell32_loop

pm_do_netreset:
    call rtl8139_netreset
    jmp shell32_loop

pm_do_netclear:
    call rtl8139_netclear
    jmp shell32_loop

pm_do_regs:
    pushad
    mov eax, [esp+28]
    mov [pm_debug_eax], eax
    mov eax, [esp+16]
    mov [pm_debug_ebx], eax
    mov eax, [esp+24]
    mov [pm_debug_ecx], eax
    mov eax, [esp+20]
    mov [pm_debug_edx], eax
    mov eax, [esp]
    mov [pm_debug_edi], eax
    mov eax, [esp+4]
    mov [pm_debug_esi], eax
    mov eax, [esp+8]
    mov [pm_debug_ebp], eax
    mov eax, [esp+12]
    mov [pm_debug_esp], eax
    mov dword [pm_debug_regs_valid], 1
    popad
    call pm_print_regs
    jmp shell32_loop

pm_do_stack:
    pushad
    mov [pm_stack_snapshot_esp], esp
    popad
    call pm_print_stack
    jmp shell32_loop

pm_do_uptime:
    mov esi, pm_uptime_msg
    call pm_print_string
    mov eax, [pm_system_ticks]
    call pm_print_uint32
    mov esi, pm_ticks_msg
    call pm_print_string
    jmp shell32_loop

pm_do_panic:
    mov esi, pm_panic_command_msg
    call pm_panic

pm_do_pages:
    call pm_print_page_stats
    jmp shell32_loop

pm_do_memmap:
    call pm_print_memmap
    jmp shell32_loop

pm_do_tasks:
    call pm_print_tasks
    jmp shell32_loop

pm_do_yield:
    call pm_yield
    jmp shell32_loop

pm_do_halt:
    mov esi, pm_halt_msg
    call pm_print_string
    jmp pm_halt

pm_print_reg_line:
    call pm_print_string
    call pm_print_hex32
    call pm_newline
    ret

pm_print_regs:
    cmp dword [pm_debug_regs_valid], 0
    jne .have_regs
    mov esi, pm_regs_empty_msg
    call pm_print_string
    ret

.have_regs:
    mov esi, pm_regs_header_msg
    call pm_print_string
    mov esi, pm_reg_eax_msg
    mov eax, [pm_debug_eax]
    call pm_print_reg_line
    mov esi, pm_reg_ebx_msg
    mov eax, [pm_debug_ebx]
    call pm_print_reg_line
    mov esi, pm_reg_ecx_msg
    mov eax, [pm_debug_ecx]
    call pm_print_reg_line
    mov esi, pm_reg_edx_msg
    mov eax, [pm_debug_edx]
    call pm_print_reg_line
    mov esi, pm_reg_esi_msg
    mov eax, [pm_debug_esi]
    call pm_print_reg_line
    mov esi, pm_reg_edi_msg
    mov eax, [pm_debug_edi]
    call pm_print_reg_line
    mov esi, pm_reg_ebp_msg
    mov eax, [pm_debug_ebp]
    call pm_print_reg_line
    mov esi, pm_reg_esp_msg
    mov eax, [pm_debug_esp]
    call pm_print_reg_line
    ret

pm_print_stack:
    mov esi, pm_stack_header_msg
    call pm_print_string
    mov ebx, [pm_stack_snapshot_esp]
    mov ecx, 8

.line:
    mov eax, ebx
    call pm_print_hex32
    mov esi, pm_colon_space_msg
    call pm_print_string
    mov eax, [ebx]
    call pm_print_hex32
    call pm_newline
    add ebx, 4
    loop .line
    ret

pm_more_pause:
    ; Simple pager pause. Press ENTER to continue.
    pushad
    mov esi, pm_more_msg
    call pm_print_string
    mov edi, pm_cmd_buffer
    call pm_read_line
    popad
    ret

pm_banner db 'ghost32',13,0
pm_memory_prefix db 'mem ',0
pm_memory_suffix db 13,0
pm_heap_ready_msg db 13,0
pm_prompt db 0
pm_help_msg db \
'Commands:',13,\
'CORE: help cls clear echo version',13,\
'MEM: mem heapinfo memmap pages',13,\
'CRYPTO: sha256test hmacsha256test hkdfsha256test chacha20test poly1305test aeadmactest aeaddecrypttest',13,\
'CRYPTO2: rngtest rfc7748test',13,\
'TLS: tlsscheduletest tlshello tlsrecv tlssecrets tlstest',13,\
'PCI: lspci netdev regs stack uptime panic',13,\
'NET: netup netstat netdiag netbar netreset netclear',13,\
'MAC: mac setmac randommac',13,\
'PING: pinggw ping pingtest pingdebug',13,\
'UDP/TCP: udpsend udpstat tcpconnect tcpget tcprecv tcprecvall tcpstat',13,\
'ARP: pingfake arp arpfake arpstat',13,\
'TX: txdemo txdump txreal txstatus',13,\
'PKT: pktdump pktinfo',13,\
'RX1: rxdump rxpoll rxpeek rxpacket rxipv4dump',13,\
'RX2: rxkick rxdebug rxreset rxmode rxtest rxstatus',13,\
'TASK: tasks yield',13,\
'GHOSTTOR: torrelays torpick torstate torconn torversions',13,\
'FS: ls cat browse run halt',13,0
pm_version_msg db 'ghost32',13,0
pm_unknown_msg db '?',13,0
pm_halt_msg db 0
pm_run_usage_msg db 'run GEX',13,0
pm_uptime_msg db 'T ',0
pm_ticks_msg db 13,0
pm_panic_command_msg db 'panic',0
pm_regs_empty_msg db 'no regs',13,0
pm_regs_header_msg db 'regs',13,0
pm_reg_eax_msg db 'A',0
pm_reg_ebx_msg db 'B',0
pm_reg_ecx_msg db 'C',0
pm_reg_edx_msg db 'D',0
pm_reg_esi_msg db 'S',0
pm_reg_edi_msg db 'I',0
pm_reg_ebp_msg db 'P',0
pm_reg_esp_msg db 'T',0
pm_stack_header_msg db 'stk',13,0
pm_colon_space_msg db ' ',0
pm_more_msg db '-- more -- ENTER',13,0
pm_debug_regs_valid dd 0
pm_debug_eax dd 0
pm_debug_ebx dd 0
pm_debug_ecx dd 0
pm_debug_edx dd 0
pm_debug_esi dd 0
pm_debug_edi dd 0
pm_debug_ebp dd 0
pm_debug_esp dd 0
pm_stack_snapshot_esp dd 0

pm_cmd_help db 'help',0
pm_cmd_clear db 'clear',0
pm_cmd_cls db 'cls',0
pm_cmd_echo db 'echo',0
pm_cmd_ls db 'ls',0
pm_cmd_cat db 'cat',0
pm_cmd_browse db 'browse',0
pm_cmd_torrelays db 'torrelays',0
pm_cmd_torpick db 'torpick',0
pm_cmd_torstate db 'torstate',0
pm_cmd_torconn db 'torconn',0
pm_cmd_torversions db 'torversions',0
pm_cmd_run db 'run',0
pm_cmd_version db 'version',0
pm_cmd_mem db 'mem',0
pm_cmd_heapinfo db 'heapinfo',0
pm_cmd_lspci db 'lspci',0
pm_cmd_netdev db 'netdev',0
pm_cmd_sha256test db 'sha256test',0
cmd_hmacsha256test db 'hmacsha256test',0
cmd_hkdfsha256test db 'hkdfsha256test',0
cmd_chacha20test db 'chacha20test',0
cmd_poly1305test db 'poly1305test',0
cmd_aeadmactest db 'aeadmactest',0
cmd_aeaddecrypttest db 'aeaddecrypttest',0
cmd_tlsscheduletest db 'tlsscheduletest',0
pm_cmd_rngtest db 'rngtest',0
pm_cmd_rfc7748test db 'rfc7748test',0
pm_cmd_netbar db 'netbar',0
pm_cmd_netup db 'netup',0
pm_cmd_netstat db 'netstat',0
pm_cmd_netdiag db 'netdiag',0
pm_cmd_mac db 'mac',0
pm_cmd_setmac db 'setmac',0
pm_cmd_randommac db 'randommac',0
pm_cmd_pinggw db 'pinggw',0
pm_cmd_ping db 'ping',0
pm_cmd_pingtest db 'pingtest',0
pm_cmd_pingdebug db 'pingdebug',0
pm_cmd_udpsend db 'udpsend',0
pm_cmd_udpstat db 'udpstat',0
pm_cmd_tcpconnect db 'tcpconnect',0
pm_cmd_tcpget db 'tcpget',0
pm_cmd_tlshello db 'tlshello',0
pm_cmd_tlsrecv db 'tlsrecv',0
pm_cmd_tlssecrets db 'tlssecrets',0
pm_cmd_tlstest db 'tlstest',0
pm_cmd_tcprecv db 'tcprecv',0
pm_cmd_tcprecvall db 'tcprecvall',0
pm_cmd_tcpstat db 'tcpstat',0
pm_cmd_pingfake db 'pingfake',0
pm_cmd_arpfake db 'arpfake',0
pm_cmd_arp db 'arp',0
pm_cmd_arpstat db 'arpstat',0
pm_cmd_txdemo db 'txdemo',0
pm_cmd_txdump db 'txdump',0
pm_cmd_txreal db 'txreal',0
pm_cmd_txstatus db 'txstatus',0
pm_cmd_pktinfo db 'pktinfo',0
pm_cmd_pktdump db 'pktdump',0
pm_cmd_rxdump db 'rxdump',0
pm_cmd_rxpoll db 'rxpoll',0
pm_cmd_rxpeek db 'rxpeek',0
pm_cmd_rxpacket db 'rxpacket',0
pm_cmd_rxipv4dump db 'rxipv4dump',0
pm_cmd_rxkick db 'rxkick',0
pm_cmd_rxdebug db 'rxdebug',0
pm_cmd_rxreset db 'rxreset',0
pm_cmd_rxmode db 'rxmode',0
pm_cmd_rxtest db 'rxtest',0
pm_cmd_rxstatus db 'rxstatus',0
pm_cmd_netreset db 'netreset',0
pm_cmd_netclear db 'netclear',0
pm_cmd_regs db 'regs',0
pm_cmd_stack db 'stack',0
pm_cmd_uptime db 'uptime',0
pm_cmd_panic db 'panic',0
pm_cmd_pages db 'pages',0
pm_cmd_memmap db 'memmap',0
pm_cmd_tasks db 'tasks',0
pm_cmd_yield db 'yield',0
pm_cmd_halt db 'halt',0
torrelays_file_name db 'TORRELAY.GHT',0
torrelays_header_msg db 'GhostTor relay candidates',13,0
torrelays_load_error_msg db 'torrelays: ',0
torrelays_ntor_prefix_msg db '  ',0
torrelays_count_msg db 'relay_count=',0
torrelays_count db 0
torrelays_line_ptr dd 0
torpick_selected_msg db 'GhostTor selected relay',13,0
torpick_not_found_msg db 'GhostTor relay not found',13,0
torstate_header_msg db 'GhostTor state',13,0
torstate_selected_yes_msg db 'selected=yes',13,0
torstate_selected_no_msg db 'selected=no',13,0
torconn_header_msg db 'GhostTor TCP connect',13,0
torconn_not_selected_msg db 'GhostTor relay not selected',13,0
torconn_bad_state_msg db 'GhostTor relay state invalid',13,0
torversions_header_msg db 'GhostTor VERSIONS cell',13,0
torversions_circ_id_msg db 'circ_id=',0
torversions_cmd_msg db 'cmd=',0
torversions_len_msg db 'len=',0
torversions_versions_msg db 'versions=3,4,5',13,0
torversions_cell_msg db 'cell=',0
torpick_index_msg db 'index=',0
torpick_nickname_msg db 'nickname=',0
torpick_ip_msg db 'ip=',0
torpick_orport_msg db 'orport=',0
torpick_flags_msg db 'flags=',0
torpick_ntor_msg db 'ntor=',0
torpick_target db 0
torpick_delim db 0
ghosttor_selected_ready db 0
ghosttor_selected_index db 0
ghosttor_selected_nickname times 32 db 0
ghosttor_selected_ip times 16 db 0
ghosttor_selected_orport times 8 db 0
ghosttor_selected_flags times 64 db 0
ghosttor_selected_ntor times 80 db 0
ghosttor_versions_payload_len dw 0
ghosttor_versions_cell_len dw 0
ghosttor_versions_cell times 16 db 0
