.section .data
array1:  .word 10, 20, 30, 40
array2:  .word 1, 2, 3, 4
result:  .space 16   // Space for the result array (4 integers)
format:  .asciz "Result: %d, %d, %d, %d\n"

.section .text
.global _start

_start:
    // Load base addresses of the arrays into registers
    ldr x0, =array1
    ldr x1, =array2

    // Load vectors from memory
    ld1 {v0.4s}, [x0]      // Load 4 x 32-bit integers from array1 into vector register v0
    ld1 {v1.4s}, [x1]      // Load 4 x 32-bit integers from array2 into vector register v1

    // Perform SIMD addition
    add v2.4s, v0.4s, v1.4s // Add corresponding 32-bit integers from v0 and v1, store results in v2

    // Store result back to memory
    ldr x2, =result         // Load the base address of the result array
    st1 {v2.4s}, [x2]       // Store the result from vector register v2 to the 'result' array

    // Load base address of result array into x4 for printing
    ldr x4, =result

    // Prepare to print the results
    ldr x0, =format         // Load address of the format string
    ldr w1, [x4]            // Load first result into w1
    ldr w2, [x4, #4]        // Load second result into w2
    ldr w3, [x4, #8]        // Load third result into w3
    ldr w4, [x4, #12]       // Load fourth result into w4

    // Call printf
    bl printf

    // Exit the program
    mov x8, #93             // System call number for exit
    mov x0, #0              // Exit status
    svc #0                  // Make the system call

