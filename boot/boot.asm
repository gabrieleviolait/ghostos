; Ghost OS v1.0-beta bootloader
; FAT12-compatible boot sector.
; Relocates itself above the kernel load area, then loads kernel sectors.

[BITS 16]
[ORG 0x7C00]

    jmp short relocate_bootloader
    nop

OEMLabel            db 'GHOSTOS '
BytesPerSector      dw 512
SectorsPerCluster   db 1
ReservedSectors     dw 65
NumberOfFATs        db 2
RootEntries         dw 224
TotalSectors16      dw 2880
MediaDescriptor     db 0xF0
SectorsPerFAT       dw 9
SectorsPerTrack     dw 18
NumberOfHeads       dw 2
HiddenSectors       dd 0
TotalSectors32      dd 0
DriveNumber         db 0
ReservedBootByte    db 0
ExtendedBootSig     db 0x29
VolumeID            dd 0x20260525
VolumeLabel         db 'GHOST OS   '
FileSystemType      db 'FAT12   '

RELOC_SEG              equ 0x0140      ; 0x0140:0x7C00 = physical 0x9000

KERNEL_SEG             equ 0x0000
KERNEL_OFFSET          equ 0x1000
KERNEL_LBA             equ 1
KERNEL_SECTORS         equ 64

SECTORS_PER_TRACK      equ 18
HEADS                  equ 2
SECTORS_PER_CYLINDER   equ SECTORS_PER_TRACK * HEADS
MAX_RETRIES            equ 3

relocate_bootloader:
    cli
    cld

    mov [BOOT_DRIVE], dl

    xor ax, ax
    mov ds, ax
    mov si, 0x7C00

    mov ax, RELOC_SEG
    mov es, ax
    mov di, 0x7C00

    mov cx, 256
    rep movsw

    jmp RELOC_SEG:start_relocated

start_relocated:
    mov ax, cs
    mov ds, ax

    xor ax, ax
    mov ss, ax
    mov sp, 0xF000

    sti

    mov si, boot_msg
    call print_string

    mov ax, KERNEL_SEG
    mov es, ax

    mov word [kernel_dest], KERNEL_OFFSET
    mov word [current_lba], KERNEL_LBA
    mov word [sectors_left], KERNEL_SECTORS

load_kernel:
    cmp word [sectors_left], 0
    je kernel_loaded

    mov byte [retry_count], MAX_RETRIES

read_sector_retry:
    call reset_disk
    call read_kernel_sector
    jnc sector_loaded

    call reset_disk
    dec byte [retry_count]
    jnz read_sector_retry

disk_error:
    mov si, disk_err_msg
    call print_string
    jmp halt

sector_loaded:
    add word [kernel_dest], 512
    inc word [current_lba]
    dec word [sectors_left]
    jmp load_kernel

kernel_loaded:
    mov si, ok_msg
    call print_string
    jmp KERNEL_SEG:KERNEL_OFFSET

read_kernel_sector:
    pusha

    mov ax, KERNEL_SEG
    mov es, ax

    mov ax, [current_lba]
    xor dx, dx
    mov bx, SECTORS_PER_CYLINDER
    div bx
    mov ch, al

    mov ax, dx
    xor dx, dx
    mov bx, SECTORS_PER_TRACK
    div bx
    mov dh, al
    mov cl, dl
    inc cl

    mov bx, [kernel_dest]
    mov ah, 0x02
    mov al, 1
    mov dl, [BOOT_DRIVE]
    int 0x13
    jc .fail

    popa
    clc
    ret

.fail:
    popa
    stc
    ret

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
    mov bh, 0
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

BOOT_DRIVE   db 0
retry_count  db 0
sectors_left dw 0
current_lba  dw 0
kernel_dest  dw 0

boot_msg db 'Ghost OS bootloader v1.0-beta',13,10
         db 'Loading kernel...',13,10,0

ok_msg db 'Kernel loaded.',13,10,0

disk_err_msg db 'Disk read error. System halted.',13,10,0

times 510-($-$$) db 0
dw 0xAA55
