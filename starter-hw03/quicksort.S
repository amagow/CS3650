.global main
.data
bad_input: .string "bad input"
int_fmt: .string "%ld"
new_line: .string "\n"
int_fmt_nl: .string "%ld\n"
int_fmt_space: .string "%ld "

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

#mov %rdi, %r12

//scanf("%ld", ptr)
  mov %rdi, %rsi
  mov $int_fmt, %rdi
  mov $0, %al
  call scanf

#mov $1, %rax Testing segmentation fault boundaries
  cmp $1, %rax
  jne read_int_error

read_int_done:
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
*xs: %r12
ii: %r15 

*/
main:
  push %r12
  push %r13
  //Initialise space on the stack;  long nn;
  enter $16, $0

  #read_int(&nn);
  lea 8(%rsp), %rdi
  call read_int

  #long* xs = malloc(nn * sizeof(long))
  mov 8(%rsp), %rax
  mov $8, %rdx
  imul %rdx
  
  mov %rax, %rdi
  call malloc
  mov %rax, %r12


  // r15 is the ii
  mov $0, %r15
for_loop_input:
/*
for (long ii = 0; ii < nn; ++ii) {
        read_int(&(xs[ii]));
    }
*/
  cmp 8(%rsp), %r15
  jge for_loop_input_done

  lea (%r12, %r15, 8), %rdi
  call read_int

  inc %r15
  jmp for_loop_input



for_loop_input_done:

#printf("%ld\n", nn);
  mov $int_fmt_nl, %rdi
  mov 8(%rsp), %rsi
  mov $0, %al
  call printf

  mov $0, %r15
  jmp for_loop_print

for_loop_print:
/*
for (long ii = 0; ii < nn; ++ii) {
        printf("%ld ", xs[ii]);
    }
*/
cmp  8(%rsp), %r15
jge for_loop_print_done

mov $int_fmt_space, %rdi
mov (%r12, %r15, 8), %rsi
mov $0, %al
call printf

inc %r15
jmp for_loop_print

for_loop_print_done:


jmp main_done

main_done:
  //printf("\n");
  mov $new_line, %rdi
  mov $0, %al
  call printf
  
  //free(xs);
  mov (%r12), %rdi
  call free
  //return 0;

  leave
  pop %r13
  pop %r12

  mov $0, %rax
  ret

