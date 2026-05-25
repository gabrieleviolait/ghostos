; v1.0 protected mode runtime: memory manager, paging, tasks, VGA shell.

CODE_SEL equ gdt_code - gdt_start
DATA_SEL equ gdt_data - gdt_start
IDT_PRESENT_INT_GATE equ 10001110b
KBD_BUFFER_MASK equ 31
VGA_BASE equ 0xB8000
PM_STACK_TOP equ 0x90000
PAGE_PRESENT_RW equ 0x00000003

TASK_SIZE equ 16
TASK_ID equ 0
TASK_STATE equ 4
TASK_ENTRY equ 8
TASK_COUNTER equ 12
TASK_READY equ 1
TASK_RUNNING equ 2
TASK_BLOCKED equ 3

collect_e820_memory_map:
    mov word [e820_count], 0
    xor ax, ax
    mov es, ax
    xor ebx, ebx
    xor bp, bp
    mov di, e820_map

.next:
    mov eax, 0xE820
    mov edx, 0x534D4150
    mov ecx, E820_ENTRY_SIZE
    int 0x15
    jc .done
    cmp eax, 0x534D4150
    jne .done
    cmp ecx, 20
    jb .done

    inc bp
    add di, E820_ENTRY_SIZE
    cmp bp, E820_MAX_ENTRIES
    jae .done
    cmp ebx, 0
    jne .next

.done:
    mov [e820_count], bp
    ret

enter_protected_mode_demo:
enter_protected_mode_shell:
    cli
    lgdt [gdt_descriptor]

    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp dword CODE_SEL:protected_mode_start

[BITS 32]

protected_mode_start:
    mov ax, DATA_SEL
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, PM_STACK_TOP

    call pm_idt_init
    lidt [idt_descriptor]
    call pm_pic_remap
    call pm_memory_init
    call pm_tasks_init

    sti

    call pm_clear_screen
    mov esi, pm_banner
    call pm_print_string
    call pm_print_system_status
    call shell32_loop

pm_halt:
    cli
.hang:
    hlt
    jmp .hang

pm_io_wait:
    push eax
    xor al, al
    out 0x80, al
    pop eax
    ret

pm_pic_remap:
    mov al, 0x11
    out 0x20, al
    call pm_io_wait
    out 0xA0, al
    call pm_io_wait

    mov al, 0x20
    out 0x21, al
    call pm_io_wait
    mov al, 0x28
    out 0xA1, al
    call pm_io_wait

    mov al, 0x04
    out 0x21, al
    call pm_io_wait
    mov al, 0x02
    out 0xA1, al
    call pm_io_wait

    mov al, 0x01
    out 0x21, al
    call pm_io_wait
    out 0xA1, al
    call pm_io_wait

    mov al, 11111101b
    out 0x21, al
    call pm_io_wait
    mov al, 11111111b
    out 0xA1, al
    call pm_io_wait
    ret

pm_idt_init:
    pushad
    xor ebx, ebx

.loop:
    mov eax, pm_default_interrupt
    call pm_set_idt_gate
    inc ebx
    cmp ebx, 256
    jb .loop

    mov ebx, 0x21
    mov eax, pm_irq1_keyboard
    call pm_set_idt_gate

    popad
    ret

pm_set_idt_gate:
    push eax
    push ebx
    push edx

    mov edx, idt_start
    shl ebx, 3
    add edx, ebx

    mov word [edx], ax
    mov word [edx+2], CODE_SEL
    mov byte [edx+4], 0
    mov byte [edx+5], IDT_PRESENT_INT_GATE
    shr eax, 16
    mov word [edx+6], ax

    pop edx
    pop ebx
    pop eax
    ret

pm_default_interrupt:
    iretd

pm_irq1_keyboard:
    pushad

    in al, 0x60
    test al, 0x80
    jnz .ack

    movzx ebx, al
    cmp ebx, 128
    jae .ack
    mov al, [pm_scancode_ascii+ebx]
    cmp al, 0
    je .ack
    call pm_kbd_buffer_put

.ack:
    mov al, 0x20
    out 0x20, al

    popad
    iretd

pm_memory_init:
    call pm_detect_memory_mb
    call pm_init_frame_bitmap
    call pm_init_paging
    call pm_heap_init
    ret

pm_detect_memory_mb:
    pushad

    mov dword [pm_detected_memory_mb], PM_MAX_MEMORY_MB
    cmp word [e820_count], 0
    je .done

    xor edx, edx
    movzx ecx, word [e820_count]
    mov esi, e820_map

