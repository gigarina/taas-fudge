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
 * @param labeling
 * @param inLRel A structure that holds the information about the defence state of in(L)
 * @return An argument that attacks in(L) and is not attacked by in(L). If there is no such argument return -1.
*/

int findBFirst(struct AAF* aaf, struct Labeling* labeling, struct InLRelation* inLRel, struct TempExclude* tempExcl) {
    std::vector<int> undefendedAttackers;
    //undefendedAttackers.reserve(g_slist_length(allAttackers));
    for (int idx = bitset__next_set_bit(inLRel->attacks, 0); idx != -1; idx = bitset__next_set_bit(inLRel->attacks, idx + 1)) {
       if(adm__inLRel_defended_get(inLRel, idx) == NO && adm__tempExcludeB_get(tempExcl, idx) == NO){
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

int findCFirst(struct AAF *aaf, struct Labeling *labeling, int b, struct TempExclude* tempExcl, GHashTable* undefendedChildren){
    if(b != -1){
          GSList* bAttackers = aaf->parents[b];
          if(bAttackers != NULL){
            std::vector<int> conflictFreeAttackers;
            conflictFreeAttackers.reserve(g_slist_length(bAttackers));
            int maxUndefendedChildren = 0;
            for (GSList *current = bAttackers; current != NULL; current = current->next){
                int currentI = *((int*) current->data);
                int currentLabel = taas__lab_get_label(labeling, currentI);
                int selfAttacking = isCSelfAttacking(aaf, tempExcl, currentI);
                bool alreadyTried = adm__tempExcludeC_get(tempExcl, currentI); // TODO delete is obsolete.
                if(currentLabel == LAB_UNDEC && !selfAttacking && adm__tempExcludeC_get(tempExcl, currentI)== NO){ // if it's not out it is not in conflict with in(L)
                    int numUndefendedChildren = *((int*) g_hash_table_lookup(undefendedChildren, current->data));
                    if(numUndefendedChildren > maxUndefendedChildren){
                        maxUndefendedChildren = numUndefendedChildren;
                        conflictFreeAttackers.clear();
                    }
                    if(numUndefendedChildren == maxUndefendedChildren){
                        conflictFreeAttackers.push_back(currentI);
                    }
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

void print(gpointer data, gpointer user_data){
    printf("%d, ", *((int*)data));
}

void printHashTableValues(GHashTable* undefendedChildren){
     GPtrArray* ks = g_hash_table_get_keys_as_ptr_array(undefendedChildren);
     GPtrArray* vs = g_hash_table_get_values_as_ptr_array(undefendedChildren);

     printf("K: ");
     g_ptr_array_foreach(ks, print, NULL);
    printf("\n");
    printf("V: ");
    g_ptr_array_foreach(vs, print, NULL);
    printf("\n\n");
}


/**
 * @brief Labels the given argument in and all the arguments it is in conflict with as out.
 * Saves the arguments in(L) defends itself against.
 * @param aaf The aaf 
 * @param labeling The Labeling of the aaf that needs to be updated
 * @param inLRel The InLRelation struct of the aaf that needs to be updated
 * @param tempExcl The TempExclude struct of the aaf that needs to be updated.
 * @param argument The argument that will be labeled in
*/
void labelIn(struct AAF *aaf, struct Labeling *labeling, struct InLRelation* inLRel, struct TempExclude* tempExcl, int argument, GHashTable* undefendedChildren){
    // If self attacking argument shall be labeled in -> do nothing
    if(!isCSelfAttacking(aaf, tempExcl, argument)){ 
        //set argument label to IN
        taas__lab_set_label(labeling, argument, LAB_IN);
        // if it is labeled in, we tried it
        adm__tempExcludeC_set(tempExcl, argument);
        //set label of all attackers of argument to OUT
        GSList* attackers = aaf->parents[argument];
        for(GSList* curr = attackers; curr != NULL; curr = curr->next){
            int currentI = *((int*)curr->data);
            taas__lab_set_label(labeling, currentI, LAB_OUT);
            adm__inLRel_attacks_set(inLRel, currentI, YES);
            GSList* currParents = aaf->parents[currentI];
            for(GSList* currP = currParents; currP != NULL; currP = currP->next){
                int* idx = (int*)currP->data;
                int currentVal = *((int*)g_hash_table_lookup(undefendedChildren, idx));
                g_hash_table_replace(undefendedChildren, idx, GINT_TO_POINTER(new int(++currentVal)));
            }
            adm__tempExcludeC_set(tempExcl, currentI);

        }
        //set label of all children of argument to OUT and 
        // save the children as inLRel against by in(L)
        GSList* victims = aaf->children[argument];
        for(GSList* curr = victims; curr != NULL; curr = curr->next){
            int currentI = *((int*)curr->data);
            taas__lab_set_label(labeling, currentI, LAB_OUT);
            adm__inLRel_defended_set(inLRel, currentI, YES);
            GSList* currParents = aaf->parents[currentI];
            for(GSList* currP = currParents; currP != NULL; currP = currP->next){
                int* idx = (int*)currP->data;
                int currentVal = *((int*)g_hash_table_lookup(undefendedChildren, idx));
                g_hash_table_replace(undefendedChildren, idx, GINT_TO_POINTER(new int(--currentVal)));
            }
            adm__tempExcludeC_set(tempExcl, currentI);

        }
        //printHashTableValues(undefendedChildren);
    }
}

/**
 * @brief Labels the given argument in and all the arguments it is in conflict with as out.
 * Saves the arguments in(L) defends itself against.
 * @param aaf The aaf 
 * @param labeling The Labeling of the aaf that needs to be updated
 * @param inLRel The InLRelation struct of the aaf that needs to be updated
 * @param tempExcl The TempExclude struct of the aaf that needs to be updated.
 * @param c The argument that will be labeled in
 * @param b The b that c was calculated with, will temporarily excluded if necessary.
*/
void labelCIn(struct AAF *aaf, struct Labeling *labeling, struct InLRelation* inLRel, struct TempExclude* tempExcl, int c, int b, GHashTable* undefendedChildren){
    labelIn(aaf, labeling, inLRel, tempExcl, c, undefendedChildren);
    adm__set_triedB_if_necessary(aaf, tempExcl, b); //TODO useless, this B is defended now
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

    // if one attacker of a has no attackers, a can't be credulously accepted. 
    GSList* aAttackers = aaf->parents[a];
    for(GSList* currAtt = aAttackers; currAtt != NULL; currAtt = currAtt->next){
        int i = *((int*)currAtt->data);
        if(aaf->parents[i] == NULL){
            printf("NO\n");
            return false;
        }
    }

    // check if queried argument is selfAttacking.
    int selfAttacking = bitset__get(aaf->loops, a);
    if(selfAttacking){
         printf("NO\n");
        return false;
    }

    // seed random number generator with current time
    // to get different random number sequences for the same vLengths
    srand(static_cast<unsigned int>(time(nullptr)));
    
    int MAX_IT = 1000 * aaf->number_of_arguments;
    for (int i = 0; i < MAX_IT; i++){  
        // Setup all structures needed for deciding DC-ADM
        struct Labeling* labeling = createLabelingForAAF(aaf);  
        struct InLRelation* inLRel = createInLRelForAAF(aaf);
        struct TempExclude* tempExcl = createTempExcludeForAAF(aaf);
        GHashTable* undefendedChildren = (GHashTable*) g_hash_table_new_full(g_int_hash, g_int_equal, NULL, deletePtr);
    
        for(int i=0; i< aaf->number_of_arguments; i++){
            GSList* children = aaf->children[i];
            g_hash_table_insert(undefendedChildren, GINT_TO_POINTER(new int(i)), GINT_TO_POINTER(new int(0)));
        }
        
        labelIn(aaf, labeling, inLRel, tempExcl, a, undefendedChildren);

        while (!adm__isAdmissible(inLRel, labeling))
        {   
            // b is attacker of in(L) that in(L) doesn't attack
            int b = findBFirst(aaf, labeling, inLRel, tempExcl);
            
            // printf("B: %d\n", b);
            // c is attacker of b, c is not in conflict with in(L)
            int c = findCFirst(aaf, labeling, b, tempExcl, undefendedChildren); 
            
            if (c == -1){ // no c found
                if(b != -1){
                    adm__tempExcludeB_set(tempExcl, b); // TODO This is useless we set it and delete it again
                }
                break; // start a new try
            }else{
                // add c to in(L)
                labelCIn(aaf, labeling, inLRel, tempExcl, c, b, undefendedChildren);
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
            g_hash_table_destroy(undefendedChildren);
            // the labeling is admissible, returns true
            return true;
        }
        // freeing the allocated memory
        taas__lab_destroy(labeling);
        adm__inLRel_destroy(inLRel);
        adm__tempExcl_destroy(tempExcl);
        g_hash_table_destroy(undefendedChildren);
       
    }
    
    // returns false if all tries were taken
     printf("NO\n");
    return false;
}

/* ============================================================================================================== */
/* == END FILE ================================================================================================== */
/* ============================================================================================================== */
