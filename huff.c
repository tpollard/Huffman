/*  ECE368 Project 2: Huffman Compression and Decompression
*   Contributors:
*       Thomas Pollard
*       Trenton Andres
*
*   File: huff.c
*
*   Created: 11/11/2011
*
*   Description:
*           This is the main program for the Huffman Compression algorithm.
*       The program takes one (1) command line argument, which is the file to
*       be compressed. The algorithm uses Huffman coding to compress the input
*       file, and stores the compressed file as <filename.extension>.huff
*
*   Last Modified: 11/11/2011
*
*/
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huff.h"

// Debug flag for printing intermediate results
//#define __DEBUG__


int main(int argc, char * argv[])
{
    /* Ensure the user entered a filename */
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return -1;
    }
    /* Open file as "binary, readonly" */
    FILE* fhd = fopen(argv[1], "rb");
        
    /*Ensure the file can be opened */
    if (fhd == NULL) {
        printf("ERROR: %s was not opened correctly.\n", argv[1]);
        return -1;
    }
    
    /* Read through file and count number of occurances of characters.
     * Character counts are stored in 'counts[]' array. Index is (int) 'char',
     * length of counts[] = 256 (one entry for each char)
     */
    unsigned long int * counts = malloc(sizeof(long) * 256);
    memset(counts, (unsigned long int) 0, sizeof(counts));      // Initialize memory to 
    int curchar;
    while ((curchar = fgetc(fhd)) != EOF) {
        (counts[curchar])++;
    }
    
    #ifdef __DEBUG__
        // print 'counts[]' for debugging purposes
        int i;
        printf("counts[]:\n[");
        for (i = 0; i < 256; i++) {
            printf("%lu, ", counts[i]);
        }
        printf("\b\b]\n");
    #endif
    
    
    
    
    /* Close the input file */
    fclose(fhd);
    
    
    return 0;
}
    


