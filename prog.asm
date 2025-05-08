_BSS SEGMENT
PUBLIC a@0x1c452dfb960        ; a
PUBLIC func@0x1c452dfadd0        ; func

a@0x1c452dfb960 DD 01H DUP(?)        ; a


_TEXT SEGMENT
func@0x1c452dfadd0 PROC
mov eax, 3
mov eax, 2
add eax, ebx
mov eax, 7
sub eax, ebx
mov eax, [a@0x1c452dfb960]
mov eax, [a@0x1c452dfb960]
add eax, 1
sub eax, 3
sub eax, 7
mov eax, [a@0x1c452dfb960]
func@0x1c452dfadd0 ENDP
_TEXT ENDS
