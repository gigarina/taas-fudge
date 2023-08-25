/* ============================================================================================================== */
/* == BEGIN FILE ================================================================================================ */
/* ============================================================================================================== */
/*
============================================================================
Name : task_dc-adm.cpp
Author : Carina Benzin
Version : 1.0
Copyright : GPL3
Description : solve function for DC-ADM
============================================================================
*/
#include <iostream>
#include <vector>
#include <glib.h>

#include "../adm_util/adm_basic_util.cpp"

bool allElementsTried(struct AAF* aaf, struct TriedArguments* tried){
    return aaf->number_of_arguments == tried->numOfTriedArgs;
}

void triedC(struct TriedArguments* tried, struct TempExclude* tempExcl, int c){
    adm__triedC_set(tried, c);
    adm__tempExcludeC_set(tempExcl, c);
}

/**
 * @brief Randomly selects an attacker of in(L) that in(L) doesn't defend itself against.
 * @param aaf
 * @param labeling
 * @param defended A structure that holds the information about the defence state of in(L)
 * @return An argument that attacks in(L) and is not attacked by in(L). If there is no such argument return -1.
*/
/*
int findBFirst(struct AAF *aaf, struct Labeling *labeling, struct DefendedAgainst* defended){
    //printf("findBFirst\n");
    GSList* allAttackers = getAllAttackersOfLIN(aaf, labeling);
    GSList* undefendedAttackers = NULL;
    // saves all arguments in undefended Attackers that are not defended against
    for (GSList *current = allAttackers; current != NULL; current = current->next){ // make new function for this on defended bitset which is like get all x labeled args
        int currentI = *((int*) current->data);
         int isCurrentDefended = adm__defended_get(defended, currentI);
        if(!isCurrentDefended){ // if it's not out it was not defended against yet
            undefendedAttackers = g_slist_prepend(undefendedAttackers, GINT_TO_POINTER(new int(currentI)));
        }
    }
    g_slist_free(allAttackers);
    if(undefendedAttackers != NULL){
        int b = getRandomArgument(undefendedAttackers);
        g_slist_free_full(undefendedAttackers, deletePtr);
        return b;
    }
    return -1; 
}*/

int getRandomArgumentVector(const std::vector<int>& v){
    if(!v.empty()){
        int listLength = v.size();
        int randomIndex = getRandomIndex(listLength);
        return v[randomIndex];
    }
    return -1;
}

int findBFirst(struct AAF* aaf, struct Labeling* labeling, struct DefendedAgainst* defended, struct TriedArguments* triedArgs, struct TempExclude* tempExcl) {
    std::vector<int> undefendedAttackers;
    std::vector<int> prioritizedUndefendedAttackers;
    //undefendedAttackers.reserve(g_slist_length(allAttackers));
    for (int idx = bitset__next_set_bit(defended->attacks, 0); idx != -1; idx = bitset__next_set_bit(defended->attacks, idx + 1)) {
        if(adm__defended_get(defended, idx) == NO && adm__triedC_get(triedArgs, idx) == NO && adm__tempExcludeB_get(tempExcl, idx) == NO){
            prioritizedUndefendedAttackers.push_back(idx);
        }
        else if(adm__defended_get(defended, idx) == NO && adm__tempExcludeB_get(tempExcl, idx) == NO){
            undefendedAttackers.push_back(idx);
        }
    }
    if (!prioritizedUndefendedAttackers.empty()) {
        int b = getRandomArgumentVector(prioritizedUndefendedAttackers);
        return b;
    }
    else if (!undefendedAttackers.empty()) {
        int b = getRandomArgumentVector(undefendedAttackers);
        return b;
    }

    return -1;
}

// int findBFirst(struct AAF* aaf, struct Labeling* labeling, struct DefendedAgainst* defended) {
//     // GSList* allAttackers = getAllAttackersOfLIN(aaf, labeling);
//     GSList* allAttackers = getAllAttackersOfLIN(defended);
//     std::vector<int> undefendedAttackers;
//     undefendedAttackers.reserve(g_slist_length(allAttackers));

