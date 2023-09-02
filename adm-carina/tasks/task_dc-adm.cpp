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



/**
 * @brief Randomly selects an attacker of in(L) that in(L) doesn't defend itself against.
 * @param aaf
 * @param inLRel A structure that holds the information about the defence state of in(L)
 * @return An argument that attacks in(L) and is not attacked by in(L). If there is no such argument return -1.
*/

int findBFirst(struct AAF* aaf, struct InLRelation* inLRel) {
    std::vector<int> undefendedAttackers;
    //undefendedAttackers.reserve(g_slist_length(allAttackers));
    for (int idx = bitset__next_set_bit(inLRel->attacks, 0); idx != -1; idx = bitset__next_set_bit(inLRel->attacks, idx + 1)) {
       if(adm__inLRel_defended_get(inLRel, idx) == NO){
            undefendedAttackers.push_back(idx);
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

int findCFirst(struct AAF *aaf, struct Labeling *labeling, int b){
    if(b != -1){
          GSList* bAttackers = aaf->parents[b];
          if(bAttackers != NULL){
            std::vector<int> conflictFreeAttackers;
            conflictFreeAttackers.reserve(g_slist_length(bAttackers));
            
            for (GSList *current = bAttackers; current != NULL; current = current->next){
                int currentI = *((int*) current->data);
                int currentLabel = taas__lab_get_label(labeling, currentI);
                int selfAttacking = isCSelfAttacking(aaf, currentI);
                if(currentLabel == LAB_UNDEC && !selfAttacking){ // if it's not out it is not in conflict with in(L)
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
 * @brief Labels the given argument in and all the arguments it is in conflict with as out.
 * Saves the arguments in(L) defends itself against.
 * @param aaf The aaf 
 * @param labeling The Labeling of the aaf that needs to be updated
 * @param inLRel The InLRelation struct of the aaf that needs to be updated
 * @param argument The argument that will be labeled in
*/
void labelIn(struct AAF *aaf, struct Labeling *labeling, struct InLRelation* inLRel, int argument){
    // If self attacking argument shall be labeled in -> do nothing
    if(!isCSelfAttacking(aaf, argument)){ 
        //set argument label to IN
        taas__lab_set_label(labeling, argument, LAB_IN);
        //set label of all attackers of argument to OUT
        GSList* attackers = aaf->parents[argument];
        for(GSList* curr = attackers; curr != NULL; curr = curr->next){
            int currentI = *((int*)curr->data);
            taas__lab_set_label(labeling, currentI, LAB_OUT);
            adm__inLRel_attacks_set(inLRel, currentI, YES);
        }
        //set label of all children of argument to OUT and 
        // save the children as inLRel against by in(L)
        GSList* victims = aaf->children[argument];
        for(GSList* curr = victims; curr != NULL; curr = curr->next){
            int currentI = *((int*)curr->data);
            taas__lab_set_label(labeling, currentI, LAB_OUT);
            adm__inLRel_defended_set(inLRel, currentI, YES);
        }
    }
}



/**
 * @brief Solves the credulous acceptance problem regarding admissible sets.
 * @param task Holds all information about the Task to perform, only needed to extract the queried argument.
 * @param aaf The aaf credulous acceptance shall be decided for
 * @param do_print if set to true prints a witness if true is returned
 * @return True if there is an admissible set in the aaf that contains the queried argument. False otherwise.
*/
bool solve_dcadm(struct TaskSpecification *task, struct AAF *aaf, bool do_print = false)
{
    // Element a
    int a = task->arg;
    
    int MAX_IT = 1000 * aaf->number_of_arguments;
    for (int i = 0; i < MAX_IT; i++){  
        // Setup all structures needed for deciding DC-ADM
        struct Labeling* labeling = createLabelingForAAF(aaf);  
        struct InLRelation* inLRel = createInLRelForAAF(aaf);
        
        labelIn(aaf, labeling, inLRel, a);

        while (!adm__isAdmissible(inLRel, labeling))
        {   
            // b is attacker of in(L) that in(L) doesn't attack
            int b = findBFirst(aaf, inLRel);

            // c is attacker of b, c is not in conflict with in(L)
            int c = findCFirst(aaf, labeling, b); 
            
            if (c == -1){ // no c found
                break; // start a new try
            }else{
                // add c to in(L)
                labelIn(aaf, labeling, inLRel, c);
            }  
        }

        if (adm__isAdmissible(inLRel, labeling))
        {
            printf("YES\n");
            if(do_print){
             printf("%s\n", taas__lab_print_as_labeling(labeling, aaf)); 
            }
            // freeing the allocated memory
            taas__lab_destroy(labeling);
            adm__inLRel_destroy(inLRel);

            // the labeling is admissible, returns true
            return true;
        }
        // freeing the allocated memory
        taas__lab_destroy(labeling);
        adm__inLRel_destroy(inLRel);
       
    }
    // returns false if all tries were taken
     printf("NO\n");
    return false;
}

/* ============================================================================================================== */
/* == END FILE ================================================================================================== */
/* ============================================================================================================== */
