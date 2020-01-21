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
*/
.global main
.text

main:
enter $0, $0

cmp $4, %rdi
jne usage_err

mov 16(%rsi), %r9
mov %r9, %rdi
call puts
jmp main_done

usage_err:
mov $usage_err_msg, %rdi
call puts
mov $1, %rax
jmp main_done

main_done:
leave
ret

.data
usage_err_msg: .string "%d %s %d = %d\n"