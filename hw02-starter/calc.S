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
    argv: %r12
    op1: %r13
    op2: %r14
*/
.global main
.text

main:
enter $0, $0

cmp $4, %rdi
jne usage_err

mov %rsi, %r12
mov 16(%r12), %rax
mov (%rax), %al

#int x = atoi(argv[1]);
mov 8(%r12), %rdi
call atoi
mov %rax, %r13

#int y = atoi(argv[3]);
mov 24(%r12), %rdi
call atoi
mov %rax, %r14

cmpb $'+, %al
je add

cmpb $'-, %al
je sub

cmpb $'*, %al
je mul

cmpb $'/, %al
je div

usage_err:
mov $usage_err_msg, %rdi
call puts
mov $1, %rax
jmp main_done


add:
add %r13, %r14
mov %r14, %r8
jmp print_output

sub:
sub %r13, %r14
mov %r14, %r8
jmp print_output

mul:
imul %r13, %r14
mov %r14, %r8
jmp print_output

div:
mov %r13, %rax
cqo
idiv %r14
mov %rax, %r8
jmp print_output

print_output:
mov $fmt, %rdi
mov %r13, %rsi
mov 16(%r12), %rdx
mov %r14, %rcx 
mov $0, %al
call printf

main_done:
leave
ret

.data
usage_err_msg: .string "Usage:\n./acalc N op N"
fmt: .string "%d %s %d = %d\n"