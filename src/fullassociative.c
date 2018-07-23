#include "common.h"

/*
 * REMEMOBER: You only have one set
 * so, only COLD_MISS and CAPACITY_MISS exist.
 * Also, you can consider there are only COLD_MISS and CONFLICT_MISS.
 * It depends on how you treat it.
 */
enum Status fullAssociativeAccess(struct Cache* cache, char op, unsigned long long address)
{
        unsigned long long group = getSelector(address, cache->s, cache->b, cache->m);  // 0
        if (group != 0) {
                Error("Not fully associative!");
        }
        unsigned long long tag = getTag(address, cache->s, cache->b, cache->m);

        bool tag_existence, valid;
        checkTagAndValid(cache, group, tag, &tag_existence, &valid);

        /* Randomized Replacing */
        if (tag_existence) {
                if (valid) {  // HIT
                        if (op == 'L') {
                                return READ_HIT;
                        } else { 
                                // you can set dirty, but no need
                                return WRITE_HIT;
                        }
                } else {  // not valid, cold miss, and must have an empty line
                        unsigned long long empty_linum;
                        bool found;
                        empty_linum = findEmptyCacheLine(cache, group, &found);
                        loadDataToSpecificCacheLine(cache, group, empty_linum, address);
                        return COLD_MISS;
                }
        } else {  // must miss
                if (valid) {  // full, need evict, random replacement
                        unsigned long long victim_linum = 0;  // randomized generated
                        evictCacheLine(cache, group, victim_linum, address);
                        return CAPACITY_MISS;
                } else {  // COLD_MISS 
                        unsigned long long empty_linum;
                        bool found;
                        empty_linum = findEmptyCacheLine(cache, group, &found);
                        loadDataToSpecificCacheLine(cache, group, empty_linum, address);
                        //return CONFLICT_MISS;
                        return COLD_MISS;
                }
        }
}
