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
//#define __DEBUGCOUNT__
//#define __DEBUGLIST__
//#define __DEBUGLISTTOTREE__
//#define __DEBUGTREE__
//#define __DEBUGCODES__
//#define __DEBUGPACK__



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
    unsigned long int counts[257][2] = {{0,0}};
    
    // Initialize memory to all 0's   
    int curchar = 0;
    while ((curchar = fgetc(fhd)) != EOF) {
        (counts[curchar][0])++;
    }
    
    #ifdef __DEBUGCOUNT__
        // print 'counts[]' for debugging purposes
        {
            int i = 0;
            printf("counts[]:\n[");
            for (i = 0; i < 256; i++) {
                printf("%lu, ", counts[i][0]);
            }
            printf("\b\b]\n");
        }
    #endif
    
    /* Loop through counts[] and create a sorted list of Nodes for each 
     * non-zero character
     */
    int i = 0;
    // Initialize list
    Node * list = NULL;
    // Iterate from 255 downto 0 so if 2 chars have same count, the one with
    // a smaller (int) representation will be inserted into the list later, 
    // so it will appear earlier in the final list
    for (i = 255; i >= 0; i--) {
        if (counts[i][0] != 0) {
            /* Create new node and insertion sort into list */
            list = insert_node(list, node_construct(counts[i][0], i));
        }
    }
    
    /* Add psuedo-EOF character as first element. 
     * The psuedo EOF character is d256, and it's count is 1
     */
    list = insert_node(list, node_construct(1, 256));
    
    #ifdef __DEBUGLIST__
        /* Print list for debugging purposes */
        print_list(list);
        printf("\n");
    #endif
    
    /* Construct a binary tree from the list. This is done using two 'queues'
     * that are implemented with linked lists. Initially, all nodes are in
     * the first queue. The node with the smallest count is popped from the 
     * queue and linked to the next smallest. This new tree is then pushed to
     * the back of the second queue, and the process is repeated until only
     * one element remains. The tree is built up from the bottom right, and
     * so it is heavy to the right side. This way, the psuedo-EOF character
     * will always be the last element of the tree
     */
     Node * otherlist = NULL;
     int done = 0;
     Node * first = NULL;
     Node * second = NULL;
     Node * tempparent = NULL;
     Node * tail = NULL;
     while (done == 0) {
        
        #ifdef __DEBUGLISTTOTREE__
            /* Print list for debugging purposes */
            printf("List:\n");
            print_two_lists(list,otherlist);
            printf("\n"*code = );
            getchar();
        #endif

        // If there isn't anything in the first list, choose the second list
        if (list == NULL) { 
            first = otherlist;
            otherlist = otherlist->next;
        }
        else { // There's something in the first list
            // If there isn't anything in the second list, choose the first
            if (otherlist == NULL) {
                first = list;
                list = list->next;
            }
            else { // There's something in both lists, 
                // Compare the two and take the one with the smallest count
                if (list->count <= otherlist->count) {
                    first = list;
                    list = list->next;
                }
                else {
                    first = otherlist;
                    otherlist = otherlist->next;
                }
            }
        }
        
        // Now choose the second node:
        // If the first list is empty...
        if (list == NULL) {
            // If there aren't any elements left, you're done
            if (otherlist == NULL) {
                done = 1;
            }
            else { // Otherwise chose the second list
                second = otherlist;
                otherlist = otherlist->next;
            }
        }
        else { // There's something in the first list...
            // If the second list is empty, choose the first list
            if (otherlist == NULL) {
                second = list;
                list = list->next;
            }
            else {
            // Otherwise, compare the two and select the smallest
                if (list->count <= otherlist->count) {
                    second = list;
                    list = list->next;
                }
                else {
                    second = otherlist;
                    otherlist = otherlist->next;
                }
            }
        }
        // If you didn't reach the end case...
        if (done == 0) {
            // Join the two nodes
            tempparent = node_construct((first->count)+(second->count), '\0');
            // Place the bigger tree (larger height) on the right
            if (first->height >= second->height) {
                tempparent->right = first;
                tempparent->left = second;
                // Store the parent's height
                tempparent->height = first->height + 1;
            }
            else {
                tempparent->right = second;
                tempparent->left = first;
                // Store the parent's height
                tempparent->height = second->height + 1;
            }
            // Remove invalid references
            first->next = tempparent;
            second->next = tempparent;
            // Push the new parent into the second list using tail
            if (otherlist == NULL) {
                tail = tempparent;
                otherlist = tail;
            }
            else {
                tail->next = tempparent;
                tail = tempparent;
            }
        }
    }
    
    // Store the root node
    Node * codetree = first;
   	
    #ifdef __DEBUGTREE__
        /* Print tree for debugging */
        printf("Code tree:\n");
        print_tree(codetree);
        printf("\n");
    #endif
    

    unsigned long int code_length = 0;
    unsigned long int code = 0;
    // using unsigned long to cut down on number of writes
    unsigned long int output = 0;
    int bits_remaining = sizeof(output) *8;
    // Attempt to open/create the file with .huff appended
    char * outname = huff_output_filename(argv[1]);
    FILE *wfp = fopen(outname,"wb");
    free(outname);
   	// Store code values in the codetable array
   	get_codes(codetree, counts, &code_length, &code);
   	
   	#ifdef __DEBUGCODES__
        // print 'codetable[][]' for debugging purposes
        {
            int i = 0;
            printf("codes[]:\n");
            for (i = 0; i < 256; i++) {
                printf("%lu, %lu\n", counts[i][0], counts[i][1]);
            }
        }
    #endif
   	
	/* Store the data in the compression tree to the header of the file */
    print_header(wfp, codetree, &bits_remaining, &output);

    /*************************************************
     * This is the start of the section of code which
     * looks up the huffman code for each character of
     * the file and packs them into an output binary 
     * file.
     * For memory efficiency's sake we re-use the counts array
     * the new usage is: 
     * counts[char][0] = code, counts[char][1] = bit length of code
     * 
     */
    // We need a reasonably sized character buffer for reading
    char cbuff[256];

    int result;
    // We have to start the file over
    rewind(fhd);
    if(wfp) {
        // fread returns number of elements read
        // if EOF then returns zero
        result = fread(cbuff, sizeof(char), 256, fhd);
        while( result > 0) {
            i = 0;
            while( i < result) {
                // use character as index, find code and length
                code_length = counts[(int)cbuff[i]][1];
                code = counts[(int)cbuff[i]][0];
                /* we have to keep track of how man bits are left
                   to fill each write to maximum. Packing operation:
                   1) Check size constraints
                   2) shift existing bits by max(bits_remaining,code_length)
                   3) write output and zero it
                   4) shift in any remainder */
                if(code_length < bits_remaining) {
                    //no problem just pack em in
                    output = (output << code_length)|code;
                    bits_remaining -= code_length;
		    		#ifdef __DEBUGPACK__
		        		print_binary(1,output,64-bits_remaining);
                        printf(" C: %c code:",cbuff[i]);
		        		print_binary(0,code,code_length);
                        printf(" brem: %d\n",bits_remaining);
                    #endif
                } 
                else if(code_length >= bits_remaining) {
                    //Can get nasty; have to overflow into next output
                    output = (output << bits_remaining)|(code>>(code_length - bits_remaining));
		    		#ifdef __DEBUGPACK__
		        		print_binary(1,output,64-bits_remaining);
                        printf(" C: %c code:",cbuff[i]);
		        		print_binary(0,code,code_length);
                        printf(" brem: %d\n",bits_remaining);
            		#endif
                    fwrite(&output,sizeof(output),1,wfp);
                    output = code; //bits already written out will be shifted away
                    bits_remaining = sizeof(output)*8 - (code_length - bits_remaining);
		    		#ifdef __DEBUGPACK__
		        		print_binary(1,output,64-bits_remaining);
                        printf(" C: %c code:",cbuff[i]);
		        		print_binary(0,code,code_length);
                        printf(" brem: %d\n",bits_remaining);
                    #endif
                }
                i++;
            }
            result = fread(cbuff, sizeof(char), 256, fhd);
        }
        if (bits_remaining != sizeof(unsigned long)*8) {
        	output = output << (64-bits_remaining);
        	fwrite(&output, sizeof(output), 1, wfp);
       	}
    }   
    else {
      printf("Failed to open output file\n");
    }
	

    /* Close the input and output files */
    fclose(fhd);
    fclose(wfp);

    
    
    return 0;
}
    


