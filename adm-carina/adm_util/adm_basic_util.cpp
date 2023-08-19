
/* ============================================================================================================== */
/* == BEGIN FILE ================================================================================================ */
/* ============================================================================================================== */
/*
============================================================================
Name : adm_basic_util.cpp
Author : Carina Benzin
Version : 1.0
Copyright : GPL3
Description : Utility functions needed for DC-ADM
============================================================================
*/

/**
 * @brief Iterates through a GSList, saves all attackers of the 
 * arguments in the list into a new GSList and returns it.
 * @param aaf
 * @param list The GSList the attacker list shall be created for.
 * @return A GSList of all the attackers of the arguments in the given list.
*/
GSList* getAllAttackersFromList(struct AAF *aaf, GSList* list){
    GSList* allAttackers = NULL;
    for(GSList* curr = list; curr != NULL; curr = curr->next){
        int currentI = *((int*)curr->data);
        // copy the attackers list, to not concat the original one
        GSList* attackers = g_slist_copy(aaf->parents[currentI]);
        if(allAttackers == NULL){ // this makes sure we don't concatenate a list with NULL
            allAttackers = attackers;
        }else{
            allAttackers = g_slist_concat(allAttackers, attackers);
        }
    }
    return allAttackers;
}

/**
 * @brief Returns a GSList of all arguments that are labeled in or out in the given bitset.
 * Lists created by this function must be freed with g_slist_free_full.
 * @param inOutBitSet Either the in or out BitSet of a labeling.
 * @return A GSList of the arguments of in(L) or out(L)
*/
GSList* getAllXLabeledArgs(struct BitSet* inOutBitSet){
    GSList* xLabeled = NULL;
    for (int idx = bitset__next_set_bit(inOutBitSet, 0); idx != -1; idx = bitset__next_set_bit(inOutBitSet, idx + 1)) {
        xLabeled = g_slist_prepend(xLabeled, GINT_TO_POINTER(new int(idx)));
    }
    return xLabeled;
}

/**
 * @brief Returns a GSList of all arguments that are labeled in in the given labeling.
 * Lists created by this function must be freed with g_slist_free_full.
 * @param labeling The labeling in(L) shall be returned from.
 * @return A GSList of the arguments of in(L)
*/
GSList* getAllInLabeledArgs(struct Labeling* labeling){
    return getAllXLabeledArgs(labeling->in);
}


/**
 * @brief Returns a GSList of all arguments that are labeled out in the given labeling.
 * Lists created by this function must be freed with g_slist_free_full.
 * @param labeling The labeling out(L) shall be returned from.
 * @return A GSList of the arguments of out(L)
*/
GSList* getAllOutLabeledArgs(struct Labeling* labeling){
    return getAllXLabeledArgs(labeling->out);
}

// 
/**
 * @brief Callback function for g_slist_free_full. 
 * This is neccessary if memory was allocated with new during the creation of the list.
 * @param data A pointer to the GSList that shall be fully freed
*/
void deletePtr(gpointer data){
    int* intPtr = (int*) data;
    delete intPtr;
}

/**
 * @brief Utility function to print a GSList
 * @param list The list to be printed
*/
void printGSList(GSList* list){
    if(list != NULL){
        for (GSList *current = list; current != NULL; current = current->next){
            int* currentIndex = (int*)current->data;
            //printf("%d -> ", *currentIndex);
        }
    }
    //printf("\n");
}



/**
 * @brief Returns a list of all attackers of in(L)
 * @param aaf
 * @param labeling // i don't think we need this
 * @return A GSList of all attackers of in(L)
*/
GSList* getAllAttackersOfLIN(struct AAF *aaf, struct Labeling *labeling){ 
    GSList* inL = getAllInLabeledArgs(labeling);
    GSList* allInAttackers = getAllAttackersFromList(aaf, inL);
    g_slist_free_full(inL, deletePtr);
    return allInAttackers;
}



/**
 * @brief Returns a random index of a list of the given length. Used to pick random arguments from lists.
 * @param listLength The length of the list
 * @return A random index of a list of the given length
*/
int getRandomIndex(int listLength){
    // seed random number generator with current time
    // to get different random number sequences for the same listLengths
    srand(static_cast<unsigned int>(time(nullptr)));
    return rand() % listLength;
}

/**
 * @brief Returns a random argument of the given list.
 * @param list The list one random argument shall be picked out of.
 * @return A random argument of the given list.
*/
int getRandomArgument(GSList* list){
    //printf("getRandomArgument!\n");
    if(list != NULL){
        int listLength = g_slist_length(list);
        //printf("List length: %d\n", listLength);
        int randomIndex = getRandomIndex(listLength);
        printf("randomIndex: %d for list length: %d\n", randomIndex, listLength);
        int randomArgument = *((int*)g_slist_nth_data(list, randomIndex));
        //printf("randomArgument: %d\n", randomArgument);
        return randomArgument;
    }
    return -1;
}


/* ============================================================================================================== */
/* == END FILE ================================================================================================== */
/* ============================================================================================================== */
