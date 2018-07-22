#include "common.h"

void Log(char* msg)
{
        printf("%s\n", msg);
}

void usage()
{
        printf("Usage: ./csim [-h] [-v] -s <s> -E <E> -b <b> -t <tracefile>\n \
        • -h: Optional help flag that prints usage info\n \
        • -v: Optional verbose flag that displays trace info \n \
        • -s <s>: Number of set index bits (S = 2^s is the number of sets) \n \
        • -E <E>: Associativity (number of lines per set) \n \
        • -b <b>: Number of block bits (B = 2^b is the block size) \n \
        • -t <tracefile>: Name of the valgrind trace to replay\n");
}
        
void parseArgs(int argc, char* argv[], int *s, unsigned long *E, \
        int *b, char tracefile[], bool *vflag)
{
        int opt;
        int count_args = 1;
        opterr = 0;
        while ((opt = getopt(argc, argv, "vs:E:b:t:")) != -1) {
                count_args ++;
                switch(opt) {
                        case 'v': *vflag = true;
                                  break;
                        case 's': *s = atoi(optarg);
                                  break;
                        case 'E': *E = atoi(optarg);
                                  break;
                        case 'b': *b = atoi(optarg);
                                  break;
                        case 't': strcpy(tracefile, optarg);  // stackoverflow
                                  break;
                        default : usage();
                                  break;
                }
        }
        if (count_args < 4) {
                usage();
                exit(-1);
        }
}

unsigned long power(unsigned long base, unsigned long exp)
{
        int accum = 1;
        for (int i = 0; i < exp; i++) {
                accum *= base;
        }
        //printf("accum is: %d\n", accum);
        return accum;
}

void hexToDec(char* buf, int len, unsigned long* address)
{
        //Log(buf);
        unsigned long s = 0;
        for (int i = len - 1; i > 0; i--) {
                //printf("i is: %d\n", i);
                int x;
                switch (buf[i]) {
                        case 'a':
                        case 'A': 
                                x = 10;
                                break;
                        case 'b':
                        case 'B': 
                                x = 11;
                                break;
                        case 'c':
                        case 'C': 
                                x = 12;
                                break;
                        case 'd':
                        case 'D': 
                                x = 13;
                                break;
                        case 'e':
                        case 'E': 
                                x = 14;
                                break;
                        case 'f':
                        case 'F': 
                                x = 15;
                                break;
                        default:
                                x = buf[i] - '0';
                                break;
                }

                //printf("hextodec i is: %d\n", i);
                s += x * power(16, i);
        }
        //printf("hextodec s is: %ld\n", s);
        *address = s;
}

void parseLine(char *line_buf, char* op, unsigned long* address, int* size)
{
        char *p = line_buf;
        if (*p == ' ') {
                p++;
        }
        *op = *p++;

        char buf[16];
        int i = 0;
        for (; *++p != ','; i++) {
                buf[i] = *p;
        }
        buf[i] = '\0';
        hexToDec(buf, i, address);

        // ignore size
}
