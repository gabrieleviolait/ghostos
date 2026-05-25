; Protected-mode FAT12 reader for the boot floppy.
; Uses the 82077-compatible floppy controller directly; no BIOS calls.

[BITS 32]

FDC_DOR equ 0x3F2
FDC_MSR equ 0x3F4
FDC_FIFO equ 0x3F5
FDC_CCR equ 0x3F7
FDC_CMD_SPECIFY equ 0x03
FDC_CMD_SENSE_INTERRUPT equ 0x08
FDC_CMD_RECALIBRATE equ 0x07
FDC_CMD_SEEK equ 0x0F
FDC_CMD_READ_DATA equ 0xE6
FDC_DRIVE0_ON equ 0x1C

pm_floppy_wait_irq:
    push ecx
    sti
    mov ecx, 0x2000000

.wait:
    cmp byte [pm_floppy_irq_fired], 0
    jne .ok
    loop .wait
    pop ecx
    stc
    ret

.ok:
    pop ecx
    clc
    ret

pm_floppy_delay:
    push ecx
    mov ecx, 0x30000
.loop:
    loop .loop
    pop ecx
    ret

pm_floppy_send_byte:
    push ebx
    push ecx
    push edx

    mov bl, al
    mov ecx, 0x10000

.wait:
    mov dx, FDC_MSR
    in al, dx
    and al, 0xC0
    cmp al, 0x80
    je .ready
    loop .wait

    pop edx
    pop ecx
    pop ebx
    stc
    ret

.ready:
    mov dx, FDC_FIFO
    mov al, bl
    out dx, al
    pop edx
    pop ecx
    pop ebx
    clc
    ret

pm_floppy_recv_byte:
    push ecx
    push edx

    mov ecx, 0x10000

.wait:
    mov dx, FDC_MSR
    in al, dx
    and al, 0xC0
    cmp al, 0xC0
    je .ready
    loop .wait

    pop edx
    pop ecx
    stc
    ret

.ready:
    mov dx, FDC_FIFO
    in al, dx
    pop edx
    pop ecx
    clc
    ret

pm_floppy_sense_interrupt:
    mov al, FDC_CMD_SENSE_INTERRUPT
    call pm_floppy_send_byte
    jc .fail
    call pm_floppy_recv_byte
    jc .fail
    mov [pm_fdc_st0], al
    call pm_floppy_recv_byte
    jc .fail
    mov [pm_fdc_cylinder], al
    clc
    ret

.fail:
    stc
    ret

pm_floppy_specify:
    mov al, FDC_CMD_SPECIFY
    call pm_floppy_send_byte
    jc .fail
    mov al, 0xDF
    call pm_floppy_send_byte
    jc .fail
    mov al, 0x02
    call pm_floppy_send_byte
    jc .fail
    clc
    ret

.fail:
    stc
    ret

pm_floppy_recalibrate:
    push ecx
    mov ecx, 3

.try:
    mov byte [pm_floppy_irq_fired], 0
    mov al, FDC_CMD_RECALIBRATE
    call pm_floppy_send_byte
    jc .next
    xor al, al
    call pm_floppy_send_byte
    jc .next
    call pm_floppy_wait_irq
    jc .next
    call pm_floppy_sense_interrupt
    jc .next
    cmp byte [pm_fdc_cylinder], 0
    je .ok

.next:
    loop .try
    pop ecx
    stc
    ret

.ok:
    pop ecx
    clc
    ret

pm_floppy_init:
    cmp byte [pm_floppy_ready], 0
    jne .ready

    mov dx, FDC_DOR
    xor al, al
    out dx, al
    call pm_floppy_delay

    mov byte [pm_floppy_irq_fired], 0
    mov al, FDC_DRIVE0_ON
    out dx, al
    call pm_floppy_wait_irq
    jc .fail

    mov dx, FDC_CCR
    xor al, al
    out dx, al

    mov ecx, 4
.sense_loop:
    call pm_floppy_sense_interrupt
    loop .sense_loop

    call pm_floppy_specify
    jc .fail
    call pm_floppy_recalibrate
    jc .fail

    mov byte [pm_floppy_ready], 1

.ready:
    clc
    ret

.fail:
    stc
    ret

pm_floppy_seek:
    mov byte [pm_floppy_irq_fired], 0
    mov al, FDC_CMD_SEEK
    call pm_floppy_send_byte
    jc .fail
    mov al, [pm_fdc_head]
    shl al, 2
    call pm_floppy_send_byte
    jc .fail
    mov al, [pm_fdc_cylinder]
    call pm_floppy_send_byte
    jc .fail
    call pm_floppy_wait_irq
    jc .fail
    call pm_floppy_sense_interrupt
    jc .fail
    mov al, [pm_fdc_seek_cylinder]
    cmp [pm_fdc_cylinder], al
    jne .fail
    clc
    ret

