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
#include <glib.h>

const int MAX_IT = 5;

// must be freed with g_slist_free_full
GSList* getAllXLabeledArgs(struct Labeling* labeling, struct BitSet* inOutBitSet){
    GSList* xLabeled = NULL;
    for (int idx = bitset__next_set_bit(inOutBitSet, 0); idx != -1; idx = bitset__next_set_bit(inOutBitSet, idx + 1)) {
        xLabeled = g_slist_prepend(xLabeled, GINT_TO_POINTER(new int(idx)));
    }
    return xLabeled;
}

// must be freed with g_slist_free_full
GSList* getAllInLabeledArgs(struct Labeling* labeling){
    return getAllXLabeledArgs(labeling, labeling->in);
}

// must be freed with g_slist_free_full
GSList* getAllOutLabeledArgs(struct Labeling* labeling){
    return getAllXLabeledArgs(labeling, labeling->out);
}

// Callback function for g_slist_free_full
void deletePtr(gpointer data){
    int* intPtr = (int*) data;
    delete intPtr;
}

void printGSList(GSList* list){
    if(list != NULL){
        for (GSList *current = list; current != NULL; current = current->next){
            int* currentIndex = (int*)current->data;
            //printf("%d -> ", *currentIndex);
        }
    }
    //printf("\n");
}


/* attackers = 1 --> gets attackers */
/* attackers = 0 --> gets victims */
GSList* getAllAttackersOrVictimsOfLIN(struct AAF *aaf, struct Labeling *labeling, bool attackers){
    // get all inLabeled Args:
    GSList* inLabeled = getAllInLabeledArgs(labeling);

    GSList* allAttackers = NULL;
     for(GSList* curr = inLabeled; curr != NULL; curr = curr->next){
        int i = *((int*) curr->data);
        GSList* attOrVictim = attackers ? aaf->parents[i] : aaf->children[i];
        GSList* attackers = g_slist_copy(attOrVictim);
        if(allAttackers == NULL){
            allAttackers = attackers;
            // //printf("NOW PRINTING ALL ATTACKERS!\n");
            //printGSList(allAttackers);
        }else{
            // //printf("NOW PRINTING ALL ATTACKERS!\n");
            //printGSList(allAttackers);
            allAttackers = g_slist_concat(allAttackers, attackers);
        }
     }
     g_slist_free_full(inLabeled, deletePtr);
     return allAttackers;
}

GSList* getAllAttackersOfLIN(struct AAF *aaf, struct Labeling *labeling){
    return getAllAttackersOrVictimsOfLIN(aaf,labeling, true);
}

GSList* getAllVictimsOfLIN(struct AAF *aaf, struct Labeling *labeling){
    return getAllAttackersOrVictimsOfLIN(aaf,labeling, false);
}

GSList* getAllConflictFreeArgsForLIN(struct AAF *aaf, struct Labeling *labeling){
     //printf("getAllConflictFreeArgsForLIN \n");
    GSList* allAttackers = getAllAttackersOfLIN(aaf, labeling);
    GSList* allVictims = getAllVictimsOfLIN(aaf, labeling);
    GSList* allConflictingArgs = g_slist_concat(allAttackers, allVictims);

    GSList* conflictFreeArgs = NULL;
   
    for(int i=0; i<(aaf->number_of_arguments); i++){
        //printf("a\n");
        gpointer iPtr = GINT_TO_POINTER(i);
         //printf("b\n");
        if(g_slist_find(allConflictingArgs, iPtr) == NULL){
             //printf("c\n");
            conflictFreeArgs = g_slist_append (conflictFreeArgs, GINT_TO_POINTER (i));
            
        }
    }
    
    if(g_slist_length(conflictFreeArgs)>0){
       //printf("Now returning conflict Free args: ");
    }else{
        //printf("NO CONFLICT FREE ARGUMENTS \n");
    }
    return conflictFreeArgs;

}

int getRandomIndex(int listLength){
    return rand() % listLength;
}

int getRandomArgument(GSList* list){
    //printf("getRandomArgument!\n");
    if(list != NULL){
        int listLength = g_slist_length(list);
        //printf("List length: %d\n", listLength);
        int randomIndex = getRandomIndex(listLength);
        //printf("randomIndex: %d\n", randomIndex);
        int randomArgument = *((int*)g_slist_nth_data(list, randomIndex));
        //printf("randomArgument: %d\n", randomArgument);
        return randomArgument;
    }else{
        //printf("Couldn't get random Argument, List is empty\n");
    }
    return -1;
}

