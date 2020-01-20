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
je fib_zero

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

leave
ret

fib_one:
mov $1, %rax
leave 
ret

fib_zero:
mov $0, %rax
leave 
ret

/*
int
main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("Usage:\n  %s N, where N > 0\n", argv[0]);
        return 0;
    }

    printf("fib(%ld) = %ld\n", atol(argv[1]), atol(argv[1]));
    return 0;
}
*/
/*
    variable mappings
        argc: %rdi
        y: %r15
*/
main:


.data
usage_error: .string "Usage:\n  %s N, where N > 0\n"
fib_result: .string "fib(%ld) = %ld\n"