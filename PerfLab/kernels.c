/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/* 
 * Please fill in the following team struct 
 */
team_t team = {
    "flyingturtle",              /* Team name */

    "Shiyin Wang",     /* First member full name */
    "wangshiy16@mails.tsinghua.edu.cn",  /* First member email address */

    "",                   /* Second member full name (leave blank if none) */
    ""                    /* Second member email addr (leave blank if none) */
};

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/

/* 
 * naive_rotate - The naive baseline version of rotate  
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
    for (j = 0; j < dim; j++)
        dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
}

/* 
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */
char pointer_descr[] = "rotate: store pointers to retrive data fast";
void rotate_pointer(int dim, pixel *src, pixel *dst) 
{
    pixel *d, *s;
    int i, j;
    for(i=0; i<dim; i=i+32){
        for(j=0; j<dim; j++){
            d = dst + RIDX(dim-1-j, i, dim);
            s = src + RIDX(i, j, dim);
            *d = *s; d++; s = s + dim;
            *d = *s; d++; s = s + dim;
            *d = *s; d++; s = s + dim;
            *d = *s; d++; s = s + dim;

            *d = *s; d++; s = s + dim;
            *d = *s; d++; s = s + dim;
            *d = *s; d++; s = s + dim;
            *d = *s; d++; s = s + dim;

            *d = *s; d++; s = s + dim;
            *d = *s; d++; s = s + dim;
            *d = *s; d++; s = s + dim;
            *d = *s; d++; s = s + dim;

            *d = *s; d++; s = s + dim;
            *d = *s; d++; s = s + dim;
            *d = *s; d++; s = s + dim;
            *d = *s; d++; s = s + dim;
            
            *d = *s; d++; s = s + dim;
            *d = *s; d++; s = s + dim;
            *d = *s; d++; s = s + dim;
            *d = *s; d++; s = s + dim;

            *d = *s; d++; s = s + dim;
            *d = *s; d++; s = s + dim;
            *d = *s; d++; s = s + dim;
            *d = *s; d++; s = s + dim;

            *d = *s; d++; s = s + dim;
            *d = *s; d++; s = s + dim;
            *d = *s; d++; s = s + dim;
            *d = *s; d++; s = s + dim;

            *d = *s; d++; s = s + dim;
            *d = *s; d++; s = s + dim;
            *d = *s; d++; s = s + dim;
            *d = *s; d++; s = s + dim;
        }


    }
            
}

