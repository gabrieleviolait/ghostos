; ============================================================
; GhostCrypto - Curve25519 / X25519
; v0.4 skeleton
; ============================================================

x25519_rfc_alice_msg db 'A ',0
x25519_rfc_bob_msg db 'B ',0
x25519_rfc_shared_msg db 'S ',0
x25519_rfc_final_msg db 'RFC ',0
x25519_rfc_pass_msg db 'PASS',13,0
x25519_rfc_fail_msg db 'FAIL',13,0
x25519_field_add_msg db 'Fa ',0
x25519_field_sub_msg db 'Fs ',0
x25519_field_mul_msg db 'Fm ',0
x25519_field_square_msg db 'Fq ',0
x25519_field_p_msg db 'Fp ',0
x25519_field_p1_msg db 'F1 ',0
x25519_inv_one_msg db 'Fi1 ',0
x25519_inv_nine_msg db 'Fi9 ',0
x25519_inv_x_msg db 'Fix ',0

; RFC7748 section 5.2 X25519 test vectors.
x25519_rfc7748_alice_private:
    db 077h, 007h, 06Dh, 00Ah, 073h, 018h, 0A5h, 07Dh
    db 03Ch, 016h, 0C1h, 072h, 051h, 0B2h, 066h, 045h
    db 0DFh, 04Ch, 02Fh, 087h, 0EBh, 0C0h, 099h, 02Ah
    db 0B1h, 077h, 0FBh, 0A5h, 01Dh, 0B9h, 02Ch, 02Ah

x25519_rfc7748_alice_public:
    db 085h, 020h, 0F0h, 009h, 089h, 030h, 0A7h, 054h
    db 074h, 08Bh, 07Dh, 0DCh, 0B4h, 03Eh, 0F7h, 05Ah
    db 00Dh, 0BFh, 03Ah, 00Dh, 026h, 038h, 01Ah, 0F4h
    db 0EBh, 0A4h, 0A9h, 08Eh, 0AAh, 09Bh, 04Eh, 06Ah

x25519_rfc7748_bob_private:
    db 05Dh, 0ABh, 008h, 07Eh, 062h, 04Ah, 08Ah, 04Bh
    db 079h, 0E1h, 07Fh, 08Bh, 083h, 080h, 00Eh, 0E6h
    db 06Fh, 03Bh, 0B1h, 029h, 026h, 018h, 0B6h, 0FDh
    db 01Ch, 02Fh, 08Bh, 027h, 0FFh, 088h, 0E0h, 0EBh

x25519_rfc7748_bob_public:
    db 0DEh, 09Eh, 0DBh, 07Dh, 07Bh, 07Dh, 0C1h, 0B4h
    db 0D3h, 05Bh, 061h, 0C2h, 0ECh, 0E4h, 035h, 037h
    db 03Fh, 083h, 043h, 0C8h, 05Bh, 078h, 067h, 04Dh
    db 0ADh, 0FCh, 07Eh, 014h, 06Fh, 088h, 02Bh, 04Fh

x25519_rfc7748_shared:
    db 04Ah, 05Dh, 09Dh, 05Bh, 0A4h, 0CEh, 02Dh, 0E1h
    db 072h, 08Eh, 03Bh, 0F4h, 080h, 035h, 0F2h, 05Eh
    db 007h, 0E2h, 01Ch, 094h, 07Dh, 019h, 0E3h, 037h
    db 06Fh, 009h, 0B3h, 0C1h, 0E1h, 061h, 074h, 02h

