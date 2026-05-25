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

pm_banner db 'Ghost OS v1.4-beta ghost32',13,13,0
pm_memory_prefix db 'Memory: ',0
pm_memory_suffix db ' MB detected',13,0
pm_heap_ready_msg db 'Heap initialized',13,0
pm_prompt db 'ghost32> ',0
pm_help_msg db 'Commands: help clear cls echo version mem heapinfo pages memmap tasks yield ls cat browse run halt',13,0
pm_version_msg db 'Ghost OS v1.4-beta / line editor',13,0
pm_unknown_msg db 'Unknown command. Type help.',13,0
pm_halt_msg db 'System halted.',13,0
pm_run_usage_msg db 'Usage: run HELLO.GEX',13,0

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
pm_cmd_pages db 'pages',0
pm_cmd_memmap db 'memmap',0
pm_cmd_tasks db 'tasks',0
pm_cmd_yield db 'yield',0
pm_cmd_halt db 'halt',0
