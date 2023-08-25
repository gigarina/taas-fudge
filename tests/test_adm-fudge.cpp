#define COMPUTATION_FINISHED 0
#define COMPUTATION_ABORTED__ANSWER_YES 1
#define COMPUTATION_ABORTED__ANSWER_NO  2
#define COMPUTATION_ABORTED__ANSWER_EMPTYSET  3
#define COMPUTATION_ABORTED__ANSWER_EMPTYEMPTYSET  4
#define COMPUTATION_FINISHED__EMPTY_GROUNDED  5
#define COMPUTATION_FINISHED__NONEMPTY_GROUNDED  6
#define COMPUTATION_FINISHED__ANSWER_NO  7

#define TRUE 1
#define FALSE 0

#include <gtest/gtest.h>

#include <stdio.h>
#include <ctype.h>
#include <cmath>
#include <glib.h>
#include <assert.h>
#include <stdbool.h>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include "../lib/pstreams-1.0.3/pstream.h"

#include "../util/bitset.c"
#include "../util/raset.c"
#include "../util/miscutil.c"

#include "/Users/Black/Documents/GitHub/taas-fudge/taas/taas_aaf.c"
#include "/Users/Black/Documents/GitHub/taas-fudge/taas/taas_inout.c"

#include "/Users/Black/Documents/GitHub/taas-fudge/taas/taas_labeling.c"

#include "/Users/Black/Documents/GitHub/taas-fudge/adm-carina/adm_util/adm_defended.cpp"
#include "/Users/Black/Documents/GitHub/taas-fudge/adm-carina/adm_util/adm_tried.cpp"
#include "/Users/Black/Documents/GitHub/taas-fudge/adm-carina/adm_util/adm_temp_exclude.cpp"
#include "/Users/Black/Documents/GitHub/taas-fudge/adm-carina/tasks/task_dc-adm.cpp"

// struct TestCaseInfo
// {
//     int queriedArg;
//     struct AAF *aaf;
//     struct Labeling* lab;
//     struct InLRelation* defended;
//     struct TriedArguments* tried;
//     struct TempExclude* tempExcl;
// };

// void init_testCaseInfo(struct TestCaseInfo* tInfo){
//     tInfo->aaf = (struct AAF*) malloc(sizeof(struct AAF));
//     tInfo->lab = createLabelingForAAF(tInfo->aaf);
//     tInfo->defended = createInLRelForAAF(tInfo->aaf);
//     tInfo->tried = createTriedArguments(tInfo->aaf);
//     tInfo->tempExcl = createTempExcludeForAAF(tInfo->aaf);
// }

// void destroy_testCaseInfo(struct TestCaseInfo* tInfo){
//     taas__aaf_destroy(tInfo->aaf);
//     adm__inLRel_destroy(tInfo->defended);
//     adm__tried_destroy(tInfo->tried);

//     //free(tInfo);
// }

//  struct TestCaseInfo* testSetup(char* argv[]){//std::vector<std::string> args){
     
//         int argc = 9;
    
//         struct SolverInformation *info = taas__solverinformation(
//                 "adm-fudge v1.0 (2023-08-17)\nCarina Benzin (carinabenzin@outlook.com)",
//                 "[tgf]",
//                 "[DC-CO,DC-PR]"
//             );
//         struct TaskSpecification *task = taas__cmd_handle(argc,argv,info);
       
//         struct TestCaseInfo* tInfo = (struct TestCaseInfo*) malloc(sizeof(struct TestCaseInfo));
//         init_testCaseInfo(tInfo);
       
//         // read file

//         // check format
//         if(task->format != NULL && strcmp(task->format,"tgf") == 0)
//             taas__readFile_tgf(task->file,tInfo->aaf);
//         tInfo->queriedArg = * (int*) g_hash_table_lookup(tInfo->aaf->arguments2ids, task->argAsString);
        
        
        
//         taas__solverinformation_destroy(info);
//         taas__cmd_destroy(task);
//         return tInfo;
//     }

// class ADMTestFixture : public ::testing::Test{
// public: 
//     int queriedArg;
//     struct AAF *aaf;
//     struct Labeling* lab;
//     struct InLRelation* defended;
//     struct TriedArguments* tried;
//     struct TempExclude* tempExcl;

//     void prepareSetUp(){
//          char* argv[] = {"./adm-fudge", "-p", "DC-CO", "-fo", "tgf", "-f", "/Users/Black/Documents/GitHub/taas-fudge/examples/graph.tgf", "-a", "2"};
//         int argc = 9;
    
