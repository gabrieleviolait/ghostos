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

pm_do_rngtest:
    call rngtest
    jmp shell32_loop

pm_do_rfc7748test:
    call x25519_rfc7748_validate
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

pm_banner db 'Ghost OS v1.5-alpha ghost32',13,13,0
pm_memory_prefix db 'Memory: ',0
pm_memory_suffix db ' MB detected',13,0
pm_heap_ready_msg db 'Heap initialized',13,0
pm_prompt db 'ghost32> ',0
pm_help_msg db \
'Commands:',13,\
'help cls clear echo version',13,\
'mem heapinfo memmap pages',13,\
'sha256test hmacsha256test rngtest',13,\
'lspci netdev netup netstat netdiag netbar',13,\
'mac setmac randommac',13,\
'txdemo txdump pktdump pktinfo txstatus',13,\
'pinggw ping pingtest pingdebug udpsend udpstat tcpconnect tcpstat pingfake arp arpfake arpstat txreal',13,\
'rxdump netreset netclear rxpoll rxpeek rxpacket rxipv4dump rxkick rxdebug rxreset rxmode rxtest rxstatus',13,\
'regs stack uptime panic',13,\
'tasks yield',13,\
'ls cat browse run halt',13,0
pm_version_msg db 'Ghost OS v1.5.5-alpha',13,0
pm_unknown_msg db 'Unknown command. Type help.',13,0
pm_halt_msg db 'System halted.',13,0
pm_run_usage_msg db 'Usage: run HELLO.GEX [args]',13,0
pm_uptime_msg db 'Uptime ticks: ',0
pm_ticks_msg db ' ticks',13,0
pm_panic_command_msg db 'panic command invoked',0
pm_regs_empty_msg db 'No register snapshot available.',13,0
pm_regs_header_msg db 'Debug register snapshot:',13,0
pm_reg_eax_msg db 'EAX=',0
pm_reg_ebx_msg db 'EBX=',0
pm_reg_ecx_msg db 'ECX=',0
pm_reg_edx_msg db 'EDX=',0
pm_reg_esi_msg db 'ESI=',0
pm_reg_edi_msg db 'EDI=',0
pm_reg_ebp_msg db 'EBP=',0
pm_reg_esp_msg db 'ESP=',0
pm_stack_header_msg db 'Stack dump:',13,0
pm_colon_space_msg db ': ',0
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
pm_cmd_run db 'run',0
pm_cmd_version db 'version',0
pm_cmd_mem db 'mem',0
pm_cmd_heapinfo db 'heapinfo',0
pm_cmd_lspci db 'lspci',0
pm_cmd_netdev db 'netdev',0
pm_cmd_sha256test db 'sha256test',0
cmd_hmacsha256test db 'hmacsha256test',0
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
