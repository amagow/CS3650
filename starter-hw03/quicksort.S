.global main
.data
bad_input: .string "bad input"
int_fmt: .string "%ld"

.text
/*
void
read_int(long* ptr)
{
    if (scanf("%ld", ptr) != 1) {
        puts("bad input");
        exit(1);
    }
}
*/
/*
Variable mappings
ptr: %r12
*/
read_int:
push %r12
enter $8, $0

mov %rdi, %r12

//scanf("%ld", ptr)
mov $int_fmt, %rdi
lea (%r12), %rsi
mov $0, %al
call scanf

cmp $1, %rax
jne read_int_error

read_int_done:
lea (%r12), %rax

leave
pop %r12
ret

read_int_error:
//puts("bad input");
mov $bad_input, %rdi
call puts

//exit(1)
mov $60, %rax
mov $1, %rdi
syscall

/*
int
main(int _argc, char* _argv[])
{
    long nn;
    read_int(&nn);

    long* xs = malloc(nn * sizeof(long));

    for (long ii = 0; ii < nn; ++ii) {
        read_int(&(xs[ii]));
    }

    quicksort(xs, 0, nn);

    printf("%ld\n", nn);
    for (long ii = 0; ii < nn; ++ii) {
        printf("%ld ", xs[ii]);
    }
    printf("\n");

    free(xs);
    return 0;
}
*/
/*
Variable Mappings
argc: %rdi
nn: 8(%rsp)


*/
main:
  push %r12
  push %r13
  enter $16, $0

  #read_int(&nn);
  add 8(%rsp), %rdi
  call read_int

  #Test
  mov $int_fmt, %rdi
  mov 8(%rsp), %rsi
  mov $0, %al
  call printf

  mov $bad_input, %rdi
  call puts

main_done:
  leave
  pop %r13
  pop %r12
  ret

