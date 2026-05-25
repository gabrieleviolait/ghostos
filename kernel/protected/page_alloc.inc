; Protected-mode physical page allocator.

[BITS 32]

pm_page_alloc_init:
    pushad

    mov edi, pm_frame_bitmap
    mov al, 0xFF
    mov ecx, PM_FRAME_BITMAP_SIZE
    cld
    rep stosb

    mov eax, [pm_detected_memory_mb]
    shl eax, 8
    cmp eax, PM_FRAME_COUNT
    jbe .cap_ok
    mov eax, PM_FRAME_COUNT

.cap_ok:
    mov [pm_page_total], eax
    mov dword [pm_page_free], 0
    mov dword [pm_page_next_hint], 1024
    cmp eax, 1024
    jbe .done

    mov ebx, eax
    sub ebx, 1024
    mov [pm_page_managed], ebx
    mov eax, 1024

.free_loop:
    cmp eax, [pm_page_total]
    jae .done
    btr dword [pm_frame_bitmap], eax
    inc dword [pm_page_free]
    inc eax
    jmp .free_loop

.done:
    popad
    ret

alloc_frame:
alloc_page:
    push ebx
    push ecx

    mov ebx, [pm_page_next_hint]
    cmp ebx, 1024
    jae .hint_ok
    mov ebx, 1024

.hint_ok:
    cmp ebx, [pm_page_total]
    jb .scan_high
    mov ebx, 1024

.scan_high:
    mov eax, ebx

.scan:
    cmp eax, [pm_page_total]
    jae .wrap
    bt dword [pm_frame_bitmap], eax
    jnc .found
    inc eax
    jmp .scan

.wrap:
    mov eax, 1024

.scan_low:
    cmp eax, ebx
    jae .fail
    bt dword [pm_frame_bitmap], eax
    jnc .found
    inc eax
    jmp .scan_low

.found:
    bts dword [pm_frame_bitmap], eax
    dec dword [pm_page_free]
    mov ebx, eax
    inc ebx
    mov [pm_page_next_hint], ebx
    shl eax, 12
    pop ecx
    pop ebx
    clc
    ret

.fail:
    xor eax, eax
    pop ecx
    pop ebx
    stc
    ret

free_frame:
free_page:
    push eax
    test eax, 0xFFF
    jnz .done
    shr eax, 12
    cmp eax, 1024
    jb .done
    cmp eax, [pm_page_total]
    jae .done
    btr dword [pm_frame_bitmap], eax
    jnc .done
    inc dword [pm_page_free]

.done:
    pop eax
    ret

pm_print_page_stats:
    mov esi, pm_pages_total_msg
    call pm_print_string
    mov eax, [pm_page_total]
    call pm_print_uint32
    call pm_newline

    mov esi, pm_pages_managed_msg
    call pm_print_string
    mov eax, [pm_page_managed]
    call pm_print_uint32
    call pm_newline

    mov esi, pm_pages_free_msg
    call pm_print_string
    mov eax, [pm_page_free]
    call pm_print_uint32
    call pm_newline

    mov esi, pm_pages_used_msg
    call pm_print_string
    mov eax, [pm_page_managed]
    sub eax, [pm_page_free]
    call pm_print_uint32
    call pm_newline
    ret

pm_page_total dd 0
pm_page_managed dd 0
pm_page_free dd 0
pm_page_next_hint dd 1024
pm_pages_total_msg db 'Total pages: ',0
pm_pages_managed_msg db 'Managed pages: ',0
pm_pages_free_msg db 'Free pages: ',0
pm_pages_used_msg db 'Used pages: ',0
pm_frame_bitmap times PM_FRAME_BITMAP_SIZE db 0