curve25519_private_key times 32 db 0
curve25519_public_key  times 32 db 0
x25519_peer_public_key times 32 db 0
x25519_shared_secret times 32 db 0
curve25519_fe_a times 32 db 0
curve25519_fe_b times 32 db 0
curve25519_fe_out times 32 db 0
curve25519_mul_temp times 32 dd 0
curve25519_mul_out_ptr dd 0
curve25519_mul_small_const times 32 db 0
x25519_x1 times 32 db 0
x25519_x2 times 32 db 0
x25519_z2 times 32 db 0
x25519_x3 times 32 db 0
x25519_z3 times 32 db 0
x25519_tmp0 times 32 db 0
x25519_tmp1 times 32 db 0
x25519_A times 32 db 0
x25519_B times 32 db 0
x25519_AA times 32 db 0
x25519_BB times 32 db 0
x25519_E times 32 db 0
x25519_C times 32 db 0
x25519_D times 32 db 0
x25519_DA times 32 db 0
x25519_CB times 32 db 0
x25519_tmp2 times 32 db 0
x25519_tmp3 times 32 db 0
x25519_swap_state db 0
x25519_rfc_fail_flag db 0
curve25519_inv_t0 times 32 db 0
curve25519_inv_t1 times 32 db 0
curve25519_inv_t2 times 32 db 0
curve25519_inv_t3 times 32 db 0
curve25519_inv_input_ptr dd 0
curve25519_inv_out_ptr dd 0

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

x25519_generate_public_skeleton:
    pushad

    call x25519_ladder_loop_skeleton

    mov esi, x25519_z2
    mov edi, curve25519_fe_out
    call curve25519_fe_invert

    mov esi, x25519_x2
    mov edi, curve25519_fe_out
    mov edx, curve25519_public_key
    call curve25519_fe_mul

    popad
    ret

x25519_generate_shared_secret_skeleton:
    pushad

    call x25519_ladder_init

    mov esi, x25519_peer_public_key
    mov edi, x25519_x1
    call curve25519_fe_copy

    mov esi, x25519_peer_public_key
    mov edi, x25519_x3
    call curve25519_fe_copy

    call x25519_ladder_loop_skeleton

    mov esi, x25519_z2
    mov edi, curve25519_fe_out
    call curve25519_fe_invert

    mov esi, x25519_x2
    mov edi, curve25519_fe_out
    mov edx, x25519_shared_secret
    call curve25519_fe_mul

    popad
    ret

x25519_rfc7748_validate:
    pushad

    call x25519_field_selftest
    call x25519_invert_selftest
    mov byte [x25519_rfc_fail_flag], 0

    mov esi, x25519_rfc7748_alice_private
    mov edi, curve25519_private_key
    call curve25519_fe_copy
    call curve25519_clamp_private
    call x25519_ladder_init
    call x25519_generate_public_skeleton
    mov esi, curve25519_public_key
    mov edi, x25519_rfc7748_alice_public
    call curve25519_memcmp32
    mov esi, x25519_rfc_alice_msg
    call x25519_rfc7748_print_result

    mov esi, x25519_rfc7748_bob_private
    mov edi, curve25519_private_key
    call curve25519_fe_copy
    call curve25519_clamp_private
    call x25519_ladder_init
    call x25519_generate_public_skeleton
    mov esi, curve25519_public_key
    mov edi, x25519_rfc7748_bob_public
    call curve25519_memcmp32
    mov esi, x25519_rfc_bob_msg
    call x25519_rfc7748_print_result

    mov esi, x25519_rfc7748_alice_private
    mov edi, curve25519_private_key
    call curve25519_fe_copy
    call curve25519_clamp_private
    mov esi, x25519_rfc7748_bob_public
    mov edi, x25519_peer_public_key
    call curve25519_fe_copy
    call x25519_generate_shared_secret_skeleton
    mov esi, x25519_shared_secret
    mov edi, x25519_rfc7748_shared
    call curve25519_memcmp32
    mov esi, x25519_rfc_shared_msg
    call x25519_rfc7748_print_result

    mov eax, 1
    cmp byte [x25519_rfc_fail_flag], 0
    je .print_final
    xor eax, eax

.print_final:
    mov esi, x25519_rfc_final_msg
    call x25519_rfc7748_print_result

    popad
    ret

x25519_rfc7748_print_result:
    push eax
    call pm_print_string
    pop eax
    cmp eax, 1
    je .pass
    mov byte [x25519_rfc_fail_flag], 1
    mov esi, x25519_rfc_fail_msg
    call pm_print_string
    ret

