.global fib
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