//     for (GSList *current = allAttackers; current != NULL; current = current->next){
//         int currentI = *((int*) current->data);
//         int isCurrentDefended = adm__defended_get(defended, currentI);
//         if (!isCurrentDefended) {
//             undefendedAttackers.push_back(currentI);
//         }
//     }
//     g_slist_free_full(allAttackers, deletePtr);
//     if (!undefendedAttackers.empty()) {
//         int b = getRandomArgumentVector(undefendedAttackers);
//         return b;
//     }

//     return -1;
// }




/**
 * @brief Picks and returns a random argument from the set of attackers of b that are not in conflict with in(L).
 * @param aaf
 * @param labeling
 * @param b The argument of which an attacker shall be picked.
 * @return An argument that attacks b and is not in conflict with in(L). If there is no such argument returns -1.
*/
// int findCFirst(struct AAF *aaf, struct Labeling *labeling, int b){
//     //printf("findCFirst\n");
//     if(b != -1){
//           GSList* bAttackers = g_slist_copy(aaf->parents[b]);
//           if(bAttackers != NULL){
//             GSList* conflictFreeAttackers = NULL; // new function
//             for (GSList *current = bAttackers; current != NULL; current = current->next){
//                 int* currentPtr = (int*) current->data;
//                 int currentI = *currentPtr;
//                 int currentLabel = taas__lab_get_label(labeling, currentI);
//                 if(currentLabel == LAB_UNDEC){ // if it's not out it is not in conflict with in(L)
//                     conflictFreeAttackers = g_slist_prepend(conflictFreeAttackers, currentPtr);
//                 }
//             }
//             if(conflictFreeAttackers != NULL){
//                 int c = getRandomArgument(conflictFreeAttackers);
//                 g_slist_free(conflictFreeAttackers);
//                 g_slist_free(bAttackers);
//                 return c;
//             }
//             g_slist_free(bAttackers);
//           }
          
//     }
//     return -1;
// }

bool isCSelfAttacking(struct AAF* aaf, struct TriedArguments* tried, struct TempExclude* tempExcl, int c){
    bool isSelfAttacking = (bool) bitset__get(aaf->loops, c);
    if(isSelfAttacking){
        adm__triedC_set(tried, c);    
        adm__tempExcludeC_set(tempExcl, c);   
    }
    return isSelfAttacking;
}

int findCFirst(struct AAF *aaf, struct Labeling *labeling, int b, struct TriedArguments* tried, struct TempExclude* tempExcl){
    //printf("findCFirst\n");
    if(b != -1){
          GSList* bAttackers = aaf->parents[b];
          if(bAttackers != NULL){
            std::vector<int> conflictFreeAttackers;
            std::vector<int> prioritizedConflictFreeAttackers;
            conflictFreeAttackers.reserve(g_slist_length(bAttackers));
             printf("C Candidates: ");
            for (GSList *current = bAttackers; current != NULL; current = current->next){
                int* currentPtr = (int*) current->data;
                int currentI = *currentPtr;
                int currentLabel = taas__lab_get_label(labeling, currentI);
                int selfAttacking = isCSelfAttacking(aaf, tried, tempExcl, currentI);
                bool alreadyTried = adm__triedC_get(tried, currentI);
                if(currentLabel == LAB_UNDEC && !selfAttacking && !alreadyTried && adm__tempExcludeC_get(tempExcl, currentI)== NO){ // if it's not out it is not in conflict with in(L)
                    printf("%d, ", currentI);
                    prioritizedConflictFreeAttackers.push_back(currentI);
                }else if(currentLabel == LAB_UNDEC && !selfAttacking && adm__tempExcludeC_get(tempExcl, currentI)== NO){ // if it's not out it is not in conflict with in(L)
                    printf("%d, ", currentI);
                    conflictFreeAttackers.push_back(currentI);
                }
            }
             printf("\n ");
            if(!prioritizedConflictFreeAttackers.empty()){
                int c = getRandomArgumentVector(prioritizedConflictFreeAttackers);
                return c;
            }else if(!conflictFreeAttackers.empty()){
                int c = getRandomArgumentVector(conflictFreeAttackers);
                return c;
            }
          }
          
    }
    return -1;
}


