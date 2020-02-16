#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void chomp(char *text)
{
    strtok(text, "\n");
}

void *read_line(char *text,int buffer_size, FILE * stream)
{
    //Read line from buffer to text
    char *rv = fgets(text, buffer_size, stream);
    if (!rv)
    {
        //If EOF is reached
        exit(0);
    }
}