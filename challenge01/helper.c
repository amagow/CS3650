#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void chomp(char *text)
{
    strtok(text, "\n");
}

char *read_line(char *text)
{
    //Read line from buffer to text
    char *rv = fgets(text, 1000, stdin);
    if (!rv)
    {
        //If EOF is reached
        exit(0);
    }
    return text;
}