char expand_out_descr[] = "rotate: expand to 32 per loop out";
void rotate_expand_out(int dim, pixel *src, pixel *dst){
    int i, j;
    for(i=0; i<dim; i=i+32){
        for(j=0; j<dim; j++){
            dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];									
            dst[RIDX(dim-1-j, i+1, dim)] = src[RIDX(i+1, j, dim)];
            dst[RIDX(dim-1-j, i+2, dim)] = src[RIDX(i+2, j, dim)];
            dst[RIDX(dim-1-j, i+3, dim)] = src[RIDX(i+3, j, dim)];
            dst[RIDX(dim-1-j, i+4, dim)] = src[RIDX(i+4, j,dim)];		
            dst[RIDX(dim-1-j, i+5, dim)] = src[RIDX(i+5, j, dim)];
            dst[RIDX(dim-1-j, i+6, dim)] = src[RIDX(i+6, j, dim)];	
            dst[RIDX(dim-1-j, i+7, dim)] = src[RIDX(i+7, j, dim)];
            dst[RIDX(dim-1-j, i+8, dim)] = src[RIDX(i+8, j, dim)];
            dst[RIDX(dim-1-j, i+9, dim)] = src[RIDX(i+9, j, dim)];
            dst[RIDX(dim-1-j, i+10, dim)] = src[RIDX(i+10, j,dim)];		
            dst[RIDX(dim-1-j, i+11, dim)] = src[RIDX(i+11, j, dim)];
            dst[RIDX(dim-1-j, i+12, dim)] = src[RIDX(i+12, j, dim)];
            dst[RIDX(dim-1-j, i+13, dim)] = src[RIDX(i+13, j, dim)];
            dst[RIDX(dim-1-j, i+14, dim)] = src[RIDX(i+14, j, dim)];
            dst[RIDX(dim-1-j, i+15, dim)] = src[RIDX(i+15, j, dim)];
            dst[RIDX(dim-1-j, i+16, dim)] = src[RIDX(i+16, j,dim)];		
            dst[RIDX(dim-1-j, i+17, dim)] = src[RIDX(i+17, j, dim)];
            dst[RIDX(dim-1-j, i+18, dim)] = src[RIDX(i+18, j, dim)];		
            dst[RIDX(dim-1-j, i+19, dim)] = src[RIDX(i+19, j, dim)];
            dst[RIDX(dim-1-j, i+20, dim)] = src[RIDX(i+20, j, dim)];	
            dst[RIDX(dim-1-j, i+21, dim)] = src[RIDX(i+21, j, dim)];
            dst[RIDX(dim-1-j, i+22, dim)] = src[RIDX(i+22, j, dim)];
            dst[RIDX(dim-1-j, i+23, dim)] = src[RIDX(i+23, j, dim)];
            dst[RIDX(dim-1-j, i+24, dim)] = src[RIDX(i+24, j,dim)];		
            dst[RIDX(dim-1-j, i+25, dim)] = src[RIDX(i+25, j, dim)];
            dst[RIDX(dim-1-j, i+26, dim)] = src[RIDX(i+26, j, dim)];
            dst[RIDX(dim-1-j, i+27, dim)] = src[RIDX(i+27, j, dim)];
            dst[RIDX(dim-1-j, i+28, dim)] = src[RIDX(i+28, j, dim)];
            dst[RIDX(dim-1-j, i+29, dim)] = src[RIDX(i+29, j, dim)];		
            dst[RIDX(dim-1-j, i+30, dim)] = src[RIDX(i+30, j, dim)];
            dst[RIDX(dim-1-j, i+31, dim)] = src[RIDX(i+31, j, dim)];
        }		
                                                        
    }
            
}


char expand_in_descr[] = "rotate: expand to 32 per loop in";
void rotate_expand_in(int dim, pixel *src, pixel *dst){
    int i, j, x, y;
    for(j=0; j<dim; j++){
        for(i=0; i<dim; i=i+32){
            x = RIDX(dim-1-j, i, dim); y = RIDX(i, j, dim);
            dst[x] = src[y];
            dst[x+1] = src[y+dim];
            dst[x+2] = src[y+dim*2];	
            dst[x+3] = src[y+dim*3];	
            dst[x+4] = src[y+dim*4];	
            dst[x+5] = src[y+dim*5];	
            dst[x+6] = src[y+dim*6];
            dst[x+7] = src[y+dim*7];
            dst[x+8] = src[y+dim*8];	
            dst[x+9] = src[y+dim*9];	
            dst[x+10] = src[y+dim*10];	
            dst[x+11] = src[y+dim*11];	
            dst[x+12] = src[y+dim*12];
            dst[x+13] = src[y+dim*13];
            dst[x+14] = src[y+dim*14];	
            dst[x+15] = src[y+dim*15];	
            dst[x+16] = src[y+dim*16];	
            dst[x+17] = src[y+dim*17];	
            dst[x+18] = src[y+dim*18];
            dst[x+19] = src[y+dim*19];
            dst[x+20] = src[y+dim*20];	
            dst[x+21] = src[y+dim*21];	
            dst[x+22] = src[y+dim*22];	
            dst[x+23] = src[y+dim*23];	
            dst[x+24] = src[y+dim*24];	
            dst[x+25] = src[y+dim*25];	
            dst[x+26] = src[y+dim*26];	
            dst[x+27] = src[y+dim*27];	
            dst[x+28] = src[y+dim*28];
            dst[x+29] = src[y+dim*29];
            dst[x+30] = src[y+dim*30];	
            dst[x+31] = src[y+dim*31];										
            
        }		
                                                        
    }
            
}

