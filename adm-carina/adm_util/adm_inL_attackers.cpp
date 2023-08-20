/* ============================================================================================================== */
/* == BEGIN FILE ================================================================================================ */
/* ============================================================================================================== */
/*
============================================================================
Name : adm_inL_attackers.cpp
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
struct attacksInL{
  // Arguments that are defended against
  struct BitSet *yes;
};

/*
 * Initialises a labeling.
 */
void adm__defended_init(struct InLRelations* defended){
  attacksInL->yes = (struct BitSet*) malloc(sizeof(struct BitSet));
}

/**
 * Returns the label of the given argument.
 */
int adm__defended_get(struct InLRelations* defended, int arg){
    if(bitset__get(attacksInL->yes,arg))
        return YES;
    else return NO;
}

/**
 * Sets the label of the given argument.
 */
void adm__defended_set(struct InLRelations* defended, int arg, int label){
  if(label == YES){
      bitset__set(attacksInL->yes,arg);
      return;
  }
  if(label == NO){
    bitset__unset(attacksInL->yes,arg);
    return;
  }
  return;
}



/**
 * gives a string representation of the labeling in the form
 * "[a1=l1,...,an=ln]" where a1,...,an are all arguments and li is t
 */
char* adm__defended_print(struct InLRelations* defended, struct AAF* aaf){
  int len = 100;
  char* str = (char*) malloc(len);
  int sidx = 0;
  str[sidx++] = '[';
  int isFirst = 1;
  for(int idx = 0; idx < aaf->number_of_arguments; idx++){
    if(sidx + strlen(aaf->ids2arguments[idx]) + 6 > len){
      len += 100;
      str = (char*) realloc(str, len);
    }
    if(isFirst != 0){
      strcpy(&str[sidx],aaf->ids2arguments[idx]);
      sidx += strlen(aaf->ids2arguments[idx]);
      str[sidx++] = '=';
      if(adm__defended_get(defended,idx) == YES)
        str[sidx++] = 'Y';
      else if(adm__defended_get(defended,idx) == NO)
        str[sidx++] = 'N';
      else
        str[sidx++] = 'X';
      isFirst = 0;
    } else{
      str[sidx++] = ',';
      strcpy(&str[sidx],aaf->ids2arguments[idx]);
      sidx += strlen(aaf->ids2arguments[idx]);
      str[sidx++] = '=';
      if(adm__defended_get(defended,idx) == YES)
        str[sidx++] = 'Y';
      else if(adm__defended_get(defended,idx) == NO)
        str[sidx++] = 'N';
      else
        str[sidx++] = 'X';
    }
  }
  str[sidx++] = ']';
  str[sidx] = '\0';
  return str;
}

/*
 * Destroys a labeling
 */
void adm__defended_destroy(struct InLRelations* defended){
  bitset__destroy(attacksInL->yes);
  free(defended);
}

 /* ============================================================================================================== */
 /* == END FILE ================================================================================================== */
 /* ============================================================================================================== */
