.global _start

.section .data
answer: .word 41

.section .text
_start:
        adrp x0, answer          // load address of the _page_ the data is at
        add x0, x0, :lo12:answer // offset within the page, 12 bits for 4KB pages, 14 bits for 16KB

        ldr w1, [x0]
        add w1, w1, #1
        str w1, [x0]

        uxtw x1, w1  // Zero-extend w1 to x1

        cmp x1, #42
        b.eq function

        b end

function:
        bl do_something
        b end

do_something:
        movz x1, #420
        ret

end:
        mov x8, #93
        mov x0, x1
        svc 0
