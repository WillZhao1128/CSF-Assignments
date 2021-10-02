/*
 * Assembly language implementation of the main function for the asm_textsearch_fns program
 * CSF Assignment 2
 * Aidan Aug, Will Zhao
 * aaug1@jhu.edu, wzhao33@jhu.edu
 */

.section .text
sInputFmt: .string "%s"

.section .bss
.align 1
line_in_file: .space (1 * 511)


	.globl main
main:
	/* TODO: implement the main function */
	
	subq $8, %rsp

	call handle_arguments
	
	leaq (%rsi, )
	
	movq $sInputFmt, %rdi
	movq $buf, %rsi
	call scanf

	movq $buf, %rdi
	call print_line

	addq $8, %rsp
	
	ret
