#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Defines
#define MAX_WORDSIZE 23

// Trie structure
struct Trie
{
    unsigned int count;
    struct Trie* next[26];
};

//Function Prototypes
void insert(struct Trie *pTrie, char *word);
int numberOfOccurances(struct Trie *pTrie, char *word);
struct Trie *deallocateTrie(struct Trie *pTrie);
struct Trie *createTrie();
int readDictionary(char *filename, char **pInWords);


// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word)
{
    if (pTrie != NULL) {
        //Check if next letter exists, create if necessary
        struct Trie* pTemp = pTrie;
        int wordLen = strlen(word);
        for (int i = 0; i < wordLen; ++i) {
            int nextLetterIndex = word[i] - 'a';
            if (pTemp->next[nextLetterIndex] == NULL) {
                pTemp->next[nextLetterIndex] = createTrie();
            }
            pTemp = pTemp->next[nextLetterIndex];
        }
        ++(pTemp->count);
    }
    else { //pTrie is NULL
        puts("Error: pTrie is NULL. Only call 'insert' with allocated Trie.");
    }
}

// computes the number of occurances of the word
int numberOfOccurances(struct Trie *pTrie, char *word)
{
    int numOccurances = 0;
    if (pTrie != NULL) {
        struct Trie* pTemp = pTrie;
        int wordLen = strlen(word);
        for (int i = 0; i < wordLen; ++i) {
            int nextLetterIndex = word[i] - 'a';
            if (pTemp->next[nextLetterIndex] == NULL) {
                break;
            }
            pTemp = pTemp->next[nextLetterIndex];
        }
        numOccurances = pTemp->count;
    }
    return numOccurances;
}

// deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie)
{
    for (int i = 0; i < 26; ++i) {
        if (pTrie->next[i] != NULL) {
            deallocateTrie(pTrie->next[i]);
        }
    }
    free(pTrie);
    return NULL;
}

// Initializes a trie structure
struct Trie *createTrie()
{
    struct Trie* newTrie = NULL;
    newTrie = (struct Trie*)malloc(sizeof(struct Trie));
    if (newTrie != NULL) {
        newTrie->count = 0;
        for (int i = 0; i < 26; ++i) {
            newTrie->next[i] = NULL;
        }
    }
    return newTrie;
}

// this function will return number of words in the dictionary,
// and real all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords)
{
    int numOfWords = -1;
    FILE* inputFile = fopen(filename, "r");
    if (inputFile != NULL) {
        fscanf(inputFile, "%d", &numOfWords);
        for (int i = 0; i < numOfWords; ++i) {
            pInWords[i] = (char*)malloc(MAX_WORDSIZE * sizeof(char));
            if (pInWords[i] != NULL) {
                fscanf(inputFile, "%s", pInWords[i]);
            }
        }
    }
    fclose(inputFile);
    return numOfWords;
}

int main(void) 
{
    char *inWords[256];

    //read the number of the words in the dictionary and insert file data into inWords
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i=0;i<numWords;++i)
    {
        printf("%s\n", inWords[i]);
    }
    // Adds each word into the trie
    struct Trie *pTrie = createTrie();
    for (int i=0;i<numWords;i++)
    {
        insert(pTrie, inWords[i]);
    }
    // Checks for number of occurances for the following words.
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"}; // 2 4 0 2 0
    for (int i=0;i<5;i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
    }
    pTrie = deallocateTrie(pTrie);
    if (pTrie != NULL) {
        printf("There is an error in this program\n");
    }
    return 0;
}