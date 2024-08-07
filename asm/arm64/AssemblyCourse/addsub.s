.text

.global _start
_start:
        mov x0, #10
        mov x1, #20
        add x2, x1, x0
        sub x2, x2, #10

        mov x8, #93
        mov x0, x2
        svc 0
