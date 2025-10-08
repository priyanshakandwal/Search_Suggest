#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h" 
#include "synonyms.h" 

char *trim_whitespace(char *str) {
    char *end;
    while(isspace((unsigned char)*str)) str++;
    if(*str == 0) return str;
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    return str;
}


DictionaryWord *createDictionaryWord(const char *word) {
    DictionaryWord *newWord = (DictionaryWord *)malloc(sizeof(DictionaryWord));
    if (newWord == NULL) { perror("malloc failed"); exit(EXIT_FAILURE); }
    strncpy(newWord->word, word, MAX_WORD_LEN - 1);
    newWord->word[MAX_WORD_LEN - 1] = '\0';
    newWord->next = NULL;
    return newWord;
}


void addToDictionary(DictionaryWord **head, const char *word) {
    DictionaryWord *newWord = createDictionaryWord(word);
    newWord->next = *head;
    *head = newWord;
}


int isWordInDictionary(DictionaryWord *head, const char *word) {
    DictionaryWord *curr = head;
    while (curr) {
        if (strcasecmp(curr->word, word) == 0) {
            return 1; 
        }
        curr = curr->next;
    }
    return 0; 
}


int loadDictionaryFromFile(DictionaryWord **head, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
      
        return 0;
    }

    char buffer[MAX_WORD_LEN];
    int wordCount = 0;

    while (fgets(buffer, MAX_WORD_LEN, file) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            len--;
        }

        if (len > 0 && len < MAX_WORD_LEN) {
            addToDictionary(head, buffer);
            wordCount++;
        }
    }

    fclose(file);
  
    return 1;
}


void freeDictionary(DictionaryWord *head) {
    DictionaryWord *curr = head;
    while (curr) {
        DictionaryWord *toFree = curr;
        curr = curr->next;
        free(toFree);
    }
}


int loadThesaurusFromFile(SynonymGroup **synHead, DictionaryWord *dictHead, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
     
        return 0;
    }

    char line_buffer[MAX_WORD_LEN * 5];
    int groupCount = 0;

    while (fgets(line_buffer, sizeof(line_buffer), file) != NULL) {
        char *main_word_raw;
        char *synonyms_list_raw;
        char *synonym_token;
        
        main_word_raw = strtok(line_buffer, ":");
        synonyms_list_raw = strtok(NULL, "\n"); 
        
        if (main_word_raw == NULL || synonyms_list_raw == NULL) continue;
        
        char *main_word = trim_whitespace(main_word_raw);

        
        if (!isWordInDictionary(dictHead, main_word)) {
            
            continue;
        }

        char temp_synonyms[MAX_WORD_LEN * 4]; 
        strncpy(temp_synonyms, synonyms_list_raw, sizeof(temp_synonyms) - 1);
        temp_synonyms[sizeof(temp_synonyms) - 1] = '\0';

        synonym_token = strtok(temp_synonyms, ",");
        
        
        while (synonym_token != NULL) {
            char *syn_word = trim_whitespace(synonym_token);

           
            if (isWordInDictionary(dictHead, syn_word)) {
                
                addSynonym(synHead, main_word, syn_word);
            } 
            
            synonym_token = strtok(NULL, ",");
        }
        groupCount++;
    }

    fclose(file);
   return 1;
}