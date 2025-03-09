
.data
includelib kernel32.lib
includelib Irvinef32.lib
ExitProcess PROTO STDCALL :DWORD
WriteConsoleA PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
GetStdHandle PROTO STDCALL :DWORD
alphabet    BYTE    "ABCDEFGHIKLMNOPQRSTUVWXYZ", 0 ; Alphabet (no 'J')
table       BYTE    25 DUP(0)                        ; 5x5 Playfair table, initialized to '\0'
concatStr   BYTE    100 DUP(0)                       ; Concatenated string buffer

plainText db  0             ; Example plaintext for testing
keyword db  0                    ; Example key for testing
separator db 'X', 0                     ; default separator

cipherText db 64 dup(?)                    ; Buffer for the resulting ciphertext

len dq 0                                    ; Length of the plaintext (use 64-bit data)
newpos1 db 0
newpos2 db 0
row1 db 0
row2 db 0
col1 db 0
col2 db 0
first db 0
second db 0
current db 0

buffer db 64 dup(0)                         ; Temporary buffer for digraph preparation
cipherMessage db "Ciphertext: ", 0          ; Message to print before ciphertext
consoleHandle dq ?                          ; Console handle (use 64-bit)
bytesWritten dq ?                           ; Number of bytes written (use 64-bit)




.code

encodeAsm PROC

    ; Saving first two arguments (input text and keyword) into r10 and r11 register respectively
    mov r10, rcx
    mov r11, rdx

    ; Overwrite the default separator by loading it from r8 (third arguments passing by function)
    mov al, r8b                     ; Load the character value directly from r8
    mov byte ptr [separator], al    ; Store it in the separator variable


    ; Clear the cipherText buffer
    lea rdi, cipherText    ; Load the address of cipherText into RDI
    mov rcx, 64            ; Set the number of bytes to clear
    xor al, al             ; Set AL to 0 (value to fill)
    cld
    rep stosb              ; Fill the buffer with 0s

    ; Clear the buffer
    lea rdi, buffer        ; Load the address of buffer into RDI
    mov rcx, 64            ; Set the number of bytes to clear
    xor al, al             ; Set AL to 0 (value to fill)
    cld
    rep stosb              ; Fill the buffer with 0s

    mov byte ptr [current], 0

    ; Convert inputs to uppercase
    mov rsi, r10
    call to_uppercase ; Check the input is uppercase or not, if not, make it to uppercase

    mov rsi, r11
    call to_uppercase ; Check the input is uppercase or not, if not, make it to uppercase

    ; Step 1: Get length of plaintext
    mov rsi, r10
    xor rcx, rcx

find_length:
    mov al, [rsi]
    cmp al, 0
    je prepare_plaintext
    inc rcx
    inc rsi
    jmp find_length

prepare_plaintext:
    ; Store length in len
    mov len, rcx
    mov rsi, r10
    lea rdi, buffer

prepare_digraphs:
    mov al, [rsi]
    cmp al, 0
    je encrypt
    inc rsi
    mov ah, [rsi]
    cmp al, ah
    jne next_char

    ; Add 'X' if characters are the same
    mov [rdi], al        ; Store first character
    inc rdi
    mov al, [separator]     ; Insert separator
    mov [rdi], al 
    inc rdi
    add len, 1          ; Adjust length counter
    jmp prepare_digraphs

next_char:
    ; Handle normal character pairs
    mov [rdi], al       ; Store first character
    inc rdi
    mov [rdi], ah       ; Store second character
    inc rdi
    inc rsi
    jmp prepare_digraphs


; Main encryption routine
encrypt:
    xor ah, ah            ; Clear AH register
    mov ax, word ptr [len] ; Move the lower 16 bits of len into ax
    mov cl, 2
    div cl                 ; Divide AX by 2, quotient in AL, remainder in AH
    cmp ah, 0
    je evenn               ; Jump to 'evenn' if AH is 0 (even)

    lea rdi, buffer       ; Load the address of buffer into RDI
    mov rax, [len]        ; Move the full 64-bit value of len into rax
    add rdi, rax          ; Add len to RDI (buffer address)
    mov al, [separator]   ; Load separator into AL
    mov [rdi], al         ; Store separator in buffer
    add qword ptr [len], 1 ; Increment len by 1 (64-bit increment)

evenn:
    mov rax, r11
    call createTable
    lea rsi, buffer
    lea rdi, cipherText

