//
// Created by n8 on 3/10/18.
//

#include <memory>
#include "gtest/gtest.h"
#include "threadsafe/map.hpp"

ts::map<int, int> IntIntMap1Through4()
{
    return ts::map<int, int>({{1, 1}, {2, 2}, {3, 3}, {4, 4}});
};

ts::map<std::string, int> StringIntMap1Through4()
{
    return ts::map<std::string, int>({{"one", 1}, {"two", 2}, {"three", 3}, {"four", 4}});
};

TEST(MapTestSingleThreadedValueType, DefaultConstructor)
{
    ts::map<int, int> map;
    EXPECT_EQ(0, map.size());
    EXPECT_EQ(0, map.capacity());
}

TEST(MapTestSingleThreadedValueType, CapacityConstructor)
{
    ts::map<int, int> map(10);
    EXPECT_EQ(0, map.size());
    EXPECT_EQ(10, map.capacity());
}

TEST(MapTestSingleThreadedValueType, CopyConstructor)
{
    ts::map<int, int> map(10);

    map.insert(1, 1);
    map.insert(2, 2);
    map.insert(3, 3);
    map.insert(4, 4);
    map.insert(5, 5);
    map.insert(6, 6);
    map.insert(7, 7);
    map.insert(8, 8);
    map.insert(9, 9);
    map.insert(10, 10);

    ts::map<int, int> other(map);

    EXPECT_EQ(map.size(), other.size());
    EXPECT_EQ(map.capacity(), other.capacity());

    for(int i = 0; i < map.size(); ++i)
    {
        EXPECT_EQ(map[i], other[i]) << "value of map[i] and other[i] differ.\n";
    }

}

TEST(MapTestSingleThreadedValueType, InitializerListConstructor)
{
    ts::map<std::string, int> map({{"one", 1}, {"two", 2}, {"three", 3}, {"four", 4}});

    EXPECT_EQ(map.size(), 4);
    EXPECT_STREQ(map.const_begin()->first.c_str(), "four"); // "four" comes first lexicographically, regardless of the initializer list's order
}

TEST(MapTestSingleThreadedValueType, FoundItem)
{
    ts::map<std::string, int> map = StringIntMap1Through4();

    std::string key = "one";
    int val = 1;
    auto it = map.find(key);
    EXPECT_NE(*it, *map.end());                   // should have found the "one" key
    EXPECT_STREQ(it->first.c_str(), key.c_str()); // it->first == "one"
    EXPECT_EQ(it->second, val);                   // it->second == 1


    key = "two";
    val = 2;
    it = map.find(key);
    EXPECT_NE(*it, *map.end());
    EXPECT_STREQ(it->first.c_str(), key.c_str());
    EXPECT_EQ(it->second, val);


    key = "three";
    val = 3;
    it = map.find(key);
    EXPECT_NE(*it, *map.end());
    EXPECT_STREQ(it->first.c_str(), key.c_str());
    EXPECT_EQ(it->second, val);

    key = "four";
    val = 4;
    it = map.find(key);
    EXPECT_NE(*it, *map.end());
    EXPECT_STREQ(it->first.c_str(), key.c_str());
    EXPECT_EQ(it->second, val);
}

TEST(MapTestSingleThreadedValueType, DidntFindItem)
{
    auto map = IntIntMap1Through4();

    EXPECT_EQ(*map.find(5), *map.end());
}

TEST(MapTestSingleThreadedValueType, ConstFoundItem)
{
    ts::map<std::string, int> map = StringIntMap1Through4();

    std::string key = "one";
    int val = 1;
    auto it = map.const_find(key);
    EXPECT_NE(*it, *map.const_end());             // should have found the "one" key
    EXPECT_STREQ(it->first.c_str(), key.c_str()); // it->first == "one"
    EXPECT_EQ(it->second, val);                   // it->second == 1

    key = "two";
    val = 2;
    it = map.const_find(key);
    EXPECT_NE(*it, *map.const_end());
    EXPECT_STREQ(it->first.c_str(), key.c_str());
    EXPECT_EQ(it->second, val);

    key = "three";
    val = 3;
    it = map.const_find(key);
    EXPECT_NE(*it, *map.const_end());
    EXPECT_STREQ(it->first.c_str(), key.c_str());
    EXPECT_EQ(it->second, val);

    key = "four";
    val = 4;
    it = map.const_find(key);
    EXPECT_NE(*it, *map.const_end());
    EXPECT_STREQ(it->first.c_str(), key.c_str());
    EXPECT_EQ(it->second, val);
}

TEST(MapTestSingleThreadedValueType, ConstDidntFindItem)
{
    auto map = IntIntMap1Through4();

    EXPECT_EQ(*map.const_find(5), *map.const_end());
}

TEST(MapTestSingleThreadedValueType, Erase)
{
    auto map = IntIntMap1Through4();

    EXPECT_NE(*map.const_find(3), *map.const_end()) << "should have returned a valid const_iterator when finding 3, got const_end() instead";

    map.erase(3);

    EXPECT_EQ(*map.const_find(3), *map.const_end()) << "expected 3 to be missing, found it instead";
}

TEST(MapTestSingleThreadedValueType, OverWriteExistingValueFromIndexer)
{
    auto map = IntIntMap1Through4();

    EXPECT_EQ(map[2], 3) << "expected value at index 2 to be 3";

    map[2] = 22;

    EXPECT_EQ(map[2], 22) << "expected value at index 2 to 22 after overwrite";
}

TEST(MapTestSingleThreadedValueType, OverwriteExistingValueDuringInsertion)
{
    auto map = IntIntMap1Through4();

    auto it = map.const_find(1);

    EXPECT_EQ(map.get(1), 1) << "expected the following pair: (1, 1), got: (" << it->first << ", " << it->second << ")";

    map.insert(1, 22);

    it = map.const_find(1);

    EXPECT_EQ(map.get(1), 22) << "expected the following pair: (1, 1), got: (" << it->first << ", " << it->second << ") after insertion overwrite";
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}