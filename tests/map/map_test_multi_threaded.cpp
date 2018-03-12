//
// Created by n8 on 3/11/18.
//

#include <thread>
#include "gtest/gtest.h"
#include "threadsafe/map.hpp"

std::shared_ptr<ts::map<int, int>> map;
int num_items = 5000000;

void Thread1()
{
    std::cout << "thread1\n";

    int j = num_items / 2;
    for(int i = 0; i < num_items / 2; ++i)
    {
        map->insert(i, i);

        //wait until thread2 has inserted whatever value j has
        while (map->find(j) == map->end()) ;

        map->erase(j++);

    }
    (*map)[0] = 5000;
}

void Thread2()
{
    std::cout << "thread2\n";

    for(int i = num_items / 2; i < num_items; ++i)
    {
        map->insert(i, i);
    }
    (*map)[1] = 10000;
}

TEST(MapTestMultiThreadedValue, MultiThreadedInsertErase)
{
    map = std::make_shared<ts::map<int, int>>(100000);

    std::thread thread1(Thread1);
    std::thread thread2(Thread2);

    thread1.join();
    thread2.join();

    EXPECT_EQ(map->size(), num_items / 2);
    EXPECT_EQ(map->get(0), 5000);
    EXPECT_EQ(map->get(1), 10000);
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}