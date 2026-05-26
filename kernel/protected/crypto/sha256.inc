; GhostCrypto v0.1 - standalone SHA-256 for protected mode.

[BITS 32]

sha256_h:
times 8 dd 0
sha256_buffer:
times 64 db 0
sha256_w:
times 64 dd 0
sha256_digest:
times 32 db 0
sha256_buffer_len dd 0
sha256_total_len dd 0
sha256_t1 dd 0
sha256_t2 dd 0
sha256_a dd 0
sha256_b dd 0
sha256_c dd 0
sha256_d dd 0
sha256_e dd 0
sha256_f dd 0
sha256_g dd 0
sha256_hh dd 0

sha256_k:
dd 0428A2F98h,071374491h,0B5C0FBCFh,0E9B5DBA5h
dd 03956C25Bh,059F111F1h,0923F82A4h,0AB1C5ED5h
dd 0D807AA98h,012835B01h,0243185BEh,0550C7DC3h
dd 072BE5D74h,080DEB1FEh,09BDC06A7h,0C19BF174h
dd 0E49B69C1h,0EFBE4786h,00FC19DC6h,0240CA1CCh
dd 02DE92C6Fh,04A7484AAh,05CB0A9DCh,076F988DAh
dd 0983E5152h,0A831C66Dh,0B00327C8h,0BF597FC7h
dd 0C6E00BF3h,0D5A79147h,006CA6351h,014292967h
dd 027B70A85h,02E1B2138h,04D2C6DFCh,053380D13h
dd 0650A7354h,0766A0ABBh,081C2C92Eh,092722C85h
dd 0A2BFE8A1h,0A81A664Bh,0C24B8B70h,0C76C51A3h
dd 0D192E819h,0D6990624h,0F40E3585h,0106AA070h
dd 019A4C116h,01E376C08h,02748774Ch,034B0BCB5h
dd 0391C0CB3h,04ED8AA4Ah,05B9CCA4Fh,0682E6FF3h
dd 0748F82EEh,078A5636Fh,084C87814h,08CC70208h
dd 090BEFFFAh,0A4506CEBh,0BEF9A3F7h,0C67178F2h

sha256_init:
    mov dword [sha256_h + 0], 06A09E667h
    mov dword [sha256_h + 4], 0BB67AE85h
    mov dword [sha256_h + 8], 03C6EF372h
    mov dword [sha256_h + 12], 0A54FF53Ah
    mov dword [sha256_h + 16], 0510E527Fh
    mov dword [sha256_h + 20], 09B05688Ch
    mov dword [sha256_h + 24], 01F83D9ABh
    mov dword [sha256_h + 28], 05BE0CD19h
    mov dword [sha256_buffer_len], 0
    mov dword [sha256_total_len], 0
    ret

; ESI=input, ECX=length.
sha256_update:
    pushad
.loop:
    cmp ecx, 0
    je .done

    mov ebx, [sha256_buffer_len]
    mov al, [esi]
    mov [sha256_buffer + ebx], al
    inc esi
    inc ebx
    mov [sha256_buffer_len], ebx
    inc dword [sha256_total_len]
    dec ecx

    cmp ebx, 64
    jne .loop
    call sha256_transform
    mov dword [sha256_buffer_len], 0
    jmp .loop

.done:
    popad
    ret

; EDI=32-byte output digest.
sha256_final:
    pushad

    mov ebx, [sha256_buffer_len]
    mov byte [sha256_buffer + ebx], 080h
    inc ebx
    mov [sha256_buffer_len], ebx

    cmp ebx, 56
    jbe .pad_to_len

.pad_block:
    cmp ebx, 64
    jae .flush_pad
    mov byte [sha256_buffer + ebx], 0
    inc ebx
    jmp .pad_block

.flush_pad:
    call sha256_transform
    xor ebx, ebx
    mov [sha256_buffer_len], ebx

.pad_to_len:
    cmp ebx, 56
    jae .write_len
    mov byte [sha256_buffer + ebx], 0
    inc ebx
    jmp .pad_to_len

.write_len:
    mov eax, [sha256_total_len]
    mov edx, eax
    shr edx, 29
    shl eax, 3

    mov byte [sha256_buffer + 56], 0
    mov byte [sha256_buffer + 57], 0
    mov byte [sha256_buffer + 58], 0
    mov byte [sha256_buffer + 59], dl
    mov ebx, eax
    shr ebx, 24
    mov [sha256_buffer + 60], bl
    mov ebx, eax
    shr ebx, 16
    mov [sha256_buffer + 61], bl
    mov ebx, eax
    shr ebx, 8
    mov [sha256_buffer + 62], bl
    mov [sha256_buffer + 63], al

    call sha256_transform

    mov esi, sha256_h
    mov ebx, [esp]
    mov ecx, 8
.digest_loop:
    lodsd
    mov edx, eax
    shr eax, 24
    mov [ebx], al
    mov eax, edx
    shr eax, 16
    mov [ebx + 1], al
    mov eax, edx
    shr eax, 8
    mov [ebx + 2], al
    mov [ebx + 3], dl
    add ebx, 4
    loop .digest_loop

    popad
    ret

; ESI=input, ECX=length, EDI=32-byte output digest.
sha256_hash_buffer:
    push esi
    push ecx
    push edi
    call sha256_init
    pop edi
    pop ecx
    pop esi
    push edi
    call sha256_update
    pop edi
    call sha256_final
    ret

sha256_transform:
    pushad

    mov esi, sha256_buffer
    mov edi, sha256_w
    mov ecx, 16
