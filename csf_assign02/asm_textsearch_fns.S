/*
 * Assembly language implementations of functions declared in textsearch_fns.h
 * CSF Assignment 2
 * Aidan Aug, Will Zhao
 * aaug1@jhu.edu, wzhao33@jhu.edu
 */

 .section .rodata
 inv_inp: .string "Invalid Input"
 newline: .string "\n"
 
.section .data
maxline: .value 511

.section .bss

.section .text
    .globl read_line
    .globl print_line
    .globl find_string_length
    .globl strings_equal
    .globl count_occurrences
    .globl handle_arguments
    .globl calc_total_occurrences
    
	
read_line:
    subq $8, %rsp           /* Align the stack pointer*/

    /* Save callee-saved register values*/
    pushq %r13                  /* buffer pointer */
    pushq %r14                  /* file pointer */
    pushq %r15                  /* count */
    subq $8, %rsp

    movq $0, %r15              /* count = 0; w b/c max val is 512, which is 9 bits*/
    movq %rdi, %r14             /* %r14 has file pointer */
    movq %rsi, %r13             /* %r13 = buf */
    
    call fgetc                  /* fgetc(%rdi = fileptr) */
    
	
.Lcheck_end_of_line:
    cmpb $10, %al               /* c != '\n'; b because working with characters */
    je .Lset_return_1

.Lcheck_EOF:
    cmpb $0, %al                /* c != EOF */
    jl .Lset_return_0

.Lcheck_MAXLINE:
    cmpq $511, %r15         /* count < MAXLINE; */
    jge .Lset_return_1
    
    movb %al, (%r13, %r15, 1)   /* buf[count] = c*/
    movq %r14, %rdi             /* %r14 has file pointer, so put back into arg1 register */
    call fgetc
    
    incq %r15                  /* count++ */
    cmpq $511, %r15
    je .Lget_next_line
    jmp .Lcheck_end_of_line

.Lset_return_0:
    movq $0, %rax               /* set return value to 0 */
    jmp .Lloop_end

.Lset_return_1:
    movq $1, %rax               /* set return value to 1 */
    jmp .Lloop_end

.Lget_next_line:
    cmpb $10, %al
    je .Lcheck_end_of_line
    cmpb $0, %al
    jl .Lcheck_end_of_line
    movq %r14, %rdi             /* %r14 has file pointer, so put back into arg1 register */
    call fgetc
    jmp .Lget_next_line


.Lloop_end:
    movq $0, (%r13, %r15, 1) /* buf[count] = '\0' */
    /* Restore callee-saved registers, and realign stack */
    addq $8, %rsp
    popq %r15
    popq %r14
    popq %r13
    addq $8, %rsp
    ret

/* -------------------------------------------------------------------------- */

print_line:
    subq $8, %rsp               /* Align the stack pointer*/

    /* Save callee-saved register values*/
    pushq %r12                   /* length */
    pushq %r13                   /* file pointer */
    pushq %r14                   /* buffer pointer */
    pushq %r15                   /* count */

    movw $0, %r15w               /* count = 0 */
    movq %rdi, %r13              /* %r13 = file pointer */
    movq %rsi, %r14              /* %r14 = buffer pointer */

    movq %r14, %rdi              /* set arg1 equal to the buffer pointer */
    call find_string_length      /* find_string_length(buf) */
    movq %rax, %r12              /* store this value in %r12 */

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
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    addq $8, %rsp
    ret
/* -------------------------------------------------------------------------- */

count_occurrences:
    subq $8, %rsp        /* Align the stack pointer*/

    /* Save callee-saved register values*/
    pushq %rbp                   /* length */
    pushq %r12                   /* occurrences */
    pushq %r13                   /* *line */
    pushq %r14                   /* *str */
    pushq %r15                   /* count */
    subq $8, %rsp

    movq $0, %r12               /* occurrences = 0 */
    movq $0, %r15               /* count == 0 */
    movq %rdi, %r13             /* %r13 = *line */
    movq %rsi, %r14             /* %r14 = *str */

    call find_string_length     
    movq %rax, %rbp             /* line_len = find_string_length(line) */

    movq %r14, %rdi
    call find_string_length     /* find_string_len(str) */
    cmpq %rax, %rbp             /* if the line length is shorter than the word, end the loop */
    jl .Lloop_count_occurrences_end

    subq %rax, %rbp             /* line_length - search_word_len = num iterations*/
    addq $1, %rbp               /* %rbp + 1 */

.Lloop_count_occurrences_top:
    cmpw %bp, %r15w             /* while count < line_len - search_word_len */
    jge .Lloop_count_occurrences_end
    leaq (%r13, %r15, 1), %rdi  /* line + i (address) */
    movq %r14, %rsi             /* arg2 is str */

    call strings_equal
    addw %ax, %r12w             /* occurrences += strings_equal(line + i, str) */
    incw %r15w
    jmp .Lloop_count_occurrences_top

.Lloop_count_occurrences_end:
    movq %r12, %rax             /* return occurrences */

    /* realign stack pointer and restore all callee-saved registers */
    addq $8, %rsp
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %rbp
    addq $8, %rsp
    ret

/* -------------------------------------------------------------------------- */
find_string_length:                 
	subq $8, %rsp	                /* Align stack */
	pushq %r15                      /* Save callee saved register value */
	movw $0, %r15w                  /* count = 0 */

.Lfind_string_length_top:
	cmpb $0, (%rdi, %r15, 1)        /* *s != '\0' */ 
	je .Lfind_string_length_done
	incw %r15w                      /* count++ */
	jmp .Lfind_string_length_top

