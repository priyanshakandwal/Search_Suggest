    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <ctype.h>
    #include "synonyms.h"


    SynonymGroup *createGroup(const char *mainWord) {
        SynonymGroup *newGroup = (SynonymGroup *)malloc(sizeof(SynonymGroup));
        if (newGroup == NULL) { perror("malloc failed"); exit(EXIT_FAILURE); }
        strncpy(newGroup->mainWord, mainWord, MAX_WORD_LEN - 1);
        newGroup->mainWord[MAX_WORD_LEN - 1] = '\0';
        newGroup->list = NULL;
        newGroup->next = NULL;
        return newGroup;
    }

    void addSynonym(SynonymGroup **head, const char *mainWord, const char *synWord) {
        SynonymGroup *curr = *head;

        while (curr) {
            if (strcasecmp(curr->mainWord, mainWord) == 0)
                break;
            curr = curr->next;
        }

        if (!curr) {
            SynonymGroup *newGroup = createGroup(mainWord);
            newGroup->next = *head;
            *head = newGroup;
            curr = newGroup;
        }

        Synonym *newSyn = (Synonym *)malloc(sizeof(Synonym));
        if (newSyn == NULL) {exit; }
        strncpy(newSyn->word, synWord, MAX_WORD_LEN - 1);
        newSyn->word[MAX_WORD_LEN - 1] = '\0';
        newSyn->next = curr->list;
        curr->list = newSyn;
    }


    void displaySynonyms(SynonymGroup *head, const char *mainWord) {
        SynonymGroup *curr = head;
        while (curr) {
            if (strcasecmp(curr->mainWord, mainWord) == 0) {
                printf("Synonyms for '%s': ", mainWord);
                Synonym *temp = curr->list;
                while (temp) {
                    printf("%s ", temp->word);
                    temp = temp->next;
                }
                printf("\n");
                return;
            }
            curr = curr->next;
        }
     
    }


    int areSynonyms(SynonymGroup *head, const char *word1, const char *word2) {
        SynonymGroup *curr = head;
        while (curr) {
            if (strcasecmp(curr->mainWord, word1) == 0) {
                Synonym *temp = curr->list;
                while (temp) {
                    if (strcasecmp(temp->word, word2) == 0)
                        return 1;
                    temp = temp->next;
                }
            }
            curr = curr->next;
        }
        return 0;
    }


    void freeAll(SynonymGroup *head) {
        while (head) {
            Synonym *temp = head->list;
            while (temp) {
                Synonym *toFree = temp;
                temp = temp->next;
                free(toFree);
            }
            SynonymGroup *toFreeGroup = head;
            head = head->next;
            free(toFreeGroup);
        }
    }