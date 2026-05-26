; ============================================================
; GhostCrypto - Curve25519 / X25519
; v0.4 skeleton
; ============================================================

curve25519test:
    call rng_next32
    mov [curve25519_private_key], eax

    call rng_next32
    mov [curve25519_private_key + 4], eax

    call rng_next32
    mov [curve25519_private_key + 8], eax

    call rng_next32
    mov [curve25519_private_key + 12], eax

    call rng_next32
    mov [curve25519_private_key + 16], eax

    call rng_next32
    mov [curve25519_private_key + 20], eax

    call rng_next32
    mov [curve25519_private_key + 24], eax

    call rng_next32
    mov [curve25519_private_key + 28], eax

    call curve25519_clamp_private

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

    ret

curve25519_test_msg db 'GhostCrypto Curve25519 stub',13,0

curve25519_private_key times 32 db 0
curve25519_public_key  times 32 db 0
curve25519_fe_a times 32 db 0
curve25519_fe_b times 32 db 0
curve25519_fe_out times 32 db 0
curve25519_mul_temp times 64 dw 0
curve25519_mul_out_ptr dd 0

curve25519_basepoint:
    db 9
    times 31 db 0

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
    ; fold carries across 64 word slots
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

    call curve25519_fe_reduce

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
    ; folds upper half back into lower half * 19

    pushad

    mov ecx, 32
    mov esi, curve25519_mul_temp
    add esi, 64          ; upper half

    mov edi, curve25519_mul_temp

.loop:
    mov al, [esi]

    mov ah, 19
    mul ah               ; AX = upper * 19

    add [edi], al

    inc esi
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