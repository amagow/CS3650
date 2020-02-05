
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// TODO: sudo apt install libbsd-dev
// This provides strlcpy
// See "man strlcpy"
#include <bsd/string.h>
#include <string.h>

#include "hashmap.h"


int
hash(char* key)
{
    //Test hash function
    size_t hh = 0;
    for (size_t ii = 0; key[ii]; ++ii) {
        hh = hh * 67 + key[ii];
    }
    return hh;
}

hashmap_pair*
make_pair(const char* key, int val)
{
    hashmap_pair* pp = calloc(1, sizeof(hashmap_pair));
    pp->key = strdup(key);
    pp->used = true;
    pp->tomb = false;
    return pp;
}

void
free_pair(hashmap_pair* pp)
{
    free(pp->key);
    free(pp);
}

hashmap*
make_hashmap_presize(int nn)
{
    hashmap* hh = calloc(1, sizeof(hashmap));
    hh->size = 0;
    hh->capacity = nn;
    hh->data = calloc(hh->capacity, sizeof(hashmap_pair*));
    // Double check "man calloc" to see what that function does.
    return hh;
}

hashmap*
make_hashmap()
{
    return make_hashmap_presize(4);
}

void
free_hashmap(hashmap* hh)
{
    for (int ii = 0; ii < trips->size; ++ii) {
        if(mm->data[ii]){
            free_pair(mm->data[ii]);
        }
    }
    free(hh->data);
    free(hh);
}

int
hashmap_has(hashmap* hh, char* kk)
{
    return hashmap_get(hh, kk) != -1;
}

int
hashmap_get(hashmap* hh, char* kk)
{
    // TODO: Return the value associated with the
    // key kk.
    // Note: return -1 for key not found.
    for(int ii = 0; ii < hh->size; ++i){
        if(!strcmp((hh->data[ii])->key, kk)){
            return (hh->data[ii])->val;
        } //strcmp 0 if both strings are equal
    }
    return -1;
}

void
grow_map(hashmap* hh)
{
    size_t nn = mm->capacity;
    hashmap_pair** data = mm->data;

    mm->capacity = 2 * nn;
    mm->data = calloc(mm->capacity, sizeof(hashmap_pair*));
    mm->size = 0;

    for (size_t ii = 0; ii < nn; ++ii) {
        for (hashmap_pair* curr = data[ii]; curr; curr = curr->next) {
            hashmap_put(mm, curr->key, curr->val);
        }
        free(data[ii]);
    }
    free(data);

}

void
hashmap_put(hashmap* hh, char* kk, int vv)
{
    // TODO: Insert the value 'vv' into the hashmap
    // for the key 'kk', replacing any existing value
    // for that key.
    if(hh->size >= (hh->capacity/2)){
        grow_map(hh);
    }

    int ii = hash(kk) & (hh->capacity - 1); //index

    //Linear probing
    while(hh->data[ii]->used || !hh->data[ii]->tomb){
        
        if(!strcmp(hh->data[ii]->key, kk)){
            hh->data[ii]->key = vv;
            return;
        }

        // ii = (ii + 1) & (hh->capacity - 1);
        ii = (ii + 1) % hh->capacity;

    }
    //Item has been deleted
    if(hh->data[ii]->tomb){
        hh->data[ii]->key = strdup(kk);
        hh->data[ii]->val = vv;
    }
    else{
        hh->data[ii] = make_pair(kk, vv);
    }
    hh->size += 1;
}

void
hashmap_del(hashmap* hh, char* kk)
{
    // TODO: Remove any value associated with
    // this key in the map.
    for(int ii = 0; ii < hh->size; ++ii){
        if(!strcmp((hh->data[ii])->key, kk)){
            (hh->data[ii])->tomb = true;
            free(hh->data[ii]->key);
        } //strcmp 0 if both strings are equal
    }
    return;

}

hashmap_pair
hashmap_get_pair(hashmap* hh, int ii)
{
    // TODO: Get the {k,v} pair stored in index 'ii'.
    return *hh->data[ii];
}

void
hashmap_dump(hashmap* hh)
{
    printf("== hashmap dump ==\n");
    for (int ii = 0; ii < hh->size; ++ii) {
        //Get hashmap at index ii
        hashmap_pair pair = hashmap_get_pair(hh, ii);
        if (!pair.used || pair.tomb) {
            continue;
        }
        printf("%d\t%s\n", pair.val, pair.key);
    }
    // TODO: Print out all the keys and values currently
    // in the map, in storage order. Useful for debugging.
}