; Process each character pair
encrypt_pair:       
    mov al, [rsi]
    cmp al, 0       ; End check
    je done 

    ; Get positions in Playfair table
    mov first, al
    mov al, [rsi+1]
    mov second, al
    mov dl, [first]
    call find_position      ; Find first character position
    mov dh, al
    mov dl, [second]
    call find_position      ; Find second character position
    mov bl, al

    ; Encrypt using Playfair rules
    mov al, dh
    mov ah, bl
    call encrypt_check_positions

store_result:
    ; Load keySquare into RSI
    lea rsi, table

    ; Extract the character at newpos1 and save it into cipherText
    movzx rax, byte ptr [newpos1] ; Load newpos1 into RAX (zero-extended)
    add rsi, rax                 ; Point RSI to the position in keySquare
    mov al, byte ptr [rsi]       ; Load the character from keySquare
    mov byte ptr [rdi], al       ; Save the character to cipherText
    inc rdi                      ; Increment cipherText pointer

    ; Reset RSI to point to keySquare
    lea rsi, table

    ; Extract the character at newpos2 and save it into cipherText
    movzx rax, byte ptr [newpos2] ; Load newpos2 into RAX (zero-extended)
    add rsi, rax                  ; Point RSI to the position in keySquare
    mov al, byte ptr [rsi]        ; Load the character from keySquare
    mov byte ptr [rdi], al        ; Save the character to cipherText
    inc rdi                       ; Increment cipherText pointer

    lea rsi, buffer
    add current, 2
    movzx rax, current 
    ; Move to next pair
    add rsi, rax                ; Move the pointer to the next digraph (pair)
    jmp encrypt_pair

done:
    ; store result to the buffer
    lea rsi, cipherText      ; Load the address of cipherText (the data to copy)
    mov rdi, r9           ; Move the buffer address (passed as fourth argument) into RDI
    mov rcx, 26          ; Move the buffer size into RCX

copy_loop:
    mov al, [rsi]          ; Get the byte from cipherText (or table)
    test al, al            ; Check if the byte is zero (end of cipherText)
    jz final_done           ; If zero, stop copying

    mov [rdi], al          ; Copy the byte to the buffer
    inc rsi                ; Move to the next byte in cipherText
    inc rdi                ; Move to the next byte in the buffer
    dec rcx                ; Decrement the remaining size
    jnz copy_loop          ; Continue until all bytes are copied

final_done:
    ret

encodeAsm ENDP


; Main Decoding Procedure
decodeAsm PROC

    ; Saving first two arguments into r10 and r11 register
    mov r10, rcx
    mov r11, rdx

    ; Clear the cipherText buffer
    lea rdi, cipherText    ; Load the address of cipherText into RDI
    mov rcx, 64            ; Set the number of bytes to clear
    xor al, al             ; Set AL to 0 (value to fill)
    cld
    rep stosb              ; Fill the buffer with 0s

    ; Clear the buffer
    lea rdi, buffer        ; Load the address of buffer into RDI
    mov rcx, 64            ; Set the number of bytes to clear
    xor al, al             ; Set AL to 0 (value to fill)
    cld
    rep stosb              ; Fill the buffer with 0s

     mov byte ptr [current], 0

    mov rsi, r10
    call to_uppercase ; Check the input is uppercase or not, if not, make it to uppercase

    mov rsi, r11
    call to_uppercase ; Check the input is uppercase or not, if not, make it to upperca

    ; Step 1: Get length of plaintext
    mov rsi, r10        ; Load ciphertext address
    xor rcx, rcx        ; Initialize length counter

find_length:
    mov al, [rsi]           ; Load current character
    cmp al, 0               ; Check for null terminator
    je prepare_plaintext    ; Jump if end of string
    inc rcx                 ; Increment length counter
    inc rsi                 ; Move to next character
    jmp find_length



; Prepare ciphertext for processing
prepare_plaintext:
    mov len, rcx        ; Store ciphertext length
    mov rsi, r10        ; Reset ciphertext pointer
    lea rdi, buffer     ; Point to working buffer


; Split ciphertext into digraphs (character pairs)
prepare_digraphs:
    mov al, [rsi]       ; Load first character
    cmp al, 0           ; End of string check
    je decrypt          ; Jump to decryption if done
    inc rsi             ; Move to next character
    mov ah, [rsi]       ; Load second character


    ; Handle normal character pairs
    mov [rdi], al
    inc rdi
    mov [rdi], ah
    inc rdi
    inc rsi
    jmp prepare_digraphs


