; ============================================================
; GhostCrypto - Curve25519 / X25519
; v0.4 skeleton
; ============================================================

x25519_rfc_alice_msg db 'A',0
x25519_rfc_bob_msg db 'B',0
x25519_fg1_msg db 'FG1',0
x25519_fg2_msg db 'FG2',0
x25519_fg0_msg db 'FG0',0
x25519_fgq_msg db 'FGQ',0
x25519_u5d_msg db 'U5D',0
x25519_ubd_msg db 'UBD',0
x25519_fp_msg db 'FP',0
x25519_f1p_msg db 'F1P',0
x25519_b9_msg db 'B9',0
x25519_fia_msg db 'FIA',0
x25519_fib_msg db 'FIB',0
x25519_iza_msg db 'IZA',0
x25519_x2a_msg db 'X2A ',0
x25519_z2a_msg db 'Z2A ',0
x25519_iva_msg db 'IVA ',0
x25519_u1x_msg db 'U1X',0
x25519_u1z_msg db 'U1Z',0
x25519_csb_msg db 'CSB',0
x25519_u2d_msg db 'U2D',0
x25519_u4d_msg db 'U4D',0
x25519_u8d_msg db 'U8D',0
x25519_u16d_msg db 'U16D',0
x25519_rfc_shared_msg db 'S',0
x25519_rfc_shared_abba_msg db 'ABBA',0
x25519_scalar_a_msg db 'SA',0
x25519_scalar_b_msg db 'SB',0
x25519_rfc_shared_ab_msg db 'AB',0
x25519_rfc_shared_ba_msg db 'BA',0
x25519_abc_msg db 'ABC ',0
x25519_exp_msg db 'EXP ',0
x25519_diff_msg db 'DIF ',0
x25519_abn_msg db 'ABN',0
x25519_shared_init_msg db 'I',0
x25519_x1b_msg db 'X1B',0
x25519_rfc_final_msg db 'R',0
x25519_rfc_pass_msg db 'PASS',13,0
x25519_rfc_fail_msg db 'FAIL',13,0
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
    db 072h, 08Eh, 03Bh, 0F4h, 080h, 035h, 00Fh, 025h
    db 0E0h, 07Eh, 021h, 0C9h, 047h, 0D1h, 09Eh, 033h
    db 076h, 0F0h, 09Bh, 03Ch, 01Eh, 016h, 017h, 042h

curve25519_private_key times 32 db 0
curve25519_public_key  times 32 db 0
x25519_peer_public_key times 32 db 0
x25519_shared_secret times 32 db 0
x25519_shared_ab times 32 db 0
x25519_shared_ba times 32 db 0
x25519_u5_run1 times 32 db 0
x25519_u5_run2 times 32 db 0
x25519_ub_run1 times 32 db 0
x25519_ub_run2 times 32 db 0
x25519_u1_expected_x times 32 db 0
x25519_u1_expected_z times 32 db 0
x25519_u2_x_run1 times 32 db 0
x25519_u2_z_run1 times 32 db 0
x25519_u2_x_run2 times 32 db 0
x25519_u2_z_run2 times 32 db 0
x25519_u4_x_run1 times 32 db 0
x25519_u4_z_run1 times 32 db 0
x25519_u4_x_run2 times 32 db 0
x25519_u4_z_run2 times 32 db 0
x25519_u8_x_run1 times 32 db 0
x25519_u8_z_run1 times 32 db 0
x25519_u8_x_run2 times 32 db 0
x25519_u8_z_run2 times 32 db 0
x25519_u16_x_run1 times 32 db 0
x25519_u16_z_run1 times 32 db 0
x25519_u16_x_run2 times 32 db 0
x25519_u16_z_run2 times 32 db 0
x25519_ab_norm times 32 db 0
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

