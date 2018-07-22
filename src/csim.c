#include "common.h"

void Error(char *msg)
{
        printf("%s\n", msg);
        exit(-1);
}

void* Malloc(size_t num_of_space)
{
        void *p = malloc(num_of_space);
        if (p == NULL) {
                Error("Malloc Failed!");
        }
        return p;
}
/* Cache is determined by (s, E, b, m)
 * S = 2^s : sets
 * E       : associativity
 * B = 2^b : block size (or cache line size)
 * m       : 64bits here
 *
 * E = 1    : direct mapping
 * S = 1    : full associative
 * otherwise: set associative
 */
void initCache(struct Cache* cache, int s, unsigned long long E, int b)
{
        cache->s = s;
        cache->E = E;
        cache->b = b;
        cache->m = 64;
        cache->full = false;
        unsigned long long S = 1;
        for (int i = 0; i < s; i++) {
                S *= 2;
        }
        cache->S = S;
        //Log("initCache");
        cache->sets = (struct Set*)Malloc(S * sizeof(struct Set));
        
        for (int i = 0; i < S; i++) {
                cache->sets[i].lines = (struct Line*)Malloc(E * sizeof(struct Line));
        }

        for (int i = 0; i < S; i++) {
                for (int j = 0; j < E; j++) {
                        (cache->sets + i)->lines[j].valid = false;
                        //(cache->sets + i)->lines[j].dirty = false;
                        (cache->sets + i)->lines[j].address = 0l;
                        (cache->sets + i)->lines[j].access_count = 0l;
                        (cache->sets + i)->lines[j].access_time = -1l;
                }
        }
}

unsigned long long getSelector(unsigned long long address, int s, int b, int m)
{
        /* eg. m = 64, s = 0, b = 6, t = 58 */
        unsigned long long t = m - s - b;
        address <<= t;
        address >>= (t + b);
        return address;
}

unsigned long long getTag(unsigned long long address, int s, int b, int m)
{
        return address >>= (b + s);
}

bool getValid(struct Cache* cache, unsigned long long address, int s, int b, int m)
{
        unsigned long long group = getSelector(address, s, b, m);
        unsigned long long tag = getTag(address, s, b, m);
        struct Line* selected_set = cache->sets[group].lines;
        for (int i = 0; i < cache->E; i++) {
                if (tag == selected_set[i].tag) {
                        return selected_set[i].valid;
                }
        }
        return false;
}

bool checkTag(struct Cache* cache, unsigned long long group, unsigned long long tag)
{
        struct Line* selected_set = cache->sets[group].lines;
        for (int i = 0; i < cache->E; i++) {
                if (tag == selected_set[i].tag) {
                        return true;
                }
        }
        return false;
}

/*
 * This function is subtle.
 * Noticing that:
 * 1. if tag_existence, then valid is the corresponding value
 * 2. else, I just search the current group to check if there is an empty cache line
 *      i). if current group is full, valid is set to true
 *      ii). else valid equals to false, which means there is still an empty cache line
 *              and we can use this information to choose the right replacing strategy.
 */
void checkTagAndValid(struct Cache* cache, unsigned long long group, unsigned long long tag, \
                bool* tag_existence, bool* valid)
{
        struct Line* selected_set = cache->sets[group].lines;
        *tag_existence = false;
        *valid = false;
        bool t_valid = true;
        for (int i = 0; i < cache->E; i++) {
                if (!selected_set[i].valid) {
                        t_valid = false;
                }
                if (tag == selected_set[i].tag) {
                        *tag_existence = true;
                        *valid = selected_set[i].valid;
                }
        }
        if (!*tag_existence) {
                *valid = t_valid;
        }
}

bool isCacheFull(struct Cache* cache)
{
        if (cache->full) {
                return true;
        } else {
                for (int i = 0; i < cache->S; i++) {
                        for (int j = 0; j < cache->E; j++) {
                                if (!cache->sets[i].lines[j].valid) {
                                        return false;
                                }
                        }
                }
                cache->full = true;
                return true;
        }
}

void evictCacheLine(struct Cache* cache, unsigned long long group, unsigned long long linum, unsigned long long address)
{
        struct Line* specific_line = &(cache->sets[group].lines[linum]);
        specific_line->valid = true;
        //specific_line->dirty = false;
        specific_line->address = address;
        specific_line->tag = getTag(address, cache->s, cache->b, cache->m);
        specific_line->access_time = g_program_counter;  // this may not good
        specific_line->access_count = 1;
}

void loadDataToSpecificCacheLine(struct Cache* cache, unsigned long long group, unsigned long long linum, unsigned long long address)
{
        evictCacheLine(cache, group, linum, address);
}

enum Status accessCache(struct Cache* cache, char op, unsigned long long address)
{
        //return directMappingAccess(cache, op, address);
        return fullAssociativeAccess(cache, op, address);
}

unsigned long long findEmptyCacheLine(struct Cache* cache, unsigned long long group, bool *found)
{
        *found = false;
        struct Line* selected_set = cache->sets[group].lines;
        for (int i = 0; i < cache->E; i++) {
                if (selected_set[i].valid == false) {  // empty line exits
                        *found = true;
                        return i;
                }
        }
        return 0;
}

void cacheInfo(struct Cache* cache)
{
       LOG("Cache Info:\n");
       LOG("S: %llu\n", cache->S);
       LOG("E: %llu\n", cache->E);
       LOG("b: %d\n", cache->b);
}
