_BSS SEGMENT
PUBLIC a@0x26fc93bb780        ; a
PUBLIC func@0x26fc93bac30        ; func

a@0x26fc93bb780 DD 01H DUP(?)        ; a


_TEXT SEGMENT
_k$ = -4        ; size = 4
func@0x26fc93bac30 PROC
mov eax, 0
sub eax, 12
sub eax, 1
mov eax, 2
imul ebx
mov eax, 3
imul 4
mov eax, 2
add eax, ebx
mov eax, 7
imul 12
idiv [_k$]
sub eax, ebx
add eax, [_k$]
mov eax, [a@0x26fc93bb780]
add eax, 1
sub eax, 3
sub eax, 7
func@0x26fc93bac30 ENDP
_TEXT ENDS
