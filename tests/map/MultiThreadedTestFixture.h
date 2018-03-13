//
// Created by n8 on 3/12/18.
//

#ifndef THREADSAFE_MULTITHREADEDTESTFIXTURE_H
#define THREADSAFE_MULTITHREADEDTESTFIXTURE_H

#include <thread>
#include "threadsafe/map.hpp"
#include "gtest/gtest.h"

class MultiThreadedTestFixture : public testing::Test
{

public:
    void SetUp()
    {
        map = std::make_shared<ts::map<int, int>>();
    }

    void TearDown()
    {

    }

    std::shared_ptr<ts::map<int, int>> map;

    int num_items;
    std::shared_ptr<std::thread> thread1;
    std::shared_ptr<std::thread> thread2;
};


#endif //THREADSAFE_MULTITHREADEDTESTFIXTURE_H
