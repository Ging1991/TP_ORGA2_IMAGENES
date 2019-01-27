%include "io.inc"

section .data

section .text
    global _enmascarar_asm    

_enmascarar_asm:

    ; Generales
    push EBP     ; guarda stack frame llamador
    mov EBP, ESP ; EBP apunta al frame actual
    
    ; Limpio todo
    MOV EAX, 0
    MOV EBX, 0
    MOV ECX, 0
    MOV EDX, 0

    ; Valores iniciales
    XOR EDX, EDX
    MOV EAX, DWORD[EBP+20]
    add EBX, 8
    DIV EBX
    MOV ECX, EAX
    

    CICLO:
 
        MOV EAX, ECX
        MOV EBX, 8
        MUL EBX
        SUB EAX, 8

        ; Muevo la imagen 1
        MOV EBX,[EBP+8]
        ADD EBX, EAX
        MOVQ MM0 ,[EBX]
        
        ; Muevo la imagen 2
        MOV EBX, [EBP+12]
        ADD EBX, EAX
        MOVQ MM1,[EBX]
        
        ; Muevo la mascara
        MOV EBX, [EBP+16]
        ADD EBX, EAX
        MOVQ MM2,[EBX]

        ; Operaciones para enmascarar
        PAND MM1, MM2   ;Elimino de la segunda imagen lo que no corresponde
        PANDN MM2, MM0  ;Elimino de la primera imagen lo que no corresponde
        POR MM1, MM2    ;Combino ambas imagenes
        
        ; Guardo en imagen 1 el resultado
        mov EBX, [EBP+8] ; img1
        ADD EBX, EAX
        MOVQ [EBX], MM1

        LOOP CICLO

    EMMS ; restaura la pila
    
    ; Generales
    mov ESP, EBP ; restaura ESP
    pop EBP ; restaura EBP
        
ret