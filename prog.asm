_BSS SEGMENT
PUBLIC a@0x1c7615b26f0        ; a
PUBLIC func@0x1c7615cc5e0        ; func
PUBLIC func2@0x1c76164de90        ; func2

a@0x1c7615b26f0 DD 01H DUP(?)        ; a


_TEXT SEGMENT
_k$ = -8        ; size = 4
_b$ = -4        ; size = 4
func@0x1c7615cc5e0 PROC
mov eax, 0
sub eax, 12
sub eax, 1
mov ebx, 2
imul ebx, eax
mov eax, 3
imul eax, 4
mov ecx, 2
add ecx, eax
mov eax, 7
imul eax, 12
mov esi, eax
cdq
idiv DWORD PTR _k$[ebp - 8]
sub ecx, esi
add ecx, DWORD PTR _k$[ebp - 8]
mov edx, [a@0x1c7615b26f0]
add edx, 1
sub edx, 3
sub edx, 7
func@0x1c7615cc5e0 ENDP
_TEXT ENDS

_TEXT SEGMENT
_k2$ = -8        ; size = 4
_b2$ = -4        ; size = 4
func2@0x1c76164de90 PROC
mov eax, 0
sub eax, 12
sub eax, 1
mov ebx, 2
imul ebx, eax
mov eax, 3
imul eax, 4
mov ecx, 2
add ecx, eax
mov eax, 7
imul eax, 12
mov esi, eax
cdq
idiv DWORD PTR _k2$[ebp - 8]
sub ecx, esi
add ecx, DWORD PTR _k2$[ebp - 8]
mov edx, [a@0x1c7615b26f0]
add edx, 1
sub edx, 3
sub edx, 7
func2@0x1c76164de90 ENDP
_TEXT ENDS
