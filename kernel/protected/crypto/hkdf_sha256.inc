; ============================================================
; GhostCrypto - HKDF-SHA256
; v0.5
; RFC 5869 Test Case 1
; ============================================================

hkdf_sha256test:
    pushad

    mov byte [hkdf_fail_flag], 0

    ; HKDF-Extract(PRK) = HMAC-SHA256(salt, IKM)
    mov esi, hkdf_test_salt
    mov ecx, hkdf_test_salt_len
    mov edx, hkdf_test_ikm
    mov ebx, hkdf_test_ikm_len
    mov edi, hkdf_prk
    call hmac_sha256_hash

    mov esi, hkdf_prk
    mov edi, hkdf_expected_prk
    mov ecx, HKDF_HASH_SIZE
    call hkdf_memcmp
    mov esi, hkdf_prk_msg
    call hkdf_print_result

    ; T(1) = HMAC-SHA256(PRK, info || 0x01)
    call hkdf_build_t1_input

    mov esi, hkdf_prk
    mov ecx, HKDF_HASH_SIZE
    mov edx, hkdf_t_input
    mov ebx, hkdf_t1_input_len
    mov edi, hkdf_t1
    call hmac_sha256_hash

    ; T(2) = HMAC-SHA256(PRK, T(1) || info || 0x02)
    call hkdf_build_t2_input

    mov esi, hkdf_prk
    mov ecx, HKDF_HASH_SIZE
    mov edx, hkdf_t_input
    mov ebx, hkdf_t2_input_len
    mov edi, hkdf_t2
    call hmac_sha256_hash

    ; OKM = first 42 bytes of T(1) || T(2)
    call hkdf_build_okm

    mov esi, hkdf_okm
    mov edi, hkdf_expected_okm
    mov ecx, hkdf_test_okm_len
    call hkdf_memcmp
    mov esi, hkdf_okm_msg
    call hkdf_print_result

    ; Final aggregate result
    mov eax, 1
    cmp byte [hkdf_fail_flag], 0
    je .print_final
    xor eax, eax

.print_final:
    mov esi, hkdf_final_msg
    call hkdf_print_result

    popad
    ret


hkdf_build_t1_input:
    pushad

    mov edi, hkdf_t_input
    mov esi, hkdf_test_info
    mov ecx, hkdf_test_info_len

.copy_info:
    lodsb
    stosb
    loop .copy_info

    mov al, 1
    stosb

    popad
    ret


hkdf_build_t2_input:
    pushad

    mov edi, hkdf_t_input

    mov esi, hkdf_t1
    mov ecx, HKDF_HASH_SIZE

.copy_t1:
    lodsb
    stosb
    loop .copy_t1

    mov esi, hkdf_test_info
    mov ecx, hkdf_test_info_len

.copy_info:
    lodsb
    stosb
    loop .copy_info

    mov al, 2
    stosb

    popad
    ret


hkdf_build_okm:
    pushad

    mov edi, hkdf_okm

    mov esi, hkdf_t1
    mov ecx, HKDF_HASH_SIZE

.copy_t1:
    lodsb
    stosb
    loop .copy_t1

    mov esi, hkdf_t2
    mov ecx, 10

.copy_t2_prefix:
    lodsb
    stosb
    loop .copy_t2_prefix

    popad
    ret


hkdf_memcmp:
    ; ESI = actual
    ; EDI = expected
    ; ECX = length
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


hkdf_print_result:
    ; ESI = label
    ; EAX = 1 pass, 0 fail
    push eax

    call pm_print_string

    pop eax
    cmp eax, 1
    je .pass

    mov byte [hkdf_fail_flag], 1
    mov esi, hkdf_fail_msg
    call pm_print_string
    ret

.pass:
    mov esi, hkdf_pass_msg
    call pm_print_string
    ret


; ============================================================
; Messages
; ============================================================

hkdf_prk_msg db 'PRK ',0
hkdf_okm_msg db 'OKM ',0
hkdf_final_msg db 'HKDF ',0
hkdf_pass_msg db 'PASS',13,0
hkdf_fail_msg db 'FAIL',13,0

; ============================================================
; RFC 5869 Test Case 1
; ============================================================

HKDF_HASH_SIZE equ 32

hkdf_test_ikm:
    times 22 db 00Bh
hkdf_test_ikm_len equ $ - hkdf_test_ikm

hkdf_test_salt:
    db 000h,001h,002h,003h,004h,005h,006h
    db 007h,008h,009h,00Ah,00Bh,00Ch
hkdf_test_salt_len equ $ - hkdf_test_salt

hkdf_test_info:
    db 0F0h,0F1h,0F2h,0F3h,0F4h
    db 0F5h,0F6h,0F7h,0F8h,0F9h
hkdf_test_info_len equ $ - hkdf_test_info

hkdf_test_okm_len equ 42

hkdf_expected_prk:
    db 007h,077h,009h,036h,02Ch,02Eh,032h,0DFh
    db 00Dh,0DCh,03Fh,00Dh,0C4h,07Bh,0BAh,063h
    db 090h,0B6h,0C7h,03Bh,0B5h,00Fh,09Ch,031h
    db 022h,0ECh,084h,04Ah,0D7h,0C2h,0B3h,0E5h

hkdf_expected_okm:
    db 03Ch,0B2h,05Fh,025h,0FAh,0ACh,0D5h,07Ah
    db 090h,043h,04Fh,064h,0D0h,036h,02Fh,02Ah
    db 02Dh,02Dh,00Ah,090h,0CFh,01Ah,05Ah,04Ch
    db 05Dh,0B0h,02Dh,056h,0ECh,0C4h,0C5h,0BFh
    db 034h,000h,072h,008h,0D5h,0B8h,087h,018h
    db 058h,065h

; ============================================================
; Buffers
; ============================================================

hkdf_fail_flag db 0

hkdf_prk times HKDF_HASH_SIZE db 0
hkdf_t1 times HKDF_HASH_SIZE db 0
hkdf_t2 times HKDF_HASH_SIZE db 0
hkdf_okm times 64 db 0
hkdf_t_input times 96 db 0

hkdf_t1_input_len equ hkdf_test_info_len + 1
hkdf_t2_input_len equ HKDF_HASH_SIZE + hkdf_test_info_len + 1