/**
 * @brief One of two conditions for an admissible labeling. 
 * Checks if all attackers of in(L) are labelled out.
 * @param aaf
 * @param labeling The labeling the condition shall be checked for
 * @return True if all attackers of in(L) are labelled out. False otherwise.
*/
bool allInAttackersAreOut(struct AAF *aaf, struct Labeling *labeling, struct DefendedAgainst* defended){
    //printf("allInAttackersAreOut\n");
    GSList* allInAttackers = getAllAttackersOfLIN(defended);
    for(GSList* curr = allInAttackers; curr != NULL; curr = curr->next){
        int currentI = *((int*)curr->data);
        int currLabel = taas__lab_get_label(labeling, currentI);
        if(currLabel != LAB_OUT){
            return false;
        }
    }
    g_slist_free_full(allInAttackers, deletePtr);
    
    return true; 
}


/**
 * @brief One of two conditions for an admissible labeling.
 * iterates through all OUT labeled arguments and
 *  checks if all of them have at least one IN labeled attacker.
 * @param aaf The aaf
 * @param labeling The labeling the condition shall be checked for
 * @return True if all out labeled arguments have at least one attacker. False otherwise.
*/
bool allOutHaveOneInAttacker(struct AAF *aaf, struct Labeling *labeling){
    //printf("allOutHaveOneInAttacker\n");
    GSList* outLabeled = getAllOutLabeledArgs(labeling);
    
    // iterate through all OUT labeled arguments and 
    // check if all of them have at least one IN labeled attacker
    for(GSList* currOut = outLabeled; currOut != NULL; currOut = currOut->next){
        int currentOut = *((int*)currOut->data);
        bool foundOneIn = false; 
        GSList* attackers = g_slist_copy(aaf->parents[currentOut]);
        for(GSList* currAtt = attackers; currAtt != NULL; currAtt = currAtt->next){
            int currentAttacker = *((int*)currAtt->data);
            if(taas__lab_get_label(labeling, currentAttacker) == LAB_IN){
                foundOneIn = true;
                break;
            }
        }
        if(!foundOneIn){
            g_slist_free_full(outLabeled, deletePtr);
            return false;
        }
    }
    g_slist_free_full(outLabeled, deletePtr);
    return true;
}

/**
 * @brief Decides if a given Labeling of an aaf is admissible or not.
 * @param aaf The aaf
 * @param labeling The labeling admissibility shall be checked for
 * @return True if labeling is admissible. False otherwise. 
*/
bool isAdmissibleLabeling(struct AAF *aaf, struct Labeling *labeling, struct DefendedAgainst* defended){
    //printf("isAdmissibleLabeling\n");
    return allInAttackersAreOut(aaf, labeling, defended) && allOutHaveOneInAttacker(aaf, labeling);
}


/**
 * @brief Labels the given argument in and all the arguments it is in conflict with as out.
 * Saves the arguments in(L) defends itself against.
 * @param aaf The aaf 
 * @param labeling The Labeling of the aaf that needs to be updated
 * @param defended The DefendedAgainst struct of the aaf that needs to be updated
 * @param argument The argument that will be labeled in
*/
void labelIn(struct AAF *aaf, struct Labeling *labeling, struct DefendedAgainst* defended, struct TriedArguments* triedArgs, struct TempExclude* tempExcl, int argument){
    // If self attacking argument shall be labeled in -> do nothing
    if(!isCSelfAttacking(aaf, triedArgs, tempExcl, argument)){ 
        //set argument label to IN
        taas__lab_set_label(labeling, argument, LAB_IN);
        // if it is labeled in, we tried it
        triedC(triedArgs, tempExcl, argument);
        //set label of all attackers of argument to OUT
        GSList* attackers = aaf->parents[argument];
        for(GSList* curr = attackers; curr != NULL; curr = curr->next){
            int currentI = *((int*)curr->data);
            taas__lab_set_label(labeling, currentI, LAB_OUT);
            adm__attacks_set(defended, currentI, YES);
            adm__tempExcludeC_set(tempExcl, currentI);

        }
        //set label of all children of argument to OUT and 
        // save the children as defended against by in(L)
        GSList* victims = aaf->children[argument];
        for(GSList* curr = victims; curr != NULL; curr = curr->next){
            int currentI = *((int*)curr->data);
            taas__lab_set_label(labeling, currentI, LAB_OUT);
            adm__defended_set(defended, currentI, YES);
            adm__tempExcludeC_set(tempExcl, currentI);

        }
    }
}

