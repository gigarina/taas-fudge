


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
 * @brief Struct to save what attackers in(L) has and 
 * which attackers in(L) defends itself against.
 */
struct InLRelation{
  // Attackers of in(L) that in(L) attacks
  struct BitSet *defended;
  // Attackers of in(L)
  struct BitSet *attacks; // maybe obsolete
};


/**
 * @brief Initializes a inLRelation struct
 * @param inLRel The inLRelation struct that shall be initialized
 */
void adm__inLRel_init(struct InLRelation* inLRel){
  inLRel->defended = (struct BitSet*) malloc(sizeof(struct BitSet));
  inLRel->attacks = (struct BitSet*) malloc(sizeof(struct BitSet));
}

/**
 * @brief Returns the defence status of the given argument.
 * @param inLRel
 * @param arg The argument the defence status shall be returned for.
 * @return YES if arg is defended, NO otherwise.
 */
int adm__inLRel_defended_get(struct InLRelation* inLRel, int arg){
    if(bitset__get(inLRel->defended,arg))
        return YES;
    else return NO;
}


/**
 * @brief Sets the defence status of the given argument to the given status.
 * @param inLRel
 * @param arg
 * @param label
 */
void adm__inLRel_defended_set(struct InLRelation* inLRel, int arg, int label){
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
 * @brief Returns the attack status of the given argument.
 * @param inLRel
 * @param arg The argument the attack status shall be returned for.
 * @return YES if arg is defended, NO otherwise.
 */
int adm__inLRel_attacks_get(struct InLRelation* inLRel, int arg){
    if(bitset__get(inLRel->attacks,arg))
        return YES;
    else return NO;
}

/**
 * @brief Sets the attack status of the given argument to the given status.
 * @param inLRel
 * @param arg
 * @param label
 */
void adm__inLRel_attacks_set(struct InLRelation* inLRel, int arg, int label){
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
 * @brief Destroys a inLRelation struct
 */
void adm__inLRel_destroy(struct InLRelation* inLRel){
  bitset__destroy(inLRel->defended);
  bitset__destroy(inLRel->attacks);
  free(inLRel);
}

 /* ============================================================================================================== */
 /* == END FILE ================================================================================================== */
 /* ============================================================================================================== */


