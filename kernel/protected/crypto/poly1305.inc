; ============================================================
; GhostCrypto - Poly1305 MAC
; RFC 8439 standalone test vector
; ============================================================

%macro POLY1305_ACCUM_MUL 3
    mov eax, [%2]
    mul dword [%3]
    add dword [%1], eax
    adc dword [%1 + 4], edx
%endmacro

POLY1305_MASK26 equ 03FFFFFFh

poly1305_mac:
    ; ESI = one-time key ptr, 32 bytes
    ; EDX = message ptr
    ; EBX = message len
    ; EDI = tag output ptr, 16 bytes
    pushad

    mov [poly1305_key_ptr], esi
    mov [poly1305_msg_ptr], edx
    mov [poly1305_msg_len], ebx
    mov [poly1305_out_ptr], edi

    call poly1305_load_r

    xor eax, eax
    mov [poly1305_h0], eax
    mov [poly1305_h1], eax
    mov [poly1305_h2], eax
    mov [poly1305_h3], eax
    mov [poly1305_h4], eax

.next_block:
    cmp dword [poly1305_msg_len], 0
    je .finish

    mov eax, [poly1305_msg_len]
    cmp eax, 16
    jbe .take_ready
    mov eax, 16

.take_ready:
    mov [poly1305_take_len], eax

    mov edi, poly1305_block
    xor eax, eax
    mov ecx, 20
    cld
    rep stosb

    mov esi, [poly1305_msg_ptr]
    mov edi, poly1305_block
    mov ecx, [poly1305_take_len]
    cld
    rep movsb

    mov eax, [poly1305_take_len]
    mov byte [poly1305_block + eax], 1

    call poly1305_add_block
    call poly1305_mul_mod

    mov eax, [poly1305_take_len]
    add [poly1305_msg_ptr], eax
    sub [poly1305_msg_len], eax
    jmp .next_block

.finish:
    call poly1305_finish

    popad
    ret

poly1305_load_r:
    push esi

    mov esi, [poly1305_key_ptr]
    call poly1305_load32
    and eax, 03FFFFFFh
    mov [poly1305_r0], eax

    mov esi, [poly1305_key_ptr]
    add esi, 3
    call poly1305_load32
    shr eax, 2
    and eax, 03FFFF03h
    mov [poly1305_r1], eax

    mov esi, [poly1305_key_ptr]
    add esi, 6
    call poly1305_load32
    shr eax, 4
    and eax, 03FFC0FFh
    mov [poly1305_r2], eax

    mov esi, [poly1305_key_ptr]
    add esi, 9
    call poly1305_load32
    shr eax, 6
    and eax, 03F03FFFh
    mov [poly1305_r3], eax

    mov esi, [poly1305_key_ptr]
    add esi, 12
    call poly1305_load32
    shr eax, 8
    and eax, 000FFFFFh
    mov [poly1305_r4], eax

    mov eax, [poly1305_r1]
    lea eax, [eax + eax * 4]
    mov [poly1305_s1], eax

    mov eax, [poly1305_r2]
    lea eax, [eax + eax * 4]
    mov [poly1305_s2], eax

    mov eax, [poly1305_r3]
    lea eax, [eax + eax * 4]
    mov [poly1305_s3], eax

    mov eax, [poly1305_r4]
    lea eax, [eax + eax * 4]
    mov [poly1305_s4], eax

    pop esi
    ret

poly1305_add_block:
    mov esi, poly1305_block
    call poly1305_load32
    and eax, POLY1305_MASK26
    add [poly1305_h0], eax

    mov esi, poly1305_block + 3
    call poly1305_load32
    shr eax, 2
    and eax, POLY1305_MASK26
    add [poly1305_h1], eax

    mov esi, poly1305_block + 6
    call poly1305_load32
    shr eax, 4
    and eax, POLY1305_MASK26
    add [poly1305_h2], eax

    mov esi, poly1305_block + 9
    call poly1305_load32
    shr eax, 6
    and eax, POLY1305_MASK26
    add [poly1305_h3], eax

    mov esi, poly1305_block + 12
    call poly1305_load32
    shr eax, 8
    and eax, POLY1305_MASK26

    cmp dword [poly1305_take_len], 16
    jne .no_hibit
    add eax, 01000000h

.no_hibit:
    add [poly1305_h4], eax
    ret

