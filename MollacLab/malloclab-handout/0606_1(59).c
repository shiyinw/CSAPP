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



/* single word (4) or double word (8) alignment */
#define DSIZE 8
#define WSIZE 4
#define CHUNKSIZE (1<<12)

#define PACK(size, alloc) ((size)|(alloc))
#define MAX(x, y) ((x)>(y)? (x):(y))

#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val))

// the size of the block
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)



#define GET_RALLOC(p) (GET(p) & 0x2)
#define GET_TAG(p)   (GET(p) & 0x2)
#define SET_RATAG(p)   (GET(p) |= 0x2)
#define REMOVE_RATAG(p) (GET(p) &= ~0x2)

// address of the header and footer
#define HDRP(p) ((char*)p - WSIZE)
#define FTRP(p) ((char*)p+GET_SIZE(HDRP(p))-DSIZE)

#define SET_PTR(p, ptr) (*(unsigned int *)(p) = (unsigned int)(ptr))

// address of the previous and next blocks
#define NEXT_BLKP(p) ((char*)(p) + GET_SIZE((char*)(p) - WSIZE))
#define PREV_BLKP(p) ((char*)(p) - GET_SIZE((char*)(p) - DSIZE))

// save the next space and previous space location in the released block
#define S_SPACE(p) ((char *)(p))
#define N_SPACE(p) ((char *)(p) + WSIZE)
// address of free block's predecessor and successor on the segregated list 
#define PRED(p) (*(char **)(p))
#define SUCC(p) (*(char **)(S_SPACE(p)))
//store the links in empty block



char *heap_listp;
static void *coalesce(void *bp){
  size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
  size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
  size_t size = GET_SIZE(HDRP(bp));
  if(prev_alloc && next_alloc)
    return bp;
  else if (prev_alloc && !next_alloc){
    //delete_node(bp);
    //delete_node(NEXT_BLKP(bp));
    size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
  }
  else if(!prev_alloc && next_alloc){
    //delete_node(bp);
    //delete_node(PREV_BLKP(bp));
    size += GET_SIZE(HDRP(PREV_BLKP(bp)));
    PUT(FTRP(bp), PACK(size, 0));
    PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
    bp = PREV_BLKP(bp);
  }
  else{
    //delete_node(bp);
    //delete_node(PREV_BLKP(bp));
    //delete_node(NEXT_BLKP(bp));
    size += GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(FTRP(NEXT_BLKP(bp)));
    PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
    PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
    bp = PREV_BLKP(bp);
  }
  return bp;
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
  //insert_node(bp,size);
  return coalesce(bp);
}

int mm_init(void)
{
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



static void *find_fit(size_t asize){
  void *bp;
  for(bp=heap_listp; GET_SIZE(HDRP(bp))>0; bp=NEXT_BLKP(bp)){
    // search from the heap until find a vaild space to store
    if(!GET_ALLOC(HDRP(bp)) && (asize <= GET_SIZE(HDRP(bp))))
      return bp;
  }
  return NULL;
}

static void place(void *bp, size_t asize){
  size_t csize = GET_SIZE(HDRP(bp));
  //delete_node(bp);
  if((csize - asize) >= (2*DSIZE)){ 
    //split this space into two part and use the first part
    PUT(HDRP(bp), PACK(asize, 1));
    PUT(FTRP(bp), PACK(asize, 1));
    bp = NEXT_BLKP(bp);
    PUT(HDRP(bp), PACK(csize-asize, 0));
    PUT(FTRP(bp), PACK(csize-asize, 0));
    //then save the second part to space list
    //insert_node(bp,csize-asize);
  }
  else{
    PUT(HDRP(bp), PACK(csize, 1));
    PUT(FTRP(bp), PACK(csize, 1));
  }
}
/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size){
  size_t asize;
  size_t extendsize;
  char *bp;
  if(size==0)
    return NULL;
  if(size<=DSIZE)
    asize = 2*DSIZE;
  else
    asize = DSIZE*((size + (DSIZE) + (DSIZE-1))/DSIZE);
  if((bp=find_fit(asize))!=NULL){
    place(bp, asize);
    return bp;
  }
  extendsize = MAX(asize, CHUNKSIZE);
  if((bp=extend_heap(extendsize/WSIZE))==NULL)
    return NULL;
  place(bp, asize);
  return bp;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr){
  size_t size = GET_SIZE(HDRP(ptr));
  PUT(HDRP(ptr), PACK(size, 0));
  PUT(FTRP(ptr), PACK(size, 0));
  //insert_node(ptr, size);
  coalesce(ptr);
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */


/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGNMENT 8
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)
#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

void *mm_realloc(void *ptr, size_t size)
{
    void *oldptr = ptr;
    void *newptr;
    size_t copySize;
    
    newptr = mm_malloc(size);
    if (newptr == NULL)
      return NULL;
    copySize = GET_SIZE(ptr) - WSIZE;
    if (size < copySize)
      copySize = size;
    memcpy(newptr, oldptr, copySize);
    mm_free(oldptr);
    return newptr;
}