.fail:
    stc
    ret

pm_floppy_dma_setup:
    push eax
    push ebx
    push edx

    mov eax, fat12_sector_buffer
    mov ebx, eax
    and ebx, 0xFFFF
    cmp ebx, 0xFE00
    ja .fail

    mov dx, 0x0A
    mov al, 0x06
    out dx, al

    mov dx, 0x0C
    xor al, al
    out dx, al

    mov dx, 0x0B
    mov al, 0x46
    out dx, al

    mov eax, fat12_sector_buffer
    mov dx, 0x04
    out dx, al
    mov al, ah
    out dx, al

    mov eax, fat12_sector_buffer
    shr eax, 16
    mov dx, 0x81
    out dx, al

    mov dx, 0x0C
    xor al, al
    out dx, al

    mov dx, 0x05
    mov ax, FAT12_BYTES_PER_SECTOR-1
    out dx, al
    mov al, ah
    out dx, al

    mov dx, 0x0A
    mov al, 0x02
    out dx, al

    pop edx
    pop ebx
    pop eax
    clc
    ret

.fail:
    pop edx
    pop ebx
    pop eax
    stc
    ret

pm_floppy_read_sector_chs:
    call pm_floppy_seek
    jc .fail
    call pm_floppy_dma_setup
    jc .fail

    mov byte [pm_floppy_irq_fired], 0
    mov al, FDC_CMD_READ_DATA
    call pm_floppy_send_byte
    jc .fail
    mov al, [pm_fdc_head]
    shl al, 2
    call pm_floppy_send_byte
    jc .fail
    mov al, [pm_fdc_seek_cylinder]
    call pm_floppy_send_byte
    jc .fail
    mov al, [pm_fdc_head]
    call pm_floppy_send_byte
    jc .fail
    mov al, [pm_fdc_sector]
    call pm_floppy_send_byte
    jc .fail
    mov al, 2
    call pm_floppy_send_byte
    jc .fail
    mov al, [pm_fdc_sector]
    call pm_floppy_send_byte
    jc .fail
    mov al, 0x1B
    call pm_floppy_send_byte
    jc .fail
    mov al, 0xFF
    call pm_floppy_send_byte
    jc .fail

    call pm_floppy_wait_irq
    jc .fail

    mov edi, pm_fdc_result
    mov ecx, 7
.result_loop:
    call pm_floppy_recv_byte
    jc .fail
    stosb
    loop .result_loop

    mov al, [pm_fdc_result]
    and al, 0xC0
    cmp al, 0
    jne .fail
    cmp byte [pm_fdc_result+1], 0
    jne .fail
    cmp byte [pm_fdc_result+2], 0
    jne .fail
    clc
    ret

.fail:
    stc
    ret

pm_disk_read_sector:
    pushad

    mov [pm_fat12_lba], ax
    call pm_floppy_init
    jc .fail

    movzx eax, word [pm_fat12_lba]
    xor edx, edx
    mov ebx, FAT12_SECTORS_PER_TRACK*FAT12_HEADS
    div ebx
    mov [pm_fdc_seek_cylinder], al
    mov [pm_fdc_cylinder], al

    mov eax, edx
    xor edx, edx
    mov ebx, FAT12_SECTORS_PER_TRACK
    div ebx
    mov [pm_fdc_head], al
    inc dl
    mov [pm_fdc_sector], dl

    call pm_floppy_read_sector_chs
    jc .fail

    popad
    clc
    ret

.fail:
    popad
    stc
    ret

pm_fat12_init:
    pushad

    xor ax, ax
    call pm_disk_read_sector
    jc .fail

    mov esi, fat12_sector_buffer
    mov al, [esi+13]
    mov [fat12_sectors_per_cluster], al

    mov ax, [esi+14]
    mov [fat12_reserved_sectors], ax
    mov [fat12_fat_start], ax

    mov al, [esi+16]
    mov [fat12_fat_count], al

    mov ax, [esi+17]
    mov [fat12_root_entries], ax
    add ax, 15
    shr ax, 4
    mov [fat12_root_sectors], ax

    mov ax, [esi+22]
    mov [fat12_sectors_per_fat], ax

    xor ax, ax
    mov al, [fat12_fat_count]
    mov bx, [fat12_sectors_per_fat]
    mul bx
    add ax, [fat12_reserved_sectors]
    mov [fat12_root_start], ax
    add ax, [fat12_root_sectors]
    mov [fat12_data_start], ax

    popad
    clc
    ret

.fail:
    popad
    stc
    ret

pm_fat12_uppercase_al:
    cmp al, 'a'
    jb .done
    cmp al, 'z'
    ja .done
    sub al, 32
