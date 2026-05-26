; Protected-mode PCI discovery and NIC detection only.

[BITS 32]

PCI_CONFIG_ADDR equ 0x0CF8
PCI_CONFIG_DATA equ 0x0CFC
PCI_ENABLE_BIT equ 0x80000000
PCI_COMMAND_OFFSET equ 0x04
PCI_CMD_IO_SPACE equ 0001h
PCI_CMD_MEM_SPACE equ 0002h
PCI_CMD_BUS_MASTER equ 0004h

PCI_MAX_RECORDS equ 32
PCI_RECORD_SIZE equ 20
PCI_REC_BUS equ 0
PCI_REC_DEV equ 1
PCI_REC_FN equ 2
PCI_REC_PROGIF equ 3
PCI_REC_VENDOR equ 4
PCI_REC_DEVICE equ 6
PCI_REC_CLASS equ 8
PCI_REC_SUBCLASS equ 9
PCI_REC_IRQ equ 10
PCI_REC_BAR0 equ 12
PCI_REC_BAR1 equ 16

PCI_NIC_GENERIC equ 1
PCI_NIC_RTL8139 equ 2
PCI_NIC_NE2K equ 3
PCI_NIC_E1000 equ 4

; Inputs: BL=bus, BH=device, CH=function, CL=offset. Returns EAX=dword.
pci_config_read32:
    push ebx
    push ecx
    push edx

    xor eax, eax
    mov al, bl
    shl eax, 16

    xor edx, edx
    mov dl, bh
    shl edx, 11
    or eax, edx

    xor edx, edx
    mov dl, ch
    shl edx, 8
    or eax, edx

    xor edx, edx
    mov dl, cl
    and dl, 0xFC
    or eax, edx
    or eax, PCI_ENABLE_BIT

    mov dx, PCI_CONFIG_ADDR
    out dx, eax
    mov dx, PCI_CONFIG_DATA
    in eax, dx

    pop edx
    pop ecx
    pop ebx
    ret

; Inputs match pci_config_read32. Returns AX=word, zero-extended in EAX.
pci_config_read16:
    push ecx
    call pci_config_read32
    test cl, 2
    jz .low_word
    shr eax, 16

.low_word:
    and eax, 0xFFFF
    pop ecx
    ret

; Inputs: BL=bus, BH=device, CH=function, CL=offset, EAX=dword.
pci_config_write32:
    push ebx
    push ecx
    push edx
    push esi

    mov esi, eax
    xor eax, eax
    mov al, bl
    shl eax, 16

    xor edx, edx
    mov dl, bh
    shl edx, 11
    or eax, edx

    xor edx, edx
    mov dl, ch
    shl edx, 8
    or eax, edx

    xor edx, edx
    mov dl, cl
    and dl, 0xFC
    or eax, edx
    or eax, PCI_ENABLE_BIT

    mov dx, PCI_CONFIG_ADDR
    out dx, eax
    mov dx, PCI_CONFIG_DATA
    mov eax, esi
    out dx, eax

    pop esi
    pop edx
    pop ecx
    pop ebx
    ret

; Inputs match pci_config_read32, AX=word to write.
pci_config_write16:
    push ebx
    push ecx
    push edx
    push esi

    mov si, ax
    call pci_config_read32
    test cl, 2
    jz .low_word

    and eax, 0000FFFFh
    movzx edx, si
    shl edx, 16
    or eax, edx
    jmp .write

.low_word:
    and eax, 0FFFF0000h
    movzx edx, si
    or eax, edx

.write:
    call pci_config_write32
    pop esi
    pop edx
    pop ecx
    pop ebx
    ret

; Inputs: BL=bus, BH=device, CH=function.
; Enables PCI I/O space and bus mastering, preserving Memory Space as-is.
pci_enable_bus_master:
    push eax
    push ecx

    mov cl, PCI_COMMAND_OFFSET
    call pci_config_read16
    mov [pci_command_before], ax
    or ax, PCI_CMD_IO_SPACE | PCI_CMD_BUS_MASTER
    mov [pci_command_after], ax
    mov cl, PCI_COMMAND_OFFSET
    call pci_config_write16
    mov cl, PCI_COMMAND_OFFSET
    call pci_config_read16
    mov [pci_command_after], ax

    pop ecx
    pop eax
    ret

; Inputs match pci_config_read32. Returns AL=byte, zero-extended in EAX.
pci_config_read8:
    push ecx
    call pci_config_read32
    and ecx, 3
    shl ecx, 3
    shr eax, cl
    and eax, 0xFF
    pop ecx
    ret

