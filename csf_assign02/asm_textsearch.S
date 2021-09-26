/*
 * Assembly language implementation of the main function
 * for the asm_textsearch_fns program
 */

.section .text
sInputFmt: .string "%s"


	.globl main
main:
	/* TODO: implement the main function */
	/*
	subq $8, %rsp

	movq $sInputFmt, %rdi
	movq $buf, %rsi
	call scanf

	movq $buf, %rdi
	call print_line

	addq $8, %rsp
	*/
	ret
