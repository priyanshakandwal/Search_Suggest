#ifndef RECENT_SEARCHES_H
#define RECENT_SEARCHES_H

#include <stdbool.h>

#define MAX_WORD_LEN 50
#define MAX_HISTORY 5

typedef struct HistoryNode {
    char search_term[MAX_WORD_LEN];
    struct HistoryNode *prev;
    struct HistoryNode *next;
} HistoryNode;

typedef struct RecentSearches {
    HistoryNode *head;
    HistoryNode *tail;
    int count;
} RecentSearches;

void initRecentSearches(RecentSearches *rs);
HistoryNode *createNewHistoryNode(const char *term);
void addRecentSearch(RecentSearches *rs, const char *term);
void displayRecentSearches(RecentSearches *rs);
void cleanupRecentSearches(RecentSearches *rs);

#endif