//         struct SolverInformation *info = taas__solverinformation(
//                 "adm-fudge v1.0 (2023-08-17)\nCarina Benzin (carinabenzin@outlook.com)",
//                 "[tgf]",
//                 "[DC-CO,DC-PR]"
//             );
//         struct TaskSpecification *task = taas__cmd_handle(argc,argv,info);
       
//         // read file

//         // check format
//         if(task->format != NULL && strcmp(task->format,"tgf") == 0)
//             taas__readFile_tgf(task->file, aaf);
//         queriedArg = * (int*) g_hash_table_lookup(aaf->arguments2ids, task->argAsString);
 
//         taas__solverinformation_destroy(info);
//         taas__cmd_destroy(task);
//     }

//     TestSetupSolveDCADM(){
//         aaf = (struct AAF*) malloc(sizeof(struct AAF));
//         prepareSetUp();
//         lab = createLabelingForAAF(aaf);
//         defended = createInLRelForAAF(aaf);
//         tried = createTriedArguments(aaf);
//         tempExcl = createTempExcludeForAAF(aaf);
//     }

//     virtual ~TestSetupSolveDCADM(){
//         taas__aaf_destroy(aaf);
//         adm__inLRel_destroy(defended);
//         adm__tried_destroy(tried);
//         adm__tempExcl_destroy(tempExcl);
//     }
   
 
//    void setQueriedArg(char* newArg){
//     queriedArg = * (int*) g_hash_table_lookup(aaf->arguments2ids, newArg);
//    }

// };


    int queriedArg;
    struct AAF *aaf;
    struct Labeling* lab;
    struct InLRelation* defended;
    struct TriedArguments* tried;
    struct TempExclude* tempExcl;

    void prepareSetUp(){
         char* argv[] = {"./adm-fudge", "-p", "DC-CO", "-fo", "tgf", "-f", "/Users/Black/Documents/GitHub/taas-fudge/examples/graph.tgf", "-a", "2"};
        int argc = 9;
    
        struct SolverInformation *info = taas__solverinformation(
                "adm-fudge v1.0 (2023-08-17)\nCarina Benzin (carinabenzin@outlook.com)",
                "[tgf]",
                "[DC-CO,DC-PR]"
            );
        struct TaskSpecification *task = taas__cmd_handle(argc,argv,info);
       
        // read file

        // check format
        if(task->format != NULL && strcmp(task->format,"tgf") == 0)
            taas__readFile_tgf(task->file, aaf);
        queriedArg = * (int*) g_hash_table_lookup(aaf->arguments2ids, task->argAsString);
 
        taas__solverinformation_destroy(info);
        taas__cmd_destroy(task);
    }

    void setUp(){
        aaf = (struct AAF*) malloc(sizeof(struct AAF));
        prepareSetUp();
        lab = createLabelingForAAF(aaf);
        defended = createInLRelForAAF(aaf);
        tried = createTriedArguments(aaf);
        tempExcl = createTempExcludeForAAF(aaf);
    }

    void tearDown(){
        taas__aaf_destroy(aaf);
        taas__lab_destroy(lab);
        adm__inLRel_destroy(defended);
        adm__tried_destroy(tried);
        adm__tempExcl_destroy(tempExcl);
    }
   
 
   void setQueriedArg(char* newArg){
    queriedArg = * (int*) g_hash_table_lookup(aaf->arguments2ids, newArg);
   }



TEST(TestSetupSolveDCADM, allLabelsUnset) {
    setUp();
    //setQueriedArg("2");
    int arg = 2;
    printf("queried Arg: %d\n", queriedArg);
    int numArgs = aaf->number_of_arguments;
    for(int i=0; i< numArgs; i++){
        int label = taas__lab_get_label(lab, i);
        EXPECT_EQ(LAB_UNDEC, label);
    }

    tearDown();
}


TEST(TestSetup, allLabelsUnset) {
   setUp();
    int arg = 2;
    int numArgs = (aaf)->number_of_arguments;
    for(int i=0; i< numArgs; i++){
        int label = taas__lab_get_label(lab, i);
        EXPECT_EQ(LAB_UNDEC, label);
    }
    tearDown();
}

