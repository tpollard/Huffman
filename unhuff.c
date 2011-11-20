/*  ECE368 Project 2: Huffman Compression and Decompression
*   Contributors:
*       Thomas Pollard
*       Trenton Andres
*
*   File: unhuff.c
*
*   Created: 11/12/2011
*
*   Description:
*           This is the main program for the Huffman Decompression algorithm.
*       The program takes one (1) command line argument, which is the file to
*       be decompressed. The algorithm uses Huffman coding to decompress the
*		input file, and stores the decompressed file as 
*		<filename.extension>.unhuff
*
*   Last Modified: 11/12/2011
*
*/
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unhuff.h"

// Debug flags for printing intermediate results
//#define __DEBUGTREE__


// Global variable declarations
FILE * ofhd;
Node * head;

int main(int argc, char * argv[])
{
    /* Ensure the user entered a filename */
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return -1;
    }
    /* Open file as "binary, readonly" */
    FILE * fhd = fopen(argv[1], "rb");
        
    /*Ensure the file can be opened */
    if (fhd == NULL) {
        printf("ERROR: %s was not opened correctly.\n", argv[1]);
        return -1;
    }
    
    head = NULL;
    Node * curpos = NULL;

    do{
	if(bits_remaining == 0) {
		
		read_result = fread(&ibuff, sizeof(unsigned long), 1, fhd);
		bits_remaining = sizeof(unsigned long)*8;
	}
	
	mybit = (ibuff >> (--bits_remaining)) & 0x0000000000000001;
	
	if(mybit) {

		if(bits_remaining > 8) {
			mybyte = (ibuff >> (bits_remaining - 8)) & 0x00000000000000FF;
			bits_remaining -= 8;
			
		} else {
			mybyte = (ibuff << (8-bits_remaining)) & 0x00000000000000FF;
			read_result = fread(&ibuff, sizeof(unsigned long), 1, fhd);
			mybyte = mybyte | ((ibuff >> (sizeof(unsigned long)*8 - 8 + bits_remaining)) & 0x00000000000000FF);
			bits_remaining = sizeof(unsigned long)*8 - (8-bits_remaining);
		} 
	}


    } while(curpos != NULL);
    
    /*  - Read the file into a buffer, stop when fread no longer returns
     *  	* Go through buffer
     *			-break bytes into bits, turning over true/false to code inside
     */


    do{
        if(bits_remaining == 0){
		read_result = fread(&ibuff, sizeof(unsigned long), 1, fhd);
		bits_remaining = sizeof(unsigned long)*8;
		if(read_result < 1) {
			printf("\n**ERROR:END OF FILE**\n");
			return 0;
		}
	}
        mybit = (ibuff >> (--bits_remaining)) & 0x0000000000000001;

    } while (curpos != NULL);
    
    /* Delete the tree */
    free_tree(head);
    
    /* Close the input file */
    fclose(fhd);
    //printf("\n");
    return 0;
}


