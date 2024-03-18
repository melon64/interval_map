#include <map>
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <ctime>

template<typename K, typename V>
class interval_map {
	friend void IntervalMapTest();
	friend void RandomTest();
	V m_valBegin;
	std::map<K,V> m_map;
public:
	// constructor associates whole range of K with val
	interval_map(V const& val)
	: m_valBegin(val)
	{}

	// Assign value val to interval [keyBegin, keyEnd).
	// Overwrite previous values in this interval.
	// Conforming to the C++ Standard Library conventions, the interval
	// includes keyBegin, but excludes keyEnd.
	// If !( keyBegin < keyEnd ), this designates an empty interval,
	// and assign must do nothing.
	void assign( K const& keyBegin, K const& keyEnd, V const& val ) {
		using iterator = typename std::map<K, V>::iterator;

		if (!(keyBegin < keyEnd)) return;

		iterator itEnd = m_map.upper_bound(keyEnd);

		if(m_map.empty() || itEnd == m_map.begin()){ //empty edge case OR keyEnd < first key in map
			if(val == m_valBegin) return;
			iterator itFirst = m_map.insert_or_assign(m_map.begin(), keyEnd, m_valBegin);
			m_map.insert_or_assign(itFirst, keyBegin, val); //amortized O(1) since keyBegin is right before it1 (it1 is the first element in the map)
			// _verify(keyBegin, keyEnd, val);
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

		// _verify(keyBegin, keyEnd, val);
	}

	// look-up of the value associated with key
	V const& operator[]( K const& key ) const {
		auto it=m_map.upper_bound(key);
		if(it==m_map.begin()) {
			return m_valBegin;
		} else {
			return (--it)->second;
		}
	}
	void _verify(const K& keyBegin, const K& keyEnd, const V& val) const
	{
		std::cout << "Verifying: " << keyBegin << " " << keyEnd << " " << val << std::endl;
		if ( m_map.empty() ){
			assert ( val == m_valBegin );
			return;
		};

		// 1. first entry should not be m_valBegin
		assert( m_map.begin()->second != m_valBegin );
		
		auto it = m_map.begin();
		V prev_val = it->second;
		while ( ++it != m_map.end() )
		{
			// 2. Adjacent intervals should have different values.
			assert( prev_val != it->second );
			prev_val = it->second;
		}

		auto it0 = --m_map.upper_bound(keyBegin);
		auto it1 = --m_map.lower_bound(keyEnd);
		// 3. There should be only one interval that covers [keyBegin, keyEnd).
		assert( it0 == it1 );

		// 4. The interval should have the desired value.
		assert( val == it0->second || m_map.upper_bound(keyBegin) == m_map.begin() );

		// 5. Canonical representation: no empty intervals, no adjacent intervals with the same value.
		it = m_map.begin();
		while ( it != m_map.end() )
		{
			auto it_next = it;
			++it_next;
			if ( it_next != m_map.end() )
			{
				// 6. No empty intervals.
				assert( it->first < it_next->first );

				// 7. No adjacent intervals with the same value.
				assert( it->second != it_next->second );
			}
			++it;
		}
	}
};

/*
 Do not make big-O more operations on K and V than necessary because you do not know how fast operations on K/V are; 
 remember that constructions, destructions and assignments are operations as well.
*/

/*
 Do not make more than one operation of amortized O(log N), in contrast to O(1), running time, where N is the number of elements in m_map.
*/

void IntervalMapTest() {
    // Test 1: Basic functionality
    interval_map<int, char> imap('A');
    imap.assign(1, 3, 'B');
    assert(imap[0] == 'A');
    assert(imap[1] == 'B');
    assert(imap[2] == 'B');
    assert(imap[3] == 'A');

    // Test 2: Overlapping intervals
    imap.assign(2, 4, 'C');
    assert(imap[0] == 'A');
    assert(imap[1] == 'B');
    assert(imap[2] == 'C');
    assert(imap[3] == 'C');
    assert(imap[4] == 'A');

    // Test 3: Empty intervals
    imap.assign(5, 5, 'D');
    assert(imap[4] == 'A');
    assert(imap[5] == 'A');

    // Test 4: Insert at the beginning, but end is existing interval
	imap.assign(-5, 1, 'E');
	assert(imap[-5] == 'E');
	assert(imap[0] == 'E');
	assert(imap[1] == 'B');
	assert(imap[2] == 'C');
	assert(imap[3] == 'C');
	assert(imap[4] == 'A');
	assert(imap[5] == 'A');

	// Test 5: Insert at the beginning, but end doesn't exist
	imap.assign(-8, -6, 'F');
	assert(imap[-8] == 'F');
	assert(imap[-7] == 'F');
	assert(imap[-6] == 'A');
	assert(imap[-5] == 'E');

	// Test 6: Insert at beginning, but value is same as m_valBegin and end exists
	imap.assign(-20, -7, 'A');
	assert(imap[-20] == 'A');
	assert(imap[-7] == 'F');
	assert(imap[-6] == 'A');

	// Test 7: Insert at beginning, but value is same as m_valBegin and end doesn't exist
	imap.assign(-20, -19, 'A');
	assert(imap[-20] == 'A');
	assert(imap[-19] == 'A');
	assert(imap[-7] == 'F');

	// Test 8: Insert at beginning, but end is before first interval
	imap.assign(-20, -10, 'G');
	assert(imap[-20] == 'G');
	assert(imap[-10] == 'A');
	assert(imap[-7] == 'F');

	// Test 9: Insert at end, but value is same as m_valBegin
	imap.assign(5, 10, 'A');
	assert(imap[5] == 'A');
	assert(imap[10] == 'A');
	assert(imap[4] == 'A');
	assert(imap[3] == 'C');

	// Test 10: Insert at end, but value is same as existing interval
	imap.assign(4, 10, 'C');
	assert(imap[4] == 'C');
	assert(imap[10] == 'A');
	assert(imap[3] == 'C');

	// Test 11: Insert at start, but value is same as existing interval
	imap.assign(-13, -9, 'F');
	assert(imap[-13] == 'F');
	assert(imap[-9] == 'A');
	assert(imap[-14] == 'G');

	// Test 12: Override whole map without default value
	imap.assign(-100, 100, 'Z');
	for (int i = -100; i < 100; i++) {
		assert(imap[i] == 'Z');
	}
	assert(imap[100] == 'A');

	// Test 13: Override whole map with default value
	imap.assign(-100, 100, 'A');
	for (int i = -200; i < 200; i++) {
		assert(imap[i] == 'A');
	}

	for(auto it = imap.m_map.begin(); it != imap.m_map.end(); ++it){
		std::cout << it->first << " " << it->second << std::endl;
	}

	imap.assign(-9996, -9983, 'D');
	imap.assign(-9983, -9944, 'C');
	imap.assign(-9944, -9910, 'O');
	imap.assign(-9910, -9882, 'Z');
	imap.assign(-9882, -9668, 'S');
	imap.assign(-9668, -9351, 'O');
	imap.assign(-9351, -8073, 'K');
	imap.assign(-8073, 1141, 'A');
	imap.assign(1141, 3410, 'J');
	imap.assign(3410, 4051, 'R');
	imap.assign(4051, 6557, 'D');
	imap.assign(6557, 6640, 'L');
	imap.assign(6640, 8094, 'Y');
	imap.assign(8094, 9019, 'H');
	imap.assign(9019, 17254, 'M');
	imap.assign(17254, 17641, 'O');
	imap.assign(17641, 18676, 'G');
	imap.assign(18676, 18728, 'K');
	imap.assign(18728, 18875, 'X');
	imap.assign(18875, 18895, 'R');
	imap.assign(18895, 19542, 'L');
	imap.assign(19542, INT_MAX, 'A'); // Assign the rest of the range to 'A'

	imap.assign(-7028, -7028, 'E');

	for(auto it = imap.m_map.begin(); it != imap.m_map.end(); ++it){
		std::cout << it->first << " " << it->second << std::endl;
	}

	std::cout<<"All tests passed!\n";
}

void RandomTest(){
	interval_map<int, char> imap('A');
	srand(time(0));
	for (int i = 0; i < 100000; i++) {
		int start = rand() % 20001 - 10000;
		int end = start + rand() % 20001 - 10000;
		char val = 'A' + rand() % 26;
		imap.assign(start, end, val);
		for(auto it = imap.m_map.begin(); it != imap.m_map.end(); ++it){
			std::cout << it->first << " " << it->second << std::endl;
		}
		if (start < end) imap._verify(start, end, val);
	}

	for (char c = 'Z'; c >= 'A'; c--) {
		imap.assign(-1000000, 10000000, c);
		imap._verify(-1000000, 10000000, c);
		std::cout << "replace all:" << std::endl;
		for(auto it = imap.m_map.begin(); it != imap.m_map.end(); ++it){
			std::cout << it->first << " " << it->second << std::endl;
		}
	}
	
}

int main() {
    IntervalMapTest();
	RandomTest();
    std::cout << "All tests passed!\n";
    return 0;
}
