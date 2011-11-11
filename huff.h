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
    
    /* Function to create a new Node and initialize values to 0 */
    Node* node_construct() {
        Node * n = malloc(sizeof(node));
        n->count = 0;
        n->character = '\0'
        n->left = NULL;
        n->right = NULL;
        return n;
    }
    
    /* Function to deallocate memory for node */
    void node_destruct(Node * n) {
        free(n);
    }
    
    /* Function to insert a node into a linked list in sorted order */
    Node* insert_node(Node * head, Node * n) {
        // If the list is empty, then return the new element
        if (head == NULL) {
            return n;
        }
        // Go through list until new element's spot is found
        Node* prev = NULL;
        Node* cur = head;
        while ((cur != NULL) && (cur->count < n->count)) {
            prev = cur;
            cur = cur->right;
        }
        // If new element is the smallest, place it at the front and return it
        if (cur == head) {
            n->right = cur;
            return n;
        }
        // Otherwise, place the new element in its position and return the head
        prev->right = n;
        n->right = cur;
        return head;
    }
            
            
            


#endif
