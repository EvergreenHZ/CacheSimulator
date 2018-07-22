#include "common.h"

// L 10,1
// M 20,1
// L 22,1
// S 18,1
// L 110,1
// L 210,1
// M 12,1
/*
 * Direct Mapping
 * E = 1
 */

enum Status directMappingAccess(struct Cache* cache, char op, unsigned long address)
{
        unsigned long group = getSelector(address, cache->s, cache->b, cache->m);
        unsigned long tag   = getTag(address, cache->s, cache->b, cache->m);

        bool tag_existence, valid;
        checkTagAndValid(cache, group, tag, &tag_existence, &valid);

        if (tag_existence) {
                if (valid) {  // HIT
                        if (op == 'L') {
                                return READ_HIT;
                        } else {
                                return WRITE_HIT;
                        }
                } else {
                        loadDataToSpecificCacheLine(cache, group, 0, address);
                        return COLD_MISS;
                }
        } else {  // must miss
                /*
                 * Please pay attention to the function checkTagAndValid
                 * comment is in csim.c
                 */
                if (valid) {
                        /* for current cache group, full
                         * we need to check if the whole cache is full
                         * then return right statue
                         */
                        evictCacheLine(cache, group, 0, address);
                        if (isCacheFull(cache)) {
                                return CAPACITY_MISS;
                        } else {
                                return CONFLICT_MISS;
                        }
                } else {
                        /* there is still a empty cache line in current group
                         * so, just load the data
                         */
                        evictCacheLine(cache, group, 0, address);
                        return CONFLICT_MISS;
                }
        }
}