poly1305_mul_mod:
    xor eax, eax
    mov [poly1305_d0], eax
    mov [poly1305_d0 + 4], eax
    mov [poly1305_d1], eax
    mov [poly1305_d1 + 4], eax
    mov [poly1305_d2], eax
    mov [poly1305_d2 + 4], eax
    mov [poly1305_d3], eax
    mov [poly1305_d3 + 4], eax
    mov [poly1305_d4], eax
    mov [poly1305_d4 + 4], eax

    POLY1305_ACCUM_MUL poly1305_d0, poly1305_h0, poly1305_r0
    POLY1305_ACCUM_MUL poly1305_d0, poly1305_h1, poly1305_s4
    POLY1305_ACCUM_MUL poly1305_d0, poly1305_h2, poly1305_s3
    POLY1305_ACCUM_MUL poly1305_d0, poly1305_h3, poly1305_s2
    POLY1305_ACCUM_MUL poly1305_d0, poly1305_h4, poly1305_s1

    POLY1305_ACCUM_MUL poly1305_d1, poly1305_h0, poly1305_r1
    POLY1305_ACCUM_MUL poly1305_d1, poly1305_h1, poly1305_r0
    POLY1305_ACCUM_MUL poly1305_d1, poly1305_h2, poly1305_s4
    POLY1305_ACCUM_MUL poly1305_d1, poly1305_h3, poly1305_s3
    POLY1305_ACCUM_MUL poly1305_d1, poly1305_h4, poly1305_s2

    POLY1305_ACCUM_MUL poly1305_d2, poly1305_h0, poly1305_r2
    POLY1305_ACCUM_MUL poly1305_d2, poly1305_h1, poly1305_r1
    POLY1305_ACCUM_MUL poly1305_d2, poly1305_h2, poly1305_r0
    POLY1305_ACCUM_MUL poly1305_d2, poly1305_h3, poly1305_s4
    POLY1305_ACCUM_MUL poly1305_d2, poly1305_h4, poly1305_s3

    POLY1305_ACCUM_MUL poly1305_d3, poly1305_h0, poly1305_r3
    POLY1305_ACCUM_MUL poly1305_d3, poly1305_h1, poly1305_r2
    POLY1305_ACCUM_MUL poly1305_d3, poly1305_h2, poly1305_r1
    POLY1305_ACCUM_MUL poly1305_d3, poly1305_h3, poly1305_r0
    POLY1305_ACCUM_MUL poly1305_d3, poly1305_h4, poly1305_s4

    POLY1305_ACCUM_MUL poly1305_d4, poly1305_h0, poly1305_r4
    POLY1305_ACCUM_MUL poly1305_d4, poly1305_h1, poly1305_r3
    POLY1305_ACCUM_MUL poly1305_d4, poly1305_h2, poly1305_r2
    POLY1305_ACCUM_MUL poly1305_d4, poly1305_h3, poly1305_r1
    POLY1305_ACCUM_MUL poly1305_d4, poly1305_h4, poly1305_r0

    call poly1305_reduce_d
    ret

poly1305_reduce_d:
    mov eax, [poly1305_d0]
    mov edx, [poly1305_d0 + 4]
    mov ecx, eax
    and ecx, POLY1305_MASK26
    mov [poly1305_h0], ecx
    shrd eax, edx, 26
    add [poly1305_d1], eax
    adc dword [poly1305_d1 + 4], 0

    mov eax, [poly1305_d1]
    mov edx, [poly1305_d1 + 4]
    mov ecx, eax
    and ecx, POLY1305_MASK26
    mov [poly1305_h1], ecx
    shrd eax, edx, 26
    add [poly1305_d2], eax
    adc dword [poly1305_d2 + 4], 0

    mov eax, [poly1305_d2]
    mov edx, [poly1305_d2 + 4]
    mov ecx, eax
    and ecx, POLY1305_MASK26
    mov [poly1305_h2], ecx
    shrd eax, edx, 26
    add [poly1305_d3], eax
    adc dword [poly1305_d3 + 4], 0

    mov eax, [poly1305_d3]
    mov edx, [poly1305_d3 + 4]
    mov ecx, eax
    and ecx, POLY1305_MASK26
    mov [poly1305_h3], ecx
    shrd eax, edx, 26
    add [poly1305_d4], eax
    adc dword [poly1305_d4 + 4], 0

    mov eax, [poly1305_d4]
    mov edx, [poly1305_d4 + 4]
    mov ecx, eax
    and ecx, POLY1305_MASK26
    mov [poly1305_h4], ecx
    shrd eax, edx, 26
    lea eax, [eax + eax * 4]
    add [poly1305_h0], eax

    mov eax, [poly1305_h0]
    mov ecx, eax
    shr eax, 26
    and ecx, POLY1305_MASK26
    mov [poly1305_h0], ecx
    add [poly1305_h1], eax
    ret

