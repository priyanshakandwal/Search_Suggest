#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "synonyms.h"   
#include "dictionary.h" 

#define WORD_LIST_FILE "wordlist.txt" 
#define THESAURUS_DATA_FILE "thesaurus_db.txt"

int main() {
    
    SynonymGroup *synonymHead = NULL;
    DictionaryWord *dictHead = NULL;
    
    printf("--- SMART SUGGEST: Synonym Module Test ---\n");

    
    printf("\n[STEP 1] Loading Dictionary (Word List) for Validation...\n");
    if (!loadDictionaryFromFile(&dictHead, WORD_LIST_FILE)) {
        fprintf(stderr, "FATAL ERROR: Could not load dictionary. Check '%s'.\n", WORD_LIST_FILE);
        return 1;
    }

    
    printf("\n[STEP 2] Loading Thesaurus Relationships and Validating Words...\n");
    
    if (!loadThesaurusFromFile(&synonymHead, dictHead, THESAURUS_DATA_FILE)) {
        fprintf(stderr, "FATAL ERROR: Could not load thesaurus data. Check '%s'.\n", THESAURUS_DATA_FILE);
        
        
        freeDictionary(dictHead);
        return 1;
    }
    
    
    printf("\n[STEP 3] Testing Synonym Lookup:\n");
    
    // ensure these words are in your thesaurus_db.txt and wordlist.txt
    displaySynonyms(synonymHead, "happy"); 
    displaySynonyms(synonymHead, "big"); 

    // Test the specific relation check
    if (areSynonyms(synonymHead, "happy", "joyful")) {
        printf("'happy' and 'joyful' ARE confirmed synonyms.\n");
    } else {
        printf("'happy' and 'joyful' are NOT confirmed synonyms.\n");
    }

    
    printf("\n[STEP 4] Memory Cleanup:\n");
    freeAll(synonymHead);
    freeDictionary(dictHead);
    printf("All memory freed successfully.\n");

    return 0;
}