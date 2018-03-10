//
// Created by n8 on 3/7/18.
//

#ifndef THREADSAFE_MAP_H
#define THREADSAFE_MAP_H


#include <vector>
#include <mutex>
#include <map>
#include <sstream>

namespace ts
{

    template<typename K, typename V>
    class map
    {

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

        std::string to_string() const;

        map<K, V>& operator=(const map<K,V> rhs);

    private:
        size_type size_unlocked() const;

        int compare(const K& k1, const K& k2) const;

        iterator find_unlocked(const K &key);

        iterator begin_unlocked();
        const_iterator const_begin_unlocked() const;

        iterator end_unlocked();
        const_iterator const_end_unlocked() const;

        iterator rbegin_unlocked();
        const_iterator const_rbegin_unlocked() const;

        iterator rend_unlocked();
        const_iterator const_rend_unlocked() const;

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
        std::lock_guard<std::mutex> lock(_mutex);
        return size_unlocked();
    }

    template<typename K, typename V>
    typename map<K, V>::iterator map<K, V>::find(const K& key)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        return find_unlocked(key);
    };

    template<typename K, typename V>
    typename map<K, V>::iterator map<K, V>::insert(const K &key, const V &value)
    {
        std::lock_guard<std::mutex> lock(_mutex);

        int index = 0;
        size_type size = size_unlocked();

        if(size > 0)
        {
            while(index < size && compare(_data[index].first, key) < 0)
                ++index;

            if(index < size && compare(_data[index].first, key) == 0)
            {
                _data[index] = std::pair<K,V>(key, value);
                return begin_unlocked() + index;
            }
        }

        return _data.insert(begin_unlocked() + index, std::pair<K, V>(key, value));
    }

    template<typename K, typename V>
    typename map<K, V>::iterator map<K, V>::erase(const K& key)
    {
        std::lock_guard<std::mutex> lock(_mutex);

        iterator it_end = end_unlocked();
        iterator it_target = find_unlocked(key);

        if(it_target != it_end)
            return _data.erase(it_target);

        return it_end;
    }

    template<typename K, typename V>
    typename map<K, V>::iterator map<K, V>::erase(size_type index)
    {
        std::lock_guard<std::mutex> lock(_mutex);
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
        std::lock_guard<std::mutex> lock(_mutex);
        return _data[index].second;
    }

    template<typename K, typename V>
    typename map<K, V>::iterator map<K,V>::begin()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        return begin_unlocked();
    }

    template<typename K, typename V>
    typename map<K, V>::iterator map<K,V>::end()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        return end_unlocked();
    }

    template<typename K, typename V>
    typename map<K, V>::iterator map<K, V>::rbegin()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        return rbegin_unlocked();
    }

    template<typename K, typename V>
    typename map<K, V>::iterator map<K, V>::rend()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        return rend_unlocked();
    }

    template<typename K, typename V>
    typename map<K, V>::const_iterator map<K, V>::const_begin() const
    {
        std::lock_guard<std::mutex> lock(_mutex);
        return const_begin_unlocked();
    }

    template<typename K, typename V>
    typename map<K, V>::const_iterator map<K, V>::const_end() const
    {
        std::lock_guard<std::mutex> lock(_mutex);
        return const_end_unlocked();
    }

    template<typename K, typename V>
    typename map<K, V>::const_iterator map<K, V>::const_rbegin() const
    {
        std::lock_guard<std::mutex> lock(_mutex);
        return const_rbegin_unlocked();
    }

    template<typename K, typename V>
    typename map<K, V>::const_iterator map<K, V>::const_rend() const
    {
        std::lock_guard<std::mutex> lock(_mutex);
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
        size_type low = 0;
        size_type high = _data.size() - 1;

        while (low <= high)
        {
            int index = (high + low) / 2;

            const std::pair<K,V>& current_pair = _data[index];
            const std::pair<K,V>& low_pair = _data[low];
            const std::pair<K,V>& high_pair = _data[high];

            if(compare(current_pair.first, key) == 0)
            {
                return begin_unlocked() + index;
            }
            else if(compare(current_pair.first, low_pair.first) < 0)
            {
                high = index - 1;
            }
            else //if(compare(current_pair.first, high_pair.first) > 0)
            {
                low = index + 1;
            }
        }

        return end_unlocked();
    }

    template<typename K, typename V>
    std::string map<K, V>::to_string() const
    {
        std::lock_guard<std::mutex> lock(_mutex);

        std::stringstream stream;

        stream << "[ ";
        for(auto it = const_begin_unlocked(); it != const_end_unlocked(); ++it)
        {
            stream << "(" + it->first << ", " << it->second << "), ";
        }

        std::string s = stream.str();

        int cut = size_unlocked() > 0 ? 2 : 1;

        return s.substr(0, s.size() - cut) + " ]";
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
        return _data.at(index).second;
    }

    template<typename K, typename V>
    map::const_iterator map<K, V>::const_find(const K &key)
    {
        return find(key);
    }
};

#endif //THREADSAFE_MAP_H
