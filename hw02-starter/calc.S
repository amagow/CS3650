/*#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    if(argc != 4 || (strncmp(argv[2], "+", 8) && strncmp(argv[2], "-", 8) && strncmp(argv[2], "*", 8) && strncmp(argv[2], "/", 8) ) ){
        puts("Usage:\n./ccalc N op N");
        return 1;
    }

    int x = atoi(argv[1]);
    int y = atoi(argv[3]);
    char * fmt = "%d %s %d = %d\n";

    if(!strncmp(argv[2], "+", 8))
        printf(fmt, x, argv[2], y, x + y);

    if(!strncmp(argv[2], "-", 8))
        printf(fmt, x, argv[2], y, x - y);
    
    if(!strncmp(argv[2], "*", 8))
        printf(fmt, x, argv[2], y, x * y);
    
    if(!strncmp(argv[2], "/", 8))
        printf(fmt, x, argv[2], y, x / y);

    return 0;
}
*/
/*
Variable mappings:
    argc: %rdi
    argv: %rsi
    op1: %r13
    operand: %r12
    op2: %r10
*/
.global main
.text

main:
enter $0, $0

cmp $4, %rdi
jne usage_err

mov 16(%rsi), %rax
mov (%rax), %al

cmpb $42, %al
je test_correct

cmpb $45, %al
je test_correct

cmpb $42, %al
je test_correct

cmpb $47, %al
je test_correct

#mov %r9, %rdi
#call puts
#jmp main_done

usage_err:
mov $usage_err_msg, %rdi
call puts
mov $1, %rax
jmp main_done

test_correct:
mov $test, %rdi
call puts
jmp main_done

add:
jmp print_output

sub:
jmp print_output

mul:
jmp print_output

div:
jmp print_output

print_output:
mov $fmt, %rdi
mov $0, %rsi
mov $1, %rdx
mov $0, %rcx 
mov $0, %al
call printf

main_done:
leave
ret

.data
usage_err_msg: .string "Usage:\n./acalc N op N"
fmt: .string "%d %d %d = %d\n"
test: .string "Jump correct"
test_fmt: .string "%d"
plus: .string "+"
minus: .string "-"
asterisk: .string "*"
forward_slash: .string "/"