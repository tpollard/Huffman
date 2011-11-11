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
        int character;
        int height;
        struct binarynode *left;
        struct binarynode *right;
        struct binarynode *next;
    } Node;
    
    /* Function to create a new Node and initialize values to 0 */
    Node* node_construct(int count, int character) {
        Node * n = malloc(sizeof(Node));
        n->count = count;
        n->character = character;
        n->height = 0;
        n->left = NULL;
        n->right = NULL;
        n->next = NULL;
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
            cur = cur->next;
        }
        // If new element is the smallest, place it at the front and return it
        if (cur == head) {
            n->next = cur;
            return n;
        }
        // Otherwise, place the new element in its position and return the head
        prev->next = n;
        n->next = cur;
        return head;
    }
    
    /* Function to free a binary tree */
    void free_tree(Node * root) {
        if (root == NULL) {
            return;
        }
        free_tree(root->right);
        free_tree(root->left);
        node_destruct(root);
    }
    
    
            
    /* Function to print a binary tree in reverese post-order */
    void print_tree(Node * root) {
        if (root == NULL) {
            return;
        }
        print_tree(root->right);
        
        if (root->character == 256) {
            printf("%lu, <EOF>, h:%d\n", root->count, root->height);
        }
        else {
            printf("%lu, <%c>, h:%d\n", root->count, (char) root->character, root->height);
        }
        print_tree(root->left);
    }
    
    /* Function to print the linked list */
    void print_list(Node * head) {
        if (head == NULL) {
            return;
        }
        if (head->character == 256) {
            printf("%lu, <EOF>\n", head->count);
        }
        else {
            printf("%lu, <%c>\n", head->count, (char) head->character);
        }
        print_list(head->next);
    }
               
    /* Function to print two linked lists side by side */
    void print_two_lists(Node * list, Node * other) {
        if (list != NULL) {
            if (list->character == 256) {
                printf("%3lu, <EOF>    ", list->count);
            }
            else {
                printf("%3lu, <%c>      ", list->count, (char) list->character);
            }
            list = list->next;
        }
        else {
                printf("              ");
        }
        if (other != NULL) {
            if (other->character == 256) {
                printf("%3lu, <EOF>    ", other->count);
            }
            else {
                printf("%3lu, <%c>      ", other->count, (char) other->character);
            }
            other = other->next;
        }
        printf("\n");
        if ((other != NULL) || (list != NULL)) {
            print_two_lists(list, other);
        }
    }        


#endif
