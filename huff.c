/*  ECE368 Project 2: Huffman Compression and Decompression
*   Contributors:
*       Thomas Pollard
*       Trenton Andres
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
    
    /* Close the input file */
    fclose(fhd);
    
    
    return 0;
}
    