void labelCIn(struct AAF *aaf, struct Labeling *labeling, struct DefendedAgainst* defended, struct TriedArguments* triedArgs, struct TempExclude* tempExcl, int b, int c){
    labelIn(aaf, labeling, defended, triedArgs, tempExcl, c);
    adm__set_triedB_if_necessary(triedArgs, tempExcl, aaf, b);
}

/**
 * @brief Creates and initializes a Labeling struct for the given aaf
 * @param aaf The AAF that the Labeling struct shall be created for
 * @return A pointer to the created Labeling struct.
*/
struct Labeling* createLabelingForAAF(struct AAF* aaf){
    //printf("createLabelingForAAF\n");
    // initialize labeling
    struct Labeling *labeling;
    labeling = (struct Labeling *)malloc(sizeof(struct Labeling));
    taas__lab_init(labeling, FALSE); 
    // set up bitset for in labeled arguments
    bitset__init(labeling->in, aaf->number_of_arguments);
    bitset__unsetAll(labeling->in);
    // set up  bitset for out labeled arguments
    bitset__init(labeling->out, aaf->number_of_arguments);
    bitset__unsetAll(labeling->out);

    return labeling;
}

/**
 * @brief Creates and initializes a DefendedAgainst struct for the given aaf
 * @param aaf The AAF that the DefendedAgainst struct shall be created for
 * @return A pointer to the created DefendedAgainst struct.
*/
struct DefendedAgainst* createDefendedForAAF(struct AAF* aaf){
    //printf("createDefendedForAAF\n");
    struct DefendedAgainst *defended;
    defended = (struct DefendedAgainst *)malloc(sizeof(struct DefendedAgainst));
    adm__defended_init(defended);
    bitset__init(defended->yes, aaf->number_of_arguments);
    bitset__unsetAll(defended->yes);
    bitset__init(defended->attacks, aaf->number_of_arguments);
    bitset__unsetAll(defended->attacks);

    return defended;
}
/**
 * @brief Creates and initializes a DefendedAgainst struct for the given aaf
 * @param aaf The AAF that the DefendedAgainst struct shall be created for
 * @return A pointer to the created DefendedAgainst struct.
*/
struct TempExclude* createTempExcludeForAAF(struct AAF* aaf, struct TriedArguments* tried){
    //printf("createDefendedForAAF\n");
    struct TempExclude *tempExcl;
    tempExcl = (struct TempExclude *)malloc(sizeof(struct TempExclude));
    adm__tempExcl_init(tempExcl);
    bitset__init(tempExcl->tempExcludeB, aaf->number_of_arguments);
    bitset__unsetAll(tempExcl->tempExcludeB);
    bitset__init(tempExcl->tempExcludeC, aaf->number_of_arguments);
    bitset__unsetAll(tempExcl->tempExcludeC);

    return tempExcl;
}

struct TriedArguments* createTriedArguments(struct AAF* aaf){
    struct TriedArguments* tried;
     tried = (struct TriedArguments *)malloc(sizeof(struct TriedArguments));
    adm__tried_init(tried);
    bitset__init(tried->triedBs, aaf->number_of_arguments);
    bitset__unsetAll(tried->triedBs);
    bitset__init(tried->triedCs, aaf->number_of_arguments);
    bitset__unsetAll(tried->triedCs);
    return tried;
}