x25519_rfc7748_validate_min:
    pushad

    mov byte [x25519_rfc_fail_flag], 0

    ; A: Alice private -> Alice public
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

    ; B: Bob private -> Bob public
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

    ; AB: Alice private * Bob public -> shared
    mov esi, x25519_rfc7748_alice_private
    mov edi, curve25519_private_key
    call curve25519_fe_copy
    call curve25519_clamp_private
    mov esi, x25519_rfc7748_bob_public
    mov edi, x25519_peer_public_key
    call curve25519_fe_copy
    call x25519_generate_shared_secret_skeleton
    mov esi, x25519_shared_secret
    mov edi, x25519_shared_ab
    call curve25519_fe_copy
    mov esi, x25519_shared_ab
    mov edi, x25519_rfc7748_shared
    call curve25519_memcmp32
    mov esi, x25519_rfc_shared_ab_msg
    call x25519_rfc7748_print_result

    ; BA: Bob private * Alice public -> shared
    mov esi, x25519_rfc7748_bob_private
    mov edi, curve25519_private_key
    call curve25519_fe_copy
    call curve25519_clamp_private
    mov esi, x25519_rfc7748_alice_public
    mov edi, x25519_peer_public_key
    call curve25519_fe_copy
    call x25519_generate_shared_secret_skeleton
    mov esi, x25519_shared_secret
    mov edi, x25519_shared_ba
    call curve25519_fe_copy
    mov esi, x25519_shared_ba
    mov edi, x25519_rfc7748_shared
    call curve25519_memcmp32
    mov esi, x25519_rfc_shared_ba_msg
    call x25519_rfc7748_print_result

    ; S: AB == BA and AB == expected
    mov esi, x25519_shared_ab
    mov edi, x25519_shared_ba
    call curve25519_memcmp32
    cmp eax, 1
    jne .print_s
    mov esi, x25519_shared_ab
    mov edi, x25519_rfc7748_shared
    call curve25519_memcmp32

.print_s:
    mov esi, x25519_rfc_shared_msg
    call x25519_rfc7748_print_result

    ; R: final aggregate result
    mov eax, 1
    cmp byte [x25519_rfc_fail_flag], 0
    je .print_r
    xor eax, eax

.print_r:
    mov esi, x25519_rfc_final_msg
    call x25519_rfc7748_print_result

    popad
    ret