.loop:
    cmp dword [esi+16], 1
    jne .next
    cmp dword [esi+4], 0
    jne .next
    cmp dword [esi+12], 0
    jne .large

    mov eax, [esi]
    add eax, [esi+8]
    jc .large
    cmp eax, edx
    jbe .next
    mov edx, eax
    jmp .next

.large:
    mov edx, PM_MAX_MEMORY_MB*1024*1024

.next:
    add esi, E820_ENTRY_SIZE
    loop .loop

    cmp edx, 0
    je .done
    shr edx, 20
    cmp edx, PM_MAX_MEMORY_MB
    jbe .store
    mov edx, PM_MAX_MEMORY_MB

.store:
    mov [pm_detected_memory_mb], edx

.done:
    popad
    ret

pm_init_frame_bitmap:
    pushad

    mov edi, pm_frame_bitmap
    mov al, 0xFF
    mov ecx, PM_FRAME_BITMAP_SIZE
    cld
    rep stosb

    mov eax, 1024
    mov ebx, [pm_detected_memory_mb]
    shl ebx, 8

.free_loop:
    cmp eax, ebx
    jae .done
    call free_frame_by_index
    inc eax
    jmp .free_loop

.done:
    popad
    ret

alloc_frame:
    push ebx
    push ecx

    xor eax, eax
    mov ecx, PM_FRAME_COUNT

.scan:
    bt dword [pm_frame_bitmap], eax
    jnc .found
    inc eax
    loop .scan

    pop ecx
    pop ebx
    stc
    ret

.found:
    bts dword [pm_frame_bitmap], eax
    shl eax, 12
    pop ecx
    pop ebx
    clc
    ret

free_frame:
    push eax
    shr eax, 12
    cmp eax, PM_FRAME_COUNT
    jae .done
    btr dword [pm_frame_bitmap], eax
.done:
    pop eax
    ret

free_frame_by_index:
    btr dword [pm_frame_bitmap], eax
    ret

pm_init_paging:
    pushad

    mov edi, pm_page_directory
    xor eax, eax
    mov ecx, 1024
    cld
    rep stosd

    mov edi, pm_first_page_table
    mov eax, PAGE_PRESENT_RW
    mov ecx, 1024

.map_loop:
    stosd
    add eax, PM_FRAME_SIZE
    loop .map_loop

    mov eax, pm_first_page_table
    or eax, PAGE_PRESENT_RW
    mov [pm_page_directory], eax

    mov eax, pm_page_directory
    mov cr3, eax
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    jmp .paging_enabled

.paging_enabled:
    popad
    ret

pm_heap_init:
    mov dword [pm_heap_current], PM_HEAP_START
    mov dword [pm_heap_end], PM_HEAP_START+PM_HEAP_SIZE
    ret

kmalloc:
    push ebx

    add ecx, 15
    and ecx, 0xFFFFFFF0
    mov eax, [pm_heap_current]
    mov ebx, eax
    add ebx, ecx
    cmp ebx, [pm_heap_end]
    ja .fail
    mov [pm_heap_current], ebx
    pop ebx
    clc
    ret

.fail:
    xor eax, eax
    pop ebx
    stc
    ret

kfree:
    ret

pm_tasks_init:
    mov dword [pm_task_count], 2
    mov dword [pm_current_task], 0

    mov edi, pm_tasks
    mov dword [edi+TASK_ID], 1
    mov dword [edi+TASK_STATE], TASK_RUNNING
    mov dword [edi+TASK_ENTRY], shell32_loop
    mov dword [edi+TASK_COUNTER], 0

    add edi, TASK_SIZE
    mov dword [edi+TASK_ID], 2
    mov dword [edi+TASK_STATE], TASK_READY
    mov dword [edi+TASK_ENTRY], pm_demo_task
    mov dword [edi+TASK_COUNTER], 0
    ret

yield:
pm_yield:
    pushad
    mov dword [pm_current_task], 1
    mov dword [pm_tasks+TASK_STATE], TASK_READY
    mov dword [pm_tasks+TASK_SIZE+TASK_STATE], TASK_RUNNING
    call pm_demo_task
    mov dword [pm_tasks+TASK_STATE], TASK_RUNNING
    mov dword [pm_tasks+TASK_SIZE+TASK_STATE], TASK_READY
    mov dword [pm_current_task], 0
    popad
    ret

