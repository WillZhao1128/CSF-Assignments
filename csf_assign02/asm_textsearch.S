/*
 * Assembly language implementation of the main function for the asm_textsearch_fns program
 * CSF Assignment 2
 * Aidan Aug, Will Zhao
 * aaug1@jhu.edu, wzhao33@jhu.edu
 */
.section .rodata
sInputFmt: .string "%s"
s_read_Fmt: .string "r"
s_file_err: .string "Could not open file\n"
s_input_err: .string "Invalid input\n"
s_c_flag: .string "-c"
s_occurrences: .string "%d occurrence(s)\n"
s_hi: .string "val is %d"

.section .bss
.align 1
line_in_file: .space (1 * 511)

.section .text
	.globl main
main:
	/* TODO: implement the main function */
	
	subq $8, %rsp 						/* Align the stack pointer */
	pushq %r12   						/* file_ind */
	pushq %r13   						/* arg c*/
	pushq %r14   						/* char **argv */
	pushq %r15  						/* fileptr */

	movq %rdi, %r13						/* save argc in %r13 */
	movq %rsi, %r14						/* save **argv in %r14 */

	call handle_arguments 				/* int file_ind = handle_arguments(argc)*/
	movq %rax, %r12

	movq (%r14, %r12, 8), %rdi 			/* argv[file_ind] */
	movq $s_read_Fmt, %rsi
	call fopen
	movq %rax, %r15
	cmpq $0, %r15 						/* if fileptr == NULL */
	je .Lfile_err

	/* int tot_occurrences = calc_total_occurrences(fileptr, argv[file_ind - 1], argc)*/
	movq %r15, %rdi
	subq $1, %r12
	movq (%r14, %r12, 8), %rsi 			/* argv[file_ind - 1] */
	movq %r13, %rdx
	call calc_total_occurrences
	movq %rax, %r12
	
	cmpq $4, %r13						/* if argc == 4 */
	jne .Lend
	movq $1, %r10

	/* if (strings_equal(argv[1], c_flag) */
	movq (%r14, %r10, 8), %rdi
	movq $s_c_flag, %rsi
	call strings_equal
	cmpq $1, %rax						/* if strings are equal, -c flag is present */
	je .Lprint_tot_occurr
	jmp .Linvalid_input

.Lprint_tot_occurr:
	movl $0, %eax
	movq stdout(%rip), %rdi 			/* pass stdout */
	movq $s_occurrences, %rsi      		/* pass message string */
	movq %r12, %rdx
	call fprintf
	movl $0, %eax
	jmp .Lend

.Lfile_err:
	movq %r15, %rdi

	movl $0, %eax
	movq stderr(%rip), %rdi 			/* pass stderr */
	movq $s_file_err, %rsi       		/* pass "Could not open file" string */
	call fprintf
	movl $0, %eax

	/* Restore callee-saved registers, and realign stack */
	popq %r15
	popq %r14   		
	popq %r13 
	popq %r12   
	addq $8, %rsp

	movq $1, %rdi
	call exit

.Linvalid_input:
	movq %r15, %rdi
	call fclose

	movl $0, %eax
	movq stderr(%rip), %rdi 			/* pass stderr */
	movq $s_input_err, %rsi       		/* pass "Invalid input" string */
	call fprintf
	movl $0, %eax
	
	/* Restore callee-saved registers, and realign stack */
	popq %r15
	popq %r14   
	popq %r13  
	popq %r12 
	addq $8, %rsp

	movq $1, %rdi
	call exit

.Lend:
	movq %r15, %rdi
	call fclose
	movq $0, %rax

	/* Restore callee-saved registers, and realign stack */
	popq %r15
	popq %r14  
	popq %r13  
	popq %r12
	addq $8, %rsp
	ret
