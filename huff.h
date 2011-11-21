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
        unsigned int character;
        int height;
        struct binarynode *left;
        struct binarynode *right;
        struct binarynode *next;
    } Node;
    
    /* Function to create a new Node and initialize values to 0 */
    Node* node_construct(int count, unsigned int character) {
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
    
    /* Function to print out an unsigned long in binary form  */
    void print_binary(int o, unsigned long d, unsigned long l) {
      unsigned long i = 0;
      unsigned long temp = d;
      temp = temp << (sizeof(unsigned long)*8 - l);
      if(o) {
          while(i++ < (sizeof(unsigned long)*8 - l)) printf(" ");
      }
      i = 0;
      while(i < l) {
          if(temp & 0x8000000000000000) {
              printf("1");
          } else {
              printf("0");
          }
          temp = temp << 1;
          i++;
      }
      //printf("\n");
    }

    /* Function to pack and write bits to output file */
    void pack(FILE *wfp, int *bits_remaining, unsigned long int *output, unsigned long int input, unsigned long length) {
        if(length < *bits_remaining){
            //no problem just pack em in
            *output = (*output << length)|input;
            *bits_remaining -= length;            
        }
        else {
            //Can get nasty; have to overflow into next output
            *output = (*output << *bits_remaining)|(input>>(length - *bits_remaining));
            fwrite(output,sizeof(char),8,wfp);
            //printf("\nWrote:  ");
            //print_binary(1,*output,(sizeof(unsigned long)*8));
            //printf("/n");
            *output = input; //bits already written out will be shifted away
            *bits_remaining = sizeof(unsigned long)*8 - (length - *bits_remaining);
        }
        //printf("\nOutput: ");
        //print_binary(1,*output,(sizeof(unsigned long)*8 - *bits_remaining));
        //printf(" BitsRem: %d", *bits_remaining);
    }

    
    /* Function to get the codes out of the binary tree */
    void get_codes(Node * root, unsigned long int codetable[257][2], unsigned long int * length, unsigned long int * code) {
    	
    	/* The root should never equal null, but just in case, return to avaid a seg fault */
    	if (root == NULL) {
    		return;
    	}
    	
    	/* If you are at a leaf node, store the node's code and the length of the code 
    	 * The node's code is a running total of the path it takes to reach the 
    	 * node. 0 is for left and 1 is for right. The length is equal to the depth of
    	 * the node in the tree 
    	 */    	
    	if (root->height == 0) {
    		codetable[root->character][0] = *code;
    		codetable[root->character][1] = *length;
    		//printf("%c, %d %d\n", (char) root->character, *code, *length);
    		return;  	
    	}
    	
    	/* If you are not at a leaf, increase the length and go down a level to the left */
    	(*length)++;
    	/* Shift the code left 1 bit (zero fill) to account for the next node's code */
    	*code = (*code)<<1;
    	get_codes(root->left, codetable, length, code);
    	
    	/* Add 1 to the code to establish that the next node is to the right */
    	(*code)++;
    	get_codes(root->right, codetable, length, code);
    	
    	/* Decrease the length and shift the code right 1 bit to return up to the parent */
    	(*length)--;
    	*code = (*code)>>1; 
    }
    
    /* Function to print the code tree into the header of the file.
     * This prints the tree in a pre-order fashion 
     */
    void print_header(FILE *wfp, Node * root, int *bits_remaining, unsigned long int *output) {
        
        if (root == NULL) {
            return;
        }
        

        /* If root is not a leaf node, print a 0, else print 1 and the char */
        if ((root->left == NULL) && (root->right == NULL)) {
            pack(wfp, bits_remaining, output, 1, 1);
            if (root->character != 256) {
                pack(wfp, bits_remaining, output, root->character, 8);
                //printf("%c\n", (char) root->character);
            }
            else {
                pack(wfp, bits_remaining, output, 255, 8);
                pack(wfp, bits_remaining, output, 0, 1);
            }
        }
        else {
            pack(wfp, bits_remaining, output, 0, 1);
        }
        
        print_header(wfp, root->left, bits_remaining, output);
        print_header(wfp, root->right, bits_remaining, output);
        
		/* Once all childern have been process, delete the node */
        node_destruct(root);
        root = NULL;
    
    }
            
    /* Function to print a binary tree in-order */
    void print_tree(Node * root) {
        if (root == NULL) {
            return;
        }
        print_tree(root->left);
        if ((root->left == NULL) && (root->right == NULL)) {
            unsigned long int count = 0;
        	if (root->character == 256) {
            	printf("%lu, <EOF>, h:%d\n", /*root->*/count, root->height);
        	}
        	else {
            	printf("%lu, <(%d)%c>, h:%d\n", /*root->*/count, root->character, (char) root->character, root->height);
        	}
       	}
        print_tree(root->right);
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
    
    /* Function to append the '.huff' to the end of an arbitrarily named file, returns pointer to string */
    char *huff_output_filename(char *filename){
        int i = 0;
        char *newname;
        while(*(filename+i) != '\0') i++;
  
        //printf("i: %d\n",i);
        newname = malloc(sizeof(char)*(i+6));
        i = 0;
        while(*(filename + i) != '\0') {
          *(newname+i) = *(filename + i);
          i++;
        }
        *(newname+i++) = '.';
        *(newname+i++) = 'h';
        *(newname+i++) = 'u';
        *(newname+i++) = 'f';
        *(newname+i++) = 'f';
        *(newname+i) = '\0';
        //printf("Old file name: %s\n", filename);
        //printf("New file name: %s\n", newname);
        return(newname);
    }
       


#endif
