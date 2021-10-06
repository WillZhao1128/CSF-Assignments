/*
 * Assembly language implementations of functions declared in textsearch_fns.h
 * CSF Assignment 2
 * Aidan Aug, Will Zhao
 * aaug1@jhu.edu, wzhao33@jhu.edu
 */

 .section .rodata
 inv_inp: .string "Invalid Input"
 newline: .string "\n"

.section .text
    .globl read_line
    .globl print_line
    .globl find_string_length
    .globl strings_equal
    .globl count_occurrences
    .globl handle_arguments
    .globl calc_total_occurrences
    
read_line:
    #define MAXLINE 511
    subq $8, %rsp                       /* Align the stack pointer*/

    /* Save callee-saved register values*/
    pushq %r13                          /* buffer pointer */
    pushq %r14                          /* file pointer */
    pushq %r15                          /* count */
    subq $8, %rsp

    movq $0, %r15                       /* count = 0 */
    movq %rdi, %r14                     /* %r14 has file pointer */
    movq %rsi, %r13                     /* %r13 has buffer to write to */
    
    call fgetc                          /* get the first character */
    
	
.Lread_line_check_end_of_line:
    cmpb $10, %al                       /* if '\n' is read, then end and return 1 */
    je .Lread_line_set_return_1

.Lread_line_check_EOF:
    cmpb $0, %al                        /* if EOF, then end and return 0  */
    jl .Lread_line_set_return_0

.Lread_line_check_MAXLINE:
    cmpw $MAXLINE, %r15w                /* if count < MAXLINE, continue. Otherwise, end. */
    jge .Lread_line_set_return_1
    
    movb %al, (%r13, %r15, 1)           /* store character in buf[count]*/
    movq %r14, %rdi                     /* %r14 has file pointer, so put back into arg1 register */
    call fgetc
    
    incw %r15w                          /* count++ */
    cmpw $MAXLINE, %r15w                /* if count == 511, then must find next line */ 
    je .Lread_line_get_next_line
    jmp .Lread_line_check_end_of_line

.Lread_line_set_return_0:
    movq $0, %rax                       /* set return value to 0 */
    jmp .Lread_line_loop_end

.Lread_line_set_return_1:
    movq $1, %rax                       /* set return value to 1 */
    jmp .Lread_line_loop_end

.Lread_line_get_next_line:
    cmpb $10, %al                       /* while a newline is not read, */
    je .Lread_line_check_end_of_line
    cmpb $0, %al                        /* and while EOF is not reached... */
    jl .Lread_line_check_end_of_line
    movq %r14, %rdi              
    call fgetc                          /* continue calling until gets next line or EOF*/
    jmp .Lread_line_get_next_line


.Lread_line_loop_end:
    movq $0, (%r13, %r15, 1)            /* buf[count] = '\0'; adds null terminator */
    /* Restore callee-saved registers, and realign stack */
    addq $8, %rsp
    popq %r15
    popq %r14
    popq %r13
    addq $8, %rsp
    ret

/* -------------------------------------------------------------------------- */

print_line:
    subq $8, %rsp                       /* Align the stack pointer*/

    /* Save callee-saved register values*/
    pushq %r12                          /* length */
    pushq %r13                          /* file pointer */
    pushq %r14                          /* buffer pointer */
    pushq %r15                          /* count */

    movw $0, %r15w                      /* count = 0 */
    movq %rdi, %r13                     /* %r13 = file pointer */
    movq %rsi, %r14                     /* %r14 = buffer pointer */

    movq %r14, %rdi                     /* set arg1 equal to the buffer pointer */
    call find_string_length             /* find_string_length(buf) */
    movq %rax, %r12                     /* store this value in %r12 */

.Lprint_line_loop_top:
    cmpw %r12w, %r15w                   /* for count < length */
    jg .Lprint_line_loop_end    
    movq (%r14,%r15,1), %rdi            /* get the character into arg1 of fputc */
    movq %r13, %rsi                     /* %rsi is not callee saved, so make sure this still points to file */
    call fputc                          /* fputc(buf[i], out) */
    incw %r15w                          /* count++ */
    jmp .Lprint_line_loop_top

.Lprint_line_loop_end:
    /* Restore callee saved registers, and realign stack */
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    addq $8, %rsp
    ret
/* -------------------------------------------------------------------------- */

count_occurrences:
    subq $8, %rsp                       /* Align the stack pointer*/

    /* Save callee-saved register values*/
    pushq %rbp                          /* length */
    pushq %r12                          /* occurrences */
    pushq %r13                          /* *line */
    pushq %r14                          /* *str */
    pushq %r15                          /* count */
    subq $8, %rsp

    movq $0, %r12                       /* occurrences = 0 */
    movq $0, %r15                       /* count == 0 */
    movq %rdi, %r13                     /* %r13 = *line */
    movq %rsi, %r14                     /* %r14 = *str */

    call find_string_length     
    movq %rax, %rbp                     /* find the line_len of the read-in line */

    movq %r14, %rdi
    call find_string_length             /* find the length of the string to be searched for */
    cmpq %rax, %rbp                     /* if the line length is shorter than the word, end the loop */
    jl .Lcount_occurrences_loop_end

    subq %rax, %rbp                     /* line_length - search_word_len = num iterations*/
    addq $1, %rbp                       /* %rbp + 1 */

.Lcount_occurrences_loop_top:
    cmpw %bp, %r15w                     /* while count < line_len - search_word_len */
    jge .Lcount_occurrences_loop_end
    leaq (%r13, %r15, 1), %rdi          /* iterate over the entire (string starts at line[count] now)*/
    movq %r14, %rsi                     /* arg2 is str */

    call strings_equal
    addw %ax, %r12w                     /* occurrences += strings_equal(line + i, str); adds 1 if string is found */
    incw %r15w                          /* count++ */
    jmp .Lcount_occurrences_loop_top

