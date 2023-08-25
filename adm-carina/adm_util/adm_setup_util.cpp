/* ============================================================================================================== */
/* == BEGIN FILE ================================================================================================ */
/* ============================================================================================================== */
/*
============================================================================
Name : adm_setup_util.cpp
Author : Carina Benzin
Version : 1.0
Copyright : GPL3
Description : Setup functions needed for DC-ADM
============================================================================
*/

/**
 * @brief Creates and initializes a Labeling struct for the given aaf
 * @param aaf The AAF that the Labeling struct shall be created for
 * @return A pointer to the created Labeling struct.
*/
struct Labeling* createLabelingForAAF(struct AAF* aaf){
    // initialize labeling
    struct Labeling *labeling;
    labeling = (struct Labeling *)malloc(sizeof(struct Labeling));
    taas__lab_init(labeling, FALSE); 
    // set up bitset for in labeled arguments
    bitset__init(labeling->in, aaf->number_of_arguments);
    bitset__unsetAll(labeling->in);
    // set up  bitset for out labeled arguments
    bitset__init(labeling->out, aaf->number_of_arguments);
    bitset__unsetAll(labeling->out);

    return labeling;
}

/**
 * @brief Creates and initializes a InLRelation struct for the given aaf
 * @param aaf The AAF that the InLRelation struct shall be created for
 * @return A pointer to the created InLRelation struct.
*/
struct InLRelation* createInLRelForAAF(struct AAF* aaf){
    struct InLRelation *inLRel;
    inLRel = (struct InLRelation *)malloc(sizeof(struct InLRelation));
    adm__inLRel_init(inLRel);
    bitset__init(inLRel->defended, aaf->number_of_arguments);
    bitset__unsetAll(inLRel->defended);
    bitset__init(inLRel->attacks, aaf->number_of_arguments);
    bitset__unsetAll(inLRel->attacks);

    return inLRel;
}
/**
 * @brief Creates and initializes a InLRelation struct for the given aaf
 * @param aaf The AAF that the InLRelation struct shall be created for
 * @return A pointer to the created InLRelation struct.
*/
struct TempExclude* createTempExcludeForAAF(struct AAF* aaf){
    //printf("createInLRelForAAF\n");
    struct TempExclude *tempExcl;
    tempExcl = (struct TempExclude *)malloc(sizeof(struct TempExclude));
    adm__tempExcl_init(tempExcl);
    bitset__init(tempExcl->tempExcludeB, aaf->number_of_arguments);
    bitset__unsetAll(tempExcl->tempExcludeB);
    bitset__init(tempExcl->tempExcludeC, aaf->number_of_arguments);
    bitset__unsetAll(tempExcl->tempExcludeC);

    return tempExcl;
}

/* ============================================================================================================== */
/* == END FILE ================================================================================================== */
/* ============================================================================================================== */