poly1305_final_carry:
    mov eax, [poly1305_h1]
    mov ecx, eax
    shr eax, 26
    and ecx, POLY1305_MASK26
    mov [poly1305_h1], ecx
    add [poly1305_h2], eax

    mov eax, [poly1305_h2]
    mov ecx, eax
    shr eax, 26
    and ecx, POLY1305_MASK26
    mov [poly1305_h2], ecx
    add [poly1305_h3], eax

    mov eax, [poly1305_h3]
    mov ecx, eax
    shr eax, 26
    and ecx, POLY1305_MASK26
    mov [poly1305_h3], ecx
    add [poly1305_h4], eax

    mov eax, [poly1305_h4]
    mov ecx, eax
    shr eax, 26
    and ecx, POLY1305_MASK26
    mov [poly1305_h4], ecx
    lea eax, [eax + eax * 4]
    add [poly1305_h0], eax

    mov eax, [poly1305_h0]
    mov ecx, eax
    shr eax, 26
    and ecx, POLY1305_MASK26
    mov [poly1305_h0], ecx
    add [poly1305_h1], eax
    ret

poly1305_finish:
    call poly1305_final_carry
    call poly1305_select_reduced
    call poly1305_serialize_tag
    ret

poly1305_select_reduced:
    mov eax, [poly1305_h0]
    add eax, 5
    mov ebx, eax
    shr ebx, 26
    and eax, POLY1305_MASK26
    mov [poly1305_g0], eax

    mov eax, [poly1305_h1]
    add eax, ebx
    mov ebx, eax
    shr ebx, 26
    and eax, POLY1305_MASK26
    mov [poly1305_g1], eax

    mov eax, [poly1305_h2]
    add eax, ebx
    mov ebx, eax
    shr ebx, 26
    and eax, POLY1305_MASK26
    mov [poly1305_g2], eax

    mov eax, [poly1305_h3]
    add eax, ebx
    mov ebx, eax
    shr ebx, 26
    and eax, POLY1305_MASK26
    mov [poly1305_g3], eax

    mov eax, [poly1305_h4]
    add eax, ebx
    sub eax, 04000000h
    mov [poly1305_g4], eax

    mov eax, [poly1305_g4]
    shr eax, 31
    dec eax
    mov [poly1305_reduce_mask], eax
    not eax
    mov [poly1305_keep_mask], eax

    mov eax, [poly1305_h0]
    and eax, [poly1305_keep_mask]
    mov ebx, [poly1305_g0]
    and ebx, [poly1305_reduce_mask]
    or eax, ebx
    mov [poly1305_h0], eax

    mov eax, [poly1305_h1]
    and eax, [poly1305_keep_mask]
    mov ebx, [poly1305_g1]
    and ebx, [poly1305_reduce_mask]
    or eax, ebx
    mov [poly1305_h1], eax

    mov eax, [poly1305_h2]
    and eax, [poly1305_keep_mask]
    mov ebx, [poly1305_g2]
    and ebx, [poly1305_reduce_mask]
    or eax, ebx
    mov [poly1305_h2], eax

    mov eax, [poly1305_h3]
    and eax, [poly1305_keep_mask]
    mov ebx, [poly1305_g3]
    and ebx, [poly1305_reduce_mask]
    or eax, ebx
    mov [poly1305_h3], eax

    mov eax, [poly1305_h4]
    and eax, [poly1305_keep_mask]
    mov ebx, [poly1305_g4]
    and ebx, [poly1305_reduce_mask]
    or eax, ebx
    mov [poly1305_h4], eax
    ret

poly1305_serialize_tag:
    mov eax, [poly1305_h0]
    mov ebx, [poly1305_h1]
    mov ecx, ebx
    shl ebx, 26
    or eax, ebx
    mov [poly1305_f0], eax

    mov eax, ecx
    shr eax, 6
    mov ebx, [poly1305_h2]
    mov ecx, ebx
    shl ebx, 20
    or eax, ebx
    mov [poly1305_f1], eax

    mov eax, ecx
    shr eax, 12
    mov ebx, [poly1305_h3]
    mov ecx, ebx
    shl ebx, 14
    or eax, ebx
    mov [poly1305_f2], eax

    mov eax, ecx
    shr eax, 18
    mov ebx, [poly1305_h4]
    shl ebx, 8
    or eax, ebx
    mov [poly1305_f3], eax

    mov esi, [poly1305_key_ptr]
    add esi, 16
    call poly1305_load32
    mov [poly1305_pad0], eax

    mov esi, [poly1305_key_ptr]
    add esi, 20
    call poly1305_load32
    mov [poly1305_pad1], eax

    mov esi, [poly1305_key_ptr]
    add esi, 24
    call poly1305_load32
    mov [poly1305_pad2], eax

    mov esi, [poly1305_key_ptr]
    add esi, 28
    call poly1305_load32
    mov [poly1305_pad3], eax

    mov eax, [poly1305_pad0]
    add [poly1305_f0], eax
    mov eax, [poly1305_pad1]
    adc [poly1305_f1], eax
    mov eax, [poly1305_pad2]
    adc [poly1305_f2], eax
    mov eax, [poly1305_pad3]
    adc [poly1305_f3], eax

    mov edi, [poly1305_out_ptr]
    mov eax, [poly1305_f0]
    call poly1305_store32
    add edi, 4
    mov eax, [poly1305_f1]
    call poly1305_store32
    add edi, 4
    mov eax, [poly1305_f2]
    call poly1305_store32
    add edi, 4
    mov eax, [poly1305_f3]
    call poly1305_store32
    ret

