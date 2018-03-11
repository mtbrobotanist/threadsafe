//
// Created by n8 on 3/7/18.
//

#ifndef THREADSAFE_MAP_H
#define THREADSAFE_MAP_H


#include <vector>
#include <mutex>
#include <map>
#include "math.hpp"


namespace ts
{

template<typename K, typename V>
class map
{

#define LOCK() std::lock_guard<std::mutex> lock(_mutex)

#define RETURN_FIND(KEY, BEGIN_FUNC, END_FUNC) \
    size_type low = 0; \
    size_type high = size_unlocked() - 1; \
    while (low <= high) \
    { \
        int index = (high + low) / 2; \
        const std::pair<K,V>& current_pair = _data[index]; \
        const std::pair<K,V>& low_pair = _data[low]; \
        const std::pair<K,V>& high_pair = _data[high]; \
        if(compare(KEY, current_pair.first) == 0) \
        { \
            return BEGIN_FUNC() + index; \
        } \
        else if(compare(current_pair.first, low_pair.first) < 0) \
        { \
            high = index - 1; \
        } \
        else\
        { \
            low = index + 1; \
        } \
    } \
    return END_FUNC() \


    public:
        typedef typename std::vector<std::pair<K,V>>::iterator iterator;
        typedef typename std::vector<std::pair<K,V>>::const_iterator const_iterator;
        typedef typename std::vector<std::pair<K,V>>::reverse_iterator reverse_iterator;
        typedef typename std::vector<std::pair<K,V>>::const_reverse_iterator const_reverse_iterator;
        typedef typename std::vector<std::pair<K,V>>::size_type size_type;

    public:
        map();
        map(size_type capacity);
        map(const map<K, V>& other);

        ~map();

        size_type size() const;

        iterator find(const K& key);
        const_iterator const_find(const K& key);

        iterator insert(const K& key, const V& value);

        iterator erase(const K &key);
        iterator erase(size_type index);

        V& operator[](const K& key);
        V& operator[](size_type index);

        const V& at(size_type index);

        iterator begin();
        const_iterator const_begin() const;

        iterator end();
        const_iterator const_end() const;

        iterator rbegin();
        const_iterator const_rbegin() const;

        iterator rend();
        const_iterator const_rend() const;

        map<K, V>& operator=(const map<K,V> rhs);

    private:
        size_type size_unlocked() const;

        int compare(const K& k1, const K& k2) const;

        iterator find_unlocked(const K &key);
        const_iterator const_find_unlocked(const K& key);

        iterator begin_unlocked();
        const_iterator const_begin_unlocked() const;

        iterator end_unlocked();
        const_iterator const_end_unlocked() const;

        iterator rbegin_unlocked();
        const_iterator const_rbegin_unlocked() const;

        iterator rend_unlocked();
        const_iterator const_rend_unlocked() const;

        iterator binary_insert_unlocked(const K &key, const V &value);

    private:
        std::vector<std::pair<K, V>> _data;
        mutable std::mutex _mutex;
    };


//////////////////////


    template<typename K, typename V>
    map<K, V>::map()
    {}

    template<typename K, typename V>
    map<K, V>::map(size_type capacity)
    {
        _data.reserve(capacity);
    }

    template<typename K, typename V>
    map<K, V>::map(const map<K, V>& other) :
        _data(other._data)
    {}

    template<typename K, typename V>
    map<K, V>::~map()
    {}


    template<typename K, typename V>
    typename map<K, V>::size_type map<K, V>::size() const
    {
        LOCK();
        return size_unlocked();
    }

    template<typename K, typename V>
    typename map<K, V>::iterator map<K, V>::find(const K& key)
    {
        LOCK();
        return find_unlocked(key);
    };

    template<typename K, typename V>
    typename map<K, V>::const_iterator map<K, V>::const_find(const K &key)
    {
        LOCK();
        return const_find_unlocked(key);
    }

    template<typename K, typename V>
    typename map<K, V>::iterator map<K, V>::insert(const K &key, const V &value)
    {
        LOCK();
        return binary_insert_unlocked(key, value);
    }

    template<typename K, typename V>
    typename map<K, V>::iterator map<K, V>::erase(const K& key)
    {
        LOCK();

        iterator it_end = end_unlocked();
        iterator it_target = find_unlocked(key);

        if(it_target != it_end)
            return _data.erase(it_target);

        return it_end;
    }

    template<typename K, typename V>
    typename map<K, V>::iterator map<K, V>::erase(size_type index)
    {
        LOCK();
        return _data.erase(begin_unlocked() + index);
    };


    template<typename K, typename V>
    V& map<K, V>::operator[](const K &key)
    {
        return find(key)->second;
    }

    template<typename K, typename V>
    V& map<K, V>::operator[](size_type index)
    {
        LOCK();
        return _data[index].second;
    }

    template<typename K, typename V>
    typename map<K, V>::iterator map<K,V>::begin()
    {
        LOCK();
        return begin_unlocked();
    }