x25519_rfc7748_validate:
    pushad

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

    ; FG1: AlicePublic * 1 == AlicePublic
    mov edi, curve25519_fe_a
    call curve25519_fe_one
    mov esi, x25519_rfc7748_alice_public
    mov edi, curve25519_fe_a
    mov edx, x25519_tmp0
    call curve25519_fe_mul
    mov esi, x25519_tmp0
    mov edi, x25519_rfc7748_alice_public
    call curve25519_memcmp32
    mov esi, x25519_fg1_msg
    call x25519_rfc7748_print_result

    ; FG2: BobPublic * 1 == BobPublic
    mov edi, curve25519_fe_a
    call curve25519_fe_one
    mov esi, x25519_rfc7748_bob_public
    mov edi, curve25519_fe_a
    mov edx, x25519_tmp0
    call curve25519_fe_mul
    mov esi, x25519_tmp0
    mov edi, x25519_rfc7748_bob_public
    call curve25519_memcmp32
    mov esi, x25519_fg2_msg
    call x25519_rfc7748_print_result

    ; FG0: AlicePublic * 0 == 0
    mov edi, curve25519_fe_a
    call curve25519_fe_zero
    mov esi, x25519_rfc7748_alice_public
    mov edi, curve25519_fe_a
    mov edx, x25519_tmp0
    call curve25519_fe_mul
    mov esi, x25519_tmp0
    mov edi, curve25519_fe_a
    call curve25519_memcmp32
    mov esi, x25519_fg0_msg
    call x25519_rfc7748_print_result

    ; FGQ: square(AlicePublic) == mul(AlicePublic,AlicePublic)
    mov esi, x25519_rfc7748_alice_public
    mov edx, x25519_tmp0
    call curve25519_fe_square

    mov esi, x25519_rfc7748_alice_public
    mov edi, x25519_rfc7748_alice_public
    mov edx, x25519_tmp1
    call curve25519_fe_mul

    mov esi, x25519_tmp0
    mov edi, x25519_tmp1
    call curve25519_memcmp32
    mov esi, x25519_fgq_msg
    call x25519_rfc7748_print_result

    ; U5D: Alice scalar * u=5 twice must be deterministic
    mov esi, x25519_rfc7748_alice_private
    mov edi, curve25519_private_key
    call curve25519_fe_copy
    call curve25519_clamp_private

    mov edi, x25519_peer_public_key
    call curve25519_fe_zero
    mov byte [x25519_peer_public_key], 5

    call x25519_generate_shared_secret_skeleton
    mov esi, x25519_shared_secret
    mov edi, x25519_u5_run1
    call curve25519_fe_copy

    mov esi, x25519_rfc7748_alice_private
    mov edi, curve25519_private_key
    call curve25519_fe_copy
    call curve25519_clamp_private

    mov edi, x25519_peer_public_key
    call curve25519_fe_zero
    mov byte [x25519_peer_public_key], 5

    call x25519_generate_shared_secret_skeleton
    mov esi, x25519_shared_secret
    mov edi, x25519_u5_run2
    call curve25519_fe_copy

    mov esi, x25519_u5_run1
    mov edi, x25519_u5_run2
    call curve25519_memcmp32
    mov esi, x25519_u5d_msg
    call x25519_rfc7748_print_result

    ; UBD: Alice scalar * BobPublic twice must be deterministic
    mov esi, x25519_rfc7748_alice_private
    mov edi, curve25519_private_key
    call curve25519_fe_copy
    call curve25519_clamp_private

    mov esi, x25519_rfc7748_bob_public
    mov edi, x25519_peer_public_key
    call curve25519_fe_copy

    call x25519_generate_shared_secret_skeleton
    mov esi, x25519_shared_secret
    mov edi, x25519_ub_run1
    call curve25519_fe_copy

    mov esi, x25519_rfc7748_alice_private
    mov edi, curve25519_private_key
    call curve25519_fe_copy
    call curve25519_clamp_private

    mov esi, x25519_rfc7748_bob_public
    mov edi, x25519_peer_public_key
    call curve25519_fe_copy

    call x25519_generate_shared_secret_skeleton
    mov esi, x25519_shared_secret
    mov edi, x25519_ub_run2
    call curve25519_fe_copy

    mov esi, x25519_ub_run1
    mov edi, x25519_ub_run2
    call curve25519_memcmp32
    mov esi, x25519_ubd_msg
    call x25519_rfc7748_print_result

    ; FP: p * 1 == 0
    mov al, 237
    call curve25519_prepare_p_temp
    mov edi, x25519_tmp0
    call curve25519_mul_temp_copy_low32

    mov edi, curve25519_fe_a
    call curve25519_fe_one

    mov esi, x25519_tmp0
    mov edi, curve25519_fe_a
    mov edx, x25519_tmp1
    call curve25519_fe_mul

    mov esi, x25519_tmp1
    xor al, al
    call curve25519_fe_is_small
    mov esi, x25519_fp_msg
    call x25519_rfc7748_print_result

    ; F1P: (p + 1) * 1 == 1
    mov al, 238
    call curve25519_prepare_p_temp
    mov edi, x25519_tmp0
    call curve25519_mul_temp_copy_low32

    mov edi, curve25519_fe_a
    call curve25519_fe_one

    mov esi, x25519_tmp0
    mov edi, curve25519_fe_a
    mov edx, x25519_tmp1
    call curve25519_fe_mul

    mov esi, x25519_tmp1
    mov al, 1
    call curve25519_fe_is_small
    mov esi, x25519_f1p_msg
    call x25519_rfc7748_print_result

    ; B9: Alice scalar * basepoint via shared path == Alice public
    mov esi, x25519_rfc7748_alice_private
    mov edi, curve25519_private_key
    call curve25519_fe_copy
    call curve25519_clamp_private

    mov esi, curve25519_basepoint
    mov edi, x25519_peer_public_key
    call curve25519_fe_copy

    call x25519_generate_shared_secret_skeleton

    mov esi, x25519_shared_secret
    mov edi, x25519_rfc7748_alice_public
    call curve25519_memcmp32
    mov esi, x25519_b9_msg
    call x25519_rfc7748_print_result

    ; FIA: AlicePublic * inv(AlicePublic) == 1
    mov esi, x25519_rfc7748_alice_public
    mov edi, curve25519_fe_out
    call curve25519_fe_invert

    mov esi, x25519_rfc7748_alice_public
    mov edi, curve25519_fe_out
    mov edx, x25519_tmp0
    call curve25519_fe_mul

    mov esi, x25519_tmp0
    mov al, 1
    call curve25519_fe_is_small
    mov esi, x25519_fia_msg
    call x25519_rfc7748_print_result

    ; FIB: BobPublic * inv(BobPublic) == 1
    mov esi, x25519_rfc7748_bob_public
    mov edi, curve25519_fe_out
    call curve25519_fe_invert

    mov esi, x25519_rfc7748_bob_public
    mov edi, curve25519_fe_out
    mov edx, x25519_tmp0
    call curve25519_fe_mul

    mov esi, x25519_tmp0
    mov al, 1
    call curve25519_fe_is_small
    mov esi, x25519_fib_msg
    call x25519_rfc7748_print_result

    mov esi, x25519_rfc7748_bob_public
    call x25519_check_shared_init
    mov esi, x25519_shared_init_msg
    call x25519_rfc7748_print_result

    mov esi, x25519_rfc7748_alice_private
    mov edi, curve25519_private_key
    call curve25519_fe_copy
    call curve25519_clamp_private
    mov esi, curve25519_private_key
    mov edi, curve25519_public_key
    call curve25519_fe_copy
    mov esi, x25519_rfc7748_bob_public
    mov edi, x25519_peer_public_key
    call curve25519_fe_copy
    call x25519_generate_shared_secret_skeleton
    mov esi, x25519_x1
    mov edi, x25519_peer_public_key
    call curve25519_memcmp32
    mov esi, x25519_x1b_msg
    call x25519_rfc7748_print_result

    ; IZA: z2_AB * inv(z2_AB) == 1
    mov esi, x25519_z2
    mov edi, curve25519_fe_out
    call curve25519_fe_invert

    mov esi, x25519_z2
    mov edi, curve25519_fe_out
    mov edx, x25519_tmp0
    call curve25519_fe_mul

    mov esi, x25519_tmp0
    mov al, 1
    call curve25519_fe_is_small
    mov esi, x25519_iza_msg
    call x25519_rfc7748_print_result

    ; Dump final AB projective/finalization buffers
    mov esi, x25519_x2a_msg
    call pm_print_string
    call pm_newline
    mov esi, x25519_x2
    call x25519_print_32

    mov esi, x25519_z2a_msg
    call pm_print_string
    call pm_newline
    mov esi, x25519_z2
    call x25519_print_32

    mov esi, x25519_iva_msg
    call pm_print_string
    call pm_newline
    mov esi, curve25519_fe_out
    call x25519_print_32

    call pm_more_pause

    ; U1B: first ladder step with x1 = BobPublic
    ; expected x3 = 4*x1^2, expected z3 = 4*x1

    ; expected_x = 4 * BobPublic^2
    mov esi, x25519_rfc7748_bob_public
    mov edx, x25519_tmp0
    call curve25519_fe_square

    mov esi, x25519_tmp0
    mov eax, 4
    mov edx, x25519_u1_expected_x
    call curve25519_fe_mul_small

    ; expected_z = 4 * BobPublic
    mov esi, x25519_rfc7748_bob_public
    mov eax, 4
    mov edx, x25519_u1_expected_z
    call curve25519_fe_mul_small

    ; init ladder state with x1/x3 = BobPublic
    call x25519_ladder_init
    mov esi, x25519_rfc7748_bob_public
    mov edi, x25519_x1
    call curve25519_fe_copy
    mov esi, x25519_rfc7748_bob_public
    mov edi, x25519_x3
    call curve25519_fe_copy

    call x25519_ladder_step_once

    mov esi, x25519_x3
    mov edi, x25519_u1_expected_x
    call curve25519_memcmp32
    mov esi, x25519_u1x_msg
    call x25519_rfc7748_print_result

    mov esi, x25519_z3
    mov edi, x25519_u1_expected_z
    call curve25519_memcmp32
    mov esi, x25519_u1z_msg
    call x25519_rfc7748_print_result

    ; CSB: cswap large buffers, swap and restore
    mov esi, x25519_rfc7748_alice_public
    mov edi, x25519_tmp0
    call curve25519_fe_copy

    mov esi, x25519_rfc7748_bob_public
    mov edi, x25519_tmp1
    call curve25519_fe_copy

    mov al, 1
    mov esi, x25519_tmp0
    mov edi, x25519_tmp1
    call curve25519_fe_cswap

    mov al, 1
    mov esi, x25519_tmp0
    mov edi, x25519_tmp1
    call curve25519_fe_cswap

    mov esi, x25519_tmp0
    mov edi, x25519_rfc7748_alice_public
    call curve25519_memcmp32
    cmp eax, 1
    jne .csb_done

    mov esi, x25519_tmp1
    mov edi, x25519_rfc7748_bob_public
    call curve25519_memcmp32

