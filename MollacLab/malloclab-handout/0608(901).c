/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
  /* Team name */
  "flyingturtle",
  /* First member's full name */
  "Shiyin Wang",
  /* First member's email address */
  "wangshiy16@mails.tsinghua.edu.cn",
  /* Second member's full name (leave blank if none) */
  "",
  /* Second member's email address (leave blank if none) */
  ""
};



// constant
#define DSIZE 8
#define WSIZE 4
#define HASHSIZE    20 
#define CHUNKSIZE (1<<12)

#define MAX(x, y) ((x)>(y)? (x):(y))
#define MIN(x, y) ((x)<(y)? (x):(y)) 

#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val))

// basic structure of a block
#define HDRP(p) ((char*)p - WSIZE)
#define FTRP(p) ((char*)p+GET_SIZE(HDRP(p))-DSIZE)
#define GET_SIZE(p) (GET(p) & ~0x7)
#define PACK(size, alloc) ((size)|(alloc))
#define NEXT_BLKP(p) ((char*)(p) + GET_SIZE((char*)(p) - WSIZE))
#define PREV_BLKP(p) ((char*)(p) - GET_SIZE((char*)(p) - DSIZE))


// indicators
#define GET_ALLOC(p) (GET(p) & 0x1)
#define GET_TAG(p)   (GET(p) & 0x2)
#define SET_RATAG(p)   (GET(p) |= 0x2)
#define REMOVE_RATAG(p) (GET(p) &= ~0x2)

// linked lists
#define PRED_PTR(ptr) ((char *)(ptr))
#define SUCC_PTR(ptr) ((char *)(ptr) + WSIZE)
#define PRED(ptr) (*(char **)(ptr))
#define SUCC(ptr) (*(char **)(SUCC_PTR(ptr)))
#define SET_PTR(p, ptr) (*(unsigned int *)(p) = (unsigned int)(ptr))

// global variables
void *spaces[HASHSIZE]; 
int i;
char *heap_listp;


static void delete_node(void *ptr);
static void insert_node(void *ptr, size_t size);
static void *coalesce(void *ptr);
static void *extend_heap(size_t words);


static void *coalesce(void *ptr){
  size_t prev_alloc = GET_ALLOC(HDRP(PREV_BLKP(ptr))) | GET_TAG(HDRP(PREV_BLKP(ptr)));
  size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(ptr)));
  size_t size = GET_SIZE(HDRP(ptr));

  if (prev_alloc && next_alloc) {
    return ptr;
  }
  else if (prev_alloc && !next_alloc) {
    delete_node(ptr);
    delete_node(NEXT_BLKP(ptr));
    size += GET_SIZE(HDRP(NEXT_BLKP(ptr)));
    PUT(HDRP(ptr), PACK(size, 0));
    PUT(FTRP(ptr), PACK(size, 0));
  } 
  else if (!prev_alloc && next_alloc) {
    delete_node(ptr);
    delete_node(PREV_BLKP(ptr));
    size += GET_SIZE(HDRP(PREV_BLKP(ptr)));
    PUT(FTRP(ptr), PACK(size, 0));
    PUT(HDRP(PREV_BLKP(ptr)), PACK(size, 0));
    ptr = PREV_BLKP(ptr);
  } 
  else {
    delete_node(ptr);
    delete_node(PREV_BLKP(ptr));
    delete_node(NEXT_BLKP(ptr));
    size += GET_SIZE(HDRP(PREV_BLKP(ptr))) + GET_SIZE(HDRP(NEXT_BLKP(ptr)));
    PUT(HDRP(PREV_BLKP(ptr)), PACK(size, 0));
    PUT(FTRP(NEXT_BLKP(ptr)), PACK(size, 0));
    ptr = PREV_BLKP(ptr);
  }
  insert_node(ptr, size);
  return ptr;
}


static void *extend_heap(size_t words){
  char *bp;
  size_t size;
  size = (words%2)? (words+1)*WSIZE : words*WSIZE;
  if((long) (bp=mem_sbrk(size))==-1)
    return NULL;
  PUT(HDRP(bp), PACK(size, 0));
  PUT(FTRP(bp), PACK(size, 0));
  PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));
  insert_node(bp, size);
  return coalesce(bp);
}

int mm_init(void)
{        
  for (i = 0; i < HASHSIZE; i++) {
    spaces[i] = NULL;
  }
  if((heap_listp=mem_sbrk(4*WSIZE)) == (void *) -1)
    return -1;
  PUT(heap_listp, 0);
  PUT(heap_listp+(1*WSIZE), PACK(DSIZE, 1));
  PUT(heap_listp+(2*WSIZE), PACK(DSIZE, 1));
  PUT(heap_listp+(3*WSIZE), PACK(0, 1));
  heap_listp += 2*WSIZE;
  if(extend_heap(CHUNKSIZE/WSIZE)==NULL)
    return -1;
  return 0;
}

static int HashVal(int size){
  int val = 0;
  while(val<HASHSIZE-1 && size>1){
    size >>=1;
    val++;
  }
  return val;
}

