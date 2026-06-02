; ============================================================
; GhostCrypto - HMAC-SHA256
; v0.2 skeleton
; ============================================================

hmac_sha256test:
    mov esi, hmac_test_key
    mov ecx, 3
    call hmac_prepare_pads

    call hmac_build_inner_input

    mov esi, hmac_inner_input
    mov ecx, HMAC_BLOCK_SIZE + hmac_test_msg_len
    mov edi, hmac_inner_hash
    call sha256_hash_buffer

    call hmac_build_outer_input

    mov esi, hmac_outer_input
    mov ecx, HMAC_BLOCK_SIZE + 32
    mov edi, hmac_final_hash
    call sha256_hash_buffer

    mov esi, hmac_sha256_test_msg
    call pm_print_string

    mov esi, hmac_ipad_msg
    call pm_print_string
    mov esi, hmac_ipad_buffer
    mov ecx, 3
    call rtl8139_dump_bytes

    mov esi, hmac_opad_msg
    call pm_print_string
    mov esi, hmac_opad_buffer
    mov ecx, 3
    call rtl8139_dump_bytes

    mov esi, hmac_inner_msg
    call pm_print_string
    mov esi, hmac_inner_hash
    mov ecx, 32
    call rtl8139_dump_bytes

    mov esi, hmac_final_msg
    call pm_print_string
    mov esi, hmac_final_hash
    mov ecx, 32
    call rtl8139_dump_bytes

    ret

hmac_sha256_hash:
    ; Generic HMAC-SHA256
    ; ESI = key ptr
    ; ECX = key len
    ; EDX = message ptr
    ; EBX = message len
    ; EDI = output ptr, 32 bytes
    ;
    ; Note: this routine currently supports keys <= 64 bytes.
    pushad

    mov [hmac_current_msg_ptr], edx
    mov [hmac_current_msg_len], ebx
    mov [hmac_output_ptr], edi

    call hmac_prepare_pads
    call hmac_build_inner_input_generic

    mov esi, hmac_inner_input
    mov ecx, HMAC_BLOCK_SIZE
    add ecx, [hmac_current_msg_len]
    mov edi, hmac_inner_hash
    call sha256_hash_buffer

    call hmac_build_outer_input

    mov esi, hmac_outer_input
    mov ecx, HMAC_BLOCK_SIZE + HMAC_HASH_SIZE
    mov edi, hmac_final_hash
    call sha256_hash_buffer

    mov esi, hmac_final_hash
    mov edi, [hmac_output_ptr]
    mov ecx, HMAC_HASH_SIZE

.copy_final:
    lodsb
    stosb
    loop .copy_final

    popad
    ret


hmac_build_inner_input_generic:
    ; builds hmac_inner_input = ipad64 || caller message
    pushad

    mov edi, hmac_inner_input
    mov esi, hmac_ipad_buffer
    mov ecx, HMAC_BLOCK_SIZE

.copy_ipad:
    lodsb
    stosb
    loop .copy_ipad

    mov esi, [hmac_current_msg_ptr]
    mov ecx, [hmac_current_msg_len]

.copy_msg:
    cmp ecx, 0
    je .done
    lodsb
    stosb
    dec ecx
    jmp .copy_msg

.done:
    popad
    ret


hmac_prepare_pads:
    ; ESI = key ptr
    ; ECX = key len
    pushad

    mov edi, hmac_ipad_buffer
    mov edx, HMAC_BLOCK_SIZE

.zero_ipad:
    mov byte [edi], HMAC_IPAD
    inc edi
    dec edx
    jnz .zero_ipad

    mov edi, hmac_opad_buffer
    mov edx, HMAC_BLOCK_SIZE

.zero_opad:
    mov byte [edi], HMAC_OPAD
    inc edi
    dec edx
    jnz .zero_opad

    mov edi, hmac_ipad_buffer
    mov ebx, hmac_opad_buffer

.xor_loop:
    cmp ecx, 0
    je .done

    mov al, [esi]
    xor [edi], al
    xor [ebx], al

    inc esi
    inc edi
    inc ebx
    dec ecx
    jmp .xor_loop

.done:
    popad
    ret

hmac_build_inner_input:
    ; builds hmac_inner_input = ipad64 || message
    pushad

    mov edi, hmac_inner_input
    mov esi, hmac_ipad_buffer
    mov ecx, HMAC_BLOCK_SIZE

.copy_ipad:
    lodsb
    stosb
    loop .copy_ipad

    mov esi, hmac_test_msg
    mov ecx, hmac_test_msg_len

.copy_msg:
    lodsb
    stosb
    loop .copy_msg

    popad
    ret


hmac_build_outer_input:
    ; builds opad64 || inner_hash32
    pushad

    mov edi, hmac_outer_input
    mov esi, hmac_opad_buffer
    mov ecx, HMAC_BLOCK_SIZE

.copy_opad:
    lodsb
    stosb
    loop .copy_opad

    mov esi, hmac_inner_hash
    mov ecx, 32

.copy_inner:
    lodsb
    stosb
    loop .copy_inner

    popad
    ret

; ============================================================
; HMAC test data/messages
; ============================================================

hmac_sha256_test_msg db 'GhostCrypto HMAC-SHA256 stub',13,0
hmac_ipad_msg db 'ipad: ',0
hmac_opad_msg db 'opad: ',0
hmac_inner_msg db 'inner: ',0
hmac_final_msg db 'final: ',0

hmac_test_key db 'key'
hmac_test_msg db 'The quick brown fox jumps over the lazy dog'
hmac_test_msg_len equ $ - hmac_test_msg

; ============================================================
; HMAC constants/buffers
; ============================================================

HMAC_BLOCK_SIZE equ 64
HMAC_HASH_SIZE  equ 32
HMAC_IPAD equ 0x36
HMAC_OPAD equ 0x5C

hmac_ipad_buffer times HMAC_BLOCK_SIZE db 0
hmac_opad_buffer times HMAC_BLOCK_SIZE db 0
hmac_inner_hash times HMAC_HASH_SIZE db 0
hmac_inner_input times 128 db 0
hmac_outer_input times 128 db 0
hmac_final_hash times 32 db 0
hmac_current_msg_ptr dd 0
hmac_current_msg_len dd 0
hmac_output_ptr dd 0

