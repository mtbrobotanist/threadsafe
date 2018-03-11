//
// Created by n8 on 3/10/18.
//

#include <iostream>
#include <sstream>
#include "threadsafe/map.hpp"

std::string to_string(ts::map<std::string, int>&);

int main(int argc, char* argv[])
{
    ts::map<std::string, int> map(26);

    map.insert("z", 26);
    map.insert("y", 25);
    map.insert("x", 24);
    map.insert("w", 23);
    map.insert("v", 22);
    map.insert("u", 21);
    map.insert("t", 20);
    map.insert("s", 19);
    map.insert("r", 18);
    map.insert("q", 17);
    map.insert("p", 16);
    map.insert("o", 15);
    map.insert("n", 14);
    map.insert("m", 13);
    map.insert("l", 13);
    map.insert("k", 11);
    map.insert("j", 10);
    map.insert("i", 9);
    map.insert("h", 8);
    map.insert("g", 7);
    map.insert("f", 6);
    map.insert("e", 5);
    map.insert("d", 4);
    map.insert("c", 3);
    map.insert("b", 2);
    map.insert("a", 1);

    std::cout << to_string(map) << std::endl;

    map.insert("a", 111);

    std::cout << to_string(map) << std::endl;

    return 0;
}

std::string to_string(ts::map<std::string, int>& map)
{
    std::stringstream stream;

    stream << "[ ";
    for(const auto& it : map)
    {
        stream << "(" + it.first << ", " << it.second << "), ";
    }

    std::string s = stream.str();

    int cut = map.size() > 0 ? 2 : 1;

    return s.substr(0, s.size() - cut) + " ]";
}
