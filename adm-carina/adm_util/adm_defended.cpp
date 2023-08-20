/* ============================================================================================================== */
/* == BEGIN FILE ================================================================================================ */
/* ============================================================================================================== */
/*
============================================================================
Name : adm_defended.cpp
Author : Carina Benzin
Version : 1.0
Copyright : GPL3
Description : Utility struct needed for DC-ADM
============================================================================
*/

#define NO 0
#define YES 1

/**
 * Struct for (partial) labelings
 * Arguments not mentioned in either in or out
 * are either undecided or not included.
 */
struct InLRelations{
  // Arguments that are defended against
  struct BitSet *defended;
  // Arguments that attack in(L)
  struct BitSet *attacks;
};

/*
 * Initialises a labeling.
 */
void adm__inLRel_init(struct InLRelations* inLRel){
  inLRel->defended = (struct BitSet*) malloc(sizeof(struct BitSet));
  inLRel->attacks = (struct BitSet*) malloc(sizeof(struct BitSet));
}

/**
 * Returns the label of the given argument.
 */
int adm__inLRel_defended_get(struct InLRelations* inLRel, int arg){
    if(bitset__get(inLRel->defended,arg))
        return YES;
    else return NO;
}

/**
 * Returns the label of the given argument.
 */
int adm__inLRel_attacks_get(struct InLRelations* inLRel, int arg){
    if(bitset__get(inLRel->attacks,arg))
        return YES;
    else return NO;
}


/**
 * Sets the label of the given argument.
 */
void adm__inLRel_defended_set(struct InLRelations* inLRel, int arg, int label){
  if(label == YES){
      bitset__set(inLRel->defended,arg);
      return;
  }
  if(label == NO){
    bitset__unset(inLRel->defended,arg);
    return;
  }
  return;
}

/**
 * Sets the label of the given argument.
 */
void adm__inLRel_attacks_set(struct InLRelations* inLRel, int arg, int label){
  if(label == YES){
      bitset__set(inLRel->attacks,arg);
      return;
  }
  if(label == NO){
    bitset__unset(inLRel->attacks,arg);
    return;
  }
  return;
}



/**
 * gives a string representation of the labeling in the form
 * "[a1=l1,...,an=ln]" where a1,...,an are all arguments and li is t
 */
// char* adm__inLRel_print(struct InLRelations* rel, struct AAF* aaf){
//   int len = 100;
//   char* str = (char*) malloc(len);
//   int sidx = 0;
//   str[sidx++] = '[';
//   int isFirst = 1;
//   for(int idx = 0; idx < aaf->number_of_arguments; idx++){
//     if(sidx + strlen(aaf->ids2arguments[idx]) + 6 > len){
//       len += 100;
//       str = (char*) realloc(str, len);
//     }
//     if(isFirst != 0){
//       strcpy(&str[sidx],aaf->ids2arguments[idx]);
//       sidx += strlen(aaf->ids2arguments[idx]);
//       str[sidx++] = '=';
//       if(adm__inLRel_get(rel,idx) == YES)
//         str[sidx++] = 'Y';
//       else if(adm__inLRel_get(rel,idx) == NO)
//         str[sidx++] = 'N';
//       else
//         str[sidx++] = 'X';
//       isFirst = 0;
//     } else{
//       str[sidx++] = ',';
//       strcpy(&str[sidx],aaf->ids2arguments[idx]);
//       sidx += strlen(aaf->ids2arguments[idx]);
//       str[sidx++] = '=';
//       if(adm__inLRel_get(defended,idx) == YES)
//         str[sidx++] = 'Y';
//       else if(adm__inLRel_get(defended,idx) == NO)
//         str[sidx++] = 'N';
//       else
//         str[sidx++] = 'X';
//     }
//   }
//   str[sidx++] = ']';
//   str[sidx] = '\0';
//   return str;
// }

/*
 * Destroys a labeling
 */
void adm__inLRel_destroy(struct InLRelations* inLRel){
  bitset__destroy(inLRel->defended);
  bitset__destroy(inLRel->attacks);
  free(inLRel);
}

 /* ============================================================================================================== */
 /* == END FILE ================================================================================================== */
 /* ============================================================================================================== */