pci_scan_bus:
    pushad

    mov dword [pm_pci_count], 0
    mov dword [pm_pci_seen], 0
    mov byte [pm_pci_scan_bus_no], 0

.bus_loop:
    mov byte [pm_pci_scan_dev], 0

.dev_loop:
    mov byte [pm_pci_scan_fn], 0

.fn_loop:
    mov bl, [pm_pci_scan_bus_no]
    mov bh, [pm_pci_scan_dev]
    mov ch, [pm_pci_scan_fn]
    xor cl, cl
    call pci_config_read32
    cmp eax, 0xFFFFFFFF
    je .next_fn
    cmp ax, 0xFFFF
    je .next_fn

    inc dword [pm_pci_seen]
    mov edx, eax
    mov eax, [pm_pci_count]
    cmp eax, PCI_MAX_RECORDS
    jae .next_fn
    imul edi, eax, PCI_RECORD_SIZE
    add edi, pm_pci_records

    mov al, [pm_pci_scan_bus_no]
    mov [edi+PCI_REC_BUS], al
    mov al, [pm_pci_scan_dev]
    mov [edi+PCI_REC_DEV], al
    mov al, [pm_pci_scan_fn]
    mov [edi+PCI_REC_FN], al
    mov [edi+PCI_REC_VENDOR], dx
    shr edx, 16
    mov [edi+PCI_REC_DEVICE], dx

    mov bl, [pm_pci_scan_bus_no]
    mov bh, [pm_pci_scan_dev]
    mov ch, [pm_pci_scan_fn]
    mov cl, 0x08
    call pci_config_read32
    mov edx, eax
    shr eax, 8
    mov [edi+PCI_REC_PROGIF], al
    mov eax, edx
    shr eax, 16
    mov [edi+PCI_REC_SUBCLASS], al
    mov eax, edx
    shr eax, 24
    mov [edi+PCI_REC_CLASS], al

    mov bl, [pm_pci_scan_bus_no]
    mov bh, [pm_pci_scan_dev]
    mov ch, [pm_pci_scan_fn]
    mov cl, 0x10
    call pci_config_read32
    mov [edi+PCI_REC_BAR0], eax

    mov bl, [pm_pci_scan_bus_no]
    mov bh, [pm_pci_scan_dev]
    mov ch, [pm_pci_scan_fn]
    mov cl, 0x14
    call pci_config_read32
    mov [edi+PCI_REC_BAR1], eax

    mov bl, [pm_pci_scan_bus_no]
    mov bh, [pm_pci_scan_dev]
    mov ch, [pm_pci_scan_fn]
    mov cl, 0x3C
    call pci_config_read8
    mov [edi+PCI_REC_IRQ], al

    inc dword [pm_pci_count]

.next_fn:
    inc byte [pm_pci_scan_fn]
    cmp byte [pm_pci_scan_fn], 8
    jb .fn_loop

    inc byte [pm_pci_scan_dev]
    cmp byte [pm_pci_scan_dev], 32
    jb .dev_loop

    inc byte [pm_pci_scan_bus_no]
    jnz .bus_loop

    popad
    ret

pm_pci_print_hex_nibble:
    push eax
    push ebx
    and eax, 0x0F
    mov al, [pm_hex_digits+eax]
    call pm_print_char
    pop ebx
    pop eax
    ret

pm_pci_print_hex8:
    push eax
    push ebx
    mov bl, al
    movzx eax, bl
    shr eax, 4
    mov al, [pm_hex_digits+eax]
    call pm_print_char
    movzx eax, bl
    and eax, 0x0F
    mov al, [pm_hex_digits+eax]
    call pm_print_char
    pop ebx
    pop eax
    ret

pm_pci_print_hex16:
    push eax
    push ebx
    mov bx, ax
    mov al, bh
    call pm_pci_print_hex8
    mov al, bl
    call pm_pci_print_hex8
    pop ebx
    pop eax
    ret

pm_pci_print_bdf:
    mov al, [edi+PCI_REC_BUS]
    call pm_pci_print_hex8
    mov al, ':'
    call pm_print_char
    mov al, [edi+PCI_REC_DEV]
    call pm_pci_print_hex8
    mov al, '.'
    call pm_print_char
    movzx eax, byte [edi+PCI_REC_FN]
    call pm_pci_print_hex_nibble
    ret

pm_pci_print_devices:
    pushad
    call pci_scan_bus
    cmp dword [pm_pci_count], 0
    jne .have_devices

    mov esi, pm_pci_none_msg
    call pm_print_string
    jmp .done

