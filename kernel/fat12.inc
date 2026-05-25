; FAT12 filesystem reader for 1.44 MB floppy images.
; Uses BIOS int 13h CHS reads and loads small files into caller buffers.

disk_read_sector:
    push ax
    push bx
    push cx
    push dx
    push es

    mov [fat12_disk_lba], ax
    mov [fat12_disk_dest], bx

    mov bx, FAT12_SECTORS_PER_TRACK
    xor dx, dx
    div bx
    inc dx
    mov cl, dl

    mov bx, FAT12_HEADS
    xor dx, dx
    div bx
    mov ch, al
    mov dh, dl

    xor ax, ax
    mov es, ax
    mov bx, [fat12_disk_dest]
    mov ax, 0x0201
    mov dl, [boot_drive]
    int 0x13
    jc .fail

    pop es
    pop dx
    pop cx
    pop bx
    pop ax
    clc
    ret

.fail:
    pop es
    pop dx
    pop cx
    pop bx
    pop ax
    stc
    ret

fat12_init:
    pusha

    xor ax, ax
    mov bx, fat12_sector_buffer
    call disk_read_sector
    jc .fail

    mov si, fat12_sector_buffer
    mov al, [si+13]
    mov [fat12_sectors_per_cluster], al

    mov ax, [si+14]
    mov [fat12_reserved_sectors], ax
    mov [fat12_fat_start], ax

    mov al, [si+16]
    mov [fat12_fat_count], al

    mov ax, [si+17]
    mov [fat12_root_entries], ax
    add ax, 15
    mov cl, 4
    shr ax, cl
    mov [fat12_root_sectors], ax

    mov ax, [si+22]
    mov [fat12_sectors_per_fat], ax

    xor ax, ax
    mov al, [fat12_fat_count]
    mov bx, [fat12_sectors_per_fat]
    mul bx
    add ax, [fat12_reserved_sectors]
    mov [fat12_root_start], ax
    add ax, [fat12_root_sectors]
    mov [fat12_data_start], ax

    popa
    clc
    ret

.fail:
    popa
    stc
    ret

fat12_make_83:
    mov byte [fat12_status], 1

    push ax
    push bx
    push cx
    push di

    mov di, fat12_search_name
    mov al, ' '
    mov cx, 11
    rep stosb

    mov di, fat12_search_name
    xor bx, bx
    mov cx, 8

.name_loop:
    mov al, [si]
    cmp al, 0
    je .name_done
    cmp al, ' '
    je .name_done
    cmp al, '.'
    je .extension_start
    cmp cx, 0
    je .bad
    call fat12_uppercase_al
    stosb
    inc si
    inc bx
    loop .name_loop

.name_full:
    mov al, [si]
    cmp al, '.'
    je .extension_start
    cmp al, 0
    je .name_done
    cmp al, ' '
    je .name_done
    jmp .bad

.extension_start:
    cmp bx, 0
    je .bad
    inc si
    mov di, fat12_search_name+8
    mov cx, 3

.extension_loop:
    mov al, [si]
    cmp al, 0
    je .ok
    cmp al, ' '
    je .ok
    cmp al, '.'
    je .bad
    cmp cx, 0
    je .bad
    call fat12_uppercase_al
    stosb
    inc si
    loop .extension_loop

    mov al, [si]
    cmp al, 0
    je .ok
    cmp al, ' '
    je .ok
    jmp .bad

.name_done:
    cmp bx, 0
    je .bad

.ok:
    mov byte [fat12_status], 0

.bad:
    pop di
    pop cx
    pop bx
    pop ax
    cmp byte [fat12_status], 0
    je .return_ok
    stc
    ret
.return_ok:
    clc
    ret

fat12_uppercase_al:
    cmp al, 'a'
    jb .done
    cmp al, 'z'
    ja .done
    sub al, 32
.done:
    ret

fat12_find_file:
    pusha
    mov byte [fat12_status], 1

    mov ax, [fat12_root_start]
    mov [fat12_current_lba], ax
    mov cx, [fat12_root_sectors]

.sector_loop:
    push cx
    mov ax, [fat12_current_lba]
    mov bx, fat12_sector_buffer
    call disk_read_sector
    jc .done_sector_pop

    mov si, fat12_sector_buffer
    mov byte [fat12_entry_count], 16

.entry_loop:
    mov al, [si]
    cmp al, 0
    je .done_sector_pop
    cmp al, 0xE5
    je .next_entry

    mov al, [si+11]
    cmp al, 0x0F
    je .next_entry
    test al, 0x08
    jnz .next_entry
    test al, 0x10
    jnz .next_entry

    call fat12_entry_matches
    jc .found

.next_entry:
    add si, 32
    dec byte [fat12_entry_count]
    jnz .entry_loop

    inc word [fat12_current_lba]
    pop cx
    loop .sector_loop
    jmp .done

.found:
    mov ax, [si+26]
    mov [fat12_first_cluster], ax
    mov ax, [si+28]
    mov [fat12_file_size_low], ax
    mov ax, [si+30]
    mov [fat12_file_size_high], ax
    mov byte [fat12_status], 0

.done_sector_pop:
    pop cx

