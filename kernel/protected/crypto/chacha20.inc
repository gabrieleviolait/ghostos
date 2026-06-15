; ============================================================
; GhostCrypto - ChaCha20 block function
; RFC 8439 block test vector
; ============================================================

chacha20_block:
    ; ESI = key ptr, 32 bytes
    ; EDX = nonce ptr, 12 bytes
    ; EAX = counter
    ; EDI = output ptr, 64 bytes
    pushad

    mov [chacha20_key_ptr], esi
    mov [chacha20_nonce_ptr], edx
    mov [chacha20_counter], eax
    mov [chacha20_output_ptr], edi

    mov dword [chacha20_state +  0], 061707865h
    mov dword [chacha20_state +  4], 03320646Eh
    mov dword [chacha20_state +  8], 079622D32h
    mov dword [chacha20_state + 12], 06B206574h

    mov esi, [chacha20_key_ptr]
    mov edi, chacha20_state + 16
    mov ecx, 8

.load_key:
    call chacha20_load32
    mov [edi], eax
    add esi, 4
    add edi, 4
    loop .load_key

    mov eax, [chacha20_counter]
    mov [chacha20_state + 48], eax

    mov esi, [chacha20_nonce_ptr]
    mov edi, chacha20_state + 52
    mov ecx, 3

.load_nonce:
    call chacha20_load32
    mov [edi], eax
    add esi, 4
    add edi, 4
    loop .load_nonce

    mov esi, chacha20_state
    mov edi, chacha20_initial_state
    mov ecx, 16
    cld
    rep movsd

    mov dword [chacha20_rounds_left], 10

.double_round:
    mov esi, chacha20_state +  0
    mov edi, chacha20_state + 16
    mov edx, chacha20_state + 32
    mov ebx, chacha20_state + 48
    call chacha20_quarter_round

    mov esi, chacha20_state +  4
    mov edi, chacha20_state + 20
    mov edx, chacha20_state + 36
    mov ebx, chacha20_state + 52
    call chacha20_quarter_round

    mov esi, chacha20_state +  8
    mov edi, chacha20_state + 24
    mov edx, chacha20_state + 40
    mov ebx, chacha20_state + 56
    call chacha20_quarter_round

    mov esi, chacha20_state + 12
    mov edi, chacha20_state + 28
    mov edx, chacha20_state + 44
    mov ebx, chacha20_state + 60
    call chacha20_quarter_round

    mov esi, chacha20_state +  0
    mov edi, chacha20_state + 20
    mov edx, chacha20_state + 40
    mov ebx, chacha20_state + 60
    call chacha20_quarter_round

    mov esi, chacha20_state +  4
    mov edi, chacha20_state + 24
    mov edx, chacha20_state + 44
    mov ebx, chacha20_state + 48
    call chacha20_quarter_round

    mov esi, chacha20_state +  8
    mov edi, chacha20_state + 28
    mov edx, chacha20_state + 32
    mov ebx, chacha20_state + 52
    call chacha20_quarter_round

    mov esi, chacha20_state + 12
    mov edi, chacha20_state + 16
    mov edx, chacha20_state + 36
    mov ebx, chacha20_state + 56
    call chacha20_quarter_round

    dec dword [chacha20_rounds_left]
    jnz .double_round

    mov esi, chacha20_state
    mov ebx, chacha20_initial_state
    mov edi, [chacha20_output_ptr]
    mov ecx, 16

.store_output:
    mov eax, [esi]
    add eax, [ebx]
    mov edx, eax
    mov [edi], dl
    shr edx, 8
    mov [edi + 1], dl
    shr edx, 8
    mov [edi + 2], dl
    shr edx, 8
    mov [edi + 3], dl

    add esi, 4
    add ebx, 4
    add edi, 4
    loop .store_output

    popad
    ret

chacha20_quarter_round:
    ; ESI = a ptr, EDI = b ptr, EDX = c ptr, EBX = d ptr
    ; Clobbers EAX/ECX.
    mov eax, [esi]
    add eax, [edi]
    mov [esi], eax
    mov ecx, [ebx]
    xor ecx, eax
    rol ecx, 16
    mov [ebx], ecx

    mov eax, [edx]
    add eax, [ebx]
    mov [edx], eax
    mov ecx, [edi]
    xor ecx, eax
    rol ecx, 12
    mov [edi], ecx

    mov eax, [esi]
    add eax, [edi]
    mov [esi], eax
    mov ecx, [ebx]
    xor ecx, eax
    rol ecx, 8
    mov [ebx], ecx

    mov eax, [edx]
    add eax, [ebx]
    mov [edx], eax
    mov ecx, [edi]
    xor ecx, eax
    rol ecx, 7
    mov [edi], ecx
    ret

chacha20_load32:
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

chacha20test:
    pushad

    mov esi, chacha20_test_key
    mov edx, chacha20_test_nonce
    mov eax, 1
    mov edi, chacha20_test_output
    call chacha20_block

    mov esi, chacha20_test_output
    mov edi, chacha20_test_expected
    mov ecx, 64
    call chacha20_memcmp

    cmp eax, 1
    je .pass

    mov esi, chacha20_fail_msg
    call pm_print_string
    jmp .done

.pass:
    mov esi, chacha20_pass_msg
    call pm_print_string

.done:
    popad
    ret

chacha20_memcmp:
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

chacha20_pass_msg db 'CHACHA20 PASS',13,0
chacha20_fail_msg db 'CHACHA20 FAIL',13,0

chacha20_test_key:
    db 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h
    db 008h, 009h, 00Ah, 00Bh, 00Ch, 00Dh, 00Eh, 00Fh
    db 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h
    db 018h, 019h, 01Ah, 01Bh, 01Ch, 01Dh, 01Eh, 01Fh

chacha20_test_nonce:
    db 000h, 000h, 000h, 009h
    db 000h, 000h, 000h, 04Ah
    db 000h, 000h, 000h, 000h

chacha20_test_expected:
    db 010h, 0F1h, 0E7h, 0E4h, 0D1h, 03Bh, 059h, 015h
    db 050h, 00Fh, 0DDh, 01Fh, 0A3h, 020h, 071h, 0C4h
    db 0C7h, 0D1h, 0F4h, 0C7h, 033h, 0C0h, 068h, 003h
    db 004h, 022h, 0AAh, 09Ah, 0C3h, 0D4h, 06Ch, 04Eh
    db 0D2h, 082h, 064h, 046h, 007h, 09Fh, 0AAh, 009h
    db 014h, 0C2h, 0D7h, 005h, 0D9h, 08Bh, 002h, 0A2h
    db 0B5h, 012h, 09Ch, 0D1h, 0DEh, 016h, 04Eh, 0B9h
    db 0CBh, 0D0h, 083h, 0E8h, 0A2h, 050h, 03Ch, 04Eh

chacha20_key_ptr dd 0
chacha20_nonce_ptr dd 0
chacha20_output_ptr dd 0
chacha20_counter dd 0
chacha20_rounds_left dd 0
chacha20_state times 16 dd 0
chacha20_initial_state times 16 dd 0
chacha20_test_output times 64 db 0
