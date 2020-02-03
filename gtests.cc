#include "gtest/gtest.h"
#include "DBFile.h"

TEST(test_open, testingopen){
    DBFile file;
    int b =  file.Create("bin/some.bin",heap, NULL);
    EXPECT_EQ(1,b);
    file.Close();
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