.pass:
    mov esi, x25519_rfc_pass_msg
    call pm_print_string
    ret

x25519_field_selftest:
    pushad

    mov al, 9
    mov edi, curve25519_fe_a
    call curve25519_fe_set_small
    mov al, 9
    mov edi, curve25519_fe_b
    call curve25519_fe_set_small

    mov esi, curve25519_fe_a
    mov edi, curve25519_fe_b
    mov edx, curve25519_fe_out
    call curve25519_fe_add
    mov esi, curve25519_fe_out
    mov al, 18
    call curve25519_fe_is_small
    mov esi, x25519_field_add_msg
    call x25519_rfc7748_print_result

    mov esi, curve25519_fe_out
    mov edi, curve25519_fe_b
    mov edx, curve25519_fe_out
    call curve25519_fe_sub
    mov esi, curve25519_fe_out
    mov al, 9
    call curve25519_fe_is_small
    mov esi, x25519_field_sub_msg
    call x25519_rfc7748_print_result

    mov esi, curve25519_fe_a
    mov edi, curve25519_fe_b
    mov edx, curve25519_fe_out
    call curve25519_fe_mul
    mov esi, curve25519_fe_out
    mov al, 81
    call curve25519_fe_is_small
    mov esi, x25519_field_mul_msg
    call x25519_rfc7748_print_result

    mov esi, curve25519_fe_a
    mov edx, curve25519_fe_out
    call curve25519_fe_square
    mov esi, curve25519_fe_out
    mov al, 81
    call curve25519_fe_is_small
    mov esi, x25519_field_square_msg
    call x25519_rfc7748_print_result

    mov al, 237
    call curve25519_prepare_p_temp
    call curve25519_fe_normalize
    mov edi, curve25519_fe_out
    call curve25519_mul_temp_copy_low32
    mov esi, curve25519_fe_out
    xor al, al
    call curve25519_fe_is_small
    mov esi, x25519_field_p_msg
    call x25519_rfc7748_print_result

    mov al, 238
    call curve25519_prepare_p_temp
    call curve25519_fe_normalize
    mov edi, curve25519_fe_out
    call curve25519_mul_temp_copy_low32
    mov esi, curve25519_fe_out
    mov al, 1
    call curve25519_fe_is_small
    mov esi, x25519_field_p1_msg
    call x25519_rfc7748_print_result

    popad
    ret

x25519_invert_selftest:
    pushad

    mov al, 1
    mov edi, curve25519_fe_a
    call curve25519_fe_set_small
    mov esi, curve25519_fe_a
    mov edi, curve25519_fe_b
    call curve25519_fe_invert
    mov esi, curve25519_fe_b
    mov al, 1
    call curve25519_fe_is_small
    mov esi, x25519_inv_one_msg
    call x25519_rfc7748_print_result

    mov al, 9
    mov edi, curve25519_fe_a
    call curve25519_fe_set_small
    call x25519_invert_product_is_one
    mov esi, x25519_inv_nine_msg
    call x25519_rfc7748_print_result

    mov edi, curve25519_fe_a
    call curve25519_fe_zero
    mov dword [curve25519_fe_a], 123456789
    call x25519_invert_product_is_one
    mov esi, x25519_inv_x_msg
    call x25519_rfc7748_print_result

    popad
    ret

x25519_invert_product_is_one:
    mov esi, curve25519_fe_a
    mov edi, curve25519_fe_b
    call curve25519_fe_invert
    mov esi, curve25519_fe_a
    mov edi, curve25519_fe_b
    mov edx, curve25519_fe_out
    call curve25519_fe_mul
    mov esi, curve25519_fe_out
    mov al, 1
    call curve25519_fe_is_small
    ret

x25519_ladder_loop_skeleton:
    pushad

    mov byte [x25519_swap_state], 0

    mov ebx, 254

