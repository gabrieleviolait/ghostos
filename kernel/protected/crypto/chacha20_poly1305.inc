; ============================================================
; GhostCrypto - ChaCha20-Poly1305 AEAD MAC construction
; RFC 8439 section 2.8.2 MAC/tag test only.
; ============================================================

[BITS 32]

AEAD_MAC_TEST_AAD_LEN equ 12
AEAD_MAC_TEST_CT_LEN equ 114
AEAD_MAC_TEST_AAD_PAD_LEN equ 4
AEAD_MAC_TEST_CT_PAD_LEN equ 14
AEAD_MAC_TEST_INPUT_LEN equ 160
AEAD_DECRYPT_BLOCK_LEN equ 64

aeadmactest:
    pushad

    mov esi, aead_mac_test_key
    mov edx, aead_mac_test_nonce
    xor eax, eax
    mov edi, aead_mac_chacha_block
    call chacha20_block

    call aead_mac_build_test_input

    mov esi, aead_mac_chacha_block
    mov edx, aead_mac_input
    mov ebx, AEAD_MAC_TEST_INPUT_LEN
    mov edi, aead_mac_test_tag
    call poly1305_mac

    mov esi, aead_mac_test_tag
    mov edi, aead_mac_expected_tag
    mov ecx, 16
    call poly1305_memcmp

    cmp eax, 1
    je .pass

    mov esi, aead_mac_fail_msg
    call pm_print_string
    jmp .done

.pass:
    mov esi, aead_mac_pass_msg
    call pm_print_string

.done:
    popad
    ret

aeaddecrypttest:
    pushad

    call aead_decrypt_test_ciphertext

    mov esi, aead_mac_input
    mov edi, aead_decrypt_expected_plaintext
    mov ecx, AEAD_MAC_TEST_CT_LEN
    call poly1305_memcmp

    cmp eax, 1
    je .pass

    mov esi, aead_decrypt_fail_msg
    call pm_print_string
    jmp .done

.pass:
    mov esi, aead_decrypt_pass_msg
    call pm_print_string

.done:
    popad
    ret

aead_mac_build_test_input:
    pushad

    mov edi, aead_mac_input
    xor eax, eax
    mov ecx, AEAD_MAC_TEST_INPUT_LEN
    cld
    rep stosb

    mov esi, aead_mac_test_aad
    mov edi, aead_mac_input
    mov ecx, AEAD_MAC_TEST_AAD_LEN
    cld
    rep movsb

    mov esi, aead_mac_test_ciphertext
    mov edi, aead_mac_input + AEAD_MAC_TEST_AAD_LEN + AEAD_MAC_TEST_AAD_PAD_LEN
    mov ecx, AEAD_MAC_TEST_CT_LEN
    cld
    rep movsb

    mov edi, aead_mac_input + AEAD_MAC_TEST_INPUT_LEN - 16
    mov eax, AEAD_MAC_TEST_AAD_LEN
    call aead_mac_store64_le
    add edi, 8
    mov eax, AEAD_MAC_TEST_CT_LEN
    call aead_mac_store64_le

    popad
    ret

aead_mac_store64_le:
    ; EAX = low 32 bits, EDI = destination. High 32 bits are zero for this test.
    mov [edi], al
    shr eax, 8
    mov [edi + 1], al
    shr eax, 8
    mov [edi + 2], al
    shr eax, 8
    mov [edi + 3], al
    mov dword [edi + 4], 0
    ret

aead_decrypt_test_ciphertext:
    pushad

    mov dword [aead_decrypt_src_ptr], aead_mac_test_ciphertext
    mov dword [aead_decrypt_dst_ptr], aead_mac_input
    mov dword [aead_decrypt_remaining], AEAD_MAC_TEST_CT_LEN
    mov dword [aead_decrypt_counter], 1

.next_block:
    cmp dword [aead_decrypt_remaining], 0
    je .done

    mov esi, aead_mac_test_key
    mov edx, aead_mac_test_nonce
    mov eax, [aead_decrypt_counter]
    mov edi, aead_mac_chacha_block
    call chacha20_block

    mov eax, [aead_decrypt_remaining]
    cmp eax, AEAD_DECRYPT_BLOCK_LEN
    jbe .take_ready
    mov eax, AEAD_DECRYPT_BLOCK_LEN

