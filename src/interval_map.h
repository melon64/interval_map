#ifndef INTERVAL_MAP_H
#define INTERVAL_MAP_H

#include <map>
#include <cassert>
#include <iostream>

template<typename K, typename V>
class interval_map {
    friend void IntervalMapTest();
    friend void RandomTest();

    V m_valBegin;
    std::map<K, V> m_map;

public:
    interval_map(V const& val);

    void assign(K const& keyBegin, K const& keyEnd, V const& val);

    V const& operator[](K const& key) const;

private:
    void _verify(const K& keyBegin, const K& keyEnd, const V& val) const;
};

#include "interval_map.tpp"

#endif