.loop:
    mov eax, ebx
    shr eax, 3
    mov dl, [curve25519_private_key + eax]

    mov ecx, ebx
    and ecx, 7
    shr dl, cl
    and dl, 1

    mov al, [x25519_swap_state]
    xor al, dl
    and al, 1

    mov esi, x25519_x2
    mov edi, x25519_x3
    call curve25519_fe_cswap

    mov esi, x25519_z2
    mov edi, x25519_z3
    call curve25519_fe_cswap

    mov [x25519_swap_state], dl

    call x25519_ladder_step_once

    cmp ebx, 0
    je .final_cswap
    dec ebx
    jmp .loop

.final_cswap:
    mov al, [x25519_swap_state]

    mov esi, x25519_x2
    mov edi, x25519_x3
    call curve25519_fe_cswap

    mov esi, x25519_z2
    mov edi, x25519_z3
    call curve25519_fe_cswap

    mov byte [x25519_swap_state], 0

    popad
    ret

x25519_ladder_step_once:
    pushad

    ; A = x2 + z2
    mov esi, x25519_x2
    mov edi, x25519_z2
    mov edx, x25519_A
    call curve25519_fe_add

    ; AA = A^2
    mov esi, x25519_A
    mov edx, x25519_AA
    call curve25519_fe_square

    ; B = x2 - z2
    mov esi, x25519_x2
    mov edi, x25519_z2
    mov edx, x25519_B
    call curve25519_fe_sub

    ; BB = B^2
    mov esi, x25519_B
    mov edx, x25519_BB
    call curve25519_fe_square

    ; E = AA - BB
    mov esi, x25519_AA
    mov edi, x25519_BB
    mov edx, x25519_E
    call curve25519_fe_sub

    ; C = x3 + z3
    mov esi, x25519_x3
    mov edi, x25519_z3
    mov edx, x25519_C
    call curve25519_fe_add

    ; D = x3 - z3
    mov esi, x25519_x3
    mov edi, x25519_z3
    mov edx, x25519_D
    call curve25519_fe_sub

    ; DA = D * A
    mov esi, x25519_D
    mov edi, x25519_A
    mov edx, x25519_DA
    call curve25519_fe_mul

    ; CB = C * B
    mov esi, x25519_C
    mov edi, x25519_B
    mov edx, x25519_CB
    call curve25519_fe_mul

    ; x3 = (DA + CB)^2
    mov esi, x25519_DA
    mov edi, x25519_CB
    mov edx, x25519_tmp0
    call curve25519_fe_add

    mov esi, x25519_tmp0
    mov edx, x25519_x3
    call curve25519_fe_square

    ; z3 = x1 * (DA - CB)^2
    mov esi, x25519_DA
    mov edi, x25519_CB
    mov edx, x25519_tmp0
    call curve25519_fe_sub

    mov esi, x25519_tmp0
    mov edx, x25519_tmp1
    call curve25519_fe_square

    mov esi, x25519_x1
    mov edi, x25519_tmp1
    mov edx, x25519_z3
    call curve25519_fe_mul

    ; x2 = AA * BB
    mov esi, x25519_AA
    mov edi, x25519_BB
    mov edx, x25519_x2
    call curve25519_fe_mul

    ; z2 = E * (AA + 121665 * E)
    mov esi, x25519_E
    mov eax, 121665
    mov edx, x25519_tmp2
    call curve25519_fe_mul_small

    mov esi, x25519_AA
    mov edi, x25519_tmp2
    mov edx, x25519_tmp3
    call curve25519_fe_add

    mov esi, x25519_E
    mov edi, x25519_tmp3
    mov edx, x25519_z2
    call curve25519_fe_mul

    popad
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

curve25519_memcmp32:
    ; ESI = buffer A
    ; EDI = buffer B
    ; EAX = 1 if equal, 0 if different

    push ecx
    push esi
    push edi

    mov eax, 1
    mov ecx, 32