.Lcount_occurrences_loop_end:
    movq %r12, %rax                     /* return occurrences */

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
	subq $8, %rsp	                    /* Align stack */
	pushq %r15                          /* Save callee saved register value */
	movw $0, %r15w                      /* count = 0 */

.Lfind_string_length_top:
	cmpb $0, (%rdi, %r15, 1)            /* iterate until *s is null ('\0') */ 
	je .Lfind_string_length_done
	incw %r15w                          /* count++ */
	jmp .Lfind_string_length_top

.Lfind_string_length_done:
	movw %r15w, %ax                     /* return count */
	pop %r15                            /* restore callee saved register value */
	addq $8, %rsp                       /* realign stack */
	ret

/* -------------------------------------------------------------------------- */

strings_equal:
    subq $8, %rsp                       /* Align the stack pointer*/

    /* Save callee-saved register values*/
    pushq %r11                          /* search_word_len */
    pushq %r12                          /* matches */
    pushq %r13                          /* s1 */
    pushq %r14                          /* s2 */
    pushq %r15                          /* count */

    movw $0, %r12w                      /* matches = 0 */
    movq %rdi, %r13                     /* %r13 contains string 1*/
    movq %rsi, %r14                     /* %r14 contains string 2*/
    movq $0, %r15                       /* count = 0 */

    movq %r14, %rdi                     /* put s2 into arg1 */
    call find_string_length             /* find string length of string 2 */
    movw %ax, %r11w                     /* %r11 = length; no function calls so can use caller-saved */

.Lstrings_equal_loop_top:
    cmpw %r11w, %r15w                   /* for count < len(string 2) */
    jge .Lstrings_equal_or_not
    movb (%r13, %r15, 1), %r10b         /* get s1[count], store in %r10 */
    movb (%r14, %r15, 1), %al           /* s1[count] == s2[count] */
    cmpb %r10b, %al
    jz .Lstrings_equal_chars_equal
    incw %r15w                          /* count++ */
    jmp .Lstrings_equal_loop_top

.Lstrings_equal_chars_equal:
    incw %r12w                          /* matches++ */
    incw %r15w                          /* count++ */
    jmp .Lstrings_equal_loop_top
    
.Lstrings_equal_or_not:                 /* if matches == search_word_len */
    cmpw %r12w, %r11w
    je .Lstrings_equal
    movq $0, %rax
    jmp .Lstrings_equal_loop_end

.Lstrings_equal:
    movq $1, %rax

.Lstrings_equal_loop_end:
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
    subq $8, %rsp                       /* align stack pointer*/
    movq $0, %rax
    cmpq $3, %rdi                       /* if arc == 3 */
    je .Largc_is_three
    cmpq $4, %rdi                       /* if arc == 4 */
    je .Largc_is_four
    movq stderr(%rip), %rdi             /* else err: pass stderr */
	movq $inv_inp, %rsi                 /* pass "Invalid Input" */
    call fprintf
    movl $0, %eax
    movq $1, %rdi
    call exit
    
.Largc_is_three:
    movl $2, %eax
    jmp .Lhandle_arguments_end

.Largc_is_four:
    movl $3, %eax
    jmp .Lhandle_arguments_end

.Lhandle_arguments_end:
    addq $8, %rsp                       /* realign stack pointer*/
    ret

/* -------------------------------------------------------------------------- */

calc_total_occurrences:
    subq $8, %rsp                       /* Align the stack pointer*/
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


.Lcalc_tot_occ_loop_top:
    cmpb $0, %al                        /* while EOF is not reached (indicated by flag) */
    je .Lcalc_tot_occ_loop_end
    pushq %rbp                          /* save callee-saved register */
    subq $512, %rsp                     /* allocate 512 bits for buffer */
    movq %rsp, %rbp                     /* update %rbp to point to allocated memory */
    
    movq %r12, %rdi             
    movq %rbp, %rsi
    call read_line                      /* flag = read_line(fileptr, buf) (reads the line)*/
    pushq %rax                          /* save flag value (pushing %al instead messing things up)*/

    movq %rbp, %rdi              
    movq %r13, %rsi
    
    call count_occurrences              /* count the total occurrences in the line */
    addq %rax, %r15                     /* tot_occurrences += count_occurrences(buf, search)*/

    cmpl $3, %r14d                      /* if argc == 3 (i.e., no "-c" flag) */
    jne .Lcalc_tot_occ_loop_iterate
    cmpq $0, %rax                       /* && num_occurrences > 0, i.e. the line had the search string*/
    jle .Lcalc_tot_occ_loop_iterate     /* if no occurrences were found, iterate to next line */

    /* fprintf(stdout, buf) */
    movl $0, %eax
    movq stdout(%rip), %rdi             /* pass stdout */
	movq %rbp, %rsi                     /* print the line */
	call fprintf
	movl $0, %eax

    /* fprintf(stdout, /n) */
    movq stdout(%rip), %rdi             /* pass stdout */
	movq $newline, %rsi                 /* print a newline character */
	call fprintf
	movl $0, %eax

.Lcalc_tot_occ_loop_iterate:
    popq %rax                           /* get the flag to see EOF */
    addq $512, %rsp
    popq %rbp                           /* free(buf) (i.e., deallocate memory) */
    jmp .Lcalc_tot_occ_loop_top

.Lcalc_tot_occ_loop_end:
    movq %r15, %rax                     /* return tot_occurrences */

    /* restore callee-saved registers and re-align stack*/
    popq %r15                       
    popq %r14                       
    popq %r13                       
    popq %r12                  

    addq $8, %rsp                   
    ret
