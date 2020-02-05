#include <stdio.h>
#include <string.h>

#include "svec.h"

void sort(svec *xs)
{
    // TODO: sort input vector by string length
    // see "man 3 strlen"
    int pos = 0;
    while (pos < xs->size)
    {
        if (pos == 0 || strlen(xs->data[pos]) < strlen(xs->data[pos - 1]))
            ++pos;
        else{
            svec_swap(xs, pos - 1, pos);
            --pos;
        }
    }
}

void chomp(char *text)
{
    // int n = strlen(text);
    // if(text[n-1] == "\n")
    //     text[n-1] = 0;
    strtok(Name, "\n");
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage:\n  %s input-file\n", argv[0]);
        return 1;
    }

    FILE *fh = fopen(argv[1], "r");
    if (!fh)
    {
        perror("open failed");
        return 1;
    }

    svec *xs = make_svec();
    char temp[128];

    while (1)
    {
        char *line = fgets(temp, 128, fh);
        if (!line)
        {
            break;
        }

        chomp(line);
        svec_push_back(xs, line);
    }

    fclose(fh);

    sort(xs);

    for (int ii = 0; ii < xs->size; ++ii)
    {
        char *line = svec_get(xs, ii);
        printf("%s\n", line);
    }

    free_svec(xs);
    return 0;
}