.csb_done:
    mov esi, x25519_csb_msg
    call x25519_rfc7748_print_result

    ; U2D: two manual ladder steps with BobPublic are deterministic on x2/z2

    ; run 1
    call x25519_ladder_init
    mov esi, x25519_rfc7748_bob_public
    mov edi, x25519_x1
    call curve25519_fe_copy
    mov esi, x25519_rfc7748_bob_public
    mov edi, x25519_x3
    call curve25519_fe_copy
    call x25519_ladder_step_once
    call x25519_ladder_step_once

    mov esi, x25519_x2
    mov edi, x25519_u2_x_run1
    call curve25519_fe_copy
    mov esi, x25519_z2
    mov edi, x25519_u2_z_run1
    call curve25519_fe_copy

    ; run 2
    call x25519_ladder_init
    mov esi, x25519_rfc7748_bob_public
    mov edi, x25519_x1
    call curve25519_fe_copy
    mov esi, x25519_rfc7748_bob_public
    mov edi, x25519_x3
    call curve25519_fe_copy
    call x25519_ladder_step_once
    call x25519_ladder_step_once

    mov esi, x25519_x2
    mov edi, x25519_u2_x_run2
    call curve25519_fe_copy
    mov esi, x25519_z2
    mov edi, x25519_u2_z_run2
    call curve25519_fe_copy

    mov esi, x25519_u2_x_run1
    mov edi, x25519_u2_x_run2
    call curve25519_memcmp32
    cmp eax, 1
    jne .u2d_done

    mov esi, x25519_u2_z_run1
    mov edi, x25519_u2_z_run2
    call curve25519_memcmp32

