// COMP2521 20T2 Assignment 1
// Dict.c ... implementation of Dictionary ADT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Dict.h"
#include "WFreq.h"
#include <ctype.h>

typedef struct _DictNode *Node;
typedef struct  _DictNode {
   WFreq  data;
   int height;
   Node   left;
   Node   right;
} DictNode;

typedef struct _DictRep {
   Node root;
} DictRep;


static Node recursiveInsert(Node n, char *w);
static Node rotateLeft(Node n);
static Node rotateRight(Node n);
static int  height(Node n);
static int  max(int a, int b);
int count(Node root);
int comp (const void * word1, const void * word2);
static Node newNode(char *w);
static void recursiveShow(Node root);
static Node recursiveFind(Node root, char *w);
int DictToArray(Node root, WFreq *wfs, int index);

// create new empty Dictionary
Dict newDict()
{
    // Allocate memory for dictionary.
   Dict d= malloc (sizeof *d);
   if (d == NULL) { 
      fprintf(stderr, "couldn't allocate Dict");
      exit(EXIT_FAILURE);
   }

   d->root = NULL;
   return d;
}

// insert new word into Dictionary
// return pointer to the (word,freq) pair for that word
WFreq *DictInsert(Dict d, char *w)
{
   // Declare and initialise struct containing new word
   char *p = strdup(w);
   WFreq data;
   data.freq = 1;
   data.word = p;

   // Assign pointer to newly created struct
   WFreq *new;
   new = &data;

   // Insert word into dictionary
   d->root = recursiveInsert(d->root, p);

   return new;
}

// Helper function to insert a node into a dictionary
// using recursion. 
static Node recursiveInsert(Node n, char *w) {

   // Insert new node if node is empty.
   if (n == NULL) {
      return newNode(w);
   }
   // Insert in appropriate subtree otherwise.
   else {
      int diff = strcmp(w, n->data.word);
      if (diff < 0) {
         n->left = recursiveInsert(n->left, w);
      }
      else if (diff > 0) {
         n->right = recursiveInsert(n->right, w);
      }

      // Rebalance tree height.
      int LHeight = height(n->left);
      int RHeight = height(n->right);
      if ((LHeight - RHeight) > 1) {
         if (strcmp(w, n->left->data.word) > 0) {
            n->left = rotateLeft(n->left);
         }
         n = rotateRight(n);
      }
      else if ((RHeight - LHeight) > 1) {
         if (strcmp(w, n->right->data.word) < 0) {
            n->right = rotateRight(n->right);
         }
         n = rotateLeft(n);
      }

      // Update height of current node.
      n->height = 1 + max(height(n->left), height(n->right));
      return n;
   }        
   return n;
}

// Helper function to allocate memory to a new node.
static Node newNode(char *w)
{
   Node n = malloc(sizeof(DictNode));
   if (n == NULL) {
      fprintf(stderr, "Insufficient memory!\n");
      exit(EXIT_FAILURE);
   }

   n->data.word = w;
   n->data.freq = 1;
   n->height = 0;
   n->left = NULL;
   n->right = NULL;
   return n;
}

// Helper function to perform left rotation on a node in a tree.
// Returns the new root node.
static Node rotateLeft(Node root) {

    // Insert new Node if Node is empty.
    if (root == NULL) {
        return NULL;
    }
    // Return current node if no right subroot.
    if (root->right == NULL) { 
        return root;
    }
    // Root node points to left child of right subtree.
    Node curr = root->right;
    if (curr->left != NULL) {
        root->right = curr->left;
    }
    else { 
        root->right = NULL;
    }
    // Old root node becomes left child of new root node.
    curr->left = root;

    // Update heights of old root node and new root node.
    root->height = max(height(root->left), height(root->right)) + 1;
    curr->height = max(height(curr->left), height(curr->right)) + 1;

    return curr;
}

// Helper function to perform right rotation on a node in a tree.
// Returns the new root node.
static Node rotateRight(Node root) {

    // Insert new Node if Node is empty.
    if (root == NULL) {
        return NULL;
    }
    // Return current node if no left subroot.
    if (root->left == NULL) { 
        return root;
    }
    // Root node points to left child of right subtree.
    Node curr = root->left;
    if (curr->right != NULL) {
        root->left = curr->right;
    }
    else { 
        root->left = NULL;
    }
    // Old root node becomes left child of new root node.
    curr->right = root;

    // Update heights of old root node and new root node.
    root->height = max(height(root->right), height(root->left)) + 1;
    curr->height = max(height(curr->right), height(curr->left)) + 1;

    return curr;
}

