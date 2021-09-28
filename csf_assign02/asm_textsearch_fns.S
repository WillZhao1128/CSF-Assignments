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
matches: .space 8

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
    /* Save callee-saved register values*/
    push %r13           /* buffer pointer */
    push %r14           /* file pointer */
    push %r15           /* count */

    movw $0, %r15w      /* count = 0; w b/c max val is 512, which is 9 bits*/
    movq %rsi, %r13     /* let r13 store address of buf */
    movq %rdi, %r14     /* %r14 has file pointer */
    movb $0, %al        /* initialize return register to 0*/
    call fgetc          /* %rdi has the file pointer */
    
	
.Lcheck_end_of_line:
    cmpb $10, %al       /* c != '\n'; b because working with characters */
    je .Lset_return_1

.Lcheck_EOF:
    cmpb $-1, %al       /* c != EOF */
    je .Lset_return_0

.Lcheck_MAXLINE:
    cmpw maxline, %r15w /*c < MAXLINE; if count = 512 and this character is a newline, */
    jg .Lset_return_1
    
    movq %rax, (%r13, %r15, 1) /* buf[count] = c*/
    movq %r14, %rdi     /* %r14 has file pointer, so put back into arg1 register */
    call fgetc
    
    incq %r15           /* count++ */
    jmp .Lcheck_end_of_line

.Lset_return_0:
    movq $0, %rax       /* set return value to 0 */
    jmp .Lloop_end

.Lset_return_1:
    movq $1, %rax       /* set return value to 1 */
    jmp .Lloop_end

.Lloop_end:
    movq $0, (%r13, %r15, 1) /* buf[count] = '\0' */
    /* Restore callee-saved registers, and realign stack */
    pop %r15
    pop %r14
    pop %r13
    addq $8, %rsp
    ret

/* -------------------------------------------------------------------------- */

print_line:
    /* %rdi has file pointer, and %rsi has buffer pointer */
    subq $8, %rsp               /* Align the stack pointer*/
    /* Save callee-saved register values*/
    push %r12                   /* length variable */
    push %r13                   /* file pointer */
    push %r14                   /* buffer pointer */
    push %r15                   /* count variable */

    movw $0, %r15w              /* count = 0 */
    movq %rdi, %r13             /* %r13 = file pointer */
    movq %rsi, %r14             /* %r14 = buffer pointer */

    movq %r14, %rdi             /* set arg1 equal to the buffer pointer */
    call find_string_length     /* find the length of the buffer pointer */
    movw %ax, %r12w             /* store this value in %r12; max value is 512 so use w */

.Lloop_top_print_line:
    cmpw %r12w, %r15w           /* for count < length */
    jg .Lloop_end_print_line    
    movq (%r14,%r15,1), %rdi    /* get the character into arg1 of fputc */
    movq %r13, %rsi             /* %rsi is not callee saved, so make sure this still points to file */
    call fputc                  /* fputc(buf[i], out)  */
    incw %r15w                  /* count++ */
    jmp .Lloop_top_print_line

.Lloop_end_print_line:
    /* Restore callee saved registers, and realign stack */
    pop %r15
    pop %r14
    pop %r13
    pop %r12
    addq $8, %rsp
    ret
/* -------------------------------------------------------------------------- */

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
find_string_length:                 /* %rdi is base address */
	subq $8, %rsp	                /* Align stack */
	pushq %r15                      /* Save callee saved register value */
	movw $0, %r15w                  /* count = 0 */

.Lfind_string_length_top:
	cmpb $0, (%rdi, %r15, 1)        /* *s != '\0' */ 
	je .Lfind_string_length_done
	incw %r15w                      /* count++ */
	jmp .Lfind_string_length_top

.Lfind_string_length_done:
	movq %r15, %rax                 /* return count */
	pop %r15                        /* restore callee saved register value */
	addq $8, %rsp                   /* realign stack */
	ret

/* -------------------------------------------------------------------------- */

strings_equal:
    subq $8, %rsp                   /* Align the stack pointer*/
    /* Save callee-saved register values*/
    push %r12                       /* matches */
    push %r13                       /* s1 */
    push %r14                       /* s2 */
    push %r15                       /* count */

    movw $0, %r11w
    movw $0, %r12w                  /* matches */
    movq %rdi, %r13                 /* %r13 contains string 1*/
    movq %rsi, %r14                 /* %r14 contains string 2*/
    movw $0, %r15w                  /* count = 0 */

    movq %rsi, %rdi                 /* put s2 into arg1 */
    call find_string_length         /* find string length of string 2 */
    movw %ax, %r11w                 /* %r11 = length; no function calls so can use caller-saved */

.Lloop_strings_equal_top:
    cmpw %r11w, %r15w                  /* for count < len(string 2) */
    jge .Lstrings_not_equal
    movq (%r13, %r15, 1), %r10      /* get s1[count], store in %r10 */
    cmp (%r14, %r15, 1), %r10       /* s1[count] == s2[count] */
    je .Lchars_equal
    incw %r15w                      /* count++ */

.Lchars_equal:
    incw %r12w                      /* matches++ */
    incw %r15w                      /* count++ */
    jmp .Lloop_strings_equal_top

.Lstrings_not_equal:
    cmpw %r12w, %r11w
    je .Lstrings_equal
    movq $0, %rax
    jmp .Lloop_end_strings_equal

.Lstrings_equal:
    movq $1, %rax

.Lloop_end_strings_equal:
    pop %r15
    pop %r14
    pop %r13
    pop %r12
    addq $8, %rsp
    ret