.have_devices:
    mov esi, pm_lspci_header_msg
    call pm_print_string
    xor ebx, ebx

.line_loop:
    cmp ebx, [pm_pci_count]
    jae .maybe_truncated
    imul edi, ebx, PCI_RECORD_SIZE
    add edi, pm_pci_records

    call pm_pci_print_bdf
    mov esi, pm_two_spaces_msg
    call pm_print_string
    mov ax, [edi+PCI_REC_VENDOR]
    call pm_pci_print_hex16
    mov al, ':'
    call pm_print_char
    mov ax, [edi+PCI_REC_DEVICE]
    call pm_pci_print_hex16
    mov esi, pm_two_spaces_msg
    call pm_print_string
    mov al, [edi+PCI_REC_CLASS]
    call pm_pci_print_hex8
    mov al, '/'
    call pm_print_char
    mov al, [edi+PCI_REC_SUBCLASS]
    call pm_pci_print_hex8
    mov al, '/'
    call pm_print_char
    mov al, [edi+PCI_REC_PROGIF]
    call pm_pci_print_hex8
    call pm_newline

    inc ebx
    jmp .line_loop

.maybe_truncated:
    mov eax, [pm_pci_seen]
    cmp eax, [pm_pci_count]
    jbe .done
    mov esi, pm_pci_truncated_msg
    call pm_print_string

.done:
    popad
    ret

pm_pci_get_nic_type:
    mov ax, [edi+PCI_REC_VENDOR]
    cmp ax, 0x10EC
    jne .check_intel
    mov ax, [edi+PCI_REC_DEVICE]
    cmp ax, 0x8139
    je .rtl8139
    cmp ax, 0x8029
    je .ne2k
    jmp .check_class

.check_intel:
    cmp ax, 0x8086
    jne .check_class
    mov ax, [edi+PCI_REC_DEVICE]
    cmp ax, 0x100E
    je .e1000
    cmp ax, 0x100F
    je .e1000
    cmp ax, 0x1004
    je .e1000
    jmp .check_class

.check_class:
    cmp byte [edi+PCI_REC_CLASS], 0x02
    jne .no
    cmp byte [edi+PCI_REC_SUBCLASS], 0x00
    jne .no
    mov esi, pm_pci_nic_generic_msg
    mov al, PCI_NIC_GENERIC
    clc
    ret

.rtl8139:
    mov esi, pm_pci_nic_rtl8139_msg
    mov al, PCI_NIC_RTL8139
    clc
    ret

.ne2k:
    mov esi, pm_pci_nic_ne2k_msg
    mov al, PCI_NIC_NE2K
    clc
    ret

.e1000:
    mov esi, pm_pci_nic_e1000_msg
    mov al, PCI_NIC_E1000
    clc
    ret

.no:
    stc
    ret

pm_pci_get_io_bar:
    mov eax, [edi+PCI_REC_BAR0]
    call pm_pci_check_io_bar
    jnc .done
    mov eax, [edi+PCI_REC_BAR1]
    call pm_pci_check_io_bar
    ret

.done:
    ret

pm_pci_check_io_bar:
    cmp eax, 0
    je .bad
    cmp eax, 0xFFFFFFFF
    je .bad
    test eax, 1
    jz .bad
    and eax, 0xFFFFFFFC
    cmp eax, 0x10000
    jae .bad
    clc
    ret

.bad:
    stc
    ret

pm_pci_print_mac_if_safe:
    cmp byte [pm_pci_nic_type], PCI_NIC_RTL8139
    jne .unavailable
    call pm_pci_get_io_bar
    jc .unavailable

    mov [pm_pci_io_base], ax
    mov esi, pm_pci_mac_msg
    call pm_print_string
    xor ebx, ebx

.mac_loop:
    mov dx, [pm_pci_io_base]
    add dx, bx
    in al, dx
    call pm_pci_print_hex8
    inc ebx
    cmp ebx, 6
    jae .mac_done
    mov al, ':'
    call pm_print_char
    jmp .mac_loop

.mac_done:
    call pm_newline
    ret

.unavailable:
    mov esi, pm_pci_mac_na_msg
    call pm_print_string
    ret

pm_pci_print_netdev:
    pushad
    call pci_scan_bus
    cmp dword [pm_pci_count], 0
    jne .scan_nics

    mov esi, pm_pci_no_netdev_msg
    call pm_print_string
    jmp .done

.scan_nics:
    xor ebx, ebx

