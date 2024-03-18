#pragma once

//Constructor
template<typename K, typename V>
interval_map<K, V>::interval_map(V const& val) : m_valBegin(val) {}

// Assign
template<typename K, typename V>
void interval_map<K, V>::assign(K const& keyBegin, K const& keyEnd, V const& val) {
   using iterator = typename std::map<K, V>::iterator;

    if (!(keyBegin < keyEnd)) return;

    iterator itEnd = m_map.upper_bound(keyEnd);

    if(m_map.empty() || itEnd == m_map.begin()){ //empty edge case OR keyEnd < first key in map
        if(val == m_valBegin) return;
        iterator itFirst = m_map.insert_or_assign(m_map.begin(), keyEnd, m_valBegin);
        m_map.insert_or_assign(itFirst, keyBegin, val); //amortized O(1) since keyBegin is right before it1 (it1 is the first element in the map)
        return;
    }

    --itEnd; //valid only if itEnd != m_map.begin()
    V &oldV = itEnd->second; //value of the interval whose key <= keyEnd
    ++itEnd;

    if (oldV != val){ //cut off interval at keyEnd, else do nothing
        itEnd = m_map.try_emplace(itEnd, keyEnd, std::move(oldV)); //either an iterator to first element with key >= keyEnd or map.end()
    }

    iterator itBegin = m_map.lower_bound(keyBegin);
    if(itBegin == m_map.begin()){ //keyBegin < first key in map or keyBegin == first key in map
        if(val != m_valBegin){
            itBegin = m_map.insert_or_assign(itBegin, keyBegin, val);
            ++itBegin; //start of erase
        } 
    }
    else{ //there is a key in the map that is <= keyBegin
        --itBegin;
        if(itBegin->second != val){
            ++itBegin;
            itBegin = m_map.insert_or_assign(itBegin, keyBegin, val); //itBegin points to the first interval that covers keyBegin and value is val

        }
        ++itBegin; //start of erase
    }
    m_map.erase(itBegin, itEnd);
}

// Operator[]
template<typename K, typename V>
V const& interval_map<K, V>::operator[](K const& key) const {
    auto it=m_map.upper_bound(key);
    if(it==m_map.begin()) {
        return m_valBegin;
    } else {
        return (--it)->second;
    }
}

// _verify (private method, used for internal checks, can be removed if not needed)
template<typename K, typename V>
void interval_map<K, V>::_verify(const K& keyBegin, const K& keyEnd, const V& val) const {
    if ( m_map.empty() ){
        assert ( val == m_valBegin );
        return;
    };

    //1. first entry should not be m_valBegin
    assert( m_map.begin()->second != m_valBegin );
    
    auto it = m_map.begin();
    V prev_val = it->second;
    while ( ++it != m_map.end() )
    {
        //2. Adjacent intervals should have different values.
        assert( prev_val != it->second );
        prev_val = it->second;
    }

    auto it0 = --m_map.upper_bound(keyBegin);
    auto it1 = --m_map.lower_bound(keyEnd);
    //3. There should be only one interval that covers [keyBegin, keyEnd).
    assert( it0 == it1 );

    //4. The interval should have the desired value.
    assert( val == it0->second || m_map.upper_bound(keyBegin) == m_map.begin() );

    //5. Canonical representation: no empty intervals
    it = m_map.begin();
    while ( it != m_map.end() )
    {
        auto it_next = it;
        ++it_next;
        if ( it_next != m_map.end() )
        {
            // 6. No empty intervals.
            assert( it->first < it_next->first );
        }
        ++it;
    }
}
