; ============================================================
; GhostCrypto - Curve25519 / X25519
; v0.4 skeleton
; ============================================================

curve25519test:
    call curve25519_load_test_scalar

    mov esi, curve25519_test_msg
    call pm_print_string

    mov esi, curve25519_private_key
    mov ecx, 32
    call rtl8139_dump_bytes

    mov esi, curve25519_basepoint
    mov ecx, 32
    call rtl8139_dump_bytes

    mov esi, curve25519_private_key
    mov edi, curve25519_fe_a
    call curve25519_fe_copy

    mov esi, curve25519_fe_a
    mov ecx, 32
    call rtl8139_dump_bytes

    mov esi, curve25519_private_key
    mov edi, curve25519_private_key
    mov edx, curve25519_fe_out
    call curve25519_fe_mul

    mov esi, curve25519_fe_out
    mov ecx, 32
    call rtl8139_dump_bytes

    mov esi, curve25519_private_key
    mov edi, curve25519_private_key
    mov edx, curve25519_fe_out
    call curve25519_fe_add

    mov esi, curve25519_fe_out
    mov edi, curve25519_private_key
    mov edx, curve25519_fe_b
    call curve25519_fe_sub

    mov esi, curve25519_fe_b
    mov ecx, 32
    call rtl8139_dump_bytes

    mov esi, curve25519_fe_out
    mov ecx, 32
    call rtl8139_dump_bytes

    mov esi, curve25519_private_key
    mov edx, curve25519_fe_out
    call curve25519_fe_square

    mov esi, curve25519_fe_out
    mov ecx, 32
    call rtl8139_dump_bytes

    mov edi, curve25519_fe_out
    call curve25519_fe_zero

    mov esi, curve25519_fe_out
    mov ecx, 32
    call rtl8139_dump_bytes

    mov edi, curve25519_fe_out
    call curve25519_fe_one

    mov esi, curve25519_fe_out
    mov ecx, 32
    call rtl8139_dump_bytes

    mov al, 9
    mov edi, curve25519_fe_out
    call curve25519_fe_set_small

    mov esi, curve25519_fe_out
    mov ecx, 32
    call rtl8139_dump_bytes

    mov al, 9
    mov edi, curve25519_fe_a
    call curve25519_fe_set_small

    mov esi, curve25519_fe_a
    mov edx, curve25519_fe_out
    call curve25519_fe_square

    mov esi, curve25519_fe_out
    mov ecx, 32
    call rtl8139_dump_bytes

    mov al, 9
    mov edi, curve25519_fe_a
    call curve25519_fe_set_small

    mov esi, curve25519_fe_a
    mov edi, curve25519_fe_a
    mov edx, curve25519_fe_out
    call curve25519_fe_mul

    mov esi, curve25519_fe_out
    mov ecx, 32
    call rtl8139_dump_bytes

    ret

x25519test:
    call x25519_ladder_init

    mov esi, x25519_test_msg
    call pm_print_string

    mov esi, x25519_before_cswap_msg
    call pm_print_string
    mov esi, x25519_x2_msg
    call pm_print_string
    mov esi, x25519_x2
    mov ecx, 32
    call rtl8139_dump_bytes

    mov esi, x25519_x3_msg
    call pm_print_string
    mov esi, x25519_x3
    mov ecx, 32
    call rtl8139_dump_bytes

    mov esi, x25519_x2
    mov edi, x25519_x3
    mov al, 1
    call curve25519_fe_cswap

    mov esi, x25519_after_cswap_msg
    call pm_print_string
    mov esi, x25519_x2_msg
    call pm_print_string
    mov esi, x25519_x2
    mov ecx, 32
    call rtl8139_dump_bytes

    mov esi, x25519_x3_msg
    call pm_print_string
    mov esi, x25519_x3
    mov ecx, 32
    call rtl8139_dump_bytes

    mov esi, x25519_x2
    mov edi, x25519_x3
    mov al, 1
    call curve25519_fe_cswap

    mov esi, x25519_restored_cswap_msg
    call pm_print_string
    mov esi, x25519_x2_msg
    call pm_print_string
    mov esi, x25519_x2
    mov ecx, 32
    call rtl8139_dump_bytes

    mov esi, x25519_x3_msg
    call pm_print_string
    mov esi, x25519_x3
    mov ecx, 32
    call rtl8139_dump_bytes

    ret