// randomly selects an attacker of in(L) and then checks if it's not defended against
int findBFirst(struct AAF *aaf, struct Labeling *labeling, struct DefendedAgainst* defended){
    //printf("\n\n\nfindBFirst()\n");
    GSList* allAttackers = getAllAttackersOfLIN(aaf, labeling);
    GSList* undefendedAttackers = NULL;
    for (GSList *current = allAttackers; current != NULL; current = current->next){
        int currentI = *((int*) current->data);
         int isCurrentDefended = adm__defended_get(defended, currentI);
        if(!isCurrentDefended){ // if it's not out it was not defended against yet
            undefendedAttackers = g_slist_prepend(undefendedAttackers, GINT_TO_POINTER(new int(currentI)));
        }
    }

    if(undefendedAttackers != NULL){
        //printf("ALL undefended Attackers: ");
        printGSList(undefendedAttackers);
        int b = getRandomArgument(undefendedAttackers);
        g_slist_free_full(undefendedAttackers, deletePtr);
        //printf("\n\n\n");
        return b;
    }else{
        //printf("No undefended Attackers!\n");
    }
      //printf("\n\n\n");
    return -1; 
}
int findBLabeling(struct AAF *aaf, struct Labeling *labeling){
    //printf("findBLabeling()\n");
    GSList* allAttackers = getAllAttackersOfLIN(aaf, labeling);
    GSList *bCandidates = NULL;
    for (GSList *current = allAttackers; current != NULL; current = current->next){
        int* currentP = (int*)current->data;
        int currentI = *currentP;
        bool found1Defender = false;
        for(int i=0; i<(aaf->number_of_arguments); i++){
            int li = taas__lab_get_label(labeling, i);
            if(li==1){
                if(taas__aaf_isAttack(aaf,i, currentI)){
                    // //printf("Current attacker of %d: %d\n", i,*currentIndex);
                    found1Defender = true;
                    break;
                }
            }
        }
        if(!found1Defender){
            bCandidates = g_slist_prepend(bCandidates, currentP);
            //printf("added %d to bCandidates: ", currentI);
            printGSList(bCandidates);
        }

    }
     g_slist_free(allAttackers);
    if(bCandidates != NULL){
        return getRandomArgument(bCandidates);
    }
    //printf("NO B FOUND, returning -1\n");
    return -1;
}

int findCFirst(struct AAF *aaf, struct Labeling *labeling, int b){
    if(b != -1){
          GSList* bAttackers = g_slist_copy(aaf->parents[b]);
          if(bAttackers != NULL){
            GSList* conflictFreeAttackers = NULL;
            for (GSList *current = bAttackers; current != NULL; current = current->next){
                int* currentPtr = (int*) current->data;
                int currentI = *currentPtr;
                int currentLabel = taas__lab_get_label(labeling, currentI);
                if(currentLabel == LAB_UNDEC){ // if it's not out it was not defended against yet
                    conflictFreeAttackers = g_slist_prepend(conflictFreeAttackers, currentPtr);
                }
            }
            if(conflictFreeAttackers != NULL){
                //printf("FINDCFIRST --> CONFLict Free Attackers");
                printGSList(conflictFreeAttackers);
                int c = getRandomArgument(conflictFreeAttackers);
                //printf("WE PICKED C: %d \n", c);
                g_slist_free(conflictFreeAttackers);
                g_slist_free(bAttackers);
                return c;
            }
            g_slist_free(bAttackers);
          }
          
    }
    return -1;
}


GSList* getAllAttackersFromList(struct AAF *aaf, GSList* list){
    GSList* allAttackers = NULL;
    for(GSList* curr = list; curr != NULL; curr = curr->next){
        int currentI = *((int*)curr->data);
        GSList* attackers = g_slist_copy(aaf->parents[currentI]);
        if(allAttackers == NULL){
            allAttackers = attackers;
        }else{
            allAttackers = g_slist_concat(allAttackers, attackers);
        }
    }
    return allAttackers;
}

bool allInAttackersAreOut(struct AAF *aaf, struct Labeling *labeling){
    //printf("------------AllInAttackersAreOut()\n");
    
    // get all Arguments labeled IN
    GSList* inLabeled = getAllInLabeledArgs(labeling);
    if(inLabeled != NULL){
        GSList* allAttackers = getAllAttackersFromList(aaf, inLabeled);
        for(GSList* curr = allAttackers; curr != NULL; curr = curr->next){
            int currentI = *((int*)curr->data);
            int currLabel = taas__lab_get_label(labeling, currentI);
            if(currLabel != LAB_OUT){
                //printf("RETURNING FALSE - Argument %d is labeled %d\n", currentI, currLabel);
                return false;
            }
        }

        //printf("ALL IN ATTACKERS ARE LABELED OUT - RETURNInG TRUE\n");
        printGSList(inLabeled);
        g_slist_free_full(inLabeled, deletePtr);
        g_slist_free(allAttackers);
    }
    return true; 
}