; Main decryption routine
decrypt:

    ; Generate Playfair table from keyword
    mov rax, r11
    call createTable

    
    ; Set up pointers for processing
    lea rsi, buffer
    lea rdi, cipherText


; Process each character pair
decrypt_pair:
    mov al, [rsi]        ; Load first character
    cmp al, 0            ; End of buffer check
    je done
    mov first, al       ; Store first character
    mov al, [rsi+1]
    mov second, al      ; Store second character
    mov dl, [first]
    call find_position
    mov dh, al

    mov dl, [second]
    call find_position
    mov bl, al

    ; Decrypt using Playfair rules
    mov al, dh
    mov ah, bl
    call decrypt_check_positions

store_result:
    ; Load keySquare into RSI
    lea rsi, table

    ; Extract the character at newpos1 and save it into cipherText
    movzx rax, byte ptr [newpos1] ; Load newpos1 into RAX (zero-extended)
    add rsi, rax                 ; Point RSI to the position in keySquare
    mov al, byte ptr [rsi]       ; Load the character from keySquare
    mov byte ptr [rdi], al       ; Save the character to cipherText
    inc rdi                      ; Increment cipherText pointer

    ; Reset RSI to point to keySquare
    lea rsi, table

    ; Extract the character at newpos2 and save it into cipherText
    movzx rax, byte ptr [newpos2] ; Load newpos2 into RAX (zero-extended)
    add rsi, rax                 ; Point RSI to the position in keySquare
    mov al, byte ptr [rsi]       ; Load the character from keySquare
    mov byte ptr [rdi], al       ; Save the character to cipherText
    inc rdi                      ; Increment cipherText pointer

    lea rsi, buffer
    add current, 2
    movzx rax, current 
    ; Move to next pair
    add rsi, rax                ; Move the pointer to the next digraph (pair)
    jmp decrypt_pair

done:
    ; Check for duplicate characters in ciphertext (debug)
    lea rdi, cipherText
    mov al, [rdi]
    mov ah, [rdi+1]
    cmp al, ah

    ; store result to the buffer
    lea rsi, cipherText      ; Load the address of cipherText (the data to copy)
    mov rdi, r9           ; Move the buffer address (passed as fourth argument) into RDI
    mov rcx, 26          ; Move the buffer size into RCX

copy_loop:
    mov al, [rsi]          ; Get the byte from cipherText (or table)
    test al, al            ; Check if the byte is zero (end of cipherText)
    jz final_done           ; If zero, stop copying

    mov [rdi], al          ; Copy the byte to the buffer
    inc rsi                ; Move to the next byte in cipherText
    inc rdi                ; Move to the next byte in the buffer
    dec rcx                ; Decrement the remaining size
    jnz copy_loop          ; Continue until all bytes are copied

final_done:
    ret

decodeAsm ENDP


find_position PROC
    ; Finds row and column of a character in the key square
    lea rsi, table
    xor rcx, rcx

find_pos_loop:
    mov al, [rsi + rcx]
    cmp al, dl
    je found
    inc rcx
    cmp rcx, 25
    jne find_pos_loop

found:
    mov al, cl
    ret
find_position ENDP


encrypt_check_positions PROC
    ; Save original registers
    push rax
    push rbx
    push rdx

    ; Check if in the same column
    mov bl, al          ; Copy index of first character to BL
    mov bh, ah          ; Copy index of second character to BH
    mov dl, 5           ; Column width of the Playfair square

    ; Clear DX and divide BL by DL (find row of first character)
    xor ah, ah          ; Clear high byte of AX
    mov al, bl          ; Load BL into AL (16-bit AX = 0:BL)
    div dl              ; Divide AX by DL (DL = 5)
    mov dh, al          ; Save row of the first character to DH
    mov [row1], al
    mov [col1], ah

    ; Clear DX and divide BH by DL (find row of second character)
    xor ah, ah          ; Clear high byte of AX
    mov al, bh          ; Load BH into AL (16-bit AX = 0:BH)
    div dl              ; Divide AX by DL (DL = 5)
    cmp dh, al          ; Compare rows
    mov ch, al
    mov [row2], al
    mov [col2], ah
    je same_row         ; Jump if rows are the same

    ; Check column
    ; Compute column of first character
    xor ah, ah          ; Clear high byte of AX

    mov al, bl          ; Load BL into AL (16-bit AX = 0:BL)
    div dl              ; AL = column of first character
    mov cl, ah          ; Save column of first character to CL

    ; Compute column of second character
    xor ah, ah          ; Clear high byte of AX
    mov al, bh          ; Load BH into AL (16-bit AX = 0:BH)
    div dl              ; AL = column of second character
    cmp cl, ah          ; Compare columns
    mov [col1], cl
    je same_column      ; Jump if columns are the same

