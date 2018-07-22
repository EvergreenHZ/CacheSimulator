#include "common.h"

unsigned long long g_program_counter = 0;

int main(int argc, char *argv[])
{
        int s = -1, b = -1;
        unsigned long long E = -1;
        char tracefile[32];
        bool vflag;

        FILE *fp;
        char line_buf[32];
        size_t len = 32;
        ssize_t read;

        parseArgs(argc, argv, &s, &E, &b, tracefile, &vflag);

        struct Cache cache;
        initCache(&cache, s, E, b);
        //cacheInfo(&cache);

        // parse trace file
        fp = fopen(tracefile, "r");
        if (fp == NULL) {
                exit(EXIT_FAILURE);
        }

        int miss = 0;
        int hit = 0;
        int cold_miss = 0;
        int conflict_miss = 0;
        int capacity_miss = 0;
        int eviction = 0;

        while (fgets(line_buf, 32, fp) != NULL) {

                g_program_counter ++;
                LOG("clock: %llu, ins: %s\n", g_program_counter, line_buf);
                //Log(line_buf);
                /* counter */
                // here, get operation(L, M, S) and address;
                char operation;
                unsigned long long address;
                int size;
                //Log(line_buf);
                parseLine(line_buf, &operation, &address, &size);
                printf("address is: %llu\n", address);

                enum Status status;
                status = accessCache(&cache, operation, address);
                switch (status) {
                        case READ_HIT: 
                        case WRITE_HIT:
                                hit++;
                                break;
                        case COLD_MISS: 
                                miss++;
                                cold_miss++;
                                break;
                        case CONFLICT_MISS: 
                                miss++;
                                conflict_miss++;
                                eviction++;
                                break;
                        case CAPACITY_MISS: 
                                miss++;
                                capacity_miss++;
                                eviction++;
                                break;
                }

        }
        printf("HIT: %d\n", hit);
        printf("MISS: %d\n", miss);

        fclose(fp);
        return 0;
}
