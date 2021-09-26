/*
 * Assembly language implementations of functions declared in
 * textsearch_fns.h
 */

.section .text

/*
 * The following is just an example of how an assembly language
 * function would be defined
 */
	.globl read_line
read_line:
	/* TODO: implementation of read_line function */
	ret

	.globl find_string_length
find_string_length: /* Assume %rdi is base address */
	subq $8, %rsp
	pushq %r10
	movq $0, %r10  /* r10 is loop counter; set to 0 */

.Lfind_string_length_top:
	cmpb $0, (%rdi, %r10, 1) /* is values at pointer null? */ 
	je .Lfind_string_length_done
	inc %r10
	jmp .Lfind_string_length_top

.Lfind_string_length_done:
	movq %r10, %rax
	pop %r10
	addq $8, %rsp
	ret

	