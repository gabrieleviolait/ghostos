; Ghost OS v0.9 bootloader
; Pure NASM 16-bit x86 real mode
; FAT12-compatible boot sector. Loads the hidden fixed-sector kernel from
; disk sectors into 0000:1000 and jumps there.

[BITS 16]
[ORG 0x7C00]

    jmp short start
    nop

OEMLabel         db 'GHOSTOS '
BytesPerSector   dw 512
SectorsPerCluster db 1
ReservedSectors  dw 33
NumberOfFATs     db 2
RootEntries      dw 224
TotalSectors16   dw 2880
MediaDescriptor  db 0xF0
SectorsPerFAT    dw 9
SectorsPerTrack  dw 18
NumberOfHeads    dw 2
HiddenSectors    dd 0
TotalSectors32   dd 0
DriveNumber      db 0
ReservedBootByte db 0
ExtendedBootSig  db 0x29
VolumeID         dd 0x20260525
VolumeLabel      db 'GHOST OS   '
FileSystemType   db 'FAT12   '

KERNEL_SEG     equ 0x0000
KERNEL_OFFSET  equ 0x1000
KERNEL_SECTORS equ 32
MAX_RETRIES    equ 3

start:
    cli
    cld

    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    sti

    mov [BOOT_DRIVE], dl

    mov si, boot_msg
    call print_string

    call reset_disk

    mov byte [retry_count], MAX_RETRIES

load_kernel:
    mov ax, KERNEL_SEG
    mov es, ax
    mov bx, KERNEL_OFFSET

    mov ah, 0x02
    mov al, KERNEL_SECTORS
    mov ch, 0x00
    mov cl, 0x02
    mov dh, 0x00
    mov dl, [BOOT_DRIVE]

    int 0x13
    jnc kernel_loaded

    call reset_disk
    dec byte [retry_count]
    jnz load_kernel

disk_error:
    mov si, disk_err_msg
    call print_string
    jmp halt

kernel_loaded:
    mov si, ok_msg
    call print_string

    jmp KERNEL_SEG:KERNEL_OFFSET

reset_disk:
    pusha
    xor ah, ah
    mov dl, [BOOT_DRIVE]
    int 0x13
    popa
    ret

print_string:
    pusha
    mov ah, 0x0E
    mov bh, 0x00
    mov bl, 0x07

.next:
    lodsb
    cmp al, 0
    je .done
    int 0x10
    jmp .next

.done:
    popa
    ret

halt:
    cli
.hang:
    hlt
    jmp .hang

BOOT_DRIVE  db 0
retry_count db 0

boot_msg     db 'Ghost OS bootloader v0.9',13,10
             db 'Loading kernel...',13,10,0

ok_msg       db 'Kernel loaded.',13,10,0

disk_err_msg db 'Disk read error. System halted.',13,10,0

times 510-($-$$) db 0
dw 0xAA55