/**
 * @brief Solves the credulous acceptance problem regarding admissible sets.
 * @param task Holds all information about the Task to perform, only needed to extract the queried argument.
 * @param aaf The aaf credulous acceptance shall be decided for
 * @param do_print if set to true prints a witness if true is returned
 * @return True if there is an admissible set in the aaf that contains the queried argument. False otherwise.
*/
bool solve_dcadm(struct TaskSpecification *task, struct AAF *aaf, bool do_print = true)
{
    //printf("solve_dcadm\n");
    // Element a
    int a = task->arg;

    // if one attacker of a has no attackers, a can't be credulously accepted. 
    GSList* aAttackers = aaf->parents[a];
    for(GSList* currAtt = aAttackers; currAtt != NULL; currAtt = currAtt->next){
        int i = *((int*)currAtt->data);
        if(aaf->parents[i] == NULL){
            printf("NO\n");
            printf("Attacker %d of %d has no Attackers\n", i, a);
            return false;
        }
    }

    // check if queried argument is selfAttacking.
    int selfAttacking = bitset__get(aaf->loops, a);
    if(selfAttacking){
         printf("NO\n");
         printf("queried Argument is self Attacking\n");
        return false;
    }
    
    struct TriedArguments* triedArgs = createTriedArguments(aaf);
    int MAX_IT = 1000 * aaf->number_of_arguments;
    for (int i = 0; i < MAX_IT; i++){  
        printf("=======================ROUND %d============================\n", i);

        // create new labeling 
        struct Labeling* labeling = createLabelingForAAF(aaf);  
        // setup Defended Bitset:
        struct DefendedAgainst* defended = createDefendedForAAF(aaf);

        struct TempExclude* tempExcl = createTempExcludeForAAF(aaf, triedArgs);
        // L(a) <- in
        labelIn(aaf, labeling, defended, triedArgs, tempExcl, a);

        while (!adm__isAdmissible(defended, labeling))
        {   
            if(allElementsTried(aaf, triedArgs)){
                adm__tried_reset(triedArgs);
            }
            // b is attacker of in(L) that in(L) doesn't attack
            int b = findBFirst(aaf, labeling, defended, triedArgs, tempExcl);
            printf("B: %d\n", b);
            // if there is one attacker in(L) can't defend itself against, 
            // argument a can't be credulously accepted
             printf("tried Cs: %s\n", adm__triedC_print(triedArgs, aaf));
            printf("tried Bs: %s\n", adm__triedB_print(triedArgs, aaf));
            if (b ==-1  || aaf->parents[b] == NULL){
                printf("No B found or b has no attackers\n");
                break;
            }
            // c is attacker of b, c is not in conflict with in(L)
            int c = findCFirst(aaf, labeling, b, triedArgs, tempExcl); 
            
           printf("C: %d\n", c);
            if (c == -1){ // no c found
                printf("No c found\n");
                adm__tempExcludeB_set(tempExcl, b);
                break; // start a new try
            }else{
                // add c to in(L)
                labelCIn(aaf, labeling, defended, triedArgs, tempExcl, b, c);
            }  
           
        }

        if (adm__isAdmissible(defended, labeling))
        {
            printf("YES\n");
            if(do_print){
             printf("%s\n", taas__lab_print_as_labeling(labeling, aaf)); 
            }
            // freeing the allocated memory
            taas__lab_destroy(labeling);
            adm__defended_destroy(defended);
            adm__tried_destroy(triedArgs);
            // the labeling is admissible, returns true
            
            return true;
        }
         printf("Labeling:  %s\n", taas__lab_print_as_labeling(labeling, aaf));
        // freeing the allocated memory
        taas__lab_destroy(labeling);
        adm__defended_destroy(defended);
        adm__tempExcl_destroy(tempExcl);
       
    }
    adm__tried_destroy(triedArgs);
    // returns false if all tries were taken
     printf("NO\n");
     printf("All tries used up\n");
    return false;
}

/* ============================================================================================================== */
/* == END FILE ================================================================================================== */
/* ============================================================================================================== */