.loop:
    mov dl, [esi]
    cmp dl, [edi]
    jne .mismatch
    inc esi
    inc edi
    dec ecx
    jnz .loop
    jmp .done

.mismatch:
    xor eax, eax

.done:
    pop edi
    pop esi
    pop ecx
    ret

curve25519_fe_is_small:
    ; ESI = field element, AL = expected low byte, EAX = match flag

    mov bl, al
    xor eax, eax
    cmp [esi], bl
    jne .done
    inc esi
    mov ecx, 31

.loop:
    cmp byte [esi], 0
    jne .done
    inc esi
    dec ecx
    jnz .loop
    mov eax, 1

.done:
    ret

curve25519_prepare_p_temp:
    ; AL = low byte. 237 for p, 238 for p+1.

    mov edi, curve25519_mul_temp
    movzx eax, al
    mov [edi], eax
    add edi, 4
    mov ecx, 30
    mov eax, 255

.mid:
    mov [edi], eax
    add edi, 4
    dec ecx
    jnz .mid

    mov eax, 127
    mov [edi], eax
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

curve25519_fe_invert:
    ; ESI = input field element
    ; EDI = output field element
    ; Computes z^(2^255 - 21), i.e. z^(p-2) for p = 2^255 - 19.

    pushad

    mov [curve25519_inv_input_ptr], esi
    mov [curve25519_inv_out_ptr], edi

    ; t0 = z^2
    mov edx, curve25519_inv_t0
    call curve25519_fe_square

    ; t1 = z^8
    mov esi, curve25519_inv_t0
    mov edx, curve25519_inv_t1
    call curve25519_fe_square
    mov esi, curve25519_inv_t1
    mov edx, curve25519_inv_t1
    call curve25519_fe_square

    ; t1 = z^9
    mov esi, [curve25519_inv_input_ptr]
    mov edi, curve25519_inv_t1
    mov edx, curve25519_inv_t1
    call curve25519_fe_mul

    ; t2 = z^11, keep a copy in t3 for the final multiply
    mov esi, curve25519_inv_t1
    mov edi, curve25519_inv_t0
    mov edx, curve25519_inv_t2
    call curve25519_fe_mul
    mov esi, curve25519_inv_t2
    mov edi, curve25519_inv_t3
    call curve25519_fe_copy

    ; t0 = z^(2^5 - 1)
    mov esi, curve25519_inv_t2
    mov edx, curve25519_inv_t2
    call curve25519_fe_square
    mov esi, curve25519_inv_t2
    mov edi, curve25519_inv_t1
    mov edx, curve25519_inv_t0
    call curve25519_fe_mul

    ; t0 = z^(2^10 - 1)
    mov esi, curve25519_inv_t0
    mov ecx, 5
    mov edi, curve25519_inv_t2
    call curve25519_fe_square_n
    mov esi, curve25519_inv_t2
    mov edi, curve25519_inv_t0
    mov edx, curve25519_inv_t0
    call curve25519_fe_mul

    ; t1 = z^(2^20 - 1)
    mov esi, curve25519_inv_t0
    mov ecx, 10
    mov edi, curve25519_inv_t2
    call curve25519_fe_square_n
    mov esi, curve25519_inv_t2
    mov edi, curve25519_inv_t0
    mov edx, curve25519_inv_t1
    call curve25519_fe_mul

    ; t1 = z^(2^40 - 1)
    mov esi, curve25519_inv_t1
    mov ecx, 20
    mov edi, curve25519_inv_t2
    call curve25519_fe_square_n
    mov esi, curve25519_inv_t2
    mov edi, curve25519_inv_t1
    mov edx, curve25519_inv_t1
    call curve25519_fe_mul

    ; t0 = z^(2^50 - 1)
    mov esi, curve25519_inv_t1
    mov ecx, 10
    mov edi, curve25519_inv_t2
    call curve25519_fe_square_n
    mov esi, curve25519_inv_t2
    mov edi, curve25519_inv_t0
    mov edx, curve25519_inv_t0
    call curve25519_fe_mul

    ; t1 = z^(2^100 - 1)
    mov esi, curve25519_inv_t0
    mov ecx, 50
    mov edi, curve25519_inv_t2
    call curve25519_fe_square_n
    mov esi, curve25519_inv_t2
    mov edi, curve25519_inv_t0
    mov edx, curve25519_inv_t1
    call curve25519_fe_mul

    ; t1 = z^(2^200 - 1)
    mov esi, curve25519_inv_t1
    mov ecx, 100
    mov edi, curve25519_inv_t2
    call curve25519_fe_square_n
    mov esi, curve25519_inv_t2
    mov edi, curve25519_inv_t1
    mov edx, curve25519_inv_t1
    call curve25519_fe_mul

    ; t0 = z^(2^250 - 1)
    mov esi, curve25519_inv_t1
    mov ecx, 50
    mov edi, curve25519_inv_t2
    call curve25519_fe_square_n
    mov esi, curve25519_inv_t2
    mov edi, curve25519_inv_t0
    mov edx, curve25519_inv_t0
    call curve25519_fe_mul

    ; out = z^(2^255 - 21)
    mov esi, curve25519_inv_t0
    mov ecx, 5
    mov edi, curve25519_inv_t2
    call curve25519_fe_square_n
    mov esi, curve25519_inv_t2
    mov edi, curve25519_inv_t3
    mov edx, [curve25519_inv_out_ptr]
    call curve25519_fe_mul

    popad
    ret