.u2d_done:
    mov esi, x25519_u2d_msg
    call x25519_rfc7748_print_result

    ; U4D: four manual ladder steps with BobPublic are deterministic on x2/z2

    ; run 1
    call x25519_ladder_init
    mov esi, x25519_rfc7748_bob_public
    mov edi, x25519_x1
    call curve25519_fe_copy
    mov esi, x25519_rfc7748_bob_public
    mov edi, x25519_x3
    call curve25519_fe_copy
    call x25519_ladder_step_once
    call x25519_ladder_step_once
    call x25519_ladder_step_once
    call x25519_ladder_step_once

    mov esi, x25519_x2
    mov edi, x25519_u4_x_run1
    call curve25519_fe_copy
    mov esi, x25519_z2
    mov edi, x25519_u4_z_run1
    call curve25519_fe_copy

    ; run 2
    call x25519_ladder_init
    mov esi, x25519_rfc7748_bob_public
    mov edi, x25519_x1
    call curve25519_fe_copy
    mov esi, x25519_rfc7748_bob_public
    mov edi, x25519_x3
    call curve25519_fe_copy
    call x25519_ladder_step_once
    call x25519_ladder_step_once
    call x25519_ladder_step_once
    call x25519_ladder_step_once

    mov esi, x25519_x2
    mov edi, x25519_u4_x_run2
    call curve25519_fe_copy
    mov esi, x25519_z2
    mov edi, x25519_u4_z_run2
    call curve25519_fe_copy

    mov esi, x25519_u4_x_run1
    mov edi, x25519_u4_x_run2
    call curve25519_memcmp32
    cmp eax, 1
    jne .u4d_done

    mov esi, x25519_u4_z_run1
    mov edi, x25519_u4_z_run2
    call curve25519_memcmp32

