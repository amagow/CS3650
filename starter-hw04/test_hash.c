#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "hashmap.h"

int main(int argc, char* argv[]){
    hashmap* hash = make_hashmap();
    map_put(hash, "abc", 1);
    map_put(hash, "def", 1);
    map_put(hash, "dca", 1);
    map_put(hash, "qow", 1);

    printf("%s\t%d\n", "abc" ,map_get(hash, "abc"));
    hashmap_dump(hash);
} 