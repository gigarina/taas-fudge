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
// #include "/Users/Black/Documents/GitHub/taas-fudge/lib/glib-2.77.0/glib/gslist.h"
#include <glib.h>
// #include "../taas/taas_aaf.c"
// #include "../taas/taas_labeling.c"
// #include "../util/bitset.c"

const int MAX_IT = 5;

// struct IsAdmissibleData {
// struct AAF* aaf;
// struct Labeling* labeling;
// };

int isAdmissible(gpointer data, gpointer user_data)
{
    struct AAF *aaf = (struct AAF *)user_data;
    // struct IsAdmissibleData admissibleData = static_cast<struct IsAdmissibleData*>(user_data);
    // struct AAF* af = admissibleData->aaf;
    // struct Labeling* lab = admissibleData->labeling;
    //printf("something else instead\n");
    //printf("is Attack? %d\n", taas__aaf_isAttack(aaf, 1, 2));
    // taas__lab_print_as_labeling(lab, af);
    //  //printf("%s\n",taas__lab_print_as_labeling(lab, af));
    //  if(af == NULL){
    //  //printf("AAF is null");
    //  }

    return 1;
}

// int isAdmissible2(gpointer data, gpointer user_data) {
// struct IsAdmissibleData* admissibleData = static_cast<struct IsAdmissibleData*>(user_data);
// struct AAF* af = admissibleData->aaf;
// struct Labeling* lab = admissibleData->labeling;
// //printf("something else instead\n");
// ////printf("is Attack? %d\n", taas__aaf_isAttack(af, 1, 2));
// //taas__lab_print_as_labeling(lab, af);
// //printf("%s\n",taas__lab_print_as_labeling(lab, af));
// // if(af == NULL){
// // //printf("AAF is null");
// // }

// return 1;
// }

