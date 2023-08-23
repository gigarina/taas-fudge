/* ============================================================================================================== */
/* == BEGIN FILE ================================================================================================ */
/* ============================================================================================================== */
/*
============================================================================
Name : adm_temp_exclude.cpp
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
struct TempExclude{
  // Arguments that are tempExcl against
  struct BitSet *tempExcludeC;
  struct BitSet *tempExcludeB;
};


/*
 * Initialises a labeling.
 */
void adm__tempExcl_init(struct TempExclude* tempExcl){
  tempExcl->tempExcludeC = (struct BitSet*) malloc(sizeof(struct BitSet));
  tempExcl->tempExcludeB = (struct BitSet*) malloc(sizeof(struct BitSet));
}


/**
 * Returns the label of the given argument.
 */
int adm__tempExcludeC_get(struct TempExclude* tempExcl, int arg){
    if(bitset__get(tempExcl->tempExcludeC,arg))
        return YES;
    else return NO;
}

int adm__tempExcludeB_get(struct TempExclude* tempExcl, int arg){
    if(bitset__get(tempExcl->tempExcludeB,arg))
        return YES;
    else return NO;
}



void adm__tempExcludeC_set(struct TempExclude* tempExcl, int arg){
  bitset__set(tempExcl->tempExcludeC,arg);
  return;
}

void adm__tempExcludeB_set(struct TempExclude* tempExcl, int arg){
  bitset__set(tempExcl->tempExcludeB,arg);
  return;
}



/*
 * Destroys a labeling
 */
void adm__tempExcl_destroy(struct TempExclude* tempExcl){
  bitset__destroy(tempExcl->tempExcludeC);
  bitset__destroy(tempExcl->tempExcludeB);
  free(tempExcl);
}

 /* ============================================================================================================== */
 /* == END FILE ================================================================================================== */
 /* ============================================================================================================== */
