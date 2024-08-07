.global _start

.section .text
_start:
    mov x0, #5            // Calculate factorial of 5
    bl factorial          // Call factorial function
    mov x1, x0            // Use the factorial result as the exit status

    // Prepare to exit
    mov x8, #93           // syscall number for exit
    mov x0, x1            // Set the exit status
    svc 0                 // System call to exit the program

factorial:
    stp x29, x30, [sp, #-16]!  // Save frame pointer and return address
    mov x29, sp                // Update the frame pointer

    sub sp, sp, #16            // Allocate space on the stack for one 64-bit register
    str x0, [sp]               // Store the current value of x0 on the stack

    cmp x0, #1                 // Compare x0 with 1
    b.le end_recursion         // If x0 <= 1, go to base case handling

    sub x0, x0, #1             // Decrease x0 by 1 for the recursive call
    bl factorial               // Recursive call

    ldr x1, [sp]               // Load the original x0 saved in the stack
    mul x0, x0, x1             // Multiply the returned value by the original x0

    add sp, sp, #16            // Clean up the stack by removing the stored x0

    ldp x29, x30, [sp], #16    // Restore frame pointer and return address
    ret                        // Return from the function

end_recursion:
    ldr x1, [sp]               // Load the original x0 (should be 1 at base case)
    mov x0, #1                 // Set the return value for the factorial of 1
    add sp, sp, #16            // Clean up the stack by removing the stored x1
    ldp x29, x30, [sp], #16    // Restore frame pointer and return address
    ret                        // Return from the function

