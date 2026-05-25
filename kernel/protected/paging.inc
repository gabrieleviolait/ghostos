; Basic protected-mode identity paging.

[BITS 32]

PAGE_PRESENT_RW equ 0x00000003

pm_init_paging:
    pushad

    mov eax, pm_page_directory
    test eax, 0xFFF
    jnz .bad_directory

    mov eax, pm_first_page_table
    test eax, 0xFFF
    jnz .bad_table

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

.bad_directory:
    mov esi, pm_paging_directory_align_msg
    call pm_panic

.bad_table:
    mov esi, pm_paging_table_align_msg
    call pm_panic

pm_paging_directory_align_msg db 'Paging error: page directory is not 4 KB aligned.',13,0
pm_paging_table_align_msg db 'Paging error: first page table is not 4 KB aligned.',13,0

align 4096
pm_page_directory times 1024 dd 0
align 4096
pm_first_page_table times 1024 dd 0