char expand_out_descr_i[] = "rotate: expand to 32 per loop out with stored RIDX";
void rotate_expand_out_i(int dim, pixel *src, pixel *dst){
    int i, j, x, y;
    for(i=0; i<dim; i=i+32){
        for(j=0; j<dim; j++){
            x = RIDX(dim-1-j, i, dim); y = RIDX(i, j, dim);
            dst[x] = src[y];
            dst[x+1] = src[y+dim];
            dst[x+2] = src[y+dim*2];	
            dst[x+3] = src[y+dim*3];	
            dst[x+4] = src[y+dim*4];	
            dst[x+5] = src[y+dim*5];	
            dst[x+6] = src[y+dim*6];
            dst[x+7] = src[y+dim*7];
            dst[x+8] = src[y+dim*8];	
            dst[x+9] = src[y+dim*9];	
            dst[x+10] = src[y+dim*10];	
            dst[x+11] = src[y+dim*11];	
            dst[x+12] = src[y+dim*12];
            dst[x+13] = src[y+dim*13];
            dst[x+14] = src[y+dim*14];	
            dst[x+15] = src[y+dim*15];	
            dst[x+16] = src[y+dim*16];	
            dst[x+17] = src[y+dim*17];	
            dst[x+18] = src[y+dim*18];
            dst[x+19] = src[y+dim*19];
            dst[x+20] = src[y+dim*20];	
            dst[x+21] = src[y+dim*21];	
            dst[x+22] = src[y+dim*22];	
            dst[x+23] = src[y+dim*23];	
            dst[x+24] = src[y+dim*24];	
            dst[x+25] = src[y+dim*25];	
            dst[x+26] = src[y+dim*26];	
            dst[x+27] = src[y+dim*27];	
            dst[x+28] = src[y+dim*28];
            dst[x+29] = src[y+dim*29];
            dst[x+30] = src[y+dim*30];	
            dst[x+31] = src[y+dim*31];										
            
        }		
                                                        
    }
            
}

char expand_16_descr[] = "rotate: expand to 16 per loop out";
void rotate_expand_16(int dim, pixel *src, pixel *dst){
    int i, j;
    for(j=0; j<dim; j++){
        for(i=0; i<dim; i=i+16){
            dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];									
            dst[RIDX(dim-1-j, i+1, dim)] = src[RIDX(i+1, j, dim)];
            dst[RIDX(dim-1-j, i+2, dim)] = src[RIDX(i+2, j, dim)];
            dst[RIDX(dim-1-j, i+3, dim)] = src[RIDX(i+3, j, dim)];
            dst[RIDX(dim-1-j, i+4, dim)] = src[RIDX(i+4, j,dim)];		
            dst[RIDX(dim-1-j, i+5, dim)] = src[RIDX(i+5, j, dim)];
            dst[RIDX(dim-1-j, i+6, dim)] = src[RIDX(i+6, j, dim)];	
            dst[RIDX(dim-1-j, i+7, dim)] = src[RIDX(i+7, j, dim)];
            dst[RIDX(dim-1-j, i+8, dim)] = src[RIDX(i+8, j, dim)];
            dst[RIDX(dim-1-j, i+9, dim)] = src[RIDX(i+9, j, dim)];
            dst[RIDX(dim-1-j, i+10, dim)] = src[RIDX(i+10, j,dim)];		
            dst[RIDX(dim-1-j, i+11, dim)] = src[RIDX(i+11, j, dim)];
            dst[RIDX(dim-1-j, i+12, dim)] = src[RIDX(i+12, j, dim)];
            dst[RIDX(dim-1-j, i+13, dim)] = src[RIDX(i+13, j, dim)];
            dst[RIDX(dim-1-j, i+14, dim)] = src[RIDX(i+14, j, dim)];
            dst[RIDX(dim-1-j, i+15, dim)] = src[RIDX(i+15, j, dim)];
        }                                                      
    }     
}

char block_4_descr[] = "4*4 block rotate";
void rotate_block_4(int dim, pixel *src, pixel *dst){
    int i, j, x, y;
    for(j=0; j<dim; j=j+4){
        for(i=0; i<dim; i=i+4){
            x = RIDX(dim-1-j, i, dim); y = RIDX(i, j, dim);
            dst[x] = src[y];
            dst[x+1] = src[y+dim];
            dst[x+2] = src[y+dim+dim];
            dst[x+3] = src[y+dim+dim+dim];
            dst[x-dim] = src[y+1];
            dst[x-dim+1] = src[y+1+dim];
            dst[x-dim+2] = src[y+1+dim+dim];
            dst[x-dim+3] = src[y+1+dim+dim+dim];
            dst[x-dim-dim] = src[y+2];
            dst[x-dim-dim+1] = src[y+2+dim];
            dst[x-dim-dim+2] = src[y+2+dim+dim];
            dst[x-dim-dim+3] = src[y+2+dim+dim+dim]; 
            dst[x-dim-dim-dim] = src[y+3];
            dst[x-dim-dim-dim+1] = src[y+3+dim];
            dst[x-dim-dim-dim+2] = src[y+3+dim+dim];
            dst[x-dim-dim-dim+3] = src[y+3+dim+dim+dim];            
        }
    }
}