pm_demo_task:
    inc dword [pm_demo_counter]
    mov eax, [pm_demo_counter]
    mov [pm_tasks+TASK_SIZE+TASK_COUNTER], eax
    mov esi, pm_demo_msg
    call pm_print_string
    mov eax, [pm_demo_counter]
    call pm_print_uint32
    call pm_newline
    ret

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

pm_print_uint32:
    pushad

    cmp eax, 0
    jne .convert
    mov al, '0'
    call pm_print_char
    jmp .done

.convert:
    xor ecx, ecx
    mov ebx, 10

.div_loop:
    xor edx, edx
    div ebx
    push edx
    inc ecx
    cmp eax, 0
    jne .div_loop

.print_loop:
    pop eax
    add al, '0'
    call pm_print_char
    loop .print_loop

.done:
    popad
    ret

pm_kbd_buffer_put:
    push ebx
    push edx

    movzx ebx, byte [pm_kbd_head]
    mov dl, bl
    inc dl
    and dl, KBD_BUFFER_MASK
    cmp dl, [pm_kbd_tail]
    je .full

    mov [pm_kbd_buffer+ebx], al
    mov [pm_kbd_head], dl

.full:
    pop edx
    pop ebx
    ret

pm_kbd_get_char:
    push ebx

.wait:
    cli
    mov al, [pm_kbd_head]
    cmp al, [pm_kbd_tail]
    jne .has_char
    sti
    hlt
    jmp .wait

.has_char:
    movzx ebx, byte [pm_kbd_tail]
    mov al, [pm_kbd_buffer+ebx]
    inc bl
    and bl, KBD_BUFFER_MASK
    mov [pm_kbd_tail], bl
    sti

    pop ebx
    ret

pm_update_cursor:
    pushad

    movzx eax, byte [pm_cursor_y]
    mov ebx, SCREEN_WIDTH
    mul ebx
    movzx ebx, byte [pm_cursor_x]
    add eax, ebx
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

    popad
    ret

pm_clear_screen:
    pushad

    mov edi, VGA_BASE
    mov ah, [pm_text_attr]
    mov al, ' '
    mov ecx, SCREEN_WIDTH*SCREEN_HEIGHT
    cld
    rep stosw

    mov byte [pm_cursor_x], 0
    mov byte [pm_cursor_y], 0
    call pm_update_cursor

    popad
    ret

pm_scroll_up:
    pushad

    mov esi, VGA_BASE + SCREEN_WIDTH*2
    mov edi, VGA_BASE
    mov ecx, (SCREEN_WIDTH*(SCREEN_HEIGHT-1)*2)/4
    cld
    rep movsd

    mov edi, VGA_BASE + SCREEN_WIDTH*(SCREEN_HEIGHT-1)*2
    mov ah, [pm_text_attr]
    mov al, ' '
    mov ecx, SCREEN_WIDTH
    rep stosw

    popad
    ret

pm_print_char:
    mov [pm_char_value], al
    pushad

    mov al, [pm_char_value]
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

    movzx eax, byte [pm_cursor_y]
    mov ebx, SCREEN_WIDTH
    mul ebx
    movzx ebx, byte [pm_cursor_x]
    add eax, ebx
    shl eax, 1

    mov edi, VGA_BASE
    add edi, eax
    mov al, [pm_char_value]
    mov ah, [pm_text_attr]
    mov [edi], ax

    inc byte [pm_cursor_x]
    cmp byte [pm_cursor_x], SCREEN_WIDTH
    jb .refresh

.newline:
    mov byte [pm_cursor_x], 0
    inc byte [pm_cursor_y]
    cmp byte [pm_cursor_y], SCREEN_HEIGHT
    jb .refresh
    call pm_scroll_up
    mov byte [pm_cursor_y], SCREEN_HEIGHT-1
    jmp .refresh

.backspace:
    cmp byte [pm_cursor_x], 0
    je .refresh
    dec byte [pm_cursor_x]

    movzx eax, byte [pm_cursor_y]
    mov ebx, SCREEN_WIDTH
    mul ebx
    movzx ebx, byte [pm_cursor_x]
    add eax, ebx
    shl eax, 1

    mov edi, VGA_BASE
    add edi, eax
    mov al, ' '
    mov ah, [pm_text_attr]
    mov [edi], ax

.refresh:
    call pm_update_cursor

.done:
    popad
    ret

pm_print_string:
    pushad

.next:
    lodsb
    cmp al, 0
    je .done
    call pm_print_char
    jmp .next

.done:
    popad
    ret

pm_newline:
    mov al, 13
    call pm_print_char
    ret

pm_read_line:
    pushad
    xor ecx, ecx

