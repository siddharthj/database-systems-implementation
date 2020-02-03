#include "gtest/gtest.h"
#include "DBFile.h"
#include <cstdio>

TEST(test_open, testingopen){
    DBFile file;
    int b =  file.Create("../bin/some.bin",heap, NULL);
    EXPECT_EQ(1,b);
    file.Close();
    remove("../bin/some.bin");
}

TEST(test_open_empty_null, testingopenemptynull){
    DBFile file;
    int b =  file.Create("",heap, NULL);
    EXPECT_EQ(0,b);
    int c =  file.Create(NULL,heap, NULL);
    EXPECT_EQ(0,c);
    file.Close();
}

TEST(test_filetype_1, testfiletypesorted){
    DBFile file;
    int b =  file.Create("../bin/some.bin",sorted, NULL);
    EXPECT_EQ(0,b);
}
TEST(test_filetype_2, testfiletypetree){
    DBFile file;
    int b =  file.Create("../bin/some.bin",tree, NULL);
    EXPECT_EQ(0,b);
}

TEST(test_open_fail, testfileopenfail){
    DBFile file;
    int b = file.Open("../bin/some.bin");
    EXPECT_EQ(0,b);
    file.Create("../bin/some.bin" ,heap, NULL);
    b = file.Open(NULL);
    EXPECT_EQ(0,b);
    remove("../bin/some.bin");
}

TEST(test_open_pass, testfileopenpass){
    DBFile file;
    file.Create("../bin/some.bin" ,heap, NULL);
    int b = file.Open("../bin/some.bin");
    EXPECT_EQ(1,b);
    file.Close();
    remove("../bin/some.bin");
}

TEST(test_close_pass, testfileclosepass){
    DBFile file;
    file.Create("../bin/some.bin" ,heap, NULL);
    int b = file.Open("../bin/some.bin");
    EXPECT_EQ(1,b);
    b = file.Close();
    EXPECT_EQ(1,b);
    remove("../bin/some.bin");
}

//----------

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
