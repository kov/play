.section .data
hello:   .asciz "Hello, world! The sum of %d and %d is %d.\n"
num1:    .word 10
num2:    .word 20

.section .text
.global _start

_start:
    // Load the address of the format string into x0 (the first argument to printf)
    ldr x0, =hello

    // Load the integers into registers
    ldr x1, =num1
    ldr w1, [x1]           // Load 32-bit value
    ldr x2, =num2
    ldr w2, [x2]           // Load 32-bit value

    // Perform addition and store result in w3
    add w3, w1, w2

    // Setup arguments for printf, ensuring correct extension
    mov x1, x1             // No need to change, correct handling of address register
    mov x2, x2             // No need to change, correct handling of address register
    mov x3, x3             // Correctly moves within the same register, ensuring upper bits are clear if previously used

    // Call printf
    bl printf

    // Exit the program by calling exit system call
    mov x8, #93            // syscall number for exit
    mov x0, #0             // exit status
    svc #0                 // make the system call

