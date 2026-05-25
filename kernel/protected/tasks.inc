; Cooperative protected-mode task abstraction.

[BITS 32]

TASK_SIZE equ 16
TASK_ID equ 0
TASK_STATE equ 4
TASK_ENTRY equ 8
TASK_COUNTER equ 12
TASK_READY equ 1
TASK_RUNNING equ 2
TASK_BLOCKED equ 3

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

pm_task_count dd 0
pm_current_task dd 0
pm_demo_counter dd 0
pm_tasks times TASK_SIZE*4 db 0

pm_task1_msg db 'Task 1: shell',13,0
pm_task2_msg db 'Task 2: demo',13,0
pm_demo_msg db 'Task 2 demo counter: ',0
pm_demo_counter_msg db 'Demo counter: ',0
