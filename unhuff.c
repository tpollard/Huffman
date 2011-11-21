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
	int read_result = 0, bits_remaining = 0;
    unsigned char mybit, mybyte;
    mybyte = 0;
    unsigned long int ibuff;
    /*Make sure the file was created by our huffing*/
	read_result = fread(&ibuff, sizeof(unsigned long), 1, fhd);
	if(ibuff != 0xBADFACE000ECE368){
		printf("Error: Not compressed using corresponding program.\n");
		return 0;
	}
    // Burn through header until tree() says no more
    do{
		if(bits_remaining == 0) {
			
			read_result = fread(&ibuff, sizeof(unsigned long), 1, fhd);
			//print_binary(0,ibuff, 64);
			//printf("\n");
			bits_remaining = sizeof(unsigned long)*8;
		}
		//Pull next bit, keep track of remainder
		mybit = (ibuff >> (--bits_remaining)) & 0x0000000000000001;
	    //if it is a one pull the next 8 bits into mybyte
		if(mybit) {
			//If we have a enough remaining bits to fill the byte, do so
			if(bits_remaining >= 8) {
				mybyte = (ibuff >> (bits_remaining - 8)) & 0x00000000000000FF;
				bits_remaining -= 8;
			//otherwise things are somewhat tricky
			} else {
				//put the remaining bits from the buff into the byte
				mybyte = (ibuff << (8-bits_remaining)) & 0x00000000000000FF;
				//pull in a new buffer
				read_result = fread(&ibuff, sizeof(unsigned long), 1, fhd);
				//Pull the remaining bits for the byte in
				mybyte = mybyte | ((ibuff >> (sizeof(unsigned long)*8 - 8 + bits_remaining)) & 0x00000000000000FF);
				//Set up the number of bits remaining in the next buffer
				bits_remaining = sizeof(unsigned long)*8 - (8-bits_remaining);
			} 
			//printf("%c\n", mybyte);
		}
		curpos = tree(mybit, mybyte, curpos);
    } while(curpos != NULL);
    
    #ifdef __DEBUGTREE__
        /* Print tree for debugging */
        printf("Code tree:\n");
        print_tree(head);
        printf("\n");
    #endif
    
    
    /*  - Read the file into a buffer, stop when fread no longer returns
     *  	* Go through buffer
     *			-break bytes into bits, turning over true/false to code inside
     */

	curpos = head;
	//Open the output file with the ".unhuff" appended
	char * outname = unhuff_output_filename(argv[1]);
    ofhd = fopen(outname,"wb");
    free(outname); //free the string!
    //Go through file until tree2() can't take it anymore
    do{
    	//Pull another 64 bit buffer if we are out bits
        if(bits_remaining == 0){
			read_result = fread(&ibuff, sizeof(unsigned long), 1, fhd);
			bits_remaining = sizeof(unsigned long)*8;
			//We shoudl always encounter an 'EOF' before the file is run out
			if(read_result < 1) {
				printf("\n**ERROR:END OF FILE**\n");
				return 0;
			}
		}
		//Pull the next bit out of buffer, keep track of number of bits
		//Pass the next data bit to the tree function along with it's position
		//from last pass.
        mybit = (ibuff >> (--bits_remaining)) & 0x0000000000000001;
		curpos = tree2(mybit, curpos);
    } while (curpos != NULL);
    
    /* Delete the tree */
    free_tree(head);
    
    /* Close the input file */
    fclose(fhd);
	fclose(ofhd);
    return 0;
}