// Helper function to find height of a node in a tree.
static int height(Node n) {
    if (n == NULL) {
        return -1;
    } else {
        return n->height;
    }
}

// Helper function to find maximum value of two integers.
static int max(int a, int b) {
    return a > b ? a : b;
}

// find Word in Dictionary
// return pointer to (word,freq) pair, if found
// otherwise return NULL
WFreq *DictFind(Dict d, char *w)
{
   Node search = recursiveFind(d->root, w);
   if (search == NULL) {
      return NULL;
   }
   else {
      WFreq *ptr;
      ptr = &search->data;
      return ptr;
   }
}

// Helper function to find a word in the dictionary using recursion.
// Returns the node containing the word, NULL otherwise.
static Node recursiveFind(Node root, char *w)
{
   if (root == NULL) {
      return NULL;
   }
   int diff = strcmp(w, root->data.word); 
   if (diff < 0) { 
      root =  recursiveFind(root->left, w);
   }
   else if (diff > 0) { 
      root = recursiveFind(root->right, w);
   }
   else { 
      return root;
   }

   return root;
}


// find top N frequently occurring words in Dict
// input: Dictionary, array of WFreqs, size of array
// returns: #WFreqs in array, modified array
int findTopN(Dict d, WFreq *wfs, int n)
{
   // Traverse through dictionary structure and store into array.
   Node root = d->root;
   int index = 0;
   DictToArray(root, wfs, index);

   // Sort the array in order of frequency and then alphabetically.
   int totalWords = count(root);
   qsort((void *)wfs, totalWords, sizeof(WFreq), comp);

   // Print out top N words.
   for (int j = 0; j < n; j++) {
      if (wfs[j].freq >= 1000) 
         printf("   %d ", wfs[j].freq);
      else if (wfs[j].freq >= 100) 
         printf("    %d ", wfs[j].freq);
      else if (wfs[j].freq < 10)
         printf("      %d ", wfs[j].freq);
      else 
         printf("     %d ", wfs[j].freq);

      printf("%s\n", wfs[j].word);
   }

   return 0;
}

// print a dictionary
void showDict(Dict d)
{
   recursiveShow(d->root);
   return;
}

// Helper function to print all nodes in a dictionary
// in alphabetical order using recursion.
void recursiveShow(Node root)
{
   if (root == NULL) {
      return;
   }
   recursiveShow(root->left);
   recursiveShow(root->right);
   printf("%s, freq %d\n", root->data.word, root->data.freq);

}

// Helper function that traverses through the dictionary
// and stores each word into an array.
int DictToArray(Node root, WFreq *wfs, int index)
{
   if (root == NULL) {
      return index;
   }
   wfs[index] = root->data;
   index++;
   if (root->left != NULL) {
      index = DictToArray(root->left, wfs, index);
   }
   if (root->right != NULL) {
      index = DictToArray(root->right, wfs, index);
   }

   return index;
}

// Helper function to compare two words by frequency and alphabetical order.
int comp (const void * word1, const void * word2)
{
   // Return 1 if word 2 has a higher frequency than word 1.
   int diff = ((WFreq*)word1)->freq - ((WFreq*)word2)->freq;
   if (diff < 0) {
      return 1;
   }
   // Return -1 if word 1 has a higher frequency than word 2.
   else if (diff > 0) {
      return -1;
   }
   else {
   // Sort alphabetically if frequencies are equal.
      int diff = strcmp(((WFreq*)word1)->word, ((WFreq*)word2)->word); 
      if (diff < 0) {
         return -1;
      }
      else if (diff > 0) {
         return 1;
      }
      else { 
         return 0;
      }
   }
}

// Count number of unique words in the dictionary.
int count(Node root)
{
    int total =  1;             
    if (root ==NULL)
        return 0;
    else {
        total += count(root->left);
        total += count(root->right);
        return total;
    }
}

