/*  ECE368 Project 2: Huffman Compression and Decompression
*   Contributors:
*       Thomas Pollard
*       Trenton Andres
*
*   File: unhuff.h
*
*   Created: 11/12/2011
*
*   Description:
*           This is the header file for the Huffman Decompression algorithm.
*       This header declares the node structure that is used to build the 
*       binary tree, as well as support functions. This header also includes
*       other function prototypes
*
*   Last Modified: 11/12/2011
*
*/

#ifndef __UNHUFFH__
#define __UNHUFFH__

#include "huff.h"

    Node * head;
    FILE * ofhd;
    
    /* This function builds the binary Huffman Coding tree */
	Node * tree(char bit, char byte, Node * curpos) {
		/* curpos == NULL when tree is finished being built (initially == head)
		 * byte is invalid unless bit == 1
		 */
		 
		/* If head == NULL, then create the head node */
		if (head == NULL) {
		    if (bit == 0) {
		        head = node_construct(0, 0);
		        return head;
		    }
		    else {
		        head = node_construct(0, byte);
		        return  NULL;
		    }
		}
                   
		/* When the tree is complete, curpos will point to NULL, so return */
		if (curpos == NULL) {
		    /* Find EOF and change value to 256 */
		    Node * n = head;
		    while (n->right != NULL) {
		        n = n->right;
		    }
		    n->character = 256;
		    return NULL;
		}
		 
		/* If the bit that was read is a zero, */
		if (bit == 0) {
		    /* Try to make a new child on the left */
		    if (curpos->left == NULL) {
		        Node * n = node_construct(0,0);
		        curpos->left = n;
		        n->next = curpos;
		        return n;
		    }
		    /* Try to make a new child on the right */
		    else if (curpos->right == NULL) {
		        Node * n = node_construct(0,0);
		        curpos->right = n;
		        n->next = curpos;
		        return n;
		    }
		    /* Travel up one level in the tree and try to add there */
		    else {
		        return tree(bit, byte, curpos->next);
		    }
		}
		
		/* If the bit is zero, add the character to a leaf node */
		if (bit == 1) {
		    /* Try to add leaf on left */
		    if (curpos->left == NULL) {
		        Node * n = node_construct(0, byte);
		        curpos->left = n;
		        n->next = curpos;
		        return curpos;
		    }
		    /* Try to add leaf on the right */
		    else if (curpos->right == NULL) {
		        Node * n = node_construct(0, byte);
		        curpos->right = n;
		        n->next = curpos;
		        return curpos;
		    }
		    /* Travel up one level in the tree and try to add there */
		    else {
		        return tree(bit, byte, curpos->next);
		    }
		}
		 
		 /* Should never reach this statement */
		return NULL;
	} 
	
	
	
	/* This function traverses the binary tree and prints a character in the
	 * output file when it reaches a leaf node
	 */
	Node * tree2(char bit, Node * curpos) {
		
		if (curpos == NULL) {
		    return NULL;
		    
		}
		
		if (bit == 1) {
		    curpos = curpos->right;
		}
		else if (bit == 0) {
		    curpos = curpos->left;
		}
		
		/* Check to see if you are at a leaf node */
		if ((curpos->right == NULL) && (curpos->left == NULL)) {
		    if (curpos->character == 256) {
		        return NULL;
		    }
		    fprintf(ofhd, "%c", (char) curpos->character);
		    return head;
		}
		 
		
		
		return curpos;
	} 


#endif
