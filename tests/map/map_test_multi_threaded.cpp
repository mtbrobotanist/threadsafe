//
// Created by n8 on 3/11/18.
//

#include <thread>
#include "gtest/gtest.h"

#include "threadsafe/map.hpp"
#include "MultiThreadedTestFixture.h"

void t1()
{

}

TEST_F(MultiThreadedTestFixture, MultiThreadedInsertEraseFind)
{
    num_items = 100000;
    map->reserve(num_items);

    thread1 = std::make_shared<std::thread>(
            [=]()
            {
                for(int i = 0; i < num_items; ++i)
                {
                    map->insert(i, i);
                }
            });

    thread2 = std::make_shared<std::thread>(
            [=]()
            {
                for(int i = 0; i < num_items; ++i)
                {
                    while(map->find(i) == map->end()) ;

                    map->erase(i);
                }
            });

    thread1->join();
    thread2->join();

    EXPECT_EQ(map->size(), 0);

    for(int i = 0; i < num_items; ++i)
    {
        EXPECT_EQ(*map->find(i), *map->end());
    }
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}