.nic_loop:
    cmp ebx, [pm_pci_count]
    jae .no_nic
    imul edi, ebx, PCI_RECORD_SIZE
    add edi, pm_pci_records
    call pm_pci_get_nic_type
    jnc .found
    inc ebx
    jmp .nic_loop

.found:
    mov [pm_pci_nic_type], al
    mov [pm_pci_nic_name], esi
    mov esi, pm_pci_nic_msg
    call pm_print_string
    mov esi, [pm_pci_nic_name]
    call pm_print_string
    call pm_newline

    mov esi, pm_pci_bdf_msg
    call pm_print_string
    call pm_pci_print_bdf
    mov esi, pm_pci_irq_msg
    call pm_print_string
    movzx eax, byte [edi+PCI_REC_IRQ]
    call pm_print_uint32
    call pm_newline

    mov esi, pm_pci_bar0_msg
    call pm_print_string
    mov eax, [edi+PCI_REC_BAR0]
    call pm_print_hex32
    mov esi, pm_pci_bar1_msg
    call pm_print_string
    mov eax, [edi+PCI_REC_BAR1]
    call pm_print_hex32
    call pm_newline

    cmp byte [pm_pci_nic_type], PCI_NIC_RTL8139
    jne .print_mac
    mov bl, [edi+PCI_REC_BUS]
    mov bh, [edi+PCI_REC_DEV]
    mov ch, [edi+PCI_REC_FN]
    call pci_enable_bus_master
    call pm_pci_print_command_debug

.print_mac:
    call pm_pci_print_mac_if_safe
    jmp .done

.no_nic:
    mov esi, pm_pci_no_netdev_msg
    call pm_print_string

.done:
    popad
    ret

pm_pci_count dd 0
pm_pci_seen dd 0
pci_command_before dw 0
pci_command_after dw 0
pm_pci_nic_name dd 0
pm_pci_io_base dw 0
pm_pci_nic_type db 0
pm_pci_scan_bus_no db 0
pm_pci_scan_dev db 0
pm_pci_scan_fn db 0
pm_pci_records times PCI_MAX_RECORDS*PCI_RECORD_SIZE db 0

pm_lspci_header_msg db 'BDF      VID:DID   CLS/SUB/PI',13,0
pm_pci_none_msg db 'No PCI devices found.',13,0
pm_pci_truncated_msg db 'PCI list truncated.',13,0
pm_two_spaces_msg db '  ',0
pm_pci_nic_msg db 'NIC: ',0
pm_pci_bdf_msg db 'BDF: ',0
pm_pci_irq_msg db ' IRQ: ',0
pm_pci_bar0_msg db 'BAR0: ',0
pm_pci_bar1_msg db ' BAR1: ',0
pm_pci_mac_msg db 'MAC: ',0
pm_pci_mac_na_msg db 'MAC: unavailable',13,0
pm_pci_no_netdev_msg db 'No NIC found.',13,0
pm_pci_nic_generic_msg db 'Ethernet NIC',0
pm_pci_nic_rtl8139_msg db 'RTL8139',0
pm_pci_nic_ne2k_msg db 'NE2000-compatible',0
pm_pci_nic_e1000_msg db 'Intel E1000',0

pm_pci_print_command_debug:
    mov esi, pm_pci_cmd_before_msg
    call pm_print_string
    movzx eax, word [pci_command_before]
    call pm_print_hex32
    call pm_newline

    mov esi, pm_pci_cmd_after_msg
    call pm_print_string
    movzx eax, word [pci_command_after]
    call pm_print_hex32
    call pm_newline

    mov esi, pm_pci_bus_master_msg
    call pm_print_string
    test word [pci_command_after], PCI_CMD_BUS_MASTER
    jz .bm_no
    mov esi, pm_pci_yes_msg
    call pm_print_string
    jmp .io
.bm_no:
    mov esi, pm_pci_no_msg
    call pm_print_string

.io:
    mov esi, pm_pci_io_space_msg
    call pm_print_string
    test word [pci_command_after], PCI_CMD_IO_SPACE
    jz .io_no
    mov esi, pm_pci_yes_msg
    call pm_print_string
    ret
.io_no:
    mov esi, pm_pci_no_msg
    call pm_print_string
    ret

pm_pci_cmd_before_msg db 'PCI CMD before: ',0
pm_pci_cmd_after_msg db 'PCI CMD after: ',0
pm_pci_bus_master_msg db 'bus_master: ',0
pm_pci_io_space_msg db 'io_space: ',0
pm_pci_yes_msg db 'yes',13,0
pm_pci_no_msg db 'no',13,0