curve25519_test_msg db 'GhostCrypto Curve25519 stub',13,0
x25519_test_msg db 'GhostCrypto X25519 ladder init',13,0
x25519_before_cswap_msg db 'before cswap',13,0
x25519_after_cswap_msg db 'after cswap',13,0
x25519_restored_cswap_msg db 'restored cswap',13,0
x25519_x1_msg db 'x1 = ',0
x25519_x2_msg db 'x2 = ',0
x25519_z2_msg db 'z2 = ',0
x25519_x3_msg db 'x3 = ',0
x25519_z3_msg db 'z3 = ',0

x25519_test_scalar:
    db 01h, 02h, 03h, 04h, 05h, 06h, 07h, 08h
    db 09h, 0Ah, 0Bh, 0Ch, 0Dh, 0Eh, 0Fh, 10h
    db 11h, 12h, 13h, 14h, 15h, 16h, 17h, 18h
    db 19h, 1Ah, 1Bh, 1Ch, 1Dh, 1Eh, 1Fh, 20h

curve25519_private_key times 32 db 0
curve25519_public_key  times 32 db 0
curve25519_fe_a times 32 db 0
curve25519_fe_b times 32 db 0
curve25519_fe_out times 32 db 0
curve25519_mul_temp times 64 dw 0
curve25519_mul_out_ptr dd 0
x25519_x1 times 32 db 0
x25519_x2 times 32 db 0
x25519_z2 times 32 db 0
x25519_x3 times 32 db 0
x25519_z3 times 32 db 0
x25519_tmp0 times 32 db 0
x25519_tmp1 times 32 db 0

curve25519_basepoint:
    db 9
    times 31 db 0

x25519_ladder_init:
    push esi
    push edi

    mov esi, curve25519_basepoint
    mov edi, x25519_x1
    call curve25519_fe_copy

    mov edi, x25519_x2
    call curve25519_fe_one

    mov edi, x25519_z2
    call curve25519_fe_zero

    mov esi, curve25519_basepoint
    mov edi, x25519_x3
    call curve25519_fe_copy

    mov edi, x25519_z3
    call curve25519_fe_one

    pop edi
    pop esi
    ret

curve25519_load_test_scalar:
    push esi
    push edi

    mov esi, x25519_test_scalar
    mov edi, curve25519_private_key
    call curve25519_fe_copy

    call curve25519_clamp_private

    pop edi
    pop esi
    ret

curve25519_clamp_private:
    ; clamp per X25519
    ; clear bits 0-2
    ; clear bit 255
    ; set bit 254

    and byte [curve25519_private_key], 248
    and byte [curve25519_private_key + 31], 127
    or  byte [curve25519_private_key + 31], 64

    ret

curve25519_fe_copy:
    ; ESI = src
    ; EDI = dst
    ; copia 32 byte

    push ecx
    mov ecx, 32

.copy:
    mov al, [esi]
    mov [edi], al

    inc esi
    inc edi

    dec ecx
    jnz .copy

    pop ecx
    ret

curve25519_fe_cswap:
    ; ESI = buffer A
    ; EDI = buffer B
    ; AL = swap flag 0/1

    push eax
    push ebx
    push ecx
    push edx
    push esi
    push edi

    test al, 1
    jz .done

    mov ecx, 32

.loop:
    mov dl, [esi]
    mov bl, [edi]
    mov [esi], bl
    mov [edi], dl

    inc esi
    inc edi

    dec ecx
    jnz .loop

.done:
    pop edi
    pop esi
    pop edx
    pop ecx
    pop ebx
    pop eax
    ret


curve25519_fe_add:
    ; ESI = a
    ; EDI = b
    ; EDX = out
    ; add with carry propagation

    push ecx
    push eax
    push ebx

    mov ecx, 32

    clc

