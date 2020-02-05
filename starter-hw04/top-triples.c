#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "hashmap.h"

void
chomp(char* text)
{
    text[strcspn(text, "\n")] = 0;
}

int
main(int argc, char* argv[])
{
    //We do not have 3 arguments
    if (argc != 3) {
        printf("Usage:\n  %s input N\n", argv[0]);
        return 1;
    }

    //Read from file
    FILE* fh = fopen(argv[1], "r");
    if (!fh) {
        perror("open failed");
        return 1;
    }

    //Number of top triples
    long NN = atol(argv[2]);

    //Create Hashmap
    hashmap* trips = make_hashmap();
    char temp[128];
    char trip[4];

    while (1) {
        //Read line from file
        char* line = fgets(temp, 128, fh);
        if (!line) {
            //If line read is unseccesfull
            break;
        }

        //Remove new line
        chomp(line);

        //Make triples of each word
        int zz = strlen(line) - 2;
        for (int ii = 0; ii < zz; ++ii) {
            for (int jj = 0; jj < 3; ++jj) {
                trip[jj] = line[ii + jj];
            }
            //Null character for the string
            trip[3] = 0;

            //Increment count of the triplet if it is present in the hashmap
            if (hashmap_has(trips, trip)) {
                int count = hashmap_get(trips, trip);
                hashmap_put(trips, trip, count + 1);
            }
            else {
                hashmap_put(trips, trip, 1);
            }
        }
    }

    //Close the stream
    fclose(fh);


    hashmap_pair tops[NN];
    for (int ii = 0; ii < NN; ++ii) {
        tops[ii].val = 0;
    }

    // TODO: 0 is probably the wrong loop bound here.
    for (int ii = 0; ii < trips->size; ++ii) {
        //Get hashmap at index ii
        hashmap_pair pair = hashmap_get_pair(trips, ii);
        if (!pair.used || pair.tomb) {
            continue;
        }

        int count = pair.val;

        // 
        for (int jj = 0; jj < NN; ++jj) {
            if (count > tops[jj].val) {
                hashmap_pair tmp = tops[jj];
                tops[jj] = pair;
                pair = tmp;
            }
        }
    }

    //Print top n tops
    for (int ii = 0; ii < NN; ++ii) {
        hashmap_pair pair = tops[ii];
        printf("%d\t%s\n", pair.val, pair.key);
    }

    free_hashmap(trips);
    return 0;
}