.read_key:
    call pm_kbd_get_char
    cmp al, 13
    je .enter
    cmp al, 8
    je .backspace
    cmp al, 32
    jb .read_key
    cmp al, 126
    ja .read_key
    cmp ecx, CMD_BUFFER_SIZE-1
    jae .read_key

    stosb
    inc ecx
    call pm_print_char
    jmp .read_key

.backspace:
    cmp ecx, 0
    je .read_key
    dec edi
    dec ecx
    mov al, 8
    call pm_print_char
    jmp .read_key

.enter:
    mov al, 0
    stosb
    call pm_newline
    popad
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

    mov edi, pm_cmd_version
    call pm_starts_with_word
    jc pm_do_version

    mov edi, pm_cmd_mem
    call pm_starts_with_word
    jc pm_do_mem

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

pm_do_version:
    mov esi, pm_version_msg
    call pm_print_string
    jmp shell32_loop

pm_do_mem:
    call pm_print_system_status
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

pm_print_tasks:
    mov esi, pm_task1_msg
    call pm_print_string
    mov esi, pm_task2_msg
    call pm_print_string
    mov esi, pm_demo_counter_msg
    call pm_print_string
    mov eax, [pm_demo_counter]
    call pm_print_uint32
    call pm_newline
    ret

[BITS 16]

gdt_start:
gdt_null:
    dq 0

gdt_code:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10011010b
    db 11001111b
    db 0x00

gdt_data:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10010010b
    db 11001111b
    db 0x00

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

idt_descriptor:
    dw idt_end - idt_start - 1
    dd idt_start

align 8
idt_start:
    times 256 dq 0
idt_end:

pm_text_attr db 0x07
pm_cursor_x db 0
pm_cursor_y db 0
pm_char_value db 0
pm_kbd_head db 0
pm_kbd_tail db 0
pm_kbd_buffer times 32 db 0
pm_cmd_buffer times CMD_BUFFER_SIZE db 0
pm_detected_memory_mb dd PM_MAX_MEMORY_MB
pm_heap_current dd 0
pm_heap_end dd 0
pm_task_count dd 0
pm_current_task dd 0
pm_demo_counter dd 0
pm_tasks times TASK_SIZE*4 db 0
e820_count dw 0
e820_map times E820_MAX_ENTRIES*E820_ENTRY_SIZE db 0

pm_scancode_ascii:
    db 0,0,'1','2','3','4','5','6','7','8','9','0','-','=',8,0
    db 'q','w','e','r','t','y','u','i','o','p','[',']',13,0,'a','s'
    db 'd','f','g','h','j','k','l',';',39,'`',0,92,'z','x','c','v'
    db 'b','n','m',',','.','/',0,'*',0,' '
    times 128-($-pm_scancode_ascii) db 0

pm_banner db 'Ghost OS 32-bit shell',13
          db 'Protected mode, paging, heap, cooperative tasks.',13,13,0
pm_memory_prefix db 'Memory: ',0
pm_memory_suffix db ' MB detected',13,0
pm_heap_ready_msg db 'Heap initialized',13,0
pm_task1_msg db 'Task 1: shell',13,0
pm_task2_msg db 'Task 2: demo',13,0
pm_demo_msg db 'Task 2 demo counter: ',0
pm_demo_counter_msg db 'Demo counter: ',0
pm_prompt db 'ghost32> ',0
pm_help_msg db 'Commands:',13
            db '  help       show this help',13
            db '  clear/cls  clear screen',13
            db '  echo TEXT  print text',13
            db '  version    show version',13
            db '  mem        show memory/task startup status',13
            db '  tasks      show task table summary',13
            db '  yield      run cooperative demo task once',13
            db '  halt       stop CPU',13,0
pm_version_msg db 'Ghost OS v1.0 / memory manager + task abstraction',13,0
pm_unknown_msg db 'Unknown command. Type help.',13,0
pm_halt_msg db 'System halted.',13,0

pm_cmd_help db 'help',0
pm_cmd_clear db 'clear',0
pm_cmd_cls db 'cls',0
pm_cmd_echo db 'echo',0
pm_cmd_version db 'version',0
pm_cmd_mem db 'mem',0
pm_cmd_tasks db 'tasks',0
pm_cmd_yield db 'yield',0
pm_cmd_halt db 'halt',0

align 4096
pm_page_directory times 1024 dd 0
pm_first_page_table times 1024 dd 0
pm_frame_bitmap times PM_FRAME_BITMAP_SIZE db 0