static void delete_node(void *ptr) {
  int hashkey = HashVal(GET_SIZE(HDRP(ptr)));
  if (PRED(ptr) != NULL) {
    if (SUCC(ptr) != NULL) {
      SET_PTR(SUCC_PTR(PRED(ptr)), SUCC(ptr));
      SET_PTR(PRED_PTR(SUCC(ptr)), PRED(ptr));
    } 
    else {
      SET_PTR(SUCC_PTR(PRED(ptr)), NULL);
      spaces[hashkey] = PRED(ptr);
    }
  } 
  else {
    if (SUCC(ptr) != NULL) {
      SET_PTR(PRED_PTR(SUCC(ptr)), NULL);
    } else {
      spaces[hashkey] = NULL;
    }
  }
  return;
}

static void insert_node(void *ptr, size_t size) { 
  int hashkey = HashVal(GET_SIZE(HDRP(ptr)));
  void *iter = spaces[HashVal(size)];
  void *pre = NULL;
  while ((iter != NULL) && (size > GET_SIZE(HDRP(iter)))) {
    pre = iter;
    iter = PRED(iter);
  }
  if (iter != NULL) {
    if (pre != NULL) {
      SET_PTR(PRED_PTR(ptr), iter);
      SET_PTR(SUCC_PTR(iter), ptr);
      SET_PTR(SUCC_PTR(ptr), pre);
      SET_PTR(PRED_PTR(pre), ptr);
    }
    else {
      SET_PTR(PRED_PTR(ptr), iter);
      SET_PTR(SUCC_PTR(iter), ptr);
      SET_PTR(SUCC_PTR(ptr), NULL);
      spaces[hashkey] = ptr;
    }
  } 
  else {
    if (pre != NULL) {
      SET_PTR(PRED_PTR(ptr), NULL);
      SET_PTR(SUCC_PTR(ptr), pre);
      SET_PTR(PRED_PTR(pre), ptr);
    }
    else {
      SET_PTR(PRED_PTR(ptr), NULL);
      SET_PTR(SUCC_PTR(ptr), NULL);
      spaces[hashkey] = ptr;
    }
  }
  return;
}


static void *find_fit(size_t asize){
  void *iter;
  
  for(i=HashVal(asize); i < HASHSIZE; i++) {
    iter = spaces[i];
    while ((iter != NULL) && ((asize > GET_SIZE(HDRP(iter))) || (GET_TAG(HDRP(iter)))))
      iter = PRED(iter);
    if (iter != NULL)
      return iter;
  }
  return NULL;
}

// inspired by https://github.com/mightydeveloper/Malloc-Lab/blob/master/mm.c
static void *place(void *ptr, size_t asize){
  size_t ptr_size = GET_SIZE(HDRP(ptr));
  size_t remainder = ptr_size - asize;
  
  delete_node(ptr);
  if (remainder <= DSIZE * 2) {
    PUT(HDRP(ptr), PACK(ptr_size, 1)); 
    PUT(FTRP(ptr), PACK(ptr_size, 1)); 
  }
  else if (asize >= 100) {
    PUT(HDRP(ptr), PACK(remainder, 0));
    PUT(FTRP(ptr), PACK(remainder, 0));
    PUT(HDRP(NEXT_BLKP(ptr)), PACK(asize, 1));
    PUT(FTRP(NEXT_BLKP(ptr)), PACK(asize, 1));
    insert_node(ptr, remainder);
    return NEXT_BLKP(ptr);
  }
  else {
    PUT(HDRP(ptr), PACK(asize, 1)); 
    PUT(FTRP(ptr), PACK(asize, 1)); 
    PUT(HDRP(NEXT_BLKP(ptr)), PACK(remainder, 0)); 
    PUT(FTRP(NEXT_BLKP(ptr)), PACK(remainder, 0)); 
    insert_node(NEXT_BLKP(ptr), remainder);
  }
  return ptr;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size){
  size_t asize;
  size_t extendsize;
  void *ptr = NULL;
  if(size==0)
    return NULL;
  if(size<=DSIZE)
    asize = 2*DSIZE;
  else
    asize = DSIZE*((size + (DSIZE) + (DSIZE-1))/DSIZE);
  ptr = find_fit(asize);
  if (ptr == NULL) {
    extendsize = MAX(asize, CHUNKSIZE);
    if ((ptr = extend_heap(extendsize)) == NULL)
      return NULL;
  }
  ptr = place(ptr, asize);
  return ptr;
}




/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr){
  size_t size = GET_SIZE(HDRP(ptr));
  REMOVE_RATAG(HDRP(NEXT_BLKP(ptr)));
  PUT(HDRP(ptr), PACK(size, 0));
  PUT(FTRP(ptr), PACK(size, 0));
  insert_node(ptr, size);
  coalesce(ptr);
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t new_size){
    size_t old_size = GET_SIZE(ptr);
    if(old_size>new_size)
      return ptr;
    
    size_t copySize = old_size - WSIZE;
    //new_size = ((new_size>>8) + 1)<<8;
    void *oldptr = ptr;
    void *newptr = mm_malloc(new_size);
    if (newptr == NULL)
      return NULL;
      
    memcpy(newptr, oldptr, MIN(new_size, copySize));
    
    if(GET_TAG(HDRP(newptr))){
      SET_PTR(PRED_PTR(SUCC(newptr)), PRED_PTR(newptr));
      SET_PTR(SUCC_PTR(PRED(newptr)), SUCC_PTR(newptr));
    }
    SET_RATAG(HDRP(newptr));
    mm_free(oldptr);
    return newptr;
}










