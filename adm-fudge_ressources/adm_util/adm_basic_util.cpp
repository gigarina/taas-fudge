
/* ============================================================================================================== */
/* == BEGIN FILE ================================================================================================ */
/* ============================================================================================================== */
/*
============================================================================
Name : adm_basic_util.cpp
Author : Carina Benzin
Version : 1.0
Copyright : GPL3
Description : Utility functions needed for DC-ADM
============================================================================
*/

/**
 * @brief decides wether the given Labeling is admissible or not.
 * For a labeling to be admissible, all in(L) attackers must be out and
 * all arguments in out(L) have at least one attacker in in(L).
 * The first condition is implicitly fulfilled by our labelIn function and 
 * the fact that we only label in Elements that are conflict free with the current in(L). 
 * The labelIn function labels all elements that are in conflict with the new Element in in(L) as out.
 * The second condition is tested in this function.
 * @param defended
 * @param lab
 * @return true if labeling is admissible, false otherwise.
*/

bool adm__isAdmissible(struct InLRelation* inLRel, struct Labeling* lab){

  for (int idx = bitset__next_set_bit(lab->out, 0); idx != -1; idx = bitset__next_set_bit(lab->out, idx + 1)) {
    if(adm__inLRel_defended_get(inLRel, idx) != 1){
      return false;
    }
  }
  return true;

}


/**
 * @brief Returns if the given C is self-attacking
 * @param aaf
 * @param c
 * @return True if c is self attacking, false otherwise.
*/
bool isCSelfAttacking(struct AAF* aaf, int c){
    bool isSelfAttacking = (bool) bitset__get(aaf->loops, c);
    return isSelfAttacking;
}



/**
 * @brief Returns a random index of a vector of the given length. Used to pick random arguments from vectors.
 * @param vLength The length of the vector
 * @return A random index of a vector of the given length
*/
int getRandomIndex(int vLength){
    // seed random number generator with current time
    // to get different random number sequences for the same vLengths
    srand(static_cast<unsigned int>(time(nullptr)));
    return rand() % vLength;
}

/**
 * @brief Returns a random argument of the given vector.
 * @param v The vector one random argument shall be picked out of.
 * @return A random argument of the given vector.
*/

int getRandomArgumentVector(const std::vector<int>& v){
    if(!v.empty()){
        int vLength = v.size();
        int randomIndex = getRandomIndex(vLength);
        return v[randomIndex];
    }
    return -1;
}


/* ============================================================================================================== */
/* == END FILE ================================================================================================== */
/* ============================================================================================================== */
