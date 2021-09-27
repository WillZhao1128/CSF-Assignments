/*
 * Assembly language implementations of functions declared in
 * textsearch_fns.h
 */
 
.section .data
maxline: .long 511

.section .text
buf: .space 511

/*
 * The following is just an example of how an assembly language
 * function would be defined
 */
	.globl read_line
read_line:
	/* TODO: implementation of read_line function */
	/*%rdi has file pointer, and %rsi has buffer pointer*/
    subq $8, %rsp /* Align the stack pointer*/
    push %r13
    push %r14
    push %r15

    movq $0, %r15 /* count = 0*/
    movq %rsi, %r13 /* let r13 store address of buf*/
    movq %rdi, %r14 /* %r14 has file pointer*/
    movq $0, %rax /*initialize return register to 0*/
    /*leaq (%rdi, %r12, 1), %r13 r13 stores address of next character in file*/
    call fgetc
    /*%rsi now holds the entire file string*/
    
	
.Lcheck_end_of_line:
    cmpb $10, %al /* c != '\n'*/
    je .Lloop_end

.Lcheck_EOF:
    cmpb $-1, %al /* c != EOF */
    je .Lloop_end

.Lcheck_MAXLINE:
    cmpq maxline, %r15 /*c < MAXLINE; if count = 512 and this character is a newline, */
    jg .Lloop_end
    
    movq %rax, (%r13, %r15, 1)
    movq %r14, %rdi /* %r14 has file pointer*/
    movq $0, %rax /*initialize return register to 0*/
    call fgetc
    
    incq %r15 /* count++ */
    jmp .Lcheck_end_of_line

.Lloop_end:
    movq $0, (%r13, %r15, 1) /* move the null term. character into address of r14 */
    pop %r15
    pop %r14
    pop %r13
    addq $8, %rsp
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

	