//
// Created by n8 on 3/11/18.
//

#include "gtest/gtest.h"
#include "threadsafe/map.hpp"


TEST(MapTestSingleThreadedPointer, DefaultConstructor)
{
    int one = 1;

    ts::map<int *, std::string> map;
    map.insert(&one, "1");

    EXPECT_NE(*map.find(&one), *map.end());
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}