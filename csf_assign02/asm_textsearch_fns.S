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
character: .space 1
matches: .space 8
return: .space 1

.section .text
    .globl read_line
    .globl print_line
    .globl find_string_length
    .globl strings_equal
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
    movq $0, %r13        /* %r13 will store the number of iterations */
    movq %rdi, %r14      /* %r14 has pointer to the line*/
    movq $0, %r15        /* count == 0*/
    movq $0, matches     /* counting the occurrences */
    
    call find_string_length
    movq %rax, %r13

    movq %rsi, %rdi
    call find_string_length
    movq %rax, length  /* find the length of the word */

    cmp length, %r13   /* if the line length is shorter than the word, end the loop */
    jl .Lloop_count_occurrences_end

    subq length, %r13  /* Calculate line_length - search_word length*/

.Lloop_count_occurrences_top:
    cmp %r13, %r15
    jge .Lloop_count_occurrences_end
    leaq (%r14, %r15, 1), %rdi
    
    call strings_equal
    addq %rax, matches
    jmp .Lloop_count_occurrences_top

.Lloop_count_occurrences_end:
    movq matches, %rax
    pop %r15
    pop %r14
    pop %r13
    addq $8, %rsp
    ret


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

/* -------------------------------------------------------------------------- */

strings_equal:
    subq $8, %rsp        /* Align the stack pointer*/
    push %r12
    push %r13            /* Save callee-saved register values*/
    push %r14            
    push %r15
    movq $0, matches         /* Save callee-saved register values*/
    movq $0, return
    movq $0, %r15            /* count = 0 */
    movq %rdi, %r13          /* %r13 contains string 1*/
    movq %rsi, %r14          /* %r14 contains string 2*/

    movq %rsi, %rdi
    call find_string_length /* find string length of string 2 */
    movq %rax, length

.Lloop_strings_equal_top:
    cmp length, %r15
    jge .Lloop_end
    movq (%r13, %r15, 1), %r12
    cmp (%r13, %r15, 1), %r12
    je .L_chars_equal

.L_chars_equal:
    incq matches
    jmp .Lloop_strings_equal_top

.L_strings_equal:
    movq $1, return

.Lloop_end_strings_equal:
    movq return, %rax
    pop %r15
    pop %r14
    pop %r13
    pop %r12
    addq $8, %rsp