.u4d_done:
    mov esi, x25519_u4d_msg
    call x25519_rfc7748_print_result

    ; U8D: eight manual ladder steps with BobPublic are deterministic on x2/z2

    ; run 1
    call x25519_ladder_init
    mov esi, x25519_rfc7748_bob_public
    mov edi, x25519_x1
    call curve25519_fe_copy
    mov esi, x25519_rfc7748_bob_public
    mov edi, x25519_x3
    call curve25519_fe_copy
    call x25519_ladder_step_once
    call x25519_ladder_step_once
    call x25519_ladder_step_once
    call x25519_ladder_step_once
    call x25519_ladder_step_once
    call x25519_ladder_step_once
    call x25519_ladder_step_once
    call x25519_ladder_step_once

    mov esi, x25519_x2
    mov edi, x25519_u8_x_run1
    call curve25519_fe_copy
    mov esi, x25519_z2
    mov edi, x25519_u8_z_run1
    call curve25519_fe_copy

    ; run 2
    call x25519_ladder_init
    mov esi, x25519_rfc7748_bob_public
    mov edi, x25519_x1
    call curve25519_fe_copy
    mov esi, x25519_rfc7748_bob_public
    mov edi, x25519_x3
    call curve25519_fe_copy
    call x25519_ladder_step_once
    call x25519_ladder_step_once
    call x25519_ladder_step_once
    call x25519_ladder_step_once
    call x25519_ladder_step_once
    call x25519_ladder_step_once
    call x25519_ladder_step_once
    call x25519_ladder_step_once

    mov esi, x25519_x2
    mov edi, x25519_u8_x_run2
    call curve25519_fe_copy
    mov esi, x25519_z2
    mov edi, x25519_u8_z_run2
    call curve25519_fe_copy

    mov esi, x25519_u8_x_run1
    mov edi, x25519_u8_x_run2
    call curve25519_memcmp32
    cmp eax, 1
    jne .u8d_done

    mov esi, x25519_u8_z_run1
    mov edi, x25519_u8_z_run2
    call curve25519_memcmp32

.u8d_done:
    mov esi, x25519_u8d_msg
    call x25519_rfc7748_print_result

    ; U16D: sixteen manual ladder steps with BobPublic are deterministic on x2/z2

    ; run 1
    call x25519_ladder_init
    mov esi, x25519_rfc7748_bob_public
    mov edi, x25519_x1
    call curve25519_fe_copy
    mov esi, x25519_rfc7748_bob_public
    mov edi, x25519_x3
    call curve25519_fe_copy
    call x25519_ladder_step_once
    call x25519_ladder_step_once
    call x25519_ladder_step_once
    call x25519_ladder_step_once
    call x25519_ladder_step_once
    call x25519_ladder_step_once
    call x25519_ladder_step_once
    call x25519_ladder_step_once
    call x25519_ladder_step_once
    call x25519_ladder_step_once
    call x25519_ladder_step_once
    call x25519_ladder_step_once
    call x25519_ladder_step_once
    call x25519_ladder_step_once
    call x25519_ladder_step_once
    call x25519_ladder_step_once

    mov esi, x25519_x2
    mov edi, x25519_u16_x_run1
    call curve25519_fe_copy
    mov esi, x25519_z2
    mov edi, x25519_u16_z_run1
    call curve25519_fe_copy

    ; run 2
    call x25519_ladder_init
    mov esi, x25519_rfc7748_bob_public
    mov edi, x25519_x1
    call curve25519_fe_copy
    mov esi, x25519_rfc7748_bob_public
    mov edi, x25519_x3
    call curve25519_fe_copy
    call x25519_ladder_step_once
    call x25519_ladder_step_once
    call x25519_ladder_step_once
    call x25519_ladder_step_once
    call x25519_ladder_step_once
    call x25519_ladder_step_once
    call x25519_ladder_step_once
    call x25519_ladder_step_once
    call x25519_ladder_step_once
    call x25519_ladder_step_once
    call x25519_ladder_step_once
    call x25519_ladder_step_once
    call x25519_ladder_step_once
    call x25519_ladder_step_once
    call x25519_ladder_step_once
    call x25519_ladder_step_once

    mov esi, x25519_x2
    mov edi, x25519_u16_x_run2
    call curve25519_fe_copy
    mov esi, x25519_z2
    mov edi, x25519_u16_z_run2
    call curve25519_fe_copy

    mov esi, x25519_u16_x_run1
    mov edi, x25519_u16_x_run2
    call curve25519_memcmp32
    cmp eax, 1
    jne .u16d_done

    mov esi, x25519_u16_z_run1
    mov edi, x25519_u16_z_run2
    call curve25519_memcmp32