different_row_and_column:
    ; Code for different rows and columns case
    ; Rule: Form a rectangle with the two characters
    ; Replace each character with the one in its row but in the column of the other character

    ; Find the new character for the first position
    mov dl, [col2]          ; Column of the second character (in AH)
    mov al, [row1]          ; Row of the first character (in DH)
    mov bl, dl              ; Save column of second character
    mov cl, 5
    mul cl                  ; Multiply row by 5 to get row offset
    add bl, al              ; Add column to calculate new position
    mov dh, bl              ; New index for the first character

    ; Find the new character for the second position
    mov dl, [col1]          ; Column of the first character (in CL)
    mov al, [row2]          ; Row of the second character (in AL)
    mov bl, dl              ; Save column of first character
    mov cl, 5
    mul cl                  ; Multiply row by 5 to get row offset
    add bl, al              ; Add column to calculate new position
    mov bh, bl              ; New index for the second character

    jmp endcheck

same_row:
    ; Rule: Shift each character to the right within the same row
    ; Wrap around if at the end of the row

    ; Find the column for the first character
    mov bl, [col1]          ; Column of the first character
    inc bl                  ; Move to the next column
    cmp bl, 5               ; Wrap around if column exceeds 4
    jne skip_first_wrap
    mov bl, 0
skip_first_wrap:
    ; Calculate the new position for the first character
    mov al, [row1]          ; Row of the first character
    mov cl, 5
    mul cl                  ; Multiply row by 5
    add al, bl              ; Add the new column
    mov dh, al              ; New index for the first character

    ; Find the column for the second character
    mov bl, [col2]          ; Column of the second character
    inc bl                  ; Move to the next column
    cmp bl, 5               ; Wrap around if column exceeds 4
    jne skip_second_wrap
    mov bl, 0
skip_second_wrap:
    ; Calculate the new position for the second character
    mov al, [row2]          ; Row of the second character
    mov cl, 5
    mul cl                  ; Multiply row by 5
    add al, bl              ; Add the new column
    mov bh, al              ; New index for the second character

    jmp endcheck

same_column:
    ; Code for same column case
    ; Rule: Shift each character down within the same column
    ; Wrap around if at the bottom of the column

    ; Find the row for the first character
    mov al, [row1]          ; Row of the first character
    inc al                  ; Move to the next row
    cmp al, 5               ; Wrap around if row exceeds 4
    jne skip_first_row_wrap
    mov al, 0
skip_first_row_wrap:
    ; Calculate the new position for the first character
    mov bl, al              ; Save new row
    mov cl, 5
    mul cl                  ; Multiply row by 5
    add al, [col1]          ; Add the column
    mov dh, al              ; New index for the first character

    ; Find the row for the second character
    mov al, [row2]          ; Row of the second character
    inc al                  ; Move to the next row
    cmp al, 5               ; Wrap around if row exceeds 4
    jne skip_second_row_wrap
    mov al, 0
skip_second_row_wrap:
    ; Calculate the new position for the second character
    mov bl, al              ; Save new row
    mov cl, 5
    mul cl                  ; Multiply row by 5
    add al, [col2]          ; Add the column
    mov bh, al              ; New index for the second character

    jmp endcheck

endcheck:
    ; Restore original registers
    mov [newpos1], dh
    mov [newpos2], bh
    pop rdx
    pop rbx
    pop rax
    ret
encrypt_check_positions ENDP

