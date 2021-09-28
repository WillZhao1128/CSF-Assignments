/*
 * Assembly language implementations of functions declared in textsearch_fns.h
 * CSF Assignment 2
 * Aidan Aug, Will Zhao
 * aaug1@jhu.edu, wzhao33@jhu.edu
 */
 
.section .data
maxline: .long 511

.section .bss
length: .space 4
char: .space 1
occurrences: .space 8

.section .text
    .globl read_line
    .globl print_line
    .globl find_string_length
    .globl count_occurrences

/*
 * The following is just an example of how an assembly language
 * function would be defined
 */
	
read_line:
	/* %rdi has file pointer, and %rsi has buffer pointer */
    subq $8, %rsp       /* Align the stack pointer*/
    push %r13           /* Save callee-saved register values*/
    push %r14
    push %r15

    movw $0, %r15w      /* count = 0; w b/c max val is 512, which is 9 bits*/
    movq %rsi, %r13     /* let r13 store address of buf*/
    movq %rdi, %r14     /* %r14 has file pointer*/
    movb $0, %al        /* initialize return register to 0*/
    call fgetc          /* %rdi has the file pointer */
    
	
.Lcheck_end_of_line:
    cmpb $10, %al /* c != '\n'*/
    je .Lset_return_1

.Lcheck_EOF:
    cmpb $-1, %al /* c != EOF */
    je .Lset_return_0

.Lcheck_MAXLINE:
    cmpw maxline, %r15w /*c < MAXLINE; if count = 512 and this character is a newline, */
    jg .Lset_return_1
    
    movq %rax, (%r13, %r15, 1)
    movq %r14, %rdi /* %r14 has file pointer*/
    movb $0, %al /*initialize return register to 0*/
    call fgetc
    
    incq %r15 /* count++ */
    jmp .Lcheck_end_of_line

.Lset_return_0:
    movq $0, %rax
    jmp .Lloop_end

.Lset_return_1:
    movq $1, %rax
    jmp .Lloop_end

.Lloop_end:
    movq $0, (%r13, %r15, 1) /* move the null term. character into address of r14 */
    pop %r15
    pop %r14
    pop %r13
    addq $8, %rsp
    ret

/* -------------------------------------------------------------------------- */

print_line:
    /* %rdi has file pointer, and %rsi has buffer pointer */
    subq $8, %rsp        /* Align the stack pointer*/
    push %r13            /* Save callee-saved register values*/
    push %r14            
    push %r15
    movq $0, %r15        /* count = 0 */
    movq %rsi, %r14      /* %r14 = buffer pointer */
    movq %rdi, %r13      /* %r13 = file pointer */

    movq %r14, %rdi      /* set arg1 equal to the buffer pointer */
    call find_string_length  /* find the length of the buffer pointer */
    movq %rax, length    /* store this value in variable length */

.Lloop_top_print_line:
    cmp length, %r15   /* for count < length, e*/
    jg .Lloop_end_print_line /* once count == lenght, end loop*/
    mov (%r14,%r15,1), %rdi  /* get the character into arg1 of fputc */
    mov %r13, %rsi
    call fputc               /* %rdi is arg1 = character, %rsi pointer to write  */
    incq %r15
    jmp .Lloop_top_print_line

.Lloop_end_print_line:
    pop %r15
    pop %r14
    pop %r13
    
    addq $8, %rsp
    ret
/* -------------------------------------------------------------------------- */
/*TODO*/
count_occurrences:
    subq $8, %rsp        /* Align the stack pointer*/
    push %r13            /* Save callee-saved register values*/
    push %r14            
    push %r15
    movq $0, %r13        /* Align the stack pointer*/
    ret

/* -------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------- */
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

	