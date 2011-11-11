/*  ECE368 Project 2: Huffman Compression and Decompression
*   Contributors:
*       Thomas Pollard
*       Trenton Andres
*
*   File: huff.h
*
*   Created: 11/11/2011
*
*   Description:
*           This is the header file for the Huffman Compression algorithm.
*       This header declares the node structure that is used to build the 
*       binary tree, as well as support functions. This header also includes
*       other function prototypes
*
*   Last Modified: 11/11/2011
*
*/

#ifndef __HUFFH__
#define __HUFFH__

    /* Defining 'Node' structure. The nodes will be used to form a binary tree
     * and also a (singly) linked list. Nodes store the character count, the
     * character, and the left* and right* pointers
     */
    typedef struct binarynode {
        unsigned long int count;
        char character;
        struct binarynode *left;
        struct binarynode *right;
    } Node;
    
    



#endif