decrypt_check_positions PROC
    ; Save original registers
    push rax
    push rbx
    push rdx

    ; Check if in the same column
    mov bl, al          ; Copy index of first character to BL
    mov bh, ah          ; Copy index of second character to BH
    mov dl, 5           ; Column width of the Playfair square

    ; Clear DX and divide BL by DL (find row of first character)
    xor ah, ah          ; Clear high byte of AX
    mov al, bl          ; Load BL into AL (16-bit AX = 0:BL)
    div dl              ; Divide AX by DL (DL = 5)
    mov dh, al          ; Save row of the first character to DH
    mov [row1], al
    mov [col1], ah

    ; Clear DX and divide BH by DL (find row of second character)
    xor ah, ah          ; Clear high byte of AX
    mov al, bh          ; Load BH into AL (16-bit AX = 0:BH)
    div dl              ; Divide AX by DL (DL = 5)
    mov [row2], al
    mov [col2], ah
    cmp dh, al          ; Compare rows
    je same_row         ; Jump if rows are the same

    ; Check column
    ; Compute column of first character
    xor ah, ah          ; Clear high byte of AX
    mov al, bl          ; Load BL into AL (16-bit AX = 0:BL)
    div dl              ; AL = column of first character
    mov cl, ah          ; Save column of first character to CL

    ; Compute column of second character
    xor ah, ah          ; Clear high byte of AX
    mov al, bh          ; Load BH into AL (16-bit AX = 0:BH)
    div dl              ; AL = column of second character
    cmp cl, ah          ; Compare columns
    mov [col1], cl
    je same_column      ; Jump if columns are the same

different_row_and_column:
    ; Code for different rows and columns case
    ; Rule: Form a rectangle with the two characters
    ; Replace each character with the one in its row but in the column of the other character

    ; Find the new character for the first position
    mov dl, [col2]          ; Column of the second character
    mov al, [row1]          ; Row of the first character
    mov cl, 5
    mul cl                  ; Multiply row by 5 to get row offset
    add al, dl              ; Add column to calculate new position
    mov dh, al              ; New index for the first character

    ; Find the new character for the second position
    mov dl, [col1]          ; Column of the first character
    mov al, [row2]          ; Row of the second character
    mov cl, 5
    mul cl                  ; Multiply row by 5 to get row offset
    add al, dl              ; Add column to calculate new position
    mov bh, al              ; New index for the second character

    jmp endcheck

same_row:
    ; Rule: Shift each character to the **left** within the same row
    ; Wrap around if at the start of the row

    ; Find the column for the first character
    mov bl, [col1]          ; Column of the first character
    dec bl                  ; Move to the previous column
    js wrap_first_left      ; Wrap around if column goes below 0
    jmp skip_first_wrap

wrap_first_left:
    mov bl, 4               ; Wrap to the last column

skip_first_wrap:
    ; Calculate the new position for the first character
    mov al, [row1]          ; Row of the first character
    mov cl, 5
    mul cl                  ; Multiply row by 5
    add al, bl              ; Add the new column
    mov dh, al              ; New index for the first character

    ; Find the column for the second character
    mov bl, [col2]          ; Column of the second character
    dec bl                  ; Move to the previous column
    js wrap_second_left     ; Wrap around if column goes below 0
    jmp skip_second_wrap

wrap_second_left:
    mov bl, 4               ; Wrap to the last column

skip_second_wrap:
    ; Calculate the new position for the second character
    mov al, [row2]          ; Row of the second character
    mul cl                  ; Multiply row by 5
    add al, bl              ; Add the new column
    mov bh, al              ; New index for the second character

    jmp endcheck

same_column:
    ; Rule: Shift each character **up** within the same column
    ; Wrap around if at the top of the column

    ; Find the row for the first character
    mov al, [row1]          ; Row of the first character
    dec al                  ; Move to the previous row
    js wrap_first_up        ; Wrap around if row goes below 0
    jmp skip_first_up

wrap_first_up:
    mov al, 4               ; Wrap to the last row

skip_first_up:
    ; Calculate the new position for the first character
    mov bl, al              ; Save new row
    mov cl, 5
    mul cl                  ; Multiply row by 5
    add al, [col1]          ; Add the column
    mov dh, al              ; New index for the first character

    ; Find the row for the second character
    mov al, [row2]          ; Row of the second character
    dec al                  ; Move to the previous row
    js wrap_second_up       ; Wrap around if row goes below 0
    jmp skip_second_up

wrap_second_up:
    mov al, 4               ; Wrap to the last row

skip_second_up:
    ; Calculate the new position for the second character
    mov bl, al              ; Save new row
    mov cl, 5
    mul cl                  ; Multiply row by 5
    add al, [col2]          ; Add the column
    mov bh, al              ; New index for the second character

