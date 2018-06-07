#include "cachelab.h"
#include "getopt.h"
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>


int global_time;

struct CacheParameter {
    int s, b, E, S, B; 
    int hits, misses, evicts;
};

struct CacheLine{
    int valid;
    unsigned long long int  tag;
    char *data;
    int last_time;
};

struct CacheSet{
    struct CacheLine *lines;
}; 

struct Cache{
    struct CacheSet *sets;
};

struct Cache initCache(struct CacheParameter par){
    struct Cache cache;
    cache.sets = (struct CacheSet*) malloc(sizeof(struct CacheSet) * par.S);
    for(int i=0; i<par.S; i++){
        cache.sets[i].lines = (struct CacheLine*) malloc(sizeof(struct CacheLine) * par.E);
        for(int j=0; j< par.E; j++){
            cache.sets[i].lines[j].valid = 0;
            cache.sets[i].lines[j].tag = 0;
            cache.sets[i].lines[j].last_time = 0;
        }
    }
    return cache;
}


int nextCacheLine(struct CacheSet set, struct CacheParameter param) {
    for (int i=0; i<param.E; i++) {
        if (set.lines[i].valid == 0)
            return i;
    }
    return -1;
}


int evictLine(struct CacheSet set, struct CacheParameter param) { 
    //sequencially find the oldest cache line and evict it, time O(E) linear
    int min, min_id;
    min = set.lines[0].last_time;
    min_id = 0;
    for (int i=1; i<param.E; i++) {
        if (min > set.lines[i].last_time) {
            min_id = i;	
            min = set.lines[i].last_time;
        }
    }
    return min_id;
}

struct CacheParameter sim(struct Cache cache, struct CacheParameter param, unsigned long long int addr) {
    int empty = 0, miss = 1;

    unsigned long long int inputTag = addr >> (param.s + param.b);
    unsigned long long int indexOfSet = (addr >> param.b) & (param.S -1);
        
    struct CacheSet cacheset = cache.sets[indexOfSet];

    for (int i=0; i<param.E; i++) 	{          
        if (cacheset.lines[i].valid) {
            if (cacheset.lines[i].tag == inputTag) {
                cacheset.lines[i].last_time = global_time ++;
                param.hits ++;
                miss = 0;
            }
       } 
        else if (!(cacheset.lines[i].valid))
            empty = 1;
    }	

    if (miss)
        param.misses++;
    else
        return param;

    int min_id = evictLine(cacheset, param);
    if (!empty){
        param.evicts++;
        struct CacheLine *l = &cacheset.lines[min_id];
        l->tag = inputTag;
        l->valid = 1;
        l->last_time = global_time++;
    }
    else{
        struct CacheLine *l = &cacheset.lines[nextCacheLine(cacheset, param)];
        l->tag = inputTag;
        l->valid = 1;
        l->last_time = global_time++;
    }						
    return param;
}



void deleteCache(struct Cache cache, struct CacheParameter par){
    for(int i=0; i< par.S; i++){
        if(cache.sets[i].lines!=NULL)
            free(cache.sets[i].lines);
    }
    if(cache.sets!=NULL)
        free(cache.sets);
}



int main(int argc, char **argv)
{
    global_time = 0;
    struct CacheParameter par;
    memset(&par, 0, sizeof(par));	

    FILE *trace;
    char *file;
    unsigned long long int addr;
    int size;
    char c;
    while( (c=getopt(argc,argv,"s:E:b:t:vh")) != -1){
        switch(c){
        case 's':
            par.s = atoi(optarg);
            par.S = 1<<atoi(optarg);
            break;
        case 'E':
            par.E = atoi(optarg);
            break;
        case 'b':
            par.b = atoi(optarg);
            par.B = 1<<atoi(optarg);
            break;
        case 't':
            file = optarg;
            break;
        case 'h':
            exit(0);
        default:
            exit(1);
        }
    }
    par.hits = 0;
    par.misses = 0;
    par.evicts = 0;
    
    struct Cache cache = initCache(par);
 	
    trace  = fopen(file, "r");
    char op;
    if (trace != NULL) {
        while (fscanf(trace, " %c %llx,%d", &op, &addr, &size) == 3) {
            switch(op) {
                case 'I':
                    break;
                case 'L':
                    par = sim(cache, par, addr);
                    break;
                case 'S':
                    par = sim(cache, par, addr);
                    break;
                case 'M':
                    par = sim(cache, par, addr);
                    par = sim(cache, par, addr);	
                    break;
                default:
                    break;
            }
        }
    }
    
    printSummary(par.hits, par.misses, par.evicts);
    deleteCache(cache, par);
    fclose(trace);

    return 0;
}
