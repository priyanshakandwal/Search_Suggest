#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Recent_S.h"

void initRecentSearches(RecentSearches *rs) {
    rs->head=rs->tail=NULL;
    rs->count=0;
}

HistoryNode *createNewHistoryNode(const char *term) {
    HistoryNode *newNode=(HistoryNode*)malloc(sizeof(HistoryNode));
    if (!newNode) {
        perror("Failed to allocate memory for history node");
        return NULL;
    }
    
    strncpy(newNode->search_term, term, MAX_WORD_LEN - 1);
    newNode->search_term[MAX_WORD_LEN-1]='\0';
    
    newNode->prev=NULL;
    newNode->next=NULL;
    
    return newNode;
}

void addRecentSearch(RecentSearches *rs, const char *term) {
    if (term==NULL || strlen(term)==0) return;

    HistoryNode *newNode=createNewHistoryNode(term);
    if (!newNode) return;

    if (rs->head==NULL) {
        rs->head=rs->tail=newNode;
        rs->count=1;
        return;
    }

    newNode->next=rs->head;
    rs->head->prev=newNode;
    rs->head=newNode;
    rs->count++;

    if (rs->count>MAX_HISTORY) {
        HistoryNode *oldest=rs->tail;
        rs->tail=oldest->prev;
        
        if (rs->tail!=NULL) {
            rs->tail->next=NULL;
        } else {
            rs->head=NULL;
        }
        
        free(oldest);
        rs->count--;
    }
}

void displayRecentSearches(RecentSearches *rs) {
    printf("\n--- Recent Searches (Max %d, Current %d) ---\n", MAX_HISTORY, rs->count);
    
    if (rs->head==NULL) {
        printf("History is empty.\n");
        return;
    }

    HistoryNode *curr=rs->head;
    int rank=1;
    while (curr!=NULL) {
        printf("  %d. %s\n", rank++, curr->search_term);
        curr=curr->next;
    }
}

void cleanupRecentSearches(RecentSearches *rs) {
    HistoryNode *current=rs->head;
    HistoryNode *next_node;
    while (current!=NULL) {
        next_node=current->next;
        free(current);
        current=next_node;
    }
    rs->head=rs->tail=NULL;
    rs->count=0;
    printf("\nRecent Searches memory cleaned up.\n");
}