.Lfind_string_length_done:
	movw %r15w, %ax                 /* return count */
	pop %r15                        /* restore callee saved register value */
	addq $8, %rsp                   /* realign stack */
	ret

/* -------------------------------------------------------------------------- */

strings_equal:
    subq $8, %rsp                   /* Align the stack pointer*/

    /* Save callee-saved register values*/
    pushq %r11                       /* search_word_len */
    pushq %r12                       /* matches */
    pushq %r13                       /* s1 */
    pushq %r14                       /* s2 */
    pushq %r15                       /* count */

    movw $0, %r12w                  /* matches = 0 */
    movq %rdi, %r13                 /* %r13 contains string 1*/
    movq %rsi, %r14                 /* %r14 contains string 2*/
    movq $0, %r15                   /* count = 0 */

    movq %r14, %rdi                 /* put s2 into arg1 */
    call find_string_length         /* find string length of string 2 */
    movw %ax, %r11w                 /* %r11 = length; no function calls so can use caller-saved */

.Lloop_strings_equal_top:
    cmpw %r11w, %r15w               /* for count < len(string 2) */
    jge .Lstrings_equal_or_not
    movb (%r13, %r15, 1), %r10b     /* get s1[count], store in %r10 */
    movb (%r14, %r15, 1), %al       /* s1[count] == s2[count] */
    cmpb %r10b, %al
    jz .Lchars_equal
    incw %r15w                      /* count++ */
    jmp .Lloop_strings_equal_top

.Lchars_equal:
    incw %r12w                      /* matches++ */
    incw %r15w                      /* count++ */
    jmp .Lloop_strings_equal_top
    
.Lstrings_equal_or_not:             /* if matches == search_word_len */
    cmpw %r12w, %r11w
    je .Lstrings_equal
    movq $0, %rax
    jmp .Lloop_end_strings_equal

.Lstrings_equal:
    movq $1, %rax

.Lloop_end_strings_equal:
    /* restore callee-saved registers and realign stack*/
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %r11
    addq $8, %rsp
    ret
/* -------------------------------------------------------------------------- */
handle_arguments:
    subq $8, %rsp                   /* align stack pointer*/
    movq $0, %rax
    cmpq $3, %rdi                   /* if arc == 3 */
    je .Largc_is_three
    cmpq $4, %rdi                   /* if arc == 4 */
    je .Largc_is_four
    movq stderr(%rip), %rdi         /* else err: pass stderr */
	movq $inv_inp, %rsi             /* pass "Invalid Input" */
    call fprintf
    movl $0, %eax
    call exit
    

.Largc_is_three:
    movl $2, %eax
    jmp .Lhandle_arguments_end

.Largc_is_four:
    movl $3, %eax
    jmp .Lhandle_arguments_end

.Lhandle_arguments_end:
    addq $8, %rsp                   /* realign stack pointer*/
    ret

/* -------------------------------------------------------------------------- */

calc_total_occurrences:
    subq $8, %rsp                   /* Align the stack pointer*/
    /* Save callee-saved register values*/
    pushq %r12                          /* fileptr */
    pushq %r13                          /* search word */
    pushq %r14                          /* argc */
    pushq %r15                          /* tot_occurrences */

    movb $1, %al                        /* flag = 1 */
    movq %rdi, %r12                     
    movq %rsi, %r13
    movl %edx, %r14d    
    movl $0, %r15d                      /* tot_occurrences = 0 */


.Lloop_calc_tot_occ_top:
    cmpb $0, %al                        /* while(flag == 1) */
    je .Lloop_calc_tot_occ_end
    pushq %rbp                          /* save callee-saved register */
    subq $512, %rsp                     /* allocate 512 bits for buffer */
    movq %rsp, %rbp                     /* char* buf = malloc(sizeof(char) * 512*/
    
    movq %r12, %rdi             
    movq %rbp, %rsi
    call read_line                      /* flag = read_line(fileptr, buf)*/
    pushq %rax                          /* save flag value (pushing %al messing things up)*/


    movq %rbp, %rdi              
    movq %r13, %rsi
    
    call count_occurrences              /* flag = read_line(fileptr, buf)*/
    
    addq %rax, %r15                     /* tot_occurrences += count_occurrences(buf, search)*/

    cmpl $3, %r14d                       /* if argc == 3 */
    jne .Lloop_calc_tot_occ_iterate
    cmpq $0, %rax                       /* && num_occurrences > 0 */
    jle .Lloop_calc_tot_occ_iterate     

    /* fprintf(stdout, buf) */
    movl $0, %eax
    movq stdout(%rip), %rdi             /* pass stderr */
	movq %rbp, %rsi                     /* pass message string */
	call fprintf
	movl $0, %eax

    /* fprintf(stdout, /n) */
    movq stdout(%rip), %rdi             /* pass stderr */
	movq $newline, %rsi                      /* pass message string */
	call fprintf
	movl $0, %eax

.Lloop_calc_tot_occ_iterate:
    popq %rax                           /* get flag */
    addq $512, %rsp
    popq %rbp                           /* free(buf) */
    jmp .Lloop_calc_tot_occ_top

.Lloop_calc_tot_occ_end:
    movq %r15, %rax                 /* return tot_occurrences */

    /* restore callee-saved registers */
    popq %r15                       
    popq %r14                       
    popq %r13                       
    popq %r12                  

    addq $8, %rsp /* re-align stack */
    ret
