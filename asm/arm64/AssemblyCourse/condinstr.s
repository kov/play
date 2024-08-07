.global _start

.section .text
_start:
    mov x0, #5
    mov x1, #10
    cmp x0, x1       // Compare x0 with x1
    b.ge greater_or_equal  // If x0 >= x1, jump to greater_or_equal
    b.lt less_than    // If x0 < x1, jump to less_than

greater_or_equal:
    mov x2, #1       // Set x2 to 1 (example operation)
    b end

less_than:
    mov x2, #0       // Set x2 to 0 (example operation)

end:
    // Exit the program
    mov x8, #93       // Set syscall number for exit
    mov x0, x2        // Use x2 as the exit status
    svc 0             // Make the syscall