    template<typename K, typename V>
    typename map<K, V>::iterator map<K,V>::end()
    {
        LOCK();
        return end_unlocked();
    }

    template<typename K, typename V>
    typename map<K, V>::iterator map<K, V>::rbegin()
    {
        LOCK();
        return rbegin_unlocked();
    }

    template<typename K, typename V>
    typename map<K, V>::iterator map<K, V>::rend()
    {
        LOCK();
        return rend_unlocked();
    }

    template<typename K, typename V>
    typename map<K, V>::const_iterator map<K, V>::const_begin() const
    {
        LOCK();
        return const_begin_unlocked();
    }

    template<typename K, typename V>
    typename map<K, V>::const_iterator map<K, V>::const_end() const
    {
        LOCK();
        return const_end_unlocked();
    }

    template<typename K, typename V>
    typename map<K, V>::const_iterator map<K, V>::const_rbegin() const
    {
        LOCK();
        return const_rbegin_unlocked();
    }

    template<typename K, typename V>
    typename map<K, V>::const_iterator map<K, V>::const_rend() const
    {
        LOCK();
        return const_rend_unlocked();
    }

    template<typename K, typename V>
    typename map<K, V>::size_type map<K, V>::size_unlocked() const
    {
        return _data.size();
    }

    template<typename K, typename V>
    typename map<K, V>::iterator map<K, V>::begin_unlocked()
    {
        return _data.begin();
    }

    template<typename K, typename V>
    typename map<K, V>::const_iterator map<K, V>::const_begin_unlocked() const
    {
        return _data.begin();
    }

    template<typename K, typename V>
    typename map<K, V>::iterator map<K, V>::end_unlocked()
    {
        return _data.end();
    }

    template<typename K, typename V>
    typename map<K, V>::const_iterator map<K, V>::const_end_unlocked() const
    {
        return _data.end();
    }

    template<typename K, typename V>
    typename map<K, V>::iterator map<K, V>::rbegin_unlocked()
    {
        return _data.rbegin();
    }

    template<typename K, typename V>
    typename map<K, V>::const_iterator map<K, V>::const_rbegin_unlocked() const
    {
        return _data.rbegin();
    }

    template<typename K, typename V>
    typename map<K, V>::iterator map<K, V>::rend_unlocked()
    {
        return _data.rend();
    }

    template<typename K, typename V>
    typename map<K, V>::const_iterator map<K, V>::const_rend_unlocked() const
    {
        return _data.rend();
    }

    template<typename K, typename V>
    int map<K, V>::compare(const K& k1, const K& k2) const
    {
        if(&k1 == &k2)
            return 0;

        return k1 < k2 ? -1 : k1 == k2 ? 0 : 1;
    }

    template<typename K, typename V>
    typename map<K, V>::iterator map<K, V>::find_unlocked(const K &key)
    {
        RETURN_FIND(key, begin_unlocked, end_unlocked);
    }

    template<typename K, typename V>
    typename map<K,V>::const_iterator map<K, V>::const_find_unlocked(const K &key)
    {
        RETURN_FIND(key, const_begin_unlocked, const_end_unlocked);
    }

    template<typename K, typename V>
    map<K, V> &map<K, V>::operator=(const map<K, V> rhs)
    {
        this->_data = rhs._data;
        return *this;
    }

    template<typename K, typename V>
    const V& map<K, V>::at(map::size_type index)
    {
        LOCK();
        return _data.at(index).second;
    }

    template<typename K, typename V>
    typename map<K, V>::iterator map<K, V>::binary_insert_unlocked(const K &key, const V &value)
    {
        size_type size = size_unlocked();
        bool empty = size == 0;

        size_type low = 0;
        size_type high = empty ? 0 : size - 1;

        size_type index = 0;

        if(!empty)
        {
            while (low <= high)
            {
                index = (high + low) / 2;

                std::pair<K, V>& current_pair = _data[index];
                const std::pair<K, V>& low_pair = _data[low];
                const std::pair<K, V>& high_pair = _data[high];

                int key_current_compared = compare(key, current_pair.first);

                if(low == high && key_current_compared != 0)
                    break;

                if (key_current_compared == 0) // this key already exists, overwrite it
                {
                    current_pair.first = key;
                    current_pair.second = value;

                    return begin() + index;
                }
                else if (key_current_compared < 0) // go left
                {
                    high = index > 0 ? index - 1 : 0;
                }
                else // go right
                {
                    low = index < size - 1 ? index + 1 : size - 1;
                }
            }

            low = clamp<size_type>(low, 0, size - 1);
            high = clamp<size_type>(high, 0, size - 1);
        }

        index = clamp<size_type>(index, 0, (high <= low) ? high : low);

        while(index < size && compare(key, _data[index].first) > 0)
            ++index;

        return _data.insert(begin_unlocked() + index, std::pair<K, V>(key, value));
    }
};

#endif //THREADSAFE_MAP_H
