_BSS SEGMENT
PUBLIC a@0x21af36366f0        ; a
PUBLIC func@0x21af36a8d30        ; func

a@0x21af36366f0 DD 01H DUP(?)        ; a


_TEXT SEGMENT
_k$ = -8        ; size = 4
_b$ = -4        ; size = 4
func@0x21af36a8d30 PROC
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
idiv DWORD PTR _k$[ebp - 8]
sub eax, ebx
add eax, DWORD PTR _k$[ebp - 8]
mov eax, [a@0x21af36366f0]
add eax, 1
sub eax, 3
sub eax, 7
func@0x21af36a8d30 ENDP
_TEXT ENDS