curve25519_fe_square_n:
    ; ESI = input, ECX = number of squares, EDI = output

    pushad

    call curve25519_fe_copy
    sub edi, 32

.loop:
    cmp ecx, 0
    je .done
    mov esi, edi
    mov edx, edi
    call curve25519_fe_square
    dec ecx
    jmp .loop

.done:
    popad
    ret

curve25519_fe_mul_small:
    ; ESI = field element
    ; EAX = small constant
    ; EDX = out

    pushad

    mov edi, curve25519_mul_small_const
    call curve25519_fe_zero

    mov edi, curve25519_mul_small_const
    mov [edi], al
    shr eax, 8
    mov [edi + 1], al
    shr eax, 8
    mov [edi + 2], al
    shr eax, 8
    mov [edi + 3], al

    mov edi, curve25519_mul_small_const
    call curve25519_fe_mul

    popad
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
    ; schoolbook multiplication with high coefficients folded into 32 dword slots

    pushad
    mov [curve25519_mul_out_ptr], edx

    ; zero 32 dwords = 128 bytes
    mov ecx, 32
    mov ebp, curve25519_mul_temp

.zero:
    mov dword [ebp], 0
    add ebp, 4
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
    movzx edx, ax          ; save product

    mov eax, ebx
    add eax, ecx
    cmp eax, 32
    jb .add_product
    sub eax, 32
    imul edx, 38           ; 2^256 == 38 mod (2^255 - 19)

.add_product:
    mov ebp, curve25519_mul_temp
    shl eax, 2             ; dword index
    add ebp, eax

    add [ebp], edx

    inc ecx
    jmp .inner

.next_outer:
    inc ebx
    jmp .outer

.done:
    call curve25519_mul_fold_carry
    mov edi, curve25519_fe_b
    call curve25519_mul_temp_copy_low32
    call curve25519_fe_reduce_real
    call curve25519_fe_normalize

    ; copy low byte of first 32 dword slots to out
    mov esi, curve25519_mul_temp
    mov edi, [curve25519_mul_out_ptr]
    mov ecx, 32

.copy:
    mov eax, [esi]
    mov [edi], al

    add esi, 4
    inc edi

    dec ecx
    jnz .copy

    popad
    ret

curve25519_mul_fold_carry:
    ; folds byte carries across 32 dword slots in curve25519_mul_temp

    push eax
    push ecx
    push edx
    push ebp

    mov ecx, 31
    mov ebp, curve25519_mul_temp

