#ifndef CSIM
#define CSIM
#include <stdbool.h>

#define ARCHITECTURE 64

extern unsigned long long g_program_counter;

enum Policy {
        DIRECT_MAPPING_REPLACEMENT, // for direct mapping
        RANDOM_REPLACEMENT,  // for full associative
        LRU,
        LFU,
};

enum Status {
        HIT,
        MISS,
        READ_HIT,
        WRITE_HIT,
        COLD_MISS,
        CONFLICT_MISS,
        CAPACITY_MISS,
};

struct Line {
        bool valid;
        //bool dirty;  // this can be ignored
        unsigned long long address;
        unsigned long long tag;
        unsigned long long access_count;  // for LFU
        unsigned long long access_time;  // for LRU
};

struct Set {
        struct Line* lines;
};

struct Cache {
        int s;
        unsigned long long S;
        unsigned long long E;
        int b;
        int m;
        bool full;
        struct Set* sets;
};

/* Parser */
void parseArgs(int argc, char* argv[], int *s, unsigned long long *E, \
                int *b, char tracefile[], bool *vlag);

void parseLine(char*, char*, unsigned long long*, int*);

/* Cache initialization and cache information */
void initCache(struct Cache* cache, int s, unsigned long long E, int b);

void cacheInfo(struct Cache* cache);

bool isCacheFull(struct Cache* cache);

/* Hleper Function */
unsigned long long getSelector(unsigned long long address, int s, int b, int m);

unsigned long long getTag(unsigned long long address, int s, int b, int m);

unsigned long long checkTagAndValid(struct Cache* cache, unsigned long long group, unsigned long long tag, 
                bool* tag_existence, bool* valid);

unsigned long long findEmptyCacheLine(struct Cache* cache, unsigned long long group, bool *found);

void updateTimeStamp(struct Cache* cache, unsigned long long group, unsigned long long linum);

void increaseAccessCount(struct Cache *cache, unsigned long long group, unsigned long long linum);

unsigned long long findVictimCacheLine(struct Cache* cache, unsigned long long group, enum Policy policy);

/* load or evict */
void evictCacheLine(struct Cache* cache, unsigned long long group, unsigned long long linenum, \
                unsigned long long address);

void loadDataToSpecificCacheLine(struct Cache* cache, unsigned long long group, \
                unsigned long long linenum, unsigned long long address);


/* Access Cache */
enum Status directMappingAccess(struct Cache* cache, char op, unsigned long long address);

enum Status fullAssociativeAccess(struct Cache* cache, char op, unsigned long long address);

enum Status setAssociativeAccess(struct Cache* cache, char op, unsigned long long address, \
                enum Policy policy);

enum Status accessCache(struct Cache*, char, unsigned long long);

#endif