char block_8_descr[] = "8*8 block rotate";
void rotate_block_8(int dim, pixel *src, pixel *dst){
    int i, j, x, y;
    for(j=0; j<dim; j=j+8){
        for(i=0; i<dim; i=i+8){
            x = RIDX(dim-1-j, i, dim); y = RIDX(i, j, dim);
            dst[x] = src[y];
            dst[x+1] = src[y+dim];
            dst[x+2] = src[y+dim*2];
            dst[x+3] = src[y+dim*3];
            dst[x+4] = src[y+dim*4];
            dst[x+5] = src[y+dim*5];
            dst[x+6] = src[y+dim*6];
            dst[x+7] = src[y+dim*7];
            
            dst[x-dim] = src[y+1];
            dst[x+1-dim] = src[y+dim+1];
            dst[x+2-dim] = src[y+dim*2+1];
            dst[x+3-dim] = src[y+dim*3+1];
            dst[x+4-dim] = src[y+dim*4+1];
            dst[x+5-dim] = src[y+dim*5+1];
            dst[x+6-dim] = src[y+dim*6+1];
            dst[x+7-dim] = src[y+dim*7+1];
            
            dst[x-dim*2] = src[y+2];
            dst[x+1-dim*2] = src[y+dim+2];
            dst[x+2-dim*2] = src[y+dim*2+2];
            dst[x+3-dim*2] = src[y+dim*3+2];
            dst[x+4-dim*2] = src[y+dim*4+2];
            dst[x+5-dim*2] = src[y+dim*5+2];
            dst[x+6-dim*2] = src[y+dim*6+2];
            dst[x+7-dim*2] = src[y+dim*7+2];
            
            dst[x-dim*3] = src[y+3];
            dst[x+1-dim*3] = src[y+dim+3];
            dst[x+2-dim*3] = src[y+dim*2+3];
            dst[x+3-dim*3] = src[y+dim*3+3];
            dst[x+4-dim*3] = src[y+dim*4+3];
            dst[x+5-dim*3] = src[y+dim*5+3];
            dst[x+6-dim*3] = src[y+dim*6+3];
            dst[x+7-dim*3] = src[y+dim*7+3];
            
            dst[x-dim*4] = src[y+4];
            dst[x+1-dim*4] = src[y+dim+4];
            dst[x+2-dim*4] = src[y+dim*2+4];
            dst[x+3-dim*4] = src[y+dim*3+4];
            dst[x+4-dim*4] = src[y+dim*4+4];
            dst[x+5-dim*4] = src[y+dim*5+4];
            dst[x+6-dim*4] = src[y+dim*6+4];
            dst[x+7-dim*4] = src[y+dim*7+4];
            
            dst[x-dim*5] = src[y+5];
            dst[x+1-dim*5] = src[y+dim+5];
            dst[x+2-dim*5] = src[y+dim*2+5];
            dst[x+3-dim*5] = src[y+dim*3+5];
            dst[x+4-dim*5] = src[y+dim*4+5];
            dst[x+5-dim*5] = src[y+dim*5+5];
            dst[x+6-dim*5] = src[y+dim*6+5];
            dst[x+7-dim*5] = src[y+dim*7+5];
            
            dst[x-dim*6] = src[y+6];
            dst[x+1-dim*6] = src[y+dim+6];
            dst[x+2-dim*6] = src[y+dim*2+6];
            dst[x+3-dim*6] = src[y+dim*3+6];
            dst[x+4-dim*6] = src[y+dim*4+6];
            dst[x+5-dim*6] = src[y+dim*5+6];
            dst[x+6-dim*6] = src[y+dim*6+6];
            dst[x+7-dim*6] = src[y+dim*7+6];
            
            dst[x-dim*7] = src[y+7];
            dst[x+1-dim*7] = src[y+dim+7];
            dst[x+2-dim*7] = src[y+dim*2+7];
            dst[x+3-dim*7] = src[y+dim*3+7];
            dst[x+4-dim*7] = src[y+dim*4+7];
            dst[x+5-dim*7] = src[y+dim*5+7];
            dst[x+6-dim*7] = src[y+dim*6+7];
            dst[x+7-dim*7] = src[y+dim*7+7];
            

            
        }
    }
}