.done:
    popa
    cmp byte [fat12_status], 0
    je .return_ok
    stc
    ret
.return_ok:
    clc
    ret

fat12_entry_matches:
    push ax
    push cx
    push di
    push si

    mov di, fat12_search_name
    mov cx, 11

.loop:
    mov al, [si]
    cmp al, [di]
    jne .no
    inc si
    inc di
    loop .loop

    pop si
    pop di
    pop cx
    pop ax
    stc
    ret

.no:
    pop si
    pop di
    pop cx
    pop ax
    clc
    ret

fat12_get_next_cluster:
    push bx
    push cx
    push dx
    push si

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

    mov bx, fat12_sector_buffer
    call disk_read_sector
    jc .fail

    mov si, fat12_sector_buffer
    add si, [fat12_fat_offset]
    mov ax, [si]
    test word [fat12_current_cluster], 1
    jz .even
    shr ax, 4

.even:
    and ax, 0x0FFF
    pop si
    pop dx
    pop cx
    pop bx
    clc
    ret

.fail:
    pop si
    pop dx
    pop cx
    pop bx
    stc
    ret

fat12_read_file:
    mov [fat12_dest_ptr], di
    mov [fat12_max_bytes], cx
    mov word [fat12_bytes_read], 0

    call fat12_make_83
    jc .fail
    call fat12_init
    jc .fail
    call fat12_find_file
    jc .fail

    cmp word [fat12_file_size_high], 0
    jne .fail

    mov ax, [fat12_file_size_low]
    mov [fat12_bytes_left], ax
    mov ax, [fat12_first_cluster]
    mov [fat12_current_cluster], ax

.cluster_loop:
    cmp word [fat12_bytes_left], 0
    je .done

    mov ax, [fat12_current_cluster]
    cmp ax, 2
    jb .done
    cmp ax, 0x0FF8
    jae .done

    sub ax, 2
    add ax, [fat12_data_start]
    mov bx, fat12_sector_buffer
    call disk_read_sector
    jc .fail

    mov ax, [fat12_bytes_left]
    cmp ax, FAT12_BYTES_PER_SECTOR
    jbe .copy_size_ok
    mov ax, FAT12_BYTES_PER_SECTOR

.copy_size_ok:
    mov bx, [fat12_max_bytes]
    sub bx, [fat12_bytes_read]
    cmp bx, 0
    je .done
    cmp ax, bx
    jbe .copy
    mov ax, bx

.copy:
    mov [fat12_copy_count], ax
    push si
    push di
    push cx
    mov si, fat12_sector_buffer
    mov di, [fat12_dest_ptr]
    mov cx, [fat12_copy_count]
    cld
    rep movsb
    mov [fat12_dest_ptr], di
    pop cx
    pop di
    pop si

    mov ax, [fat12_copy_count]
    add [fat12_bytes_read], ax
    sub [fat12_bytes_left], ax

    mov ax, [fat12_current_cluster]
    call fat12_get_next_cluster
    jc .fail
    mov [fat12_current_cluster], ax
    jmp .cluster_loop

.done:
    mov di, [fat12_dest_ptr]
    mov byte [di], 0
    mov ax, [fat12_bytes_read]
    clc
    ret

.fail:
    stc
    ret

fat12_list_root:
    call fat12_init
    jc .fail

    mov si, ls_header
    call print_string
    mov byte [fat12_seen_count], 0

    mov ax, [fat12_root_start]
    mov [fat12_current_lba], ax
    mov cx, [fat12_root_sectors]

.sector_loop:
    push cx
    mov ax, [fat12_current_lba]
    mov bx, fat12_sector_buffer
    call disk_read_sector
    jc .done_sector_pop

    mov si, fat12_sector_buffer
    mov byte [fat12_entry_count], 16

.entry_loop:
    mov al, [si]
    cmp al, 0
    je .done_sector_pop
    cmp al, 0xE5
    je .next_entry

    mov al, [si+11]
    cmp al, 0x0F
    je .next_entry
    test al, 0x08
    jnz .next_entry
    test al, 0x10
    jnz .next_entry

    call fat12_print_entry_name
    inc byte [fat12_seen_count]

.next_entry:
    add si, 32
    dec byte [fat12_entry_count]
    jnz .entry_loop

    inc word [fat12_current_lba]
    pop cx
    loop .sector_loop
    jmp .done

.done_sector_pop:
    pop cx

.done:
    cmp byte [fat12_seen_count], 0
    jne .ok
    mov si, ls_empty_msg
    call print_string
.ok:
    clc
    ret

.fail:
    mov si, fat_error_msg
    call print_string
    stc
    ret

fat12_print_entry_name:
    push ax
    push cx
    push si

    mov cx, 8
.name_loop:
    mov al, [si]
    cmp al, ' '
    je .name_done
    call print_char
    inc si
    loop .name_loop

.name_done:
    pop si
    push si
    cmp byte [si+8], ' '
    je .newline
    mov al, '.'
    call print_char
    add si, 8
    mov cx, 3

.ext_loop:
    mov al, [si]
    cmp al, ' '
    je .newline
    call print_char
    inc si
    loop .ext_loop

.newline:
    call newline
    pop si
    pop cx
    pop ax
    ret