.u16d_done:
    mov esi, x25519_u16d_msg
    call x25519_rfc7748_print_result

    mov esi, x25519_shared_secret
    mov edi, x25519_shared_ab
    call curve25519_fe_copy
    mov esi, curve25519_private_key
    mov edi, curve25519_public_key
    call curve25519_memcmp32
    mov esi, x25519_scalar_a_msg
    call x25519_rfc7748_print_result

    mov esi, x25519_rfc7748_bob_private
    mov edi, curve25519_private_key
    call curve25519_fe_copy
    call curve25519_clamp_private
    mov esi, curve25519_private_key
    mov edi, curve25519_public_key
    call curve25519_fe_copy
    mov esi, x25519_rfc7748_alice_public
    mov edi, x25519_peer_public_key
    call curve25519_fe_copy
    call x25519_generate_shared_secret_skeleton
    mov esi, x25519_shared_secret
    mov edi, x25519_shared_ba
    call curve25519_fe_copy
    mov esi, curve25519_private_key
    mov edi, curve25519_public_key
    call curve25519_memcmp32
    mov esi, x25519_scalar_b_msg
    call x25519_rfc7748_print_result

    mov esi, x25519_shared_ab
    mov edi, x25519_shared_ba
    call curve25519_memcmp32
    mov esi, x25519_rfc_shared_abba_msg
    call x25519_rfc7748_print_result

    mov esi, x25519_shared_ab
    mov edi, x25519_rfc7748_shared
    call curve25519_memcmp32
    mov esi, x25519_rfc_shared_ab_msg
    call x25519_rfc7748_print_result

    ; ABN: normalize computed AB once more and compare with expected
    mov esi, x25519_shared_ab
    mov edi, x25519_ab_norm
    call curve25519_fe_copy

    mov esi, x25519_ab_norm
    call curve25519_bytes_to_mul_temp
    call curve25519_fe_normalize
    mov edi, x25519_ab_norm
    call curve25519_mul_temp_copy_low32

    mov esi, x25519_ab_norm
    mov edi, x25519_rfc7748_shared
    call curve25519_memcmp32
    mov esi, x25519_abn_msg
    call x25519_rfc7748_print_result

    ; Full dump: computed AB
    mov esi, x25519_abc_msg
    call pm_print_string
    call pm_newline
    mov esi, x25519_shared_ab
    call x25519_print_32

    ; Full dump: expected shared
    mov esi, x25519_exp_msg
    call pm_print_string
    call pm_newline
    mov esi, x25519_rfc7748_shared
    call x25519_print_32

    ; First differing byte: index computed expected
    mov esi, x25519_shared_ab
    mov edi, x25519_rfc7748_shared
    call x25519_print_first_diff

    call pm_more_pause

    mov esi, x25519_shared_ba
    mov edi, x25519_rfc7748_shared
    call curve25519_memcmp32
    mov esi, x25519_rfc_shared_ba_msg
    call x25519_rfc7748_print_result

    mov esi, x25519_shared_ba
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

curve25519_bytes_to_mul_temp:
    ; ESI = 32-byte buffer
    ; converts bytes to curve25519_mul_temp dword slots
    pushad

    mov edi, curve25519_mul_temp
    mov ecx, 32

.bt_loop:
    xor eax, eax
    mov al, [esi]
    mov [edi], eax
    inc esi
    add edi, 4
    dec ecx
    jnz .bt_loop

    popad
    ret

x25519_print_edge8:
    ; ESI = 32-byte buffer
    ; prints first 4 bytes, '..', last 4 bytes
    pushad

    mov ecx, 4