bool isAdmissibleTest(struct AAF *aaf, struct Labeling *labeling)
{
    for (int i = 0; i < (aaf->number_of_arguments); i++)
    {
        int li = taas__lab_get_label(labeling, i);
        // for(int j=0; j <(aaf->number_of_arguments); j++){
        // //printf("is Attack? %d\n",taas__aaf_isAttack(aaf,j,i));
        // if(taas__aaf_isAttack(aaf,j, i)==1){
        // int lj = taas__lab_get_label(labeling, j);
        // //printf("L(%d) = %d, L(%d) = %d\n", i, li, j, lj);
        // if(li==lj){
        // //printf("they match!\n");
        // return false;
        // }
        // }
        // }
        if (li == 1)
        {
            for (int j = 0; j < (aaf->number_of_arguments); j++)
            {
                // //printf("is Attack? %d\n",taas__aaf_isAttack(aaf,j,i));
                if (taas__aaf_isAttack(aaf, j, i) == 1)
                {
                    int lj = taas__lab_get_label(labeling, j);
                    if (lj == 1)
                    {
                        //printf("NOT Admissible\n");
                        return false;
                    }
                }
            }
        }
        if (li == 2)
        {
            for (int j = 0; j < (aaf->number_of_arguments); j++)
            {
                // //printf("is Attack? %d\n",taas__aaf_isAttack(aaf,j,i));
                if (taas__aaf_isAttack(aaf, j, i) == 1)
                {
                    int lj = taas__lab_get_label(labeling, j);
                    if (lj == 2)
                    {
                        //printf("NOT Admissible\n");
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

void printIntegerGSList(GSList *list)
{
    while (list != NULL)
    {
        int num = *((int*)list->data);
        //printf("%d\n", num);
        list = list->next;
    }
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


bool isAdmissibleTest2(struct AAF *aaf, struct Labeling *labeling)
{
    for (int i = 0; i < (aaf->number_of_arguments); i++)
    {   //printf("Are we even incrementing i? i= %d\n", i);
        int li = taas__lab_get_label(labeling, i);
        GSList *attackers = g_slist_copy(aaf->parents[i]);
        bool found1LabeledIn = false;
        for (GSList *current = attackers; current != NULL; current = current->next)
        {
            int *currentIndex = (int *)current->data;
            int lcur = taas__lab_get_label(labeling, *currentIndex);
            if (li == 1)
            {
                if (lcur == 1)
                {
                    //printf("L(%d)= IN - Found one attacker labelled IN, returning FALSE\n", i);
                    return false;
                }
            }
            else if (li == 2)
            {
                if (lcur == 1)
                {
                    //printf("L(%d) = OUT - Found one attacker labelled IN\n", i);
                    found1LabeledIn = true;
                    break;
                }
            }
        }
        if (!found1LabeledIn)
        {
            //printf("L(%d) = OUT - There is not one attacker labelled IN, returning FALSE\n", i);
            return false;
        }
    }
    //printf("RETURNING TRUE\n");
    return true;
}

bool isAdmissibleTest3(struct AAF *aaf, struct Labeling *labeling){
    for(int i=0; i<(aaf->number_of_arguments); i++){
        int li = taas__lab_get_label(labeling, i);
        if(li==1){
            GSList* attackers = g_slist_copy(aaf->parents[i]);
            for (GSList *current = attackers; current != NULL; current = current->next){
                int* currentIndex = (int*)current->data;
                //printf("Current attacker of %d: %d\n", i,*currentIndex);
                int lcur = taas__lab_get_label(labeling, *currentIndex);
                if(lcur==1){
                    //printf("L(%d)= IN - Found one attacker %d labelled IN, returning FALSE\n", i, *currentIndex);
                    return false;
                }

            }
        }
        if(li==2){
            GSList* attackers = g_slist_copy(aaf->parents[i]);
            if(g_slist_length(attackers)>0){
                bool found1LabeledIn = false;
                for (GSList *current = attackers; current != NULL; current = current->next){
                    int* currentIndex = (int*)current->data;
                    //printf("Current attacker of %d: %d\n", i,*currentIndex);
                    int lcur = taas__lab_get_label(labeling, *currentIndex);
                    if(lcur==1){
                        found1LabeledIn = true;
                        break;
                    }

                }
                if(!found1LabeledIn){
                    //printf("L(%d) = OUT - There is not one attacker labelled IN, returning FALSE\n", i);
                    return false;
                }
            }
        }

    }
    //printf("RETURNING TRUE\n");
    return true;
}

// bool naiveIsAdmissible(struct AAF *aaf, struct Labeling *labeling){
//     // check for conflict-freeness
//     //printf("Now checking for conflict freeness\n");
//     GSList* inLabeledArgs = NULL;
//     //printf("inLabeledArgs length before anything: %d\n", g_slist_length(inLabeledArgs));
//     // //printf("LINE a\n");
//      for(int i=0; i<(aaf->number_of_arguments); i++){
//         // //printf("LINE b\n");
//         int li = taas__lab_get_label(labeling, i);
//         // //printf("LINE c\n");
//         if(li==1){
//             // //printf("LINE d\n");
//             //printf("Adding %d to inLabeledArgs\n", i);
//             inLabeledArgs = g_slist_append(inLabeledArgs, GINT_TO_POINTER(i));
//              //printf("LINE e\n");
//         }
//      }
//      //printf("inLabeledArgs length: %d\n", g_slist_length(inLabeledArgs));
//     //printf("-----inLabeledArgs: ");
//     printGSList(inLabeledArgs);
//      for(GSList* cur = inLabeledArgs; cur != NULL; cur = cur->next){
//     //  for(int i=0; i<(g_slist_length(inLabeledArgs)); i++){
//         int i = 0;
//         //printf("Index: %d\n", i);
//         //printf("LINE 1\n");
//         //GSList* ithInLabeled = g_slist_nth(inLabeledArgs,i);
//         int inLabeled1 = *(g_slist_nth_data(inLabeledArgs,i));
//         //printf("LINE 2\n");
//         //int* inLabeled1 = (int*)ithInLabeled->data;
//         //printf("inLabeled1 %d\n", *inLabeled1);
//         //printf("LINE 3\n");
//         for(int j=0; j<(g_slist_length(inLabeledArgs)); j++){
//             //printf("LINE 4\n");
//             GSList* jthInLabeled = g_slist_nth(inLabeledArgs,j);
//             //printf("LINE 5\n");
//             int* inLabeled2 = (int*)jthInLabeled->data;
//             //printf("inLabeled2 %d\n", *inLabeled2);
//             taas__aaf_isAttack(aaf, 0, 2);

//             //printf("LINE 6\n");
//             //printf("Is attack between %d -> %d? \n", *inLabeled1, *inLabeled2);
//             int isAttack = taas__aaf_isAttack(aaf, *inLabeled1, *inLabeled2);
//             ////printf("Is attack between %d -> %d? %d\n", *inLabeled1, *inLabeled2, isAttack);
//             if(isAttack){ //TODO!!!!! <-- THIS DOESN'T WORK
//                 //printf("LINE 7\n");
//                 return false;
//             }
//             //printf("LINE 8\n");
//         }
//         //printf("LINE 9\n");
//      }
//      //printf("LINE 10\n");

//      // check for acceptability
//     // GSList* allAttackers = nullptr;
//     // for(int i=0; i<(g_slist_length(inLabeledArgs)); i++){
//     //     GSList* ithInLabeled = g_slist_nth(inLabeledArgs,i);
//     //     int* inLabeled1 = (int*)ithInLabeled->data;
//     //     GSList* attackers = (aaf->parents[i]);
//     //     allAttackers = g_slist_concat(allAttackers, attackers);
//     // }
//     // if(g_slist_length(allAttackers)>0){
//     // for(int i=0; i<(g_slist_length(inLabeledArgs)); i++){
//     //     GSList* ithInLabeled = g_slist_nth(inLabeledArgs,i);
//     //     int* inLabeled1 = (int*)ithInLabeled->data;

//     //     bool found1Defender = false;
//     //     for(int j=0; j<(g_slist_length(allAttackers)); j++){
//     //         GSList* jthInLabeled = g_slist_nth(allAttackers,j);
//     //         int* inLabeled2 = (int*)jthInLabeled->data;
//     //         if(taas__aaf_isAttack(aaf, *inLabeled1, *inLabeled2)){
//     //             found1Defender = true;
//     //             break;
//     //         }
//     //     }
//     //     if(!found1Defender){
//     //         return false;
//     //     }
//     //  }
//     // }
//     //printf("You did it, now returning TRUE\n");
//     return true;

// }

bool conflictFree(struct AAF *aaf, struct Labeling *labeling){ //TODO --> Returns true for self attacking args!
    for(int i=0; i<(aaf->number_of_arguments); i++){
        int li = taas__lab_get_label(labeling, i);
        if(li==1){
            GSList* attackers = g_slist_copy(aaf->parents[i]);
            for (GSList *current = attackers; current != NULL; current = current->next){
                int* currentIndex = (int*)current->data;
                // //printf("Current attacker of %d: %d\n", i,*currentIndex);
                int lcur = taas__lab_get_label(labeling, *currentIndex);
                if(lcur==1){
                    g_slist_free(attackers);
                    //printf("RETURNING FALSE- NOT conflict free: %d attacks %d\n", *currentIndex, i );
                    return false;
                }
            }
            g_slist_free(attackers);
        }
    }
    //printf("RETURNING TRUE - is Conflict Free\n");
    return true;
}

/* attackers = 1 --> gets attackers */
/* attackers = 0 --> gets victims */
GSList* getAllAttackersOrVictimsOfLIN(struct AAF *aaf, struct Labeling *labeling, bool attackers){
     GSList* allAttackers = NULL;
     for(int i=0; i<(aaf->number_of_arguments); i++){
        int li = taas__lab_get_label(labeling, i);
        if(li==1){
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
     }
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

int findB(struct AAF *aaf, struct Labeling *labeling){
    //printf("findB()\n");
    GSList* allAttackers = getAllAttackersOfLIN(aaf, labeling);
    GSList *bCandidates = NULL;
    for (GSList *current = allAttackers; current != NULL; current = current->next){
        int currentI = *((int*)current->data);
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
            bCandidates = g_slist_prepend(bCandidates, GINT_TO_POINTER(new int(currentI)));
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

int findC(struct AAF *aaf, struct Labeling *labeling, int b){
    //printf("findC()\n");
    GSList* bAttackers = g_slist_copy(aaf->parents[b]);
    //printf("Attackers of %d : ", b);
    printGSList(bAttackers);
    GSList* allAttackers = getAllAttackersOfLIN(aaf, labeling);
    GSList* allVictims = getAllVictimsOfLIN(aaf, labeling);
    GSList* allConflictingArgs = g_slist_concat(allAttackers, allVictims);

    GSList* cCandidates = NULL;
    //printf("All conflicting args of IN(L):");
    //printf("%s : ", taas__lab_print_as_labeling(labeling, aaf));
    printGSList(allConflictingArgs);
    if(allConflictingArgs != NULL){ // faster than list length > 0
        for (GSList *current = bAttackers; current != NULL; current = current->next){
            if(g_slist_find(allConflictingArgs, current) == NULL){
                int currentI = *((int*)current->data);
                // g_slist_free(allConflictingArgs);
                // //printf("RETURNING C: %d \n", currentI);
                // return currentI;
                cCandidates = g_slist_prepend(cCandidates, GINT_TO_POINTER(new int(currentI)));
                //printf("added %d to cCandidates: ", currentI);
                printGSList(cCandidates);
            }
        }    
    }
    g_slist_free(allConflictingArgs);
    if(cCandidates != NULL){
        return getRandomArgument(cCandidates);
    }
     //printf("NO C FOUND, returning -1\n");
    return -1; // <-- TODO this is just a placeholder
}

bool acceptable(struct AAF *aaf, struct Labeling *labeling){
    GSList* allAttackers = getAllAttackersOfLIN(aaf, labeling);
   
    for (GSList *current = allAttackers; current != NULL; current = current->next){
        int currentI = *((int*)current->data);
        // //printf("This is a normal integer: %d and this is a pointer: %d\n", currentI, *currentIndex);
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
            g_slist_free(allAttackers);
            //printf("RETURNING FALSE - not Acceptable - There is no Arg in the set defending it against attacker: %d\n", currentI);
            return false;
        }

    }
    g_slist_free(allAttackers);
    //printf("RETURNING TRUE- is Acceptable\n");
    return true;
}

bool finalIsAdmissible(struct AAF *aaf, struct Labeling *labeling){
    return conflictFree(aaf,labeling) && acceptable(aaf, labeling);
}

bool solve_dcadm(struct TaskSpecification *task, struct AAF *aaf, struct Labeling *grounded, bool do_print = true)
{
    // Element a
    int a = task->arg;
    //printf("This is my Argument: %d \n", a);
    for (int i = 0; i < MAX_IT; i++)
    {
        //printf("+++++++++++++++++++++RUNDE %d+++++++++++++++++++++++\n", i);
        // L <- undec
        // create new labeling as the other one is already set.
        struct Labeling *labeling;
        labeling = (struct Labeling *)malloc(sizeof(struct Labeling));
        taas__lab_init(labeling, TRUE); // TODO --> decide if we want only in and out or also undec
        bitset__clone(aaf->initial, labeling->in);
        bitset__unsetAll(labeling->in);
        // all arguments attacked by initial arguments are out
        bitset__init(labeling->out, aaf->number_of_arguments);
        bitset__unsetAll(labeling->out);

        // L(a) <- in
        taas__lab_set_label(labeling, a, 1);
        // taas__lab_set_label(labeling, 1, 1); // stack overflow if ./taas-fudge -p DC-ADM -fo tgf -f complexGraph.tgf -a 4 -sat ./lib


        while (!finalIsAdmissible(aaf, labeling))
        {   //printf("-----------------------in While--------------------\n");
            //printf("Got into the while loop with Labeling: %s\n", taas__lab_print_as_labeling(labeling, aaf));
            // //B = Randomly select attacker of L(in), that no Argument in L(in) defends against
            int b = findB(aaf, labeling);
            

            // //C = Randomly select attacker of B, such that if L(C)= in, L is conflict-free
            // --> find all conflict free candidates
            //getAllConflictFreeArgsForLIN(aaf, labeling);
            int c = findC(aaf, labeling, b); // --> breaks in second iteration
            

            if (c == -1){
                break;
            }else{
                //printf("FOUND C: %d\n", c);
                taas__lab_set_label(labeling, c, 1);
                //printf("updated labeling: %s \n", taas__lab_print_as_labeling(labeling, aaf));
                //break;
            }
            //L(C) = in
            ////printf("%s\n", taas__lab_print_as_labeling(labeling, aaf));
            //break;
             //printf("-----------------------end While--------------------\n");
        }
        if (finalIsAdmissible(aaf, labeling))
        {
            ////printf("%s\n", taas__lab_print_as_labeling(labeling, aaf));
            //printf("!!!!! SUCESS!!!!!\n");
            printf("YES\n");
            return true;
        }
    }

    //printf("+++++++ FAILURE +++++++\n");
    //printf("Now I can finally work on this properly\n");
     printf("NO\n");
    return false;
}

/* ============================================================================================================== */
/* == END FILE ================================================================================================== */
/* ============================================================================================================== */