.take_ready:
    mov [aead_decrypt_take_len], eax

    mov esi, [aead_decrypt_src_ptr]
    mov edi, [aead_decrypt_dst_ptr]
    mov edx, aead_mac_chacha_block
    mov ecx, [aead_decrypt_take_len]

.xor_bytes:
    cmp ecx, 0
    je .block_done
    mov al, [esi]
    xor al, [edx]
    mov [edi], al
    inc esi
    inc edi
    inc edx
    dec ecx
    jmp .xor_bytes

.block_done:
    mov eax, [aead_decrypt_take_len]
    add dword [aead_decrypt_src_ptr], eax
    add dword [aead_decrypt_dst_ptr], eax
    sub dword [aead_decrypt_remaining], eax
    inc dword [aead_decrypt_counter]
    jmp .next_block

.done:
    popad
    ret

aead_mac_pass_msg db 'AEAD MAC PASS',13,0
aead_mac_fail_msg db 'AEAD MAC FAIL',13,0
aead_decrypt_pass_msg db 'AEAD DECRYPT PASS',13,0
aead_decrypt_fail_msg db 'AEAD DECRYPT FAIL',13,0

aead_mac_test_aad:
    db 050h, 051h, 052h, 053h, 0C0h, 0C1h, 0C2h, 0C3h
    db 0C4h, 0C5h, 0C6h, 0C7h

aead_mac_test_key:
    db 080h, 081h, 082h, 083h, 084h, 085h, 086h, 087h
    db 088h, 089h, 08Ah, 08Bh, 08Ch, 08Dh, 08Eh, 08Fh
    db 090h, 091h, 092h, 093h, 094h, 095h, 096h, 097h
    db 098h, 099h, 09Ah, 09Bh, 09Ch, 09Dh, 09Eh, 09Fh

aead_mac_test_nonce:
    db 007h, 000h, 000h, 000h, 040h, 041h, 042h, 043h
    db 044h, 045h, 046h, 047h

aead_mac_test_ciphertext:
    db 0D3h, 01Ah, 08Dh, 034h, 064h, 08Eh, 060h, 0DBh
    db 07Bh, 086h, 0AFh, 0BCh, 053h, 0EFh, 07Eh, 0C2h
    db 0A4h, 0ADh, 0EDh, 051h, 029h, 06Eh, 008h, 0FEh
    db 0A9h, 0E2h, 0B5h, 0A7h, 036h, 0EEh, 062h, 0D6h
    db 03Dh, 0BEh, 0A4h, 05Eh, 08Ch, 0A9h, 067h, 012h
    db 082h, 0FAh, 0FBh, 069h, 0DAh, 092h, 072h, 08Bh
    db 01Ah, 071h, 0DEh, 00Ah, 09Eh, 006h, 00Bh, 029h
    db 005h, 0D6h, 0A5h, 0B6h, 07Eh, 0CDh, 03Bh, 036h
    db 092h, 0DDh, 0BDh, 07Fh, 02Dh, 077h, 08Bh, 08Ch
    db 098h, 003h, 0AEh, 0E3h, 028h, 009h, 01Bh, 058h
    db 0FAh, 0B3h, 024h, 0E4h, 0FAh, 0D6h, 075h, 094h
    db 055h, 085h, 080h, 08Bh, 048h, 031h, 0D7h, 0BCh
    db 03Fh, 0F4h, 0DEh, 0F0h, 08Eh, 04Bh, 07Ah, 09Dh
    db 0E5h, 076h, 0D2h, 065h, 086h, 0CEh, 0C6h, 04Bh
    db 061h, 016h

aead_mac_expected_tag:
    db 01Ah, 0E1h, 00Bh, 059h, 04Fh, 009h, 0E2h, 06Ah
    db 07Eh, 090h, 02Eh, 0CBh, 0D0h, 060h, 006h, 091h

aead_decrypt_expected_plaintext:
    db 'Ladies and Gentlemen of the class of ',027h,'99: If I could offer you only one tip for the future, sunscreen would be it.'

aead_decrypt_src_ptr dd 0
aead_decrypt_dst_ptr dd 0
aead_decrypt_remaining dd 0
aead_decrypt_counter dd 0
aead_decrypt_take_len dd 0
aead_mac_chacha_block times 64 db 0
aead_mac_input times AEAD_MAC_TEST_INPUT_LEN db 0
aead_mac_test_tag times 16 db 0
