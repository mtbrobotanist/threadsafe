//
// Created by n8 on 3/7/18.
//

#ifndef THREADSAFE_MAP_H
#define THREADSAFE_MAP_H


#include <vector>
#include <mutex>
#include "math.hpp"


namespace ts
{

template<typename K, typename V>
class map
{

#define RETURN_FIND_UNLOCKED(KEY, IT_BEGIN_UNLOCKED, IT_END_UNLOCKED) \
    size_type low = 0; \
    size_type high = size_unlocked() - 1; \
    while (low <= high) \
    { \
        size_type index = (high + low) / 2; \
        const value_type& current_pair = _data[index]; \
        const value_type& low_pair = _data[low]; \
        const value_type& high_pair = _data[high]; \
        int key_current_compared = compare(KEY, current_pair.first); \
        if(key_current_compared == 0) \
        { \
            return IT_BEGIN_UNLOCKED + index; \
        } \
        else if(key_current_compared < 0) \
        { \
            high = index - 1; \
        } \
        else \
        { \
            low = index + 1; \
        } \
    } \
    return IT_END_UNLOCKED; \

    typedef std::lock_guard<std::mutex> scoped_lock;

    public:
        typedef typename std::pair<K, V> value_type;
        typedef typename std::vector<value_type>::iterator iterator;
        typedef typename std::vector<value_type>::const_iterator const_iterator;
        typedef typename std::vector<value_type>::reverse_iterator reverse_iterator;
        typedef typename std::vector<value_type>::const_reverse_iterator const_reverse_iterator;
        typedef typename std::vector<value_type>::size_type size_type;

    public:
        map();
        map(size_type capacity);
        map(const map<K, V>& other);
        map(const std::initializer_list<value_type>& list);

        ~map();

        map<K, V>& operator=(const map<K, V> rhs);
        bool operator==(const map<K, V>& rhs) const;
        V& operator[](size_type index);

        size_type capacity() const;
        void reserve(size_type);
        size_type size() const;

        V& get(const K& key);

        iterator find(const K& key);
        const_iterator const_find(const K& key) const;

        iterator insert(const K& key, const V& value);

        iterator erase(const K &key);
        iterator erase(size_type index);

        const V& at(size_type index);

        iterator begin();
        const_iterator const_begin() const;

        iterator end();
        const_iterator const_end() const;

        iterator rbegin();
        const_iterator const_rbegin() const;

        iterator rend();
        const_iterator const_rend() const;

    private:
        size_type capacity_unlocked() const;
        size_type size_unlocked() const;

        int compare(const K& k1, const K& k2) const;

        iterator find_unlocked(const K &key);
        const_iterator const_find_unlocked(const K& key) const;

        iterator begin_unlocked();
        const_iterator const_begin_unlocked() const;

        iterator end_unlocked();
        const_iterator const_end_unlocked() const;

        iterator rbegin_unlocked();
        const_iterator const_rbegin_unlocked() const;

        iterator rend_unlocked();
        const_iterator const_rend_unlocked() const;

        iterator binary_insert_unlocked(const K &key, const V &value);

        K& deref_key(K &k) const { return k; }
        K& deref_key(K *k) const { return *k; }
        const K& deref_key(const K &k) const { return k; }
        const K& deref_key(const K *k) const { return *k; }

    private:
        std::vector<value_type> _data;
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
    map<K, V>::map(const std::initializer_list<map::value_type>& list)
    {
        scoped_lock lock(_mutex);
        for(auto& it : list)
        {
            binary_insert_unlocked(it.first, it.second);
        }
    }

    template<typename K, typename V>
    map<K, V>::~map()
    {}

    template<typename K, typename V>
    map<K, V> &map<K, V>::operator=(const map<K, V> rhs)
    {
        this->_data = rhs._data;
        return *this;
    }

    template<typename K, typename V>
    bool map<K, V>::operator==(const map<K, V> & rhs) const
    {
        scoped_lock lock(_mutex);
        return this->_data == rhs._data;
    }

    template<typename K, typename V>
    V& map<K, V>::operator[](size_type index)
    {
        scoped_lock lock(_mutex);
        return _data[index].second;
    }


    template<typename K, typename V>
    typename map<K, V>::size_type map<K, V>::capacity() const
    {
        scoped_lock lock(_mutex);
        return capacity_unlocked();
    }

    template<typename K, typename V>
    void map<K, V>::reserve(map::size_type capacity)
    {
        _data.reserve(capacity);
    }

    template<typename K, typename V>
    typename map<K, V>::size_type map<K, V>::size() const
    {
        scoped_lock lock(_mutex);
        return size_unlocked();
    }

    template<typename K, typename V>
    V& map<K, V>::get(const K &key)
    {
        return find(key)->second;
    }

    template<typename K, typename V>
    typename map<K, V>::iterator map<K, V>::find(const K& key)
    {
        scoped_lock lock(_mutex);
        return find_unlocked(key);
    };