TEST(TestSetup, noneDefended) {
    setUp();
   int arg = 2;
    int numArgs = (aaf)->number_of_arguments;
    for(int i=0; i< numArgs; i++){
        EXPECT_FALSE(adm__inLRel_defended_get(defended, i));
    }
    tearDown();
}

TEST(TestSetup, noneTried) {
    setUp();
   int arg = 2;
    int numArgs = (aaf)->number_of_arguments;
    for(int i=0; i< numArgs; i++){
        EXPECT_FALSE(adm__triedB_get(tried, i));
        EXPECT_FALSE(adm__triedC_get(tried, i));
    }
    tearDown();
}

TEST(labelIn, detachedArg) {
    setUp();

    int arg = 0;
    int numArgs = aaf->number_of_arguments;
    
    labelIn(aaf, lab, defended, tried, tempExcl, arg);
    for(int i=0; i< numArgs; i++){
        int label = taas__lab_get_label(lab, i);
        if(i == arg){
            EXPECT_EQ(LAB_IN, label);
            EXPECT_TRUE(adm__triedC_get(tried, i));
        }else{
            EXPECT_EQ(LAB_UNDEC, label);
            EXPECT_FALSE(adm__triedC_get(tried, i));
        }
        EXPECT_FALSE(adm__inLRel_defended_get(defended, i));
    }
    tearDown();
}

TEST(labelIn, leafAttacking) {
    setUp();
    int arg = 2;
    int numArgs = (aaf)->number_of_arguments;
    
    labelIn(aaf, lab, defended, tried, tempExcl, arg);
    for(int i=0; i< numArgs; i++){
        int label = taas__lab_get_label(lab, i);
        if(i == arg){
            EXPECT_EQ(LAB_IN, label);
            EXPECT_TRUE(adm__triedC_get(tried, i));
            EXPECT_FALSE(adm__inLRel_defended_get(defended, i));
        }else if(i == 3){
            EXPECT_EQ(LAB_OUT, label);
            EXPECT_TRUE(adm__inLRel_defended_get(defended, i));
        }else{
            EXPECT_EQ(LAB_UNDEC, label);
            EXPECT_FALSE(adm__inLRel_defended_get(defended, i));
        }
        
    }
    tearDown();
}

TEST(labelIn, attackedLeaf) {
    setUp();
    int arg = 4;
    int numArgs = (aaf)->number_of_arguments;
    
    labelIn(aaf, lab, defended, tried, tempExcl, arg);
    for(int i=0; i< numArgs; i++){
        int label = taas__lab_get_label(lab, i);
        if(i == arg){
            EXPECT_EQ(LAB_IN, label);
            EXPECT_TRUE(adm__triedC_get(tried, i));
        }else if(i == 3){
            EXPECT_EQ(LAB_OUT, label);
            EXPECT_FALSE(adm__triedC_get(tried, i));
        }else{
            EXPECT_EQ(LAB_UNDEC, label);
            EXPECT_FALSE(adm__triedC_get(tried, i));
        }
        EXPECT_FALSE(adm__inLRel_defended_get(defended, i));
    }
    tearDown();
}

TEST(labelIn, leafDefendingItself) {
    setUp();
    int arg = 1;
    int numArgs = (aaf)->number_of_arguments;
    
    labelIn(aaf, lab, defended, tried, tempExcl, arg);
    for(int i=0; i< numArgs; i++){
        int label = taas__lab_get_label(lab, i);
        if(i == arg){
            EXPECT_EQ(LAB_IN, label);
        }else if(i == 3){
            EXPECT_EQ(LAB_OUT, label);
        }else{
            EXPECT_EQ(LAB_UNDEC, label);
        }
    }
    tearDown();
}

TEST(labelIn, detachedSelfAttackingArg) {
    setUp();
   int arg = 5;
    int numArgs = (aaf)->number_of_arguments;
    
    labelIn(aaf, lab, defended, tried, tempExcl, arg);
    for(int i=0; i< numArgs; i++){
        int label = taas__lab_get_label(lab, i);
        EXPECT_EQ(LAB_UNDEC, label);
    }
    tearDown();
}

TEST(labelIn, attachedSelfAttackingArg) {
    setUp();
    int arg = 6;
    int numArgs = (aaf)->number_of_arguments;
    
    labelIn(aaf, lab, defended, tried, tempExcl, arg);
    for(int i=0; i< numArgs; i++){
        int label = taas__lab_get_label(lab, i);
        EXPECT_EQ(LAB_UNDEC, label);
    }
    tearDown();
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