.loop:
    mov al, [esi]
    mov bl, [edi]

    adc al, bl

    mov [edx], al

    inc esi
    inc edi
    inc edx

    dec ecx
    jnz .loop

    pop ebx
    pop eax
    pop ecx
    ret

curve25519_fe_sub:
    ; ESI = a
    ; EDI = b
    ; EDX = out
    ; subtraction with borrow propagation

    push ecx
    push eax
    push ebx

    mov ecx, 32

    clc

.loop:
    mov al, [esi]
    mov bl, [edi]

    sbb al, bl

    mov [edx], al

    inc esi
    inc edi
    inc edx

    dec ecx
    jnz .loop

    pop ebx
    pop eax
    pop ecx
    ret

curve25519_fe_mul:
    ; ESI = a
    ; EDI = b
    ; EDX = out
    ; early schoolbook multiplication with 16-bit temp slots
    ; NOT final Curve25519 reduction yet

    pushad
    mov [curve25519_mul_out_ptr], edx

    ; zero 64 words = 128 bytes
    mov ecx, 64
    mov ebp, curve25519_mul_temp

.zero:
    mov word [ebp], 0
    add ebp, 2
    dec ecx
    jnz .zero

    xor ebx, ebx

.outer:
    cmp ebx, 32
    jge .done

    xor ecx, ecx

.inner:
    cmp ecx, 32
    jge .next_outer

    mov al, [esi + ebx]
    mov dl, [edi + ecx]
    mul dl                 ; AX = AL * DL
    mov si, ax             ; save product

    mov ebp, curve25519_mul_temp
    mov eax, ebx
    add eax, ecx
    shl eax, 1             ; word index
    add ebp, eax

    mov ax, [ebp]
    add ax, si
    mov [ebp], ax

    inc ecx
    jmp .inner

.next_outer:
    inc ebx
    jmp .outer

.done:
    call curve25519_mul_fold_carry
    call curve25519_fe_reduce
    call curve25519_mul_fold_carry

    ; copy low byte of first 32 word slots to out
    mov esi, curve25519_mul_temp
    mov edi, [curve25519_mul_out_ptr]
    mov ecx, 32

.copy:
    mov ax, [esi]
    mov [edi], al

    add esi, 2
    inc edi

    dec ecx
    jnz .copy

    popad
    ret

curve25519_mul_fold_carry:
    ; folds byte carries across 64 word slots in curve25519_mul_temp

    push eax
    push ecx
    push edx
    push ebp

    mov ecx, 63
    mov ebp, curve25519_mul_temp

.fold:
    mov ax, [ebp]
    mov dx, ax
    and ax, 00FFh
    mov [ebp], ax

    shr dx, 8
    add [ebp + 2], dx

    add ebp, 2
    dec ecx
    jnz .fold

    pop ebp
    pop edx
    pop ecx
    pop eax
    ret

curve25519_fe_square:
    ; ESI = a
    ; EDX = out

    push edi

    mov edi, esi
    call curve25519_fe_mul

    pop edi
    ret

curve25519_fe_reduce:
    ; very early pseudo reduction
    ; folds temp[32..63] back into temp[0..31] * 19

    pushad

    mov ecx, 32
    mov esi, curve25519_mul_temp
    add esi, 64          ; upper word slots: temp[32]

    mov edi, curve25519_mul_temp

.loop:
    mov ax, [esi]
    mov bx, 19
    mul bx               ; DX:AX = upper word * 19

    add [edi], ax
    mov word [esi], 0

    add esi, 2
    add edi, 2

    dec ecx
    jnz .loop

    popad
    ret

curve25519_fe_zero:
    ; EDI = out
    push ecx
    push eax

    xor al, al
    mov ecx, 32

.loop:
    mov [edi], al
    inc edi
    dec ecx
    jnz .loop

    pop eax
    pop ecx
    ret


curve25519_fe_one:
    ; EDI = out
    call curve25519_fe_zero
    mov byte [edi - 32], 1
    ret


curve25519_fe_set_small:
    ; AL = small value
    ; EDI = out
    push eax
    call curve25519_fe_zero
    pop eax
    mov byte [edi - 32], al
    ret