.done:
    ret

pm_fat12_make_83:
    mov byte [fat12_status], 1

    push eax
    push ebx
    push ecx
    push edi

    mov edi, fat12_search_name
    mov al, ' '
    mov ecx, 11
    rep stosb

    mov edi, fat12_search_name
    xor ebx, ebx
    mov ecx, 8

.name_loop:
    mov al, [esi]
    cmp al, 0
    je .name_done
    cmp al, ' '
    je .name_done
    cmp al, '.'
    je .extension_start
    cmp ecx, 0
    je .bad
    call pm_fat12_uppercase_al
    stosb
    inc esi
    inc ebx
    loop .name_loop

.name_full:
    mov al, [esi]
    cmp al, '.'
    je .extension_start
    cmp al, 0
    je .name_done
    cmp al, ' '
    je .name_done
    jmp .bad

.extension_start:
    cmp ebx, 0
    je .bad
    inc esi
    mov edi, fat12_search_name+8
    mov ecx, 3

.extension_loop:
    mov al, [esi]
    cmp al, 0
    je .ok
    cmp al, ' '
    je .ok
    cmp al, '.'
    je .bad
    cmp ecx, 0
    je .bad
    call pm_fat12_uppercase_al
    stosb
    inc esi
    loop .extension_loop

    mov al, [esi]
    cmp al, 0
    je .ok
    cmp al, ' '
    je .ok
    jmp .bad

.name_done:
    cmp ebx, 0
    je .bad

.ok:
    mov byte [fat12_status], 0

.bad:
    pop edi
    pop ecx
    pop ebx
    pop eax
    cmp byte [fat12_status], 0
    je .return_ok
    stc
    ret

.return_ok:
    clc
    ret

pm_fat12_entry_matches:
    push eax
    push ecx
    push edi
    push esi

    mov edi, fat12_search_name
    mov ecx, 11

.loop:
    mov al, [esi]
    cmp al, [edi]
    jne .no
    inc esi
    inc edi
    loop .loop

    pop esi
    pop edi
    pop ecx
    pop eax
    stc
    ret

.no:
    pop esi
    pop edi
    pop ecx
    pop eax
    clc
    ret

pm_fat12_find_file:
    pushad
    mov byte [fat12_status], 1

    mov ax, [fat12_root_start]
    mov [fat12_current_lba], ax
    movzx ecx, word [fat12_root_sectors]

.sector_loop:
    push ecx
    mov ax, [fat12_current_lba]
    call pm_disk_read_sector
    jc .done_sector_pop

    mov esi, fat12_sector_buffer
    mov byte [fat12_entry_count], 16

.entry_loop:
    mov al, [esi]
    cmp al, 0
    je .done_sector_pop
    cmp al, 0xE5
    je .next_entry

    mov al, [esi+11]
    cmp al, 0x0F
    je .next_entry
    test al, 0x08
    jnz .next_entry
    test al, 0x10
    jnz .next_entry

    call pm_fat12_entry_matches
    jc .found

.next_entry:
    add esi, 32
    dec byte [fat12_entry_count]
    jnz .entry_loop

    inc word [fat12_current_lba]
    pop ecx
    loop .sector_loop
    jmp .done

.found:
    mov ax, [esi+26]
    mov [fat12_first_cluster], ax
    mov ax, [esi+28]
    mov [fat12_file_size_low], ax
    mov ax, [esi+30]
    mov [fat12_file_size_high], ax
    mov byte [fat12_status], 0

.done_sector_pop:
    pop ecx

.done:
    popad
    cmp byte [fat12_status], 0
    je .return_ok
    stc
    ret

.return_ok:
    clc
    ret

pm_fat12_get_next_cluster:
    push ebx
    push ecx
    push edx
    push esi

    mov [fat12_current_cluster], ax
    mov bx, ax
    shr bx, 1
    add bx, ax

    mov ax, bx
    xor dx, dx
    mov cx, FAT12_BYTES_PER_SECTOR
    div cx
    add ax, [fat12_fat_start]
    mov [fat12_fat_offset], dx

    call pm_disk_read_sector
    jc .fail

    mov esi, fat12_sector_buffer
    movzx edx, word [fat12_fat_offset]
    add esi, edx
    mov ax, [esi]
    test word [fat12_current_cluster], 1
    jz .even
    shr ax, 4

.even:
    and ax, 0x0FFF
    pop esi
    pop edx
    pop ecx
    pop ebx
    clc
    ret

.fail:
    pop esi
    pop edx
    pop ecx
    pop ebx
    stc
    ret