.load_w:
    xor eax, eax
    lodsb
    shl eax, 24
    mov ebx, eax
    xor eax, eax
    lodsb
    shl eax, 16
    or ebx, eax
    xor eax, eax
    lodsb
    shl eax, 8
    or ebx, eax
    xor eax, eax
    lodsb
    or ebx, eax
    mov [edi], ebx
    add edi, 4
    loop .load_w

    mov ecx, 16
.extend_w:
    mov eax, [sha256_w + ecx*4 - 8]
    mov ebx, eax
    ror eax, 17
    mov edx, ebx
    ror edx, 19
    xor eax, edx
    mov edx, ebx
    shr edx, 10
    xor eax, edx

    mov ebx, [sha256_w + ecx*4 - 60]
    mov edx, ebx
    ror ebx, 7
    mov edi, edx
    ror edi, 18
    xor ebx, edi
    shr edx, 3
    xor ebx, edx

    add eax, ebx
    add eax, [sha256_w + ecx*4 - 28]
    add eax, [sha256_w + ecx*4 - 64]
    mov [sha256_w + ecx*4], eax
    inc ecx
    cmp ecx, 64
    jb .extend_w

    mov eax, [sha256_h + 0]
    mov [sha256_a], eax
    mov eax, [sha256_h + 4]
    mov [sha256_b], eax
    mov eax, [sha256_h + 8]
    mov [sha256_c], eax
    mov eax, [sha256_h + 12]
    mov [sha256_d], eax
    mov eax, [sha256_h + 16]
    mov [sha256_e], eax
    mov eax, [sha256_h + 20]
    mov [sha256_f], eax
    mov eax, [sha256_h + 24]
    mov [sha256_g], eax
    mov eax, [sha256_h + 28]
    mov [sha256_hh], eax

    xor ecx, ecx
.round:
    mov eax, [sha256_e]
    mov ebx, eax
    ror eax, 6
    mov edx, ebx
    ror edx, 11
    xor eax, edx
    mov edx, ebx
    ror edx, 25
    xor eax, edx

    mov ebx, [sha256_e]
    mov edx, [sha256_f]
    and ebx, edx
    mov esi, [sha256_e]
    not esi
    and esi, [sha256_g]
    xor ebx, esi

    add eax, ebx
    add eax, [sha256_hh]
    add eax, [sha256_k + ecx*4]
    add eax, [sha256_w + ecx*4]
    mov [sha256_t1], eax

    mov eax, [sha256_a]
    mov ebx, eax
    ror eax, 2
    mov edx, ebx
    ror edx, 13
    xor eax, edx
    mov edx, ebx
    ror edx, 22
    xor eax, edx

    mov ebx, [sha256_a]
    mov edx, [sha256_b]
    and ebx, edx
    mov esi, [sha256_a]
    and esi, [sha256_c]
    xor ebx, esi
    mov esi, [sha256_b]
    and esi, [sha256_c]
    xor ebx, esi

    add eax, ebx
    mov [sha256_t2], eax

    mov eax, [sha256_g]
    mov [sha256_hh], eax
    mov eax, [sha256_f]
    mov [sha256_g], eax
    mov eax, [sha256_e]
    mov [sha256_f], eax
    mov eax, [sha256_d]
    add eax, [sha256_t1]
    mov [sha256_e], eax
    mov eax, [sha256_c]
    mov [sha256_d], eax
    mov eax, [sha256_b]
    mov [sha256_c], eax
    mov eax, [sha256_a]
    mov [sha256_b], eax
    mov eax, [sha256_t1]
    add eax, [sha256_t2]
    mov [sha256_a], eax

    inc ecx
    cmp ecx, 64
    jb .round

    mov eax, [sha256_a]
    add [sha256_h + 0], eax
    mov eax, [sha256_b]
    add [sha256_h + 4], eax
    mov eax, [sha256_c]
    add [sha256_h + 8], eax
    mov eax, [sha256_d]
    add [sha256_h + 12], eax
    mov eax, [sha256_e]
    add [sha256_h + 16], eax
    mov eax, [sha256_f]
    add [sha256_h + 20], eax
    mov eax, [sha256_g]
    add [sha256_h + 24], eax
    mov eax, [sha256_hh]
    add [sha256_h + 28], eax

    popad
    ret

sha256_print_digest:
    pushad
    mov esi, sha256_digest
    mov ecx, 32
.loop:
    lodsb
    mov bl, al
    shr al, 4
    call sha256_print_hex_nibble
    mov al, bl
    and al, 0Fh
    call sha256_print_hex_nibble
    loop .loop
    call pm_newline
    popad
    ret

sha256_print_hex_nibble:
    push ebx
    movzx ebx, al
    mov al, [sha256_hex_digits + ebx]
    call pm_print_char
    pop ebx
    ret

sha256test:
    mov esi, sha256test_empty_msg
    call pm_print_string
    mov esi, sha256_empty_input
    xor ecx, ecx
    mov edi, sha256_digest
    call sha256_hash_buffer
    call sha256_print_digest

    mov esi, sha256test_abc_msg
    call pm_print_string
    mov esi, sha256_abc_input
    mov ecx, sha256_abc_len
    mov edi, sha256_digest
    call sha256_hash_buffer
    call sha256_print_digest
    ret

sha256_empty_input db 0
sha256_abc_input db 'abc'
sha256_abc_len equ $ - sha256_abc_input
sha256test_empty_msg db 'SHA256("") =',13,0
sha256test_abc_msg db 'SHA256("abc") =',13,0
sha256_hex_digits db '0123456789abcdef'
