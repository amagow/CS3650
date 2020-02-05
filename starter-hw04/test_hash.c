#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "hashmap.h"

int main(int argc, char* argv[]){
    hashmap* hash = make_hashmap();
    hashmap_put(hash, "abc", 1);
    hashmap_put(hash, "def", 1);
    hashmap_put(hash, "dca", 1);
    hashmap_put(hash, "qow", 1);

    printf("%s\t%d\n", "abc" ,hashmap_get(hash, "abc"));
    hashmap_dump(hash);
} 