#ifndef CSIM
#define CSIM
#include <stdbool.h>

#define ARCHITECTURE 64

extern unsigned long g_program_counter;

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
        unsigned long address;
        unsigned long tag;
        unsigned long access_count;  // for LFU
        unsigned long access_time;  // for LRU
};

struct Set {
        struct Line* lines;
};

struct Cache {
        int s;
        unsigned long S;
        unsigned long E;
        int b;
        int m;
        bool full;
        struct Set* sets;
};

unsigned long getSelector(unsigned long address, int s, int b, int m);

unsigned long getTag(unsigned long address, int s, int b, int m);

bool getValid(struct Cache* cache, unsigned long address, int s, int, int);

bool checkTag(struct Cache* cache, unsigned long group, unsigned long tag);

void checkTagAndValid(struct Cache* cache, unsigned long group, unsigned long tag, 
                bool* tag_existence, bool* valid);

void initCache(struct Cache* cache, int s, unsigned long E, int b);

void parseArgs(int argc, char* argv[], int *s, unsigned long *E, \
                int *b, char tracefile[], bool *vlag);

void parseLine(char*, char*, unsigned long*, int*);

void evictCacheLine(struct Cache* cache, unsigned long group, unsigned long linenum, unsigned long address);

void loadDataToSpecificCacheLine(struct Cache* cache, unsigned long group, \
                unsigned long linenum, unsigned long address);

bool isCacheFull(struct Cache* cache);

enum Status directMappingAccess(struct Cache* cache, char op, unsigned long address);

enum Status accessCache(struct Cache*, char, unsigned long);

//bool isGroupFull
#endif
