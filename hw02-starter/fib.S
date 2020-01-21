.global main
/*
long fib(long x){
    if(x == 0) return 0;
    if(x == 1)  return 1;
    return fib(x - 1) + fib(x-2);
}
*/
/*
    variable mappings
        x: %r15
        return: %r14
*/
.text

fib:
enter $0, $0

cmp $0, %rdi
je fib_zero

cmp $1, %rdi
je fib_one

push %r15
push %r14

mov %rdi, %r15
mov $0, %r14

//fib(x-1)
dec %r15
mov %r15, %rdi
call fib
add %rax, %r14

//fib(x-2)
dec %r15
mov %r15, %rdi
call fib
add %rax, %r14

mov %r14, %rax

pop %r14
pop %r15

jmp fib_done

fib_one:
mov $1, %rax
jmp fib_done

fib_zero:
mov $0, %rax
jmp fib_done

fib_done:
leave 
ret

/*
int
main(int argc, char* argv[])
{
    if (argc != 2 || atol(argv[1]) < 0) {
        printf("Usage:\n  %s N, where N > 0\n", argv[0]);
        return 1;
    }
    printf("fib(%ld) = %ld\n", atol(argv[1]), fib(atol(argv[1])));
    return 0;
}
*/
/*
    variable mappings
        argc: %rdi
        argv: %rsi
        y = argv[1]
        y: %r15
*/
main:
enter $0, $0

cmp $2, %rdi
jne usage_error

//atol(argv[1])
mov 8(%rsi), %rdi
call atol
cmp $0, %rax
jl usage_error

mov %rax, %r15


mov %r15, %rdi
call fib

mov $fib_result, %rdi
mov %r15, %rsi
mov %rax, %rdx
mov $0, %al
call printf
mov $0, %rax
jmp main_end

usage_error:
mov $usage_error_msg, %rdi
call puts
mov $1, %rax
jmp main_end

main_end:
leave
ret


.data
usage_error_msg: .string "Usage:\n  %s N, where N > 0"
fib_result: .string "fib(%ld) = %ld\n"