    template<typename K, typename V>
    typename map<K, V>::const_iterator map<K, V>::const_find(const K &key) const
    {
        scoped_lock lock(_mutex);
        return const_find_unlocked(key);
    }

    template<typename K, typename V>
    typename map<K, V>::iterator map<K, V>::insert(const K &key, const V &value)
    {
        scoped_lock lock(_mutex);
        return binary_insert_unlocked(key, value);
    }

    template<typename K, typename V>
    typename map<K, V>::iterator map<K, V>::erase(const K& key)
    {
        scoped_lock lock(_mutex);

        iterator it_end = end_unlocked();
        iterator it_target = find_unlocked(key);

        if(it_target != it_end)
            return _data.erase(it_target);

        return it_end;
    }

    template<typename K, typename V>
    typename map<K, V>::iterator map<K, V>::erase(size_type index)
    {
        scoped_lock lock(_mutex);
        return _data.erase(begin_unlocked() + index);
    };

    template<typename K, typename V>
    typename map<K, V>::iterator map<K, V>::begin()
    {
        scoped_lock lock(_mutex);
        return begin_unlocked();
    }

    template<typename K, typename V>
    typename map<K, V>::iterator map<K, V>::end()
    {
        scoped_lock lock(_mutex);
        return end_unlocked();
    }

    template<typename K, typename V>
    typename map<K, V>::iterator map<K, V>::rbegin()
    {
        scoped_lock lock(_mutex);
        return rbegin_unlocked();
    }

    template<typename K, typename V>
    typename map<K, V>::iterator map<K, V>::rend()
    {
        scoped_lock lock(_mutex);
        return rend_unlocked();
    }

    template<typename K, typename V>
    typename map<K, V>::const_iterator map<K, V>::const_begin() const
    {
        scoped_lock lock(_mutex);
        return const_begin_unlocked();
    }

    template<typename K, typename V>
    typename map<K, V>::const_iterator map<K, V>::const_end() const
    {
        scoped_lock lock(_mutex);
        return const_end_unlocked();
    }

    template<typename K, typename V>
    typename map<K, V>::const_iterator map<K, V>::const_rbegin() const
    {
        scoped_lock lock(_mutex);
        return const_rbegin_unlocked();
    }

    template<typename K, typename V>
    typename map<K, V>::const_iterator map<K, V>::const_rend() const
    {
        scoped_lock lock(_mutex);
        return const_rend_unlocked();
    }

    template<typename K, typename V>
    typename map<K, V>::size_type map<K, V>::capacity_unlocked() const
    {
        return _data.capacity();
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
        auto k1_d = deref_key(k1);
        auto k2_d = deref_key(k2);

        return k1_d < k2_d ? -1 : k1_d == k2_d ? 0 : 1;
    }

    template<typename K, typename V>
    typename map<K, V>::iterator map<K, V>::find_unlocked(const K &key)
    {
        size_type low = 0;
        size_type high = size_unlocked() - 1;

        while (low <= high)
        {
            size_type index = (high + low) / 2;

            const value_type& current_pair = _data[index];
            const value_type& low_pair = _data[low];
            const value_type& high_pair = _data[high];

            int key_current_compared = compare(key, current_pair.first);

            if(key_current_compared == 0)
            {
                return begin_unlocked() + index;
            }
            else if(key_current_compared < 0)
            {
                high = index - 1;
            }
            else
            {
                low = index + 1;
            }
        }
        return end_unlocked();
    }

    template<typename K, typename V>
    typename map<K, V>::const_iterator map<K, V>::const_find_unlocked(const K &key) const
    {
        size_type low = 0;
        size_type high = size_unlocked() - 1;

        while (low <= high)
        {
            size_type index = (high + low) / 2;

            const value_type& current_pair = _data[index];
            const value_type& low_pair = _data[low];
            const value_type& high_pair = _data[high];

            int key_current_compared = compare(key, current_pair.first);

            if(key_current_compared == 0)
            {
                return const_begin_unlocked() + index;
            }
            else if(key_current_compared < 0)
            {
                high = index - 1;
            }
            else
            {
                low = index + 1;
            }
        }
        return const_end_unlocked();
    }

    template<typename K, typename V>
    const V& map<K, V>::at(map::size_type index)
    {
        scoped_lock lock(_mutex);
        return _data.at(index).second;
    }

    template<typename K, typename V>
    typename map<K, V>::iterator map<K, V>::binary_insert_unlocked(const K& key, const V& value)
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

                value_type& current_pair = _data[index];
                const value_type& low_pair = _data[low];
                const value_type& high_pair = _data[high];

                int key_current_compared = compare(key, current_pair.first);

                if(low == high && key_current_compared != 0)
                    break;

                if (key_current_compared == 0) // this key already exists, overwrite it
                {
                    current_pair.first = key;
                    current_pair.second = value;

                    return begin_unlocked() + index;
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

        return _data.insert(begin_unlocked() + index, value_type(key, value));
    }
};

#endif //THREADSAFE_MAP_H
