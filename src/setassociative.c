#include "common.h"

enum Status setAssociativeAccess(struct Cache* cache, char op, \
                unsigned long long address, enum Policy policy)
{
        unsigned long long group = getSelector(address, cache->s, cache->b, cache->m);
        unsigned long long tag = getTag(address, cache->s, cache->b, cache->m);

        bool tag_existence, valid;
        unsigned long long linum = checkTagAndValid(cache, group, tag, &tag_existence, &valid);

        if (tag_existence) {
                if (valid) {
                        Log("tag_existence, valid: HIT");
                        updateTimeStamp(cache, group, linum);
                        increaseAccessCount(cache, group, linum);
                        if (op == 'L') {
                                return READ_HIT;
                        } else {
                                return WRITE_HIT;
                        }
                } else {  // cold miss
                        Log("tag_existence, not valid: COLD_MISS");
                        unsigned long long empty_linum;
                        bool found;
                        empty_linum = findEmptyCacheLine(cache, group, &found);
                        if (empty_linum >= cache->E) {
                                Error("empty_linum >= cache->E");
                        }
                        if (!found) {
                                Error("cache should have empty line, but not!");
                        }
                        loadDataToSpecificCacheLine(cache, group, empty_linum, address);
                        return COLD_MISS;
                }
        } else {
                if (valid) {  // current cache set if full
                        unsigned long long victim_cache_linum = findVictimCacheLine(cache, group, policy);
                        if (victim_cache_linum >= cache->E) {
                                Log("***********************************");
                                Log("victim_cache_linum >= cache->E!!!");
                                Log("***********************************");
                                Error("Fatal Error!!!");
                        }
                        evictCacheLine(cache, group, victim_cache_linum, address);

                        if (isCacheFull(cache)) {
                                Log("not tag_existence, valid: CAPACITY_MISS");
                                return CAPACITY_MISS;
                        } else {
                                Log("not tag_existence, valid: CONFLICT_MISS");
                                return CONFLICT_MISS;
                        }
                } else {  // COLD_MISS
                        Log("not tag_existence, not valid: COLD_MISS");
                        unsigned long long empty_linum;
                        bool found;
                        empty_linum = findEmptyCacheLine(cache, group, &found);
                        if (!found) {
                                Error("cache should have empty line, but not!");
                        }
                        loadDataToSpecificCacheLine(cache, group, empty_linum, address);
                        return COLD_MISS;
                }
        }
}
