; Stable protected-mode syscall ABI for GEX programs.
;
; ABI:
;   EDX = syscall number
;   ESI = arg1
;   EDI = arg2
;   ECX = arg length / optional size
;   EAX = return value

[BITS 32]

SYS_PRINT equ 0
SYS_READ equ 1
SYS_EXIT equ 2
SYS_GET_VERSION equ 3
SYS_GET_TICKS equ 4

SYS_OK equ 0
SYS_ERR_UNKNOWN equ 0xFFFFFFFF
SYS_ERR_INVAL equ 0xFFFFFFFE

GEX_ABI_VERSION equ 0x00010405
GEX_META_MAGIC equ 0x4D584547
GEX_META_MAGIC_OFF equ 0
GEX_META_ABI_OFF equ 4
GEX_META_ARGC_OFF equ 8
GEX_META_ARGV_OFF equ 12
GEX_META_SYSCALL_OFF equ 16
GEX_META_VERSION_OFF equ 20
GEX_META_SIZE equ 24

pm_syscall_dispatch:
    cmp edx, SYS_PRINT
    je .print
    cmp edx, SYS_READ
    je .read
    cmp edx, SYS_EXIT
    je .exit
    cmp edx, SYS_GET_VERSION
    je .get_version
    cmp edx, SYS_GET_TICKS
    je .get_ticks

    mov eax, SYS_ERR_UNKNOWN
    ret

.print:
    cmp esi, 0
    je .invalid
    call pm_print_string
    xor eax, eax
    ret

.read:
    cmp edi, 0
    je .invalid
    call pm_gex_read_line
    ret

.exit:
    cmp byte [pm_gex_running], 1
    jne .invalid
    mov [pm_gex_exit_status], esi
    mov esp, [pm_gex_saved_esp]
    jmp pm_gex_after_call

.get_version:
    mov eax, pm_sys_version_string
    ret

.get_ticks:
    mov eax, [pm_system_ticks]
    ret

.invalid:
    mov eax, SYS_ERR_INVAL
    ret

pm_gex_legacy_syscall:
    push edx
    cmp eax, SYS_EXIT
    jne .dispatch
    mov esi, ebx

.dispatch:
    mov edx, eax
    call pm_syscall_dispatch
    pop edx
    ret

pm_sys_version_string db 'Ghost OS v1.5-alpha',0