char curr_descr[] = "rotate: current working version";
void rotate(int dim, pixel *src, pixel *dst){
    rotate_expand_out(dim, src, dst);
}

/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_rotate_functions() 
{
    add_rotate_function(&naive_rotate, naive_rotate_descr);
    add_rotate_function(&rotate_pointer, pointer_descr); 
    add_rotate_function(&rotate_expand_out, expand_out_descr);
    add_rotate_function(&rotate_expand_in, expand_in_descr);
    add_rotate_function(&rotate_expand_out_i, expand_out_descr_i); 
    add_rotate_function(&rotate_expand_16, expand_16_descr); 
    add_rotate_function(&rotate, curr_descr);
    add_rotate_function(&rotate_block_4, block_4_descr);
    add_rotate_function(&rotate_block_8, block_8_descr);
    /* ... Register additional test functions here */
}


/***************
 * SMOOTH KERNEL
 **************/

/***************************************************************
 * Various typedefs and helper functions for the smooth function
 * You may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct {
    int red;
    int green;
    int blue;
    int num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/* 
 * initialize_pixel_sum - Initializes all fields of sum to 0 
 */
static void initialize_pixel_sum(pixel_sum *sum) 
{
    sum->red = sum->green = sum->blue = 0;
    sum->num = 0;
    return;
}

/* 
 * accumulate_sum - Accumulates field values of p in corresponding 
 * fields of sum 
 */
static void accumulate_sum(pixel_sum *sum, pixel p) 
{
    sum->red += (int) p.red;
    sum->green += (int) p.green;
    sum->blue += (int) p.blue;
    sum->num++;
    return;
}

/* 
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel 
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum) 
{
    current_pixel->red = (unsigned short) (sum.red/sum.num);
    current_pixel->green = (unsigned short) (sum.green/sum.num);
    current_pixel->blue = (unsigned short) (sum.blue/sum.num);
    return;
}

/* 
 * avg - Returns averaged pixel value at (i,j) 
 */
static pixel avg(int dim, int i, int j, pixel *src) 
{
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++) 
	for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++) 
	    accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}

/******************************************************
 * Your different versions of the smooth kernel go here
 ******************************************************/

/*
 * naive_smooth - The naive baseline version of smooth 
 */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
}


