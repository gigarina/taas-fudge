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
//#include "/Users/Black/Documents/GitHub/taas-fudge/lib/glib-2.77.0/glib/gslist.h"
#include <glib.h>




const int MAX_IT = 5;


// struct IsAdmissibleData {
// struct AAF* aaf;
// struct Labeling* labeling;
// };


int isAdmissible(gpointer data, gpointer user_data){
struct AAF* aaf = (struct AAF*)user_data;
// struct IsAdmissibleData admissibleData = static_cast<struct IsAdmissibleData*>(user_data);
// struct AAF* af = admissibleData->aaf;
// struct Labeling* lab = admissibleData->labeling;
printf("something else instead\n");
printf("is Attack? %d\n",taas__aaf_isAttack(aaf,1,2));
//taas__lab_print_as_labeling(lab, af);
// printf("%s\n",taas__lab_print_as_labeling(lab, af));
// if(af == NULL){
// printf("AAF is null");
// }


return 1;
}


// int isAdmissible2(gpointer data, gpointer user_data) {
// struct IsAdmissibleData* admissibleData = static_cast<struct IsAdmissibleData*>(user_data);
// struct AAF* af = admissibleData->aaf;
// struct Labeling* lab = admissibleData->labeling;
// printf("something else instead\n");
// //printf("is Attack? %d\n", taas__aaf_isAttack(af, 1, 2));
// //taas__lab_print_as_labeling(lab, af);
// printf("%s\n",taas__lab_print_as_labeling(lab, af));
// // if(af == NULL){
// // printf("AAF is null");
// // }


// return 1;
// }


bool isAdmissibleTest(struct AAF* aaf, struct Labeling* labeling){
for(int i=0; i< (aaf->number_of_arguments); i++){
int li = taas__lab_get_label(labeling, i);
// for(int j=0; j <(aaf->number_of_arguments); j++){
// printf("is Attack? %d\n",taas__aaf_isAttack(aaf,j,i));
// if(taas__aaf_isAttack(aaf,j, i)==1){
// int lj = taas__lab_get_label(labeling, j);
// printf("L(%d) = %d, L(%d) = %d\n", i, li, j, lj);
// if(li==lj){
// printf("they match!\n");
// return false;
// }
// }
// }
if(li == 1){
for(int j=0; j <(aaf->number_of_arguments); j++){
//printf("is Attack? %d\n",taas__aaf_isAttack(aaf,j,i));
if(taas__aaf_isAttack(aaf,j, i)==1){
int lj = taas__lab_get_label(labeling, j);
if(lj == 1){
printf("NOT Admissible\n");
return false;
}
}
}
}
if(li == 2){
for(int j=0; j <(aaf->number_of_arguments); j++){
//printf("is Attack? %d\n",taas__aaf_isAttack(aaf,j,i));
if(taas__aaf_isAttack(aaf,j, i)==1){
int lj = taas__lab_get_label(labeling, j);
if(lj == 2){
printf("NOT Admissible\n");
return false;
}
}
}
}
}
return true;
}


void printIntegerGSList(GSList *list) {
while (list != NULL) {
int num = GPOINTER_TO_INT(list->data);
printf("%d\n", num);
list = list->next;
}
}


bool isAdmissibleTest2(struct AAF* aaf, struct Labeling* labeling){
for(int i=0; i<(aaf->number_of_arguments); i++){
int li = taas__lab_get_label(labeling, i);
GSList* attackers = (aaf->parents[i]);
bool found1LabeledIn = false;
for(GSList* current=attackers; current != NULL; current = current->next){
int* currentIndex = (int*)current->data;
int lcur = taas__lab_get_label(labeling, *currentIndex);
if(li==1){
if(lcur==1){
return false;
}
}else if(li==2){
if(lcur==1){
found1LabeledIn = true;
break;
}
}
}
if(!found1LabeledIn){
return false;
}
}
return true;
}


bool solve_dcadm(struct TaskSpecification *task, struct AAF* aaf, struct Labeling* grounded, bool do_print = true){
printf("You are right! <3\n");
//Element a
int a = task->arg;
printf("This is my Argument: %d \n", a);
// for(int i =0; i<sizeof(aaf->ids2arguments); i++){
// g_slist_foreach((aaf->parents[0]),(*GFunc)(gpointer, user_data), );
// }
for(int i = 0; i< MAX_IT; i++ ){
// L <- undec
//create new labeling as the other one is already set.
struct Labeling* labeling;
labeling = (struct Labeling*) malloc(sizeof(struct Labeling));
taas__lab_init(labeling,TRUE);//TODO --> decide if we want only in and out or also undec
bitset__clone(aaf->initial, labeling->in);
bitset__unsetAll(labeling->in);
// all arguments attacked by initial arguments are out
bitset__init(labeling->out, aaf->number_of_arguments);
bitset__unsetAll(labeling->out);


//L(a) <- in
taas__lab_set_label(labeling, a, 1);
// GSList* attackers = (aaf->parents[2]);
// int index = 0;
// GSList *felement = g_slist_nth(attackers, index);
// GSList *selement = g_slist_nth(attackers, 1);
// printf("Element at index %d: \n", index);
// int* first = (int*)felement->data;
// int* second = (int*)selement->data;
// printf("first attacker %d, second attacker %d of element 2\n", *first, *second);
// int* firstElement = (int*)attackers->data;
// printf("First element of aaf->parents[%d]: %d\n", 2, *firstElement);


while(!isAdmissibleTest2(aaf, labeling)){
printf("Got into the while loop\n");
// //B = Randomly select attacker of L(in), that no Argument in L(in) defends against
// //C = Randomly select attacker of B, such that if L(C)= in, L is conflict-free
// if (/* C == Null*/){
// break;
// }
// //L(C) = in
break;
}
if(isAdmissibleTest2(aaf, labeling)){
printf("%s\n",taas__lab_print_as_labeling(labeling, aaf));
printf("!!!!! SUCESS!!!!!");
return true;
}


}
printf("+++++++ FAILURE +++++++");
return false;
}




/* ============================================================================================================== */
/* == END FILE ================================================================================================== */
/* ============================================================================================================== */