bool allOutHaveOneInAttacker(struct AAF *aaf, struct Labeling *labeling){
    //printf("------------allOutHaveOneInAttacker()\n");
    GSList* outLabeled = NULL;
    // get all Arguments labeled IN
    for (int idx = bitset__next_set_bit(labeling->out, 0); idx != -1; idx = bitset__next_set_bit(labeling->out, idx + 1)) {
        outLabeled = g_slist_prepend(outLabeled, GINT_TO_POINTER(new int(idx)));
    }
    printGSList(outLabeled);
    for(GSList* currOut = outLabeled; currOut != NULL; currOut = currOut->next){
        int currentOut = *((int*)currOut->data);
        bool foundOneIn = false; 
        GSList* attackers = g_slist_copy(aaf->parents[currentOut]);
        for(GSList* currAtt = attackers; currAtt != NULL; currAtt = currAtt->next){
            int currentAttacker = *((int*)currAtt->data);
            if(taas__lab_get_label(labeling, currentAttacker) == LAB_IN){
                foundOneIn = true;
                //printf("Found %d attacker of %d labeled IN\n", currentAttacker, currentOut);
                break;
            }
        }
        if(!foundOneIn){
            //printf("RETURN FALSE - DIDN't find attacker of %d labeled IN\n", currentOut);
            g_slist_free_full(outLabeled, deletePtr);
            return false;
        }
    }
    //printf("RETURNING TRUE \n");
    g_slist_free_full(outLabeled, deletePtr);
    return true;
}

bool isAdmissibleLabeling(struct AAF *aaf, struct Labeling *labeling){
    return allInAttackersAreOut(aaf, labeling) && allOutHaveOneInAttacker(aaf, labeling);
}

void labelIn(struct AAF *aaf, struct Labeling *labeling, struct DefendedAgainst* defended, int argument){
    //set label In
    taas__lab_set_label(labeling, argument, LAB_IN);
    //set label of all attackers out
    GSList* attackers = aaf->parents[argument];
    for(GSList* curr = attackers; curr != NULL; curr = curr->next){
        int currentI = *((int*)curr->data);
        taas__lab_set_label(labeling, currentI, LAB_OUT);

    }
    GSList* victims = aaf->children[argument];
    for(GSList* curr = victims; curr != NULL; curr = curr->next){
        int currentI = *((int*)curr->data);
        taas__lab_set_label(labeling, currentI, LAB_OUT);
        adm__defended_set(defended, currentI, YES);

    }
}

bool solve_dcadm(struct TaskSpecification *task, struct AAF *aaf, bool do_print = true)
{
    // Element a
    int a = task->arg;
    
    for (int i = 0; i < MAX_IT; i++)
    {   
        // create new labeling 
        struct Labeling *labeling;
        labeling = (struct Labeling *)malloc(sizeof(struct Labeling));
        taas__lab_init(labeling, FALSE); 
        bitset__clone(aaf->initial, labeling->in);
        bitset__unsetAll(labeling->in);
        // all arguments attacked by initial arguments are out
        bitset__init(labeling->out, aaf->number_of_arguments);
        bitset__unsetAll(labeling->out);
        //printf("printing the Labelling: %s \n", taas__lab_print_as_labeling(labeling, aaf));
        
        // setup Defended Bitset:
        struct DefendedAgainst *defended;
        defended = (struct DefendedAgainst *)malloc(sizeof(struct DefendedAgainst));
        adm__defended_init(defended);
        bitset__init(defended->yes, aaf->number_of_arguments);
        bitset__unsetAll(defended->yes);
        
        

        // L(a) <- in
        labelIn(aaf, labeling, defended, a);
       
        while (!isAdmissibleLabeling(aaf, labeling))
        {   
            int b = findBFirst(aaf, labeling, defended);
            
            int c = findCFirst(aaf, labeling, b);
            

            if (c == -1){
                break;
            }else{
               labelIn(aaf, labeling, defended, c);
            }  
        }
        if (isAdmissibleLabeling(aaf, labeling))
        {
            printf("YES\n");
            return true;
        }
        taas__lab_destroy(labeling);
        adm__defended_destroy(defended);
    }
     printf("NO\n");
    return false;
}

/* ============================================================================================================== */
/* == END FILE ================================================================================================== */
/* ============================================================================================================== */