char descrete[] = "smooth: seperate the circumstances";
void smooth_descrete(int dim, pixel *src, pixel *dst){
    int a1, a2, b1, b2, c1, c2;
    a1 = 0; b1 = dim;
    dst[0].red = (src[0].red + src[1].red + src[b1].red + src[b1+1].red)>>2;
    dst[0].blue = (src[0].blue + src[1].blue + src[b1].blue + src[b1+1].blue)>>2;
    dst[0].green = (src[0].green + src[1].green + src[b1].green + src[b1+1].green)>>2;
    a2 = dim-2; b2 = dim*2-2;
    dst[a2+1].red = (src[a2].red + src[a2+1].red + src[b2].red + src[b2+1].red)>>2;
    dst[a2+1].blue = (src[a2].blue + src[a2+1].blue + src[b2].blue + src[b2+1].blue)>>2;
    dst[a2+1].green = (src[a2].green + src[a2+1].green + src[b2].green + src[b2+1].green)>>2;
    a1 = dim*(dim-2); b1 = dim*(dim-1);
    dst[b1].red = (src[a1].red + src[a1+1].red + src[b1].red + src[b1+1].red)>>2;
    dst[b1].blue = (src[a1].blue + src[a1+1].blue + src[b1].blue + src[b1+1].blue)>>2;
    dst[b1].green = (src[a1].green + src[a1+1].green + src[b1].green + src[b1+1].green)>>2;
    a2 = dim*(dim-1)-2; b2 = dim*dim-2;
    dst[b2+1].red = (src[a2].red + src[a2+1].red + src[b2].red + src[b2+1].red)>>2;
    dst[b2+1].blue = (src[a2].blue + src[a2+1].blue + src[b2].blue + src[b2+1].blue)>>2;
    dst[b2+1].green = (src[a2].green + src[a2+1].green + src[b2].green + src[b2+1].green)>>2;
    
    int i, j;
    for(i=1; i< dim-1; i++){
        a1 = RIDX(0, i-1, dim); b1 = RIDX(1, i-1, dim);
        dst[a1+1].red = (src[a1].red + src[a1+1].red + src[a1+2].red + src[b1].red + src[b1+1].red + src[b1+2].red)/6;
        dst[a1+1].green = (src[a1].green + src[a1+1].green + src[a1+2].green + src[b1].green + src[b1+1].green + src[b1+2].green)/6;
        dst[a1+1].blue = (src[a1].blue + src[a1+1].blue + src[a1+2].blue + src[b1].blue + src[b1+1].blue + src[b1+2].blue)/6;
    }
    
    for(i=1; i< dim-1; i++){
        a1 = RIDX(dim-2, i-1, dim); b1 = RIDX(dim-1, i-1, dim);
        dst[b1+1].red = (src[a1].red + src[a1+1].red + src[a1+2].red + src[b1].red + src[b1+1].red + src[b1+2].red)/6;
        dst[b1+1].green = (src[a1].green + src[a1+1].green + src[a1+2].green + src[b1].green + src[b1+1].green + src[b1+2].green)/6;
        dst[b1+1].blue = (src[a1].blue + src[a1+1].blue + src[a1+2].blue + src[b1].blue + src[b1+1].blue + src[b1+2].blue)/6;
    }
    
    for(i=1; i< dim-1; i++){
        a1 = RIDX(i-1, 0, dim); b1 = RIDX(i, 0, dim); c1 = RIDX(i+1, 0, dim);
        dst[b1].red = (src[a1].red + src[a1+1].red + src[c1].red + src[b1].red + src[b1+1].red + src[c1+1].red)/6;
        dst[b1].green = (src[a1].green + src[a1+1].green + src[c1].green + src[b1].green + src[b1+1].green + src[c1+1].green)/6;
        dst[b1].blue = (src[a1].blue + src[a1+1].blue + src[c1].blue + src[b1].blue + src[b1+1].blue + src[c1+1].blue)/6;
    }
    
    for(i=1; i< dim-1; i++){
        a1 = RIDX(i-1, dim-2, dim); b1 = RIDX(i, dim-2, dim); c1 = RIDX(i+1, dim-2, dim);
        dst[b1+1].red = (src[a1].red + src[a1+1].red + src[c1].red + src[b1].red + src[b1+1].red + src[c1+1].red)/6;
        dst[b1+1].green = (src[a1].green + src[a1+1].green + src[c1].green + src[b1].green + src[b1+1].green + src[c1+1].green)/6;
        dst[b1+1].blue = (src[a1].blue + src[a1+1].blue + src[c1].blue + src[b1].blue + src[b1+1].blue + src[c1+1].blue)/6;
    }

    
    for(i=1; i<dim-1; i++){
        for(j=1; j<dim-1; j++){
            a1 = RIDX(i-1, j-1, dim); b1 = RIDX(i, j-1, dim); c1 = RIDX(i+1, j-1, dim);
            dst[b1+1].red = (src[a1].red + src[a1+1].red + src[a1+2].red + src[b1].red + src[b1+1].red + src[b1+2].red + src[c1].red + src[c1+1].red + src[c1+2].red)/9;
            dst[b1+1].green = (src[a1].green + src[a1+1].green + src[a1+2].green + src[b1].green + src[b1+1].green + src[b1+2].green + src[c1].green + src[c1+1].green + src[c1+2].green)/9;
            dst[b1+1].blue = (src[a1].blue + src[a1+1].blue + src[a1+2].blue + src[b1].blue + src[b1+1].blue + src[b1+2].blue + src[c1].blue + src[c1+1].blue + src[c1+2].blue)/9;
        }
    }
}


