.section .data
output: .asciz "Result: "
number: .space 50  // Space for the number and newline

.section .text
.global _start

_start:
    // Initialize integer to print
    mov x0, #123            // Load the integer to print into x0

    // Point x10 to the end of the number buffer
    adr x10, number + 50    // Point x10 directly to the end of the buffer

convert_loop:
    // Prepare for division to isolate digits
    mov x2, #10
    udiv x3, x0, x2         // Divide x0 by 10, x3 is quotient
    msub x4, x3, x2, x0     // Multiply quotient by 10 and subtract from original number to find remainder
    add w4, w4, #'0'        // Convert remainder to ASCII character in w4
    strb w4, [x10, #-1]!    // Decrement buffer pointer, then store ASCII character at new buffer position
    mov x0, x3              // Update x0 with the quotient
    cbnz x3, convert_loop   // Continue if quotient is not zero

    // Write "Result: " to standard output
    mov x0, #1              // STDOUT file descriptor
    adr x1, output          // Address of the output label
    mov x2, #8              // Length of "Result: "
    mov x8, #64             // Syscall number for write
    svc #0                  // Invoke system call

    // Prepare to print the number
    mov x0, #1              // STDOUT file descriptor
    mov x1, x10             // x10 points to the start of the number
    ldr x2, =number + 50    // Address of the end of the buffer
    sub x2, x2, x1          // Calculate the length of the number
    mov x8, #64             // Syscall number for write
    svc #0                  // Invoke system call to print the number

    // Exit
    mov x0, #0              // Use 0 as the exit status
    mov x8, #93             // Syscall number for exit
    svc #0                  // System call to exit the program

