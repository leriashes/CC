_BSS SEGMENT
PUBLIC a@0x1fe712a5460        ; a
PUBLIC b@0x1fe7133bae0        ; b
PUBLIC c@0x1fe7133be10        ; c
PUBLIC func1@0x1fe7133c140        ; func1
PUBLIC func2@0x1fe7133d900        ; func2
PUBLIC func3@0x1fe71342310        ; func3

a@0x1fe712a5460 DD 01H DUP(?)        ; a
b@0x1fe7133bae0 DD 01H DUP(?)        ; b
c@0x1fe7133be10 DD 01H DUP(?)        ; c



_TEXT SEGMENT
_b$ = -16        ; size = 4
_k$ = -12        ; size = 4
_kb$ = -8        ; size = 4
_k$2 = -4        ; size = 4
func1@0x1fe7133c140 PROC
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
$LN3@func1@0x1fe7133c140:
    mov DWORD PTR _k$[ebp], 1

$LN4@func1@0x1fe7133c140:
    mov eax, DWORD PTR _k$[ebp]
    add eax, 100

$LN5@func1@0x1fe7133c140:
    mov DWORD PTR _kb$[ebp], eax

$LN6@func1@0x1fe7133c140:
    mov DWORD PTR _k$2[ebp], 1

$LN7@func1@0x1fe7133c140:
    mov DWORD PTR _yu$[ebp], 100

$LN8@func1@0x1fe7133c140:
    cdq
    mov eax, DWORD PTR _kb$[ebp]
    idiv DWORD PTR _b$[ebp]

$LN9@func1@0x1fe7133c140:
    imul eax, 10

$LN10@func1@0x1fe7133c140:
    add eax, 2

$LN11@func1@0x1fe7133c140:
    mov DWORD PTR _b$[ebp], eax

$LN12@func1@0x1fe7133c140:
    mov eax, 1
    cmp eax, 2
    jge $LN24@func1@0x1fe7133c140

$LN14@func1@0x1fe7133c140:
    mov ebx, 100
    sub ebx, 900

$LN15@func1@0x1fe7133c140:
    mov DWORD PTR _b$[ebp], ebx

$LN16@func1@0x1fe7133c140:
    mov ebx, DWORD PTR _b$[ebp]
    cmp ebx, 200
    je $LN22@func1@0x1fe7133c140

$LN18@func1@0x1fe7133c140:
    mov ecx, DWORD PTR _b$[ebp]
    add ecx, 20

$LN19@func1@0x1fe7133c140:
    mov DWORD PTR _b$[ebp], ecx

$LN20@func1@0x1fe7133c140:
    jmp $LN22@func1@0x1fe7133c140

$LN21@func1@0x1fe7133c140:
    jmp $LN16@func1@0x1fe7133c140

$LN23@func1@0x1fe7133c140:
    jmp $LN12@func1@0x1fe7133c140

$LN26@func1@0x1fe7133c140:
    pop eax
    pop ebx
    pop ecx
    pop edx
    pop esi
    pop edi
    mov esp, epb
    pop ebp
    ret 0
func1@0x1fe7133c140 ENDP
_TEXT ENDS


_TEXT SEGMENT
_a$ = -8        ; size = 4
_sum$ = -4        ; size = 4
func2@0x1fe7133d900 PROC
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
$LN31@func2@0x1fe7133d900:
    mov DWORD PTR _a$[ebp], 0

$LN32@func2@0x1fe7133d900:
    mov eax, DWORD PTR _a$[ebp]
    add eax, DWORD PTR _a$[ebp]

$LN33@func2@0x1fe7133d900:
    add eax, DWORD PTR _a$[ebp]

$LN34@func2@0x1fe7133d900:
    mov DWORD PTR _sum$[ebp], eax

$LN35@func2@0x1fe7133d900:
    mov eax, DWORD PTR _a$[ebp]
    imul eax, 4

$LN36@func2@0x1fe7133d900:
    mov ebx, DWORD PTR _sum$[ebp]
    sub ebx, eax

$LN37@func2@0x1fe7133d900:
    mov [b@0x1fe7133bae0], ebx

$LN38@func2@0x1fe7133d900:
    pop eax
    pop ebx
    pop ecx
    pop edx
    pop esi
    pop edi
    mov esp, epb
    pop ebp
    ret 0
func2@0x1fe7133d900 ENDP
_TEXT ENDS


_TEXT SEGMENT
func3@0x1fe71342310 PROC
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
$LN43@func3@0x1fe71342310:
    mov eax, [c@0x1fe7133be10]
    imul eax, [a@0x1fe712a5460]

$LN44@func3@0x1fe71342310:
    mov ebx, [b@0x1fe7133bae0]
    add ebx, eax

$LN45@func3@0x1fe71342310:
    cdq
    mov eax, [b@0x1fe7133bae0]
    idiv [c@0x1fe7133be10]

$LN46@func3@0x1fe71342310:
    mov ecx, eax
    cdq
    idiv 2

$LN47@func3@0x1fe71342310:
    sub ebx, ecx

$LN48@func3@0x1fe71342310:
    mov [a@0x1fe712a5460], ebx

$LN49@func3@0x1fe71342310:
    pop eax
    pop ebx
    pop ecx
    pop edx
    pop esi
    pop edi
    mov esp, epb
    pop ebp
    ret 0
func3@0x1fe71342310 ENDP
_TEXT ENDS