.first:
    lodsb
    call pm_print_hex8
    mov al, ' '
    call pm_print_char
    loop .first

    mov al, '.'
    call pm_print_char
    mov al, '.'
    call pm_print_char
    mov al, ' '
    call pm_print_char

    add esi, 24
    mov ecx, 4

.last:
    lodsb
    call pm_print_hex8
    mov al, ' '
    call pm_print_char
    loop .last

    call pm_newline

    popad
    ret

x25519_print_32:
    ; ESI = 32-byte buffer
    ; prints 32 bytes as 4 lines of 8 bytes
    pushad

    mov ebx, 4

.row:
    mov ecx, 8

.col:
    lodsb
    call pm_print_hex8
    mov al, ' '
    call pm_print_char
    loop .col

    call pm_newline
    dec ebx
    jnz .row

    popad
    ret

x25519_print_first_diff:
    ; ESI = computed
    ; EDI = expected
    ; prints: DIF index computed expected
    pushad

    mov ebx, 0

.diff_loop:
    cmp ebx, 32
    jge .done

    mov al, [esi + ebx]
    mov dl, [edi + ebx]
    cmp al, dl
    jne .found

    inc ebx
    jmp .diff_loop

.found:
    mov esi, x25519_diff_msg
    call pm_print_string

    mov eax, ebx
    call pm_print_uint32

    mov al, ' '
    call pm_print_char

    mov al, [esp + 28] ; dummy-safe overwrite below avoided by reloading pointers impossible after pushad
    ; Reload original computed/expected pointers from pushad stack is messy,
    ; so use global known buffers for this diagnostic:
    mov al, [x25519_shared_ab + ebx]
    call pm_print_hex8

    mov al, ' '
    call pm_print_char

    mov al, [x25519_rfc7748_shared + ebx]
    call pm_print_hex8

    call pm_newline

.done:
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

x25519_check_shared_init:
    mov [curve25519_inv_input_ptr], esi
    call x25519_ladder_init
    mov esi, [curve25519_inv_input_ptr]
    mov edi, x25519_x1
    call curve25519_fe_copy
    mov esi, [curve25519_inv_input_ptr]
    mov edi, x25519_x3
    call curve25519_fe_copy

    mov esi, x25519_x1
    mov edi, [curve25519_inv_input_ptr]
    call curve25519_memcmp32
    cmp eax, 1
    jne .fail
    mov esi, x25519_x3
    mov edi, [curve25519_inv_input_ptr]
    call curve25519_memcmp32
    cmp eax, 1
    jne .fail
    mov esi, x25519_x2
    mov al, 1
    call curve25519_fe_is_small
    cmp eax, 1
    jne .fail
    mov esi, x25519_z2
    xor al, al
    call curve25519_fe_is_small
    cmp eax, 1
    jne .fail
    mov esi, x25519_z3
    mov al, 1
    call curve25519_fe_is_small
    ret

.fail:
    xor eax, eax
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

curve25519_fe_is_u32:
    ; ESI = field element, EAX = expected low dword, EAX = match flag

    mov ebx, eax
    xor eax, eax
    cmp [esi], ebx
    jne .done
    add esi, 4
    mov ecx, 28

.loop:
    cmp byte [esi], 0
    jne .done
    inc esi
    dec ecx
    jnz .loop
    mov eax, 1

.done:
    ret

curve25519_fe_is_p_minus_2:
    mov eax, 0
    cmp byte [esi], 235
    jne .done
    inc esi
    mov ecx, 30

.mid:
    cmp byte [esi], 255
    jne .done
    inc esi
    dec ecx
    jnz .mid

    cmp byte [esi], 127
    jne .done
    inc eax

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
    ; subtraction modulo p, with borrow compensated by adding p

    push ecx
    push eax
    push ebx
    push edx

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

    jnc .done

    pop edx
    push edx

    clc
    mov al, [edx]
    add al, 237
    mov [edx], al
    inc edx
    mov ecx, 30

.add_mid:
    mov al, [edx]
    adc al, 255
    mov [edx], al
    inc edx
    dec ecx
    jnz .add_mid

    mov al, [edx]
    adc al, 127
    mov [edx], al

.done:
    pop edx
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
    call .conditional_sub_p
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
