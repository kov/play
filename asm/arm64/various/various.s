.global _asmstart

.section .data
asmpow_format: .asciz "pow result: %ld\n"

.section .text
_asmstart:
        mov x0, #10
        mov x1, #2

        bl asmpow

        // Save register to the stack, so the call to printf doesn't clobber it.
        str x0, [sp, #-16]!

        mov x1, x0
        ldr x0, =asmpow_format

        bl printf

        // Pop the return of pow we had saved on the stack, and reduce the stack.
        ldr x0, [sp], #16
        b exit

.global asmpow
asmpow:
        // Save the number we want to raise to the power of x1.
        mov x3, x0

asmpow_loop:
        // Stop when we get to 1.
        cmp x1, #1
        b.le asmpow_end

        mul x0, x0, x3

        sub x1, x1, #1
        b asmpow_loop

asmpow_end:
        ret

// Exit, we rely on caller setting the return value on x0.
exit:
        mov x8, #93
        svc 0
