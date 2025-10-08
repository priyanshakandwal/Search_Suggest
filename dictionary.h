#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "synonyms.h" 

typedef struct DictionaryWord {
    char word[MAX_WORD_LEN];
    struct DictionaryWord *next;
} DictionaryWord;


DictionaryWord *createDictionaryWord(const char *word);
void addToDictionary(DictionaryWord **head, const char *word);
int isWordInDictionary(DictionaryWord *head, const char *word);
int loadDictionaryFromFile(DictionaryWord **head, const char *filename);
void freeDictionary(DictionaryWord *head);

int loadThesaurusFromFile(SynonymGroup **synHead, DictionaryWord *dictHead, const char *filename);

#endif 