poly1305_load32:
    ; ESI = little-endian source, EAX = word. Clobbers EBX.
    movzx eax, byte [esi]
    movzx ebx, byte [esi + 1]
    shl ebx, 8
    or eax, ebx
    movzx ebx, byte [esi + 2]
    shl ebx, 16
    or eax, ebx
    movzx ebx, byte [esi + 3]
    shl ebx, 24
    or eax, ebx
    ret

poly1305_store32:
    ; EAX = word, EDI = little-endian destination. Clobbers EDX.
    mov edx, eax
    mov [edi], dl
    shr edx, 8
    mov [edi + 1], dl
    shr edx, 8
    mov [edi + 2], dl
    shr edx, 8
    mov [edi + 3], dl
    ret

poly1305test:
    pushad

    mov esi, poly1305_test_key
    mov edx, poly1305_test_msg
    mov ebx, poly1305_test_msg_len
    mov edi, poly1305_test_tag
    call poly1305_mac

    mov esi, poly1305_test_tag
    mov edi, poly1305_test_expected
    mov ecx, 16
    call poly1305_memcmp

    cmp eax, 1
    je .pass

    mov esi, poly1305_fail_msg
    call pm_print_string
    jmp .done

.pass:
    mov esi, poly1305_pass_msg
    call pm_print_string

.done:
    popad
    ret

poly1305_memcmp:
    ; ESI = actual, EDI = expected, ECX = len
    ; EAX = 1 equal, 0 different
    push esi
    push edi
    push ecx

.compare:
    cmp ecx, 0
    je .equal
    mov al, [esi]
    cmp al, [edi]
    jne .different
    inc esi
    inc edi
    dec ecx
    jmp .compare

.equal:
    mov eax, 1
    jmp .done

.different:
    xor eax, eax

.done:
    pop ecx
    pop edi
    pop esi
    ret

poly1305_pass_msg db 'POLY1305 PASS',13,0
poly1305_fail_msg db 'POLY1305 FAIL',13,0

poly1305_test_key:
    db 085h, 0D6h, 0BEh, 078h, 057h, 055h, 06Dh, 033h
    db 07Fh, 044h, 052h, 0FEh, 042h, 0D5h, 006h, 0A8h
    db 001h, 003h, 080h, 08Ah, 0FBh, 00Dh, 0B2h, 0FDh
    db 04Ah, 0BFh, 0F6h, 0AFh, 041h, 049h, 0F5h, 01Bh

poly1305_test_msg db 'Cryptographic Forum Research Group'
poly1305_test_msg_len equ $ - poly1305_test_msg

poly1305_test_expected:
    db 0A8h, 006h, 01Dh, 0C1h, 030h, 051h, 036h, 0C6h
    db 0C2h, 02Bh, 08Bh, 0AFh, 00Ch, 001h, 027h, 0A9h

poly1305_key_ptr dd 0
poly1305_msg_ptr dd 0
poly1305_msg_len dd 0
poly1305_out_ptr dd 0
poly1305_take_len dd 0

poly1305_r0 dd 0
poly1305_r1 dd 0
poly1305_r2 dd 0
poly1305_r3 dd 0
poly1305_r4 dd 0
poly1305_s1 dd 0
poly1305_s2 dd 0
poly1305_s3 dd 0
poly1305_s4 dd 0

poly1305_h0 dd 0
poly1305_h1 dd 0
poly1305_h2 dd 0
poly1305_h3 dd 0
poly1305_h4 dd 0

poly1305_g0 dd 0
poly1305_g1 dd 0
poly1305_g2 dd 0
poly1305_g3 dd 0
poly1305_g4 dd 0

poly1305_d0 dq 0
poly1305_d1 dq 0
poly1305_d2 dq 0
poly1305_d3 dq 0
poly1305_d4 dq 0

poly1305_f0 dd 0
poly1305_f1 dd 0
poly1305_f2 dd 0
poly1305_f3 dd 0
poly1305_pad0 dd 0
poly1305_pad1 dd 0
poly1305_pad2 dd 0
poly1305_pad3 dd 0
poly1305_reduce_mask dd 0
poly1305_keep_mask dd 0

poly1305_block times 20 db 0
poly1305_test_tag times 16 db 0
