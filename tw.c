// COMP2521 20T2 Assignment 1
// z5255845 (Justin Gonzaga)
// tw.c ... compute top N most frequent words in file F
// Usage: ./tw [Nwords] File

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include "Dict.h"
#include "WFreq.h"
#include "stemmer.h"

#define STOPWORDS "stopwords"
#define MAXLINE 1000
#define MAXWORD 100

#define STARTING "*** START OF"
#define ENDING   "*** END OF"

#define isWordChar(c) (isalnum(c) || (c) == '\'' || (c) == '-')

typedef enum { NO, YES } status;

int main( int argc, char *argv[])
{
   status Processing = NO; // Only begin processing words after ***START OF
   char *token;            // Tokenise words
   char *space = " \r\n"; // Delimiter
   FILE  *in;         // currently open file
   Dict   stopwords;  // dictionary of stopwords
   Dict   wfreqs;     // dictionary of words from book
   WFreq *results;    // array of top N (word,freq) pairs
                      // (dynamically allocated)
   
   char *fileName;    // name of file containing book text
   int   nWords;      // number of top frequency words to show

   char   line[MAXLINE];  // current input line
   char   word[MAXWORD];  // current word

   // process command-line args
   switch (argc) {
   case 2:
      nWords = 10;
      fileName = argv[1];
      break;
   case 3:
      nWords = atoi(argv[1]);
      if (nWords < 10) nWords = 10;
      fileName = argv[2];
      break;
   default:
      fprintf(stderr,"Usage: %s [Nwords] File\n", argv[0]);
      exit(EXIT_FAILURE);
   }

   // Build dictionary of stopwords.
   stopwords = newDict();
   
   // ERROR HANDLING - no stopwords dictionary.
   in = fopen("stopwords", "r");
   if (in == NULL) {
      fprintf(stderr, "Can't open stopwords\n");
      exit(EXIT_FAILURE);
   }

   // Insert words from stopwords file into stopwords dictionary.
   while (fscanf(in, "%s", word) != EOF) {
      DictInsert(stopwords, word);
   }
   fclose(in);

   // Build new dictionary.
   wfreqs = newDict();

   // open File for reading.
   in = fopen(fileName, "r");

   // ERROR HANDLING - Invalid File.
   if (in == NULL) {
      fprintf(stderr, "Can't open %s\n", fileName);
      exit(EXIT_FAILURE);
   }

   // Skip to line containing "*** START OF".
   for(int lineNum = 1; fgets(line, MAXWORD, in) != NULL; lineNum++) {
      if ((strstr(line, STARTING)) != NULL) {
         // go to next line
         fgets(line, MAXWORD, in); 
         Processing = YES;
         break;
      } 
   }

   // ERROR HANDLING - "*** START OF" not found.
   if (Processing != YES) {
      fprintf(stderr, "Not a Project Gutenberg book\n");
      exit(EXIT_FAILURE);
   }

   // Insert all words from file into dictionary (excluding stopwords).
   // while not EOF and not reached line containing "*** END OF":
   while (fgets(line, MAXLINE,in) != NULL && strstr(line, ENDING) == NULL) {
      // For each character in the line:
      for(int i =0; i < strlen(line); i++){
         // ensure characters are all lower-case.
         line[i] = tolower(line[i]);

         // Replace all non-word characters (excluding spaces) with space.
         if(isWordChar(line[i]) == 0 && line[i] != ' ') {
            line[i]= ' ';
         }
      }   
      
      // for each Word on the current line:
      for (token = strtok(line, space); token != NULL; token = strtok(NULL, space)) {

         // if not a stop word then:
         if (DictFind(stopwords, token) == NULL) {
            // ensure word is more than one character.
            int j = strlen(token) - 1;
            if (j <= 0) continue;

            // apply stemmer to Word.
            int k = stem(token, 0, j);
            token[k+1] = '\0';

            // if not in dictionary, add Word to dictionary.
            if (DictFind(wfreqs, token) == NULL) {
               DictInsert(wfreqs, token);
            }
            else {
               // increment counter for Word.
               WFreq *search = DictFind(wfreqs, token);
               search->freq++;
            }
         } 
      }
   }
   fclose(in);

   // Declare empty array of WFreq structs and print top N words.
   WFreq *wfs = (WFreq*) malloc(MAXLINE*MAXWORD*sizeof(WFreq));
   findTopN(wfreqs, wfs, nWords);

   // Free allocated memory.
   free(wfs);
   free(wfreqs);
   free(stopwords);

   return EXIT_SUCCESS;
}