endcheck:
    ; Restore original registers
    mov [newpos1], dh
    mov [newpos2], bh
    pop rdx
    pop rbx
    pop rax
    ret
decrypt_check_positions ENDP


createTable PROC

    ; clear the concatStr
    lea rdi, concatStr  ; Load the address of concatStr into RDI
    mov rcx, 100        ; Set the number of bytes to clear (100 bytes)
    xor al, al          ; Set AL to 0 (value to fill)
    cld                 ; Clear the direction flag to ensure forward movement
    rep stosb           ; Fill the buffer with 0s

    ; clear the table
    lea rdi, table         ; Load the address of the table into RDI
    mov rcx, 25            ; Set RCX to the number of bytes to clear (25)
    xor al, al             ; Set AL to 0 (value to clear the table with)
    cld
    rep stosb              ; Repeat STOSB (store AL into each byte) RCX times

    mov rdi, r11             ; Load address of the keyword into RDI

    ; Check if the keyword address is null
    cmp rdi, 0                   ; Check if the address is null
    je  error_null_keyword       ; If null, jump to error handler

    ; Load address to RSI after clearing the table
    lea rsi, concatStr           ; Load address of concatStr into RSI (destination buffer)

concat_keyword:
    mov al, [rdi]                ; Load byte from [RDI] (keyword) into AL
    cmp al, 0                    ; Check for null terminator
    je  concat_alphabet          ; If null, jump to concatenate alphabet
    mov [rsi], al                ; Store AL into [RSI] (concatStr)
    inc rdi                      ; Increment RDI to the next character in keyword
    inc rsi                      ; Increment RSI to the next position in concatStr
    jmp concat_keyword           ; Repeat

concat_alphabet:
    lea rdi, alphabet            ; Load address of alphabet into RDI
concat_loop:
    mov al, [rdi]                ; Load byte from [RDI] (alphabet) into AL
    cmp al, 0                    ; Check for null terminator
    je  transform_to_uppercase   ; If null, move to next step
    mov [rsi], al                ; Store AL into [RSI] (concatStr)
    inc rdi                      ; Increment RDI to the next character in alphabet
    inc rsi                      ; Increment RSI to the next position in concatStr
    jmp concat_loop              ; Repeat

transform_to_uppercase:
    lea rsi, concatStr
    call to_uppercase

populate_table:
    lea rsi, concatStr           ; Start of concatenated string
    lea rdi, table               ; Start of 5x5 table
populate_outer:
    mov al, [rsi]                ; Load character from concatStr
    cmp al, 0                    ; Null terminator check
    je  done                     ; End if null

    ; Check for duplicates
    lea rdx, table               ; Address of table for duplicate check
check_duplicate:
    mov bl, [rdx]                ; Load byte from table
    cmp bl, 0                    ; End of table check
    je  add_to_table             ; If end of table, no duplicate
    cmp bl, al                   ; Compare with current character
    je  skip_character           ; If duplicate, skip
    inc rdx                      ; Move to the next byte in table
    jmp check_duplicate
add_to_table:
    mov [rdi], al                 ; Add character to the table
    inc rdi                       ; Move to the next cell in the table
    inc rsi                       ; Move to the next character in concatStr
    jmp populate_outer
skip_character:
    inc rsi                       ; Move to the next character in concatStr
    jmp populate_outer

done:
    ret

error_null_keyword:
    ; Handle null keyword error here (optional: set an error code, etc.)
    mov rax, 1  ; Set RAX to an error code (e.g., 1 for null error)
    ret

createTable ENDP


to_uppercase PROC
    ; Input: Address of the string in RSI
    ; Output: The string at RSI is converted to uppercase
    ; Assumes strings are null-terminated

start_conversion:
    mov al, [rsi]         ; Load the current character
    cmp al, 0             ; Check for null terminator
    je done_conversion    ; Exit if end of string
    cmp al, 'a'           ; Check if character is lowercase
    jb skip_conversion    ; Skip if it's not a letter below 'a'
    cmp al, 'z'           ; Check if character is above 'z'
    ja skip_conversion    ; Skip if it's not a letter above 'z'
    sub al, 32            ; Convert to uppercase
    mov [rsi], al         ; Store the uppercase character

skip_conversion:
    inc rsi               ; Move to the next character
    jmp start_conversion  ; Repeat for the next character

done_conversion:
    ret
to_uppercase ENDP



END
