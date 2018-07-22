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

void initCache(struct Cache* cache, int s, unsigned long long E, int b);

void cacheInfo(struct Cache* cache);

/* Parser */
void parseArgs(int argc, char* argv[], int *s, unsigned long long *E, \
                int *b, char tracefile[], bool *vlag);

void parseLine(char*, char*, unsigned long long*, int*);

/* Hleper Function */
unsigned long long getSelector(unsigned long long address, int s, int b, int m);

unsigned long long getTag(unsigned long long address, int s, int b, int m);

bool getValid(struct Cache* cache, unsigned long long address, int s, int, int);

bool checkTag(struct Cache* cache, unsigned long long group, unsigned long long tag);

void checkTagAndValid(struct Cache* cache, unsigned long long group, unsigned long long tag, 
                bool* tag_existence, bool* valid);


void evictCacheLine(struct Cache* cache, unsigned long long group, unsigned long long linenum, unsigned long long address);

void loadDataToSpecificCacheLine(struct Cache* cache, unsigned long long group, \
                unsigned long long linenum, unsigned long long address);

bool isCacheFull(struct Cache* cache);

unsigned long long findEmptyCacheLine(struct Cache* cache, unsigned long long group, bool *found);

// void eviction(struct Cache* cache, unsigned long long group, unsigned long long address, enum Policy);


/* Access Cache */
enum Status directMappingAccess(struct Cache* cache, char op, unsigned long long address);

enum Status fullAssociativeAccess(struct Cache* cache, char op, unsigned long long address);

enum Status accessCache(struct Cache*, char, unsigned long long);

#endif