char descrete_2[] = "smooth: seperate the circumstances and 2 points in each loop";
void smooth_descrete_2(int dim, pixel *src, pixel *dst){
    int a1, a2, b1, b2, c1, c2;
    a1 = 0; b1 = dim;
    dst[0].red = (src[0].red + src[1].red + src[b1].red + src[b1+1].red)>>2;
    dst[0].blue = (src[0].blue + src[1].blue + src[b1].blue + src[b1+1].blue)>>2;
    dst[0].green = (src[0].green + src[1].green + src[b1].green + src[b1+1].green)>>2;
    a2 = dim-2; b2 = dim*2-2;
    dst[a2+1].red = (src[a2].red + src[a2+1].red + src[b2].red + src[b2+1].red)>>2;
    dst[a2+1].blue = (src[a2].blue + src[a2+1].blue + src[b2].blue + src[b2+1].blue)>>2;
    dst[a2+1].green = (src[a2].green + src[a2+1].green + src[b2].green + src[b2+1].green)>>2;
    a1 = dim*(dim-2); b1 = dim*(dim-1);
    dst[b1].red = (src[a1].red + src[a1+1].red + src[b1].red + src[b1+1].red)>>2;
    dst[b1].blue = (src[a1].blue + src[a1+1].blue + src[b1].blue + src[b1+1].blue)>>2;
    dst[b1].green = (src[a1].green + src[a1+1].green + src[b1].green + src[b1+1].green)>>2;
    a2 = dim*(dim-1)-2; b2 = dim*dim-2;
    dst[b2+1].red = (src[a2].red + src[a2+1].red + src[b2].red + src[b2+1].red)>>2;
    dst[b2+1].blue = (src[a2].blue + src[a2+1].blue + src[b2].blue + src[b2+1].blue)>>2;
    dst[b2+1].green = (src[a2].green + src[a2+1].green + src[b2].green + src[b2+1].green)>>2;
    
    int i, j;
    for(i=1; i< dim-1; i=i+2){
        a1 = RIDX(0, i-1, dim); b1 = RIDX(1, i-1, dim);
        dst[a1+1].red = (src[a1].red + src[a1+1].red + src[a1+2].red + src[b1].red + src[b1+1].red + src[b1+2].red)/6;
        dst[a1+1].green = (src[a1].green + src[a1+1].green + src[a1+2].green + src[b1].green + src[b1+1].green + src[b1+2].green)/6;
        dst[a1+1].blue = (src[a1].blue + src[a1+1].blue + src[a1+2].blue + src[b1].blue + src[b1+1].blue + src[b1+2].blue)/6;
        dst[a1+2].red = (src[a1+1].red + src[a1+2].red + src[a1+3].red + src[b1+1].red + src[b1+2].red + src[b1+3].red)/6;
        dst[a1+2].green = (src[a1+1].green + src[a1+2].green + src[a1+3].green + src[b1+1].green + src[b1+2].green + src[b1+3].green)/6;
        dst[a1+2].blue = (src[a1+1].blue + src[a1+2].blue + src[a1+3].blue + src[b1+1].blue + src[b1+2].blue + src[b1+3].blue)/6;
    }
    
    for(i=1; i< dim-1; i=i+2){
        a1 = RIDX(dim-2, i-1, dim); b1 = RIDX(dim-1, i-1, dim);
        dst[b1+1].red = (src[a1].red + src[a1+1].red + src[a1+2].red + src[b1].red + src[b1+1].red + src[b1+2].red)/6;
        dst[b1+1].green = (src[a1].green + src[a1+1].green + src[a1+2].green + src[b1].green + src[b1+1].green + src[b1+2].green)/6;
        dst[b1+1].blue = (src[a1].blue + src[a1+1].blue + src[a1+2].blue + src[b1].blue + src[b1+1].blue + src[b1+2].blue)/6;
        dst[b1+2].red = (src[a1+1].red + src[a1+2].red + src[a1+3].red + src[b1+1].red + src[b1+2].red + src[b1+3].red)/6;
        dst[b1+2].green = (src[a1+1].green + src[a1+2].green + src[a1+3].green + src[b1+1].green + src[b1+2].green + src[b1+3].green)/6;
        dst[b1+2].blue = (src[a1+1].blue + src[a1+2].blue + src[a1+3].blue + src[b1+1].blue + src[b1+2].blue + src[b1+3].blue)/6;
    }
    
    for(i=1; i< dim-1; i++){
        a1 = RIDX(i-1, 0, dim); b1 = RIDX(i, 0, dim); c1 = RIDX(i+1, 0, dim);
        dst[b1].red = (src[a1].red + src[a1+1].red + src[c1].red + src[b1].red + src[b1+1].red + src[c1+1].red)/6;
        dst[b1].green = (src[a1].green + src[a1+1].green + src[c1].green + src[b1].green + src[b1+1].green + src[c1+1].green)/6;
        dst[b1].blue = (src[a1].blue + src[a1+1].blue + src[c1].blue + src[b1].blue + src[b1+1].blue + src[c1+1].blue)/6;
    }
    
    for(i=1; i< dim-1; i++){
        a1 = RIDX(i-1, dim-2, dim); b1 = RIDX(i, dim-2, dim); c1 = RIDX(i+1, dim-2, dim);
        dst[b1+1].red = (src[a1].red + src[a1+1].red + src[c1].red + src[b1].red + src[b1+1].red + src[c1+1].red)/6;
        dst[b1+1].green = (src[a1].green + src[a1+1].green + src[c1].green + src[b1].green + src[b1+1].green + src[c1+1].green)/6;
        dst[b1+1].blue = (src[a1].blue + src[a1+1].blue + src[c1].blue + src[b1].blue + src[b1+1].blue + src[c1+1].blue)/6;
    }

    
    for(i=1; i<dim-1; i++){
        for(j=1; j<dim-1; j=j+2){
            a1 = RIDX(i-1, j-1, dim); b1 = RIDX(i, j-1, dim); c1 = RIDX(i+1, j-1, dim);
            dst[b1+1].red = (src[a1].red + src[a1+1].red + src[a1+2].red + src[b1].red + src[b1+1].red + src[b1+2].red + src[c1].red + src[c1+1].red + src[c1+2].red)/9;
            dst[b1+1].green = (src[a1].green + src[a1+1].green + src[a1+2].green + src[b1].green + src[b1+1].green + src[b1+2].green + src[c1].green + src[c1+1].green + src[c1+2].green)/9;
            dst[b1+1].blue = (src[a1].blue + src[a1+1].blue + src[a1+2].blue + src[b1].blue + src[b1+1].blue + src[b1+2].blue + src[c1].blue + src[c1+1].blue + src[c1+2].blue)/9;
            
            dst[b1+2].red = (src[a1+1].red + src[a1+2].red + src[a1+3].red + src[b1+1].red + src[b1+2].red + src[b1+3].red + src[c1+1].red + src[c1+2].red + src[c1+3].red)/9;
            dst[b1+2].green = (src[a1+1].green + src[a1+2].green + src[a1+3].green + src[b1+1].green + src[b1+2].green + src[b1+3].green + src[c1+1].green + src[c1+2].green + src[c1+3].green)/9;
            dst[b1+2].blue = (src[a1+1].blue + src[a1+2].blue + src[a1+3].blue + src[b1+1].blue + src[b1+2].blue + src[b1+3].blue + src[c1+1].blue + src[c1+2].blue + src[c1+3].blue)/9;
        }
    }
}



/*
 * smooth - Your current working version of smooth. 
 * IMPORTANT: This is the version you will be graded on
 */
char smooth_descr[] = "smooth: Current working version";
void smooth(int dim, pixel *src, pixel *dst) 
{
    smooth_descrete_2(dim, src, dst);
}


/********************************************************************* 
 * register_smooth_functions - Register all of your different versions
 *     of the smooth kernel with the driver by calling the
 *     add_smooth_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_smooth_functions() {
    add_smooth_function(&smooth, smooth_descr);
    add_smooth_function(&naive_smooth, naive_smooth_descr);
    add_smooth_function(&smooth_descrete, descrete);
    add_smooth_function(&smooth_descrete_2, descrete_2);
    /* ... Register additional test functions here */
}

