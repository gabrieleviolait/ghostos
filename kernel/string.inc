; Small shell string helpers.

skip_spaces:
    cmp byte [si], ' '
    jne .done
    inc si
    jmp skip_spaces
.done:
    ret

skip_first_word:
    call skip_spaces
.loop:
    cmp byte [si], 0
    je .done
    cmp byte [si], ' '
    je .done
    inc si
    jmp .loop
.done:
    ret

starts_with_word:
    pusha
.loop:
    mov al, [di]
    cmp al, 0
    je .word_end
    cmp al, [si]
    jne .no
    inc si
    inc di
    jmp .loop

.word_end:
    mov al, [si]
    cmp al, 0
    je .yes
    cmp al, ' '
    je .yes

.no:
    popa
    clc
    ret

.yes:
    popa
    stc
    ret
