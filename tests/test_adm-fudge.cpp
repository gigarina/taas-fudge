#include <gtest/gtest.h>
#include <string>
#include <vector>

class ADMTestFixture : public ::testing::Test{
protected: 
    static void SetUpTestSuite(){
        std::vector<std::string> args = {"./adm-fudge", "-p", "DC-CO", "-fo", "tgf", "-f", "../examples/graph.tgf", "-a", "0"};
        int argc = args.size();
        struct SolverInformation *info = taas__solverinformation(
                "adm-fudge v1.0 (2023-08-17)\nCarina Benzin (carinabenzin@outlook.com)",
                "[tgf]",
                "[DC-CO,DC-PR]"
            );
        struct TaskSpecification *task = taas__cmd_handle(argc,args,info);
        
        // read file
        struct AAF *aaf = (struct AAF*) malloc(sizeof(struct AAF));
        // check format
        if(task->format != NULL && strcmp(task->format,"tgf") == 0)
            taas__readFile_tgf(task->file,aaf);
        taas__update_arg_param(task,aaf);
        // Solve the Problem with solve_dcadm
    


    }

    static void TearDownTestSuite(){
        taas__aaf_destroy(aaf);

        taas__solverinformation_destroy(info);
        taas__cmd_destroy(task);
    }

};

TEST(Test, isEqual) {
    
   EXPECT_EQ(4, 5);
   

}
TEST(Test, isNotEqual) {
   EXPECT_NE(2,5);
   
}
TEST(Test, expectsEqualButIsnt) {
   EXPECT_EQ(5, 2);
  
   
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
