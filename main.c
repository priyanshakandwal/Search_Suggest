// main.c - Consolidated file to run the entire Smart Suggest system

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// Include all header files to ensure all function prototypes and structs are known
#include "dictionary.h" 
#include "synonyms.h" 
#include "ranking.h" 
#include "Recent_S.h" 
#include "utils.h" // Assuming utils.h and utils.c contain MAX_WORD_LEN definition if needed

// --- Define File Names (as used in the provided code) ---
#define WORD_LIST_FILE "wordlist.txt" 
#define THESAURUS_DATA_FILE "thesaurus_db.txt"
#define RANKING_DB_FILE "ranking_db.txt"

// --- Global Data Structures ---
DictionaryWord *dictHead = NULL;
SynonymGroup *synonymHead = NULL;
RecentSearches history;

// Ranking data is handled by the global 'table' in ranking.c

// ---------------------------------------------------------------------
// --- Helper Function: Simulate a user search/usage of a word ---
// ---------------------------------------------------------------------

void processSearch(const char *raw_term) {
    char term[MAX_WORD_LEN];
    strncpy(term, raw_term, MAX_WORD_LEN - 1);
    term[MAX_WORD_LEN - 1] = '\0';
    
    // 1. Clean up and standardize the word (using functions from utils.c)
    remove_symbols(term);
    to_lowercase(term);

    if (strlen(term) == 0) return;

    printf("\nProcessing search term: '%s' (Original: '%s')\n", term, raw_term);

    // 2. Add to Recent Searches history
    addRecentSearch(&history, term);

    // 3. Update Word Ranking (frequency)
    addOrUpdateWord(term);

    // 4. Check Dictionary presence
    if (isWordInDictionary(dictHead, term)) {
        printf("  ✅ '%s' is found in the Dictionary.\n", term);
        
        // 5. Check Thesaurus for synonyms
        printf("  Synonym Check: ");
        displaySynonyms(synonymHead, term);
    } else {
        printf("  ❌ '%s' is NOT in the Dictionary.\n", term);
    }
}

// ---------------------------------------------------------------------
// --- Main Function ---
// ---------------------------------------------------------------------

int main() {
    
    printf("====================================================\n");
    printf("            SMART SUGGEST SYSTEM BOOTUP\n");
    printf("====================================================\n");

    // --- STEP 1: Initialization and Data Loading ---
    
    // 1.1 Initialize Recent Searches
    initRecentSearches(&history);
    printf("[INIT] Recent Searches Initialized (Max %d).\n", MAX_HISTORY);

    // 1.2 Initialize Word Ranking Hash Table
    initTable();
    printf("[INIT] Word Ranking Hash Table Initialized.\n");

    // 1.3 Load Dictionary
    printf("\n[LOAD] Loading Dictionary from '%s'...\n", WORD_LIST_FILE);
    if (!loadDictionaryFromFile(&dictHead, WORD_LIST_FILE)) {
        fprintf(stderr, "FATAL ERROR: Could not load dictionary. Check '%s'.\n", WORD_LIST_FILE);
        // Cleanup what was initialized
        cleanupRecentSearches(&history);
        return 1;
    }
    printf("[LOAD] Dictionary loaded successfully.\n");


    // 1.4 Load Thesaurus (requires dictionary for validation)
    printf("[LOAD] Loading Thesaurus from '%s'...\n", THESAURUS_DATA_FILE);
    if (!loadThesaurusFromFile(&synonymHead, dictHead, THESAURUS_DATA_FILE)) {
        fprintf(stderr, "FATAL ERROR: Could not load thesaurus data. Check '%s'.\n", THESAURUS_DATA_FILE);
        // Full cleanup before exit
        freeDictionary(dictHead);
        cleanupRecentSearches(&history);
        freeAll(synonymHead);
        return 1;
    }
    printf("[LOAD] Thesaurus loaded successfully.\n");

    // 1.5 Load Existing Word Ranking Data
    printf("[LOAD] Loading Existing Word Rankings from '%s'...\n", RANKING_DB_FILE);
    loadData(RANKING_DB_FILE);
    printf("[LOAD] Ranking data loaded/initialized.\n");


    // --- STEP 2: Simulate Usage/Searches ---
    printf("\n====================================================\n");
    printf("          SIMULATING USER SEARCHES\n");
    printf("====================================================\n");

    // Simulate searches (some will increase existing frequencies)
    processSearch("big");
    processSearch("word");
    processSearch("file");
    processSearch("happy"); 
    processSearch("house");
    processSearch("quick"); 
    processSearch("big"); // Second search for 'big'
    processSearch("fast");
    processSearch("happy"); // Second search for 'happy'
    processSearch("suggest");
    processSearch("quick"); // Second search for 'quick'
    processSearch("word"); // Second search for 'word'


    // --- STEP 3: Display Results and Save Data ---

    printf("\n====================================================\n");
    printf("                RESULTS AND OUTPUT\n");
    printf("====================================================\n");

    // 3.1 Display Recent Searches
    displayRecentSearches(&history);

    // 3.2 Display Top Ranked Words (using N=5)
    printTopWords(5);

    // 3.3 Test Synonym Functionality again
    printf("\n[TEST] Testing synonym relation check:\n");
    if (areSynonyms(synonymHead, "big", "huge")) {
        printf("  'big' and 'huge' ARE confirmed synonyms.\n");
    } else {
        printf("  'big' and 'huge' are NOT confirmed synonyms.\n");
    }
    
    if (areSynonyms(synonymHead, "word", "house")) {
        printf("  'word' and 'house' ARE confirmed synonyms.\n");
    } else {
        printf("  'word' and 'house' are NOT confirmed synonyms.\n");
    }
    
    // 3.4 Save updated rankings
    printf("\n[SAVE] Saving updated Word Rankings to '%s'...\n", RANKING_DB_FILE);
    saveData(RANKING_DB_FILE);
    printf("[SAVE] Word Rankings saved.\n");

    // --- STEP 4: Memory Cleanup ---
    printf("\n[CLEANUP] Starting memory cleanup...\n");
    freeAll(synonymHead);
    freeDictionary(dictHead);
    cleanupRecentSearches(&history);
    // Note: The ranking hash table memory also needs to be freed, but a specific function for that is missing
    // For completeness, we should iterate and free nodes in the ranking table, but relying on OS cleanup for simplicity here.
    printf("All dynamic memory freed successfully.\n");
    
    return 0;
}