.fold:
    mov eax, [ebp]
    mov edx, eax
    and eax, 000000FFh
    mov [ebp], eax

    shr edx, 8
    add [ebp + 4], edx

    add ebp, 4
    dec ecx
    jnz .fold

    pop ebp
    pop edx
    pop ecx
    pop eax
    ret

curve25519_mul_temp_copy_low32:
    ; EDI = 32-byte output buffer

    push eax
    push ecx
    push esi
    push edi

    mov esi, curve25519_mul_temp
    mov ecx, 32

.copy:
    mov eax, [esi]
    mov [edi], al
    add esi, 4
    inc edi
    dec ecx
    jnz .copy

    pop edi
    pop esi
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

curve25519_fe_reduce_real:
    ; p = 2^255 - 19, so 2^255 == 19 mod p.
    ; High byte coefficients were folded during accumulation as 2^256 == 38.
    ; This pass folds the remaining bit 255 from byte 31 into byte 0 as *19.

    pushad

    mov ebp, curve25519_mul_temp + 31*4
    mov eax, [ebp]
    mov edx, eax
    and eax, 0000007Fh
    mov [ebp], eax
    shr edx, 7
    imul edx, 19
    add [curve25519_mul_temp], edx

    popad
    ret

curve25519_fe_normalize:
    ; Normalize low 255 bits in curve25519_mul_temp.

    pushad

    call .propagate_0_to_30
    call .fold_bit255
    call .propagate_0_to_30
    call .fold_bit255
    call .propagate_0_to_30
    call .conditional_sub_p

    popad
    ret

.propagate_0_to_30:
    push eax
    push ecx
    push edx
    push ebp

    mov ecx, 31
    mov ebp, curve25519_mul_temp

.prop_loop:
    mov eax, [ebp]
    mov edx, eax
    and eax, 000000FFh
    mov [ebp], eax
    shr edx, 8
    add [ebp + 4], edx
    add ebp, 4
    dec ecx
    jnz .prop_loop

    pop ebp
    pop edx
    pop ecx
    pop eax
    ret

.fold_bit255:
    push eax
    push edx
    push ebp

    mov ebp, curve25519_mul_temp + 31*4
    mov eax, [ebp]
    mov edx, eax
    and eax, 0000007Fh
    mov [ebp], eax
    shr edx, 7
    imul edx, 19
    add [curve25519_mul_temp], edx

    pop ebp
    pop edx
    pop eax
    ret

.conditional_sub_p:
    push eax
    push ebx
    push ecx
    push edx
    push ebp

    mov ebp, curve25519_mul_temp + 31*4
    mov ecx, 31

.cmp_loop:
    mov eax, [ebp]
    mov edx, 255
    cmp ecx, 31
    jne .cmp_not_top
    mov edx, 127

.cmp_not_top:
    cmp ecx, 0
    jne .cmp_slot
    mov edx, 237

.cmp_slot:
    cmp eax, edx
    ja .do_subtract
    jb .done_conditional

    sub ebp, 4
    dec ecx
    jns .cmp_loop
    jmp .do_subtract

.do_subtract:
    xor ebx, ebx
    mov ebp, curve25519_mul_temp

    mov eax, [ebp]
    sub eax, 237
    call .finish_sub_limb
    mov [ebp], eax
    add ebp, 4

    mov ecx, 30

.sub_mid:
    mov eax, [ebp]
    sub eax, 255
    sub eax, ebx
    call .finish_sub_limb
    mov [ebp], eax
    add ebp, 4
    dec ecx
    jnz .sub_mid

    mov eax, [ebp]
    sub eax, 127
    sub eax, ebx
    call .finish_sub_limb
    mov [ebp], eax

.done_conditional:
    pop ebp
    pop edx
    pop ecx
    pop ebx
    pop eax
    ret

.finish_sub_limb:
    xor ebx, ebx
    test eax, 80000000h
    jz .finish_no_borrow
    add eax, 256
    mov ebx, 1

.finish_no_borrow:
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
