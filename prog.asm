_BSS SEGMENT
PUBLIC a@0x1cd5eda4270        ; a
PUBLIC b@0x1cd5ee0fbd0        ; b
PUBLIC h@0x1cd5ee0ff00        ; h
PUBLIC g@0x1cd5ee10230        ; g
PUBLIC jk@0x1cd5ee10610        ; jk
PUBLIC j@0x1cd5edb9ae0        ; j
PUBLIC k@0x1cd5ee13550        ; k
PUBLIC m@0x1cd5ee13a10        ; m
PUBLIC main@0x1cd5ee13680        ; main
PUBLIC rq@0x1cd5ee13ed0        ; rq

a@0x1cd5eda4270 DQ 01H DUP(?)        ; a
b@0x1cd5ee0fbd0 DW 01H DUP(?)        ; b
h@0x1cd5ee0ff00 DW 01H DUP(?)        ; h
g@0x1cd5ee10230 DW 01H DUP(?)        ; g
k@0x1cd5ee13550 DQ 01H DUP(?)        ; k
m@0x1cd5ee13a10 DQ 01H DUP(?)        ; m
rq@0x1cd5ee13ed0 DD 01H DUP(?)        ; rq


_TEXT SEGMENT
n$ = -12        ; size = 8
k2$ = -4        ; size = 4
k$ = 0        ; size = 2
jk@0x1cd5ee10610 PROC
jk@0x1cd5ee10610 ENDP
_TEXT ENDS

_TEXT SEGMENT
n$ = -64        ; size = 8
fh$ = -56        ; size = 8
k2$ = -48        ; size = 4
k$ = -44        ; size = 2
a$1 = -40        ; size = 4
b$1 = -36        ; size = 4
n$1 = -32        ; size = 4
n$2 = -24        ; size = 8
n5$2 = -16        ; size = 8
fh$2 = -8        ; size = 8
j@0x1cd5edb9ae0 PROC
j@0x1cd5edb9ae0 ENDP
_TEXT ENDS

_TEXT SEGMENT
main@0x1cd5ee13680 PROC
main@0x1cd5ee13680 ENDP
_TEXT ENDS
