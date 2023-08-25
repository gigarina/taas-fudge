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
 * @brief A struct to narrow the search space for every iteration.
 */
struct TempExclude{
  // C candidates that can be excluded for this iteration
  struct BitSet *tempExcludeC;
  // B candidates that can be excluded for this iteration
  struct BitSet *tempExcludeB;
};


/**
 * @brief initializes a tempExclude struct
 * @param tempExcl The tempExclude that shall be initialized
 */
void adm__tempExcl_init(struct TempExclude* tempExcl){
  tempExcl->tempExcludeC = (struct BitSet*) malloc(sizeof(struct BitSet));
  tempExcl->tempExcludeB = (struct BitSet*) malloc(sizeof(struct BitSet));
}


/**
 * @brief Returns if the given argument is temporarily excluded in the tempExcludeC BitSet.
 * @param tempExcl The TempExclude struct we read the status off
 * @param arg The argument the exclusion status shall be returned
 * @return YES if the argument is temporarily excluded, NO otherwise.
 */
int adm__tempExcludeC_get(struct TempExclude* tempExcl, int arg){
    if(bitset__get(tempExcl->tempExcludeC,arg))
        return YES;
    else return NO;
}

/**
 * @brief Returns if the given argument is temporarily excluded in the tempExcludeB BitSet.
 * @param tempExcl The TempExclude struct we read the status off
 * @param arg The argument the exclusion status shall be returned
 * @return YES if the argument is temporarily excluded, NO otherwise.
 */
int adm__tempExcludeB_get(struct TempExclude* tempExcl, int arg){
    if(bitset__get(tempExcl->tempExcludeB,arg))
        return YES;
    else return NO;
}


/**
 * @brief Sets the temporary exclusion status of the given argument to 1 in the tempExcludeC Bitset.
 * @param tempExcl The TempExclude struct of which the status is set
 * @param arg The argument the exclusion status shall be set
 */
void adm__tempExcludeC_set(struct TempExclude* tempExcl, int arg){
  bitset__set(tempExcl->tempExcludeC,arg);
  return;
}

/**
 * @brief Sets the temporary exclusion status of the given argument to 1 in the tempExcludeB Bitset.
 * @param tempExcl The TempExclude struct of which the status is set
 * @param arg The argument the exclusion status shall be set
 */
void adm__tempExcludeB_set(struct TempExclude* tempExcl, int arg){
  bitset__set(tempExcl->tempExcludeB,arg);
  return;
}



/**
 * @brief Destroys the given TempExclude struct and frees all allocated memory.
 * @param tempExcl The TempExclude struct to destroy 
 */
void adm__tempExcl_destroy(struct TempExclude* tempExcl){
  bitset__destroy(tempExcl->tempExcludeC);
  bitset__destroy(tempExcl->tempExcludeB);
  free(tempExcl);
}

 /* ============================================================================================================== */
 /* == END FILE ================================================================================================== */
 /* ============================================================================================================== */
