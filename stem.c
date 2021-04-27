// stems words and prints them one per line

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stemmer.h"

#define MAXWORD 100

int main(void) {
    char word[MAXWORD + 1];
    while (scanf("%s", word) == 1) {
        int end = stem(word, 0, strlen(word) - 1);
        word[end + 1] = '\0';
        printf("%s\n", word);
    }
}

//       for (token = strtok(strdup(line), space); token != NULL; token = strtok(NULL, space)) {
//          // if not a stopword
//          if (DictFind(stopwords, token) == NULL) {
//             // printf("token is %s\n", token);
//             // apply stemmer to Word
// //             printf("token is currently %s\n", token);
// //             int j = strlen(token) - 1;
// //             if (j <= 0) break;
// //             int k = stem(token, 0, j);
// //             printf("token now is %s\n", token);
// //             strcpy(token, token);
// //             token[k+1] = '\0';
// //             printf("token is now %s\n", token);


//    while (fgets(line, MAXLINE,in) != NULL && strstr(line, ENDING) == NULL) {
//       // For each Word on the current line:
//       // printf("line is %s\n");
//       for (token = strtok(strdup(line), space); token != NULL; token = strtok(NULL, space)) {
//          // if not a stopword
//          if (DictFind(stopwords, token) == NULL) {
//             // printf("token is %s\n", token);
//             // apply stemmer to Word
//             token = makeLower(token);
//             int j = strlen(token) - 1;
//             if (j <= 0) break;
//             int k = stem(token, 0, j);
//             strcpy(token, token);
//             token[k+1] = '\0';

//             // if not in dictionary
//             if (DictFind(wfreqs, token) == NULL) {
//                // add Word to dictionary
//                DictInsert(wfreqs, token);
//             } else {
//                // increment counter for Word
//                WFreq *search = DictFind(wfreqs, token);
//                search->freq++;
//             }
//          }
//       }
//    }

//    char *makeLower (char *w) {

//    char *p = strdup(w);
//    for (int i = 0; p[i] != '\0'; i++) {
//       if (p[i] >= 'A' && p[i] <= 'Z') {
//          p[i] +=32;
//       }
//    }
//    return p;
// }