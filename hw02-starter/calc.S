#include <stdio.h>
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