pm_fat12_read_file:
    mov [pm_fat12_dest_ptr], edi
    mov [pm_fat12_max_bytes], ecx
    mov dword [pm_fat12_bytes_read], 0

    call pm_fat12_make_83
    jc .fail
    call pm_fat12_init
    jc .fail
    call pm_fat12_find_file
    jc .fail

    cmp word [fat12_file_size_high], 0
    jne .fail

    movzx eax, word [fat12_file_size_low]
    mov [pm_fat12_bytes_left], eax
    mov ax, [fat12_first_cluster]
    mov [fat12_current_cluster], ax

.cluster_loop:
    cmp dword [pm_fat12_bytes_left], 0
    je .done

    mov ax, [fat12_current_cluster]
    cmp ax, 2
    jb .done
    cmp ax, 0x0FF8
    jae .done

    sub ax, 2
    add ax, [fat12_data_start]
    call pm_disk_read_sector
    jc .fail

    mov eax, [pm_fat12_bytes_left]
    cmp eax, FAT12_BYTES_PER_SECTOR
    jbe .copy_size_ok
    mov eax, FAT12_BYTES_PER_SECTOR

.copy_size_ok:
    mov ebx, [pm_fat12_max_bytes]
    sub ebx, [pm_fat12_bytes_read]
    cmp ebx, 0
    je .done
    cmp eax, ebx
    jbe .copy
    mov eax, ebx

.copy:
    mov [pm_fat12_copy_count], eax
    push esi
    push edi
    push ecx
    mov esi, fat12_sector_buffer
    mov edi, [pm_fat12_dest_ptr]
    mov ecx, [pm_fat12_copy_count]
    cld
    rep movsb
    mov [pm_fat12_dest_ptr], edi
    pop ecx
    pop edi
    pop esi

    mov eax, [pm_fat12_copy_count]
    add [pm_fat12_bytes_read], eax
    sub [pm_fat12_bytes_left], eax

    mov ax, [fat12_current_cluster]
    call pm_fat12_get_next_cluster
    jc .fail
    mov [fat12_current_cluster], ax
    jmp .cluster_loop

.done:
    mov edi, [pm_fat12_dest_ptr]
    mov byte [edi], 0
    mov eax, [pm_fat12_bytes_read]
    clc
    ret

.fail:
    stc
    ret

pm_fat12_print_entry_name:
    push eax
    push ecx
    push esi

    mov ecx, 8
.name_loop:
    mov al, [esi]
    cmp al, ' '
    je .name_done
    call pm_print_char
    inc esi
    loop .name_loop

.name_done:
    pop esi
    push esi
    cmp byte [esi+8], ' '
    je .newline
    mov al, '.'
    call pm_print_char
    add esi, 8
    mov ecx, 3

.ext_loop:
    mov al, [esi]
    cmp al, ' '
    je .newline
    call pm_print_char
    inc esi
    loop .ext_loop

.newline:
    call pm_newline
    pop esi
    pop ecx
    pop eax
    ret

pm_fat12_list_root:
    call pm_fat12_init
    jc .fail

    mov esi, ls_header
    call pm_print_string
    mov byte [fat12_seen_count], 0

    mov ax, [fat12_root_start]
    mov [fat12_current_lba], ax
    movzx ecx, word [fat12_root_sectors]

.sector_loop:
    push ecx
    mov ax, [fat12_current_lba]
    call pm_disk_read_sector
    jc .done_sector_pop

    mov esi, fat12_sector_buffer
    mov byte [fat12_entry_count], 16

.entry_loop:
    mov al, [esi]
    cmp al, 0
    je .done_sector_pop
    cmp al, 0xE5
    je .next_entry

    mov al, [esi+11]
    cmp al, 0x0F
    je .next_entry
    test al, 0x08
    jnz .next_entry
    test al, 0x10
    jnz .next_entry

    call pm_fat12_print_entry_name
    inc byte [fat12_seen_count]

.next_entry:
    add esi, 32
    dec byte [fat12_entry_count]
    jnz .entry_loop

    inc word [fat12_current_lba]
    pop ecx
    loop .sector_loop
    jmp .done

.done_sector_pop:
    pop ecx

.done:
    cmp byte [fat12_seen_count], 0
    jne .ok
    mov esi, ls_empty_msg
    call pm_print_string

.ok:
    clc
    ret

.fail:
    mov esi, fat_error_msg
    call pm_print_string
    stc
    ret

pm_floppy_irq_fired db 0
pm_floppy_ready db 0
pm_fdc_st0 db 0
pm_fdc_cylinder db 0
pm_fdc_seek_cylinder db 0
pm_fdc_head db 0
pm_fdc_sector db 0
pm_fdc_result times 7 db 0
pm_fat12_lba dw 0
pm_fat12_dest_ptr dd 0
pm_fat12_max_bytes dd 0
pm_fat12_bytes_read dd 0
pm_fat12_bytes_left dd 0
pm_fat12_copy_count dd 0
