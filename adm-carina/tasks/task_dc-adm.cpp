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

const int MAX_IT = 15000000;

 
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

int findBFirst(struct AAF* aaf, struct Labeling* labeling, struct DefendedAgainst* defended) {
    GSList* allAttackers = getAllAttackersOfLIN(aaf, labeling);
    std::vector<int> undefendedAttackers;
    undefendedAttackers.reserve(g_slist_length(allAttackers));

    for (GSList *current = allAttackers; current != NULL; current = current->next){
        int currentI = *((int*) current->data);
        int isCurrentDefended = adm__defended_get(defended, currentI);
        if (!isCurrentDefended) {
            undefendedAttackers.push_back(currentI);
        }
    }

    if (!undefendedAttackers.empty()) {
        int b = getRandomArgumentVector(undefendedAttackers);
        return b;
    }

    return -1;
}




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
int findCFirst(struct AAF *aaf, struct Labeling *labeling, int b){
    //printf("findCFirst\n");
    if(b != -1){
          GSList* bAttackers = aaf->parents[b];
          if(bAttackers != NULL){
            std::vector<int> conflictFreeAttackers;
            conflictFreeAttackers.reserve(g_slist_length(bAttackers));
            for (GSList *current = bAttackers; current != NULL; current = current->next){
                int* currentPtr = (int*) current->data;
                int currentI = *currentPtr;
                int currentLabel = taas__lab_get_label(labeling, currentI);
                if(currentLabel == LAB_UNDEC){ // if it's not out it is not in conflict with in(L)
                    conflictFreeAttackers.push_back(currentI);
                }
            }
            if(!conflictFreeAttackers.empty()){
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
bool allInAttackersAreOut(struct AAF *aaf, struct Labeling *labeling){
    //printf("allInAttackersAreOut\n");
    GSList* allInAttackers = getAllAttackersOfLIN(aaf, labeling);
    for(GSList* curr = allInAttackers; curr != NULL; curr = curr->next){
        int currentI = *((int*)curr->data);
        int currLabel = taas__lab_get_label(labeling, currentI);
        if(currLabel != LAB_OUT){
            return false;
        }
    }
    g_slist_free(allInAttackers);
    
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
        GSList* attackers = aaf->parents[currentOut];
        for(GSList* currAtt = attackers; currAtt != NULL; currAtt = currAtt->next){
            int currentAttacker = *((int*)currAtt->data);
            if(taas__lab_get_label(labeling, currentAttacker) == LAB_IN){
                foundOneIn = true;
                break;
            }
        }
        g_slist_free_full(outLabeled, deletePtr);
        if(!foundOneIn){
            return false;
        }
    }
    return true;
}

/**
 * @brief Decides if a given Labeling of an aaf is admissible or not.
 * @param aaf The aaf
 * @param labeling The labeling admissibility shall be checked for
 * @return True if labeling is admissible. False otherwise. 
*/
bool isAdmissibleLabeling(struct AAF *aaf, struct Labeling *labeling){
    //printf("isAdmissibleLabeling\n");
    return allInAttackersAreOut(aaf, labeling) && allOutHaveOneInAttacker(aaf, labeling);
}

/**
 * @brief Labels the given argument in and all the arguments it is in conflict with as out.
 * Saves the arguments in(L) defends itself against.
 * @param aaf The aaf 
 * @param labeling The Labeling of the aaf that needs to be updated
 * @param defended The DefendedAgainst struct of the aaf that needs to be updated
 * @param argument The argument that will be labeled in
*/
void labelIn(struct AAF *aaf, struct Labeling *labeling, struct DefendedAgainst* defended, int argument){
    //printf("labelIn\n");
    //set argument label to IN
    taas__lab_set_label(labeling, argument, LAB_IN);
    //set label of all attackers of argument to OUT
    GSList* attackers = aaf->parents[argument];
    for(GSList* curr = attackers; curr != NULL; curr = curr->next){
        int currentI = *((int*)curr->data);
        taas__lab_set_label(labeling, currentI, LAB_OUT);

    }
    //set label of all children of argument to OUT and 
    // save the children as defended against by in(L)
    GSList* victims = aaf->children[argument];
    for(GSList* curr = victims; curr != NULL; curr = curr->next){
        int currentI = *((int*)curr->data);
        taas__lab_set_label(labeling, currentI, LAB_OUT);
        adm__defended_set(defended, currentI, YES);

    }
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

    return defended;
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
    
    for (int i = 0; i < MAX_IT; i++){  
   // for(;;){ // infinite MAX_IT, needs timeout to stop if not true
        // create new labeling 
        struct Labeling* labeling = createLabelingForAAF(aaf);  
        // setup Defended Bitset:
        struct DefendedAgainst* defended = createDefendedForAAF(aaf);
        // L(a) <- in
        labelIn(aaf, labeling, defended, a);
       
        while (!isAdmissibleLabeling(aaf, labeling))
        {   
            // b is attacker of in(L) that in(L) doesn't attack
            int b = findBFirst(aaf, labeling, defended);
            // c is attacker of b, c is not in conflict with in(L)
            int c = findCFirst(aaf, labeling, b);  

            if (c == -1){ // no c found
                break; // start a new try
            }else{
                // add c to in(L)
                labelIn(aaf, labeling, defended, c);
            }  
        }
        if (isAdmissibleLabeling(aaf, labeling))
        {
            printf("YES\n");
            if(do_print){
             //printf("%s\n", taas__lab_print_as_labeling(labeling, aaf)); 
            }
            // freeing the allocated memory
            taas__lab_destroy(labeling);
            adm__defended_destroy(defended);
            // the labeling is admissible, returns true
            
            return true;
        }
        // freeing the allocated memory
        taas__lab_destroy(labeling);
        adm__defended_destroy(defended);
    }
    // returns false if all tries were taken
     printf("NO\n");
    return false;
}

/* ============================================================================================================== */
/* == END FILE ================================================================================================== */
/* ============================================================================================================== */
