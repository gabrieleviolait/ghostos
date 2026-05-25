; Sample Ghost Executable for the v1.5-alpha loader.

[BITS 32]

GEX_LOAD_ADDR equ 0x00180000
GEX_HEADER_SIZE equ 32
SYS_PRINT equ 0
SYS_EXIT equ 2
GEX_META_SYSCALL_OFF equ 16

[ORG GEX_LOAD_ADDR - GEX_HEADER_SIZE]

    db 'GEX1'
    dd start - image_start
    dd image_start
    dd image_end - image_start
    dd 0
    times GEX_HEADER_SIZE-($-$$) db 0

image_start:
start:
    mov edx, SYS_PRINT
    mov esi, hello_msg
    call dword [ebp+GEX_META_SYSCALL_OFF]

    xor esi, esi
    mov edx, SYS_EXIT
    call dword [ebp+GEX_META_SYSCALL_OFF]
    ret

hello_msg db 'Hello from HELLO.GEX',13,0

image_end:
