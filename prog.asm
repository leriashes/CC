_BSS SEGMENT
PUBLIC a@0x186ff642050        ; a
PUBLIC b@0x186ff6dbbc0        ; b
PUBLIC c@0x186ff6dbef0        ; c
PUBLIC func1@0x186ff6dc220        ; func1
PUBLIC func2@0x186ff6dd9e0        ; func2
PUBLIC func3@0x186ff6ddfd0        ; func3

a@0x186ff642050 DD 01H DUP(?)        ; a
b@0x186ff6dbbc0 DD 01H DUP(?)        ; b
c@0x186ff6dbef0 DD 01H DUP(?)        ; c


_TEXT SEGMENT
_b$ = -16        ; size = 4
_k$ = -12        ; size = 4
_kb$ = -8        ; size = 4
_k$2 = -4        ; size = 4
func1@0x186ff6dc220 PROC
push ebp
mov ebp, esp
and esp, -8
sub esp, 16
push eax
push ebx
push ecx
push edx
push esi
push edi
mov eax, DWORD PTR _k$[ebp - 12]
add eax, 100
mov ebx, eax
cdq
mov eax, DWORD PTR _kb$[ebp - 8]
idiv DWORD PTR _b$[ebp - 16]
imul eax, 10
add eax, 2
func1@0x186ff6dc220_end:
pop eax
pop ebx
pop ecx
pop edx
pop esi
pop edi
mov esp, epb
pop ebp
ret 0
func1@0x186ff6dc220 ENDP
_TEXT ENDS

_TEXT SEGMENT
_a$ = -8        ; size = 4
_sum$ = -4        ; size = 4
func2@0x186ff6dd9e0 PROC
push ebp
mov ebp, esp
and esp, -8
sub esp, 8
push eax
push ebx
push ecx
push edx
push esi
push edi
mov eax, DWORD PTR _a$[ebp - 8]
add eax, DWORD PTR _a$[ebp - 8]
add eax, DWORD PTR _a$[ebp - 8]
mov ebx, DWORD PTR _a$[ebp - 8]
imul ebx, 4
mov ecx, DWORD PTR _sum$[ebp - 4]
sub ecx, ebx
func2@0x186ff6dd9e0_end:
pop eax
pop ebx
pop ecx
pop edx
pop esi
pop edi
mov esp, epb
pop ebp
ret 0
func2@0x186ff6dd9e0 ENDP
_TEXT ENDS

_TEXT SEGMENT
func3@0x186ff6ddfd0 PROC
push ebp
mov ebp, esp
and esp, -8
sub esp, 0
push eax
push ebx
push ecx
push edx
push esi
push edi
mov eax, [c@0x186ff6dbef0]
imul eax, [a@0x186ff642050]
mov ebx, [b@0x186ff6dbbc0]
add ebx, eax
cdq
mov eax, [b@0x186ff6dbbc0]
idiv [c@0x186ff6dbef0]
sub ebx, eax
func3@0x186ff6ddfd0_end:
pop eax
pop ebx
pop ecx
pop edx
pop esi
pop edi
mov esp, epb
pop ebp
ret 0
func3@0x186ff6ddfd0 ENDP
_TEXT ENDS
