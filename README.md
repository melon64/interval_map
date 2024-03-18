
# interval_map

An ADT implementation of an [interval_map](https://download.racket-lang.org/docs/5.0/html/unstable/interval-map.html) in C++ with included unit testing with GTest.




## Run Locally

Clone the project

```bash
  git clone https://github.com/melon64/interval_map.git
```

Go to the project directory

```bash
  cd interval_map
```

Create a build directory and build using CMake

```bash
  mkdir build
  cd build
  cmake .. -G "MinGW Makefiles"
```

Run the tests
```bash
  cd build/test
  runUnitTests.exe

```


## Installation and Usage

Assuming repo has been cloned and built:

```bash
  cd build
  make install
```
Make sure you have administrator privileges. 

Then include as so in your local project's CMakeLists.txt:
```bash
  include_directories({path to installation}) #eg. C:/Program Files (x86)/interval_map/include
```
and simply declare the header in your project:
```C++
  #include "interval_map.h"
  //your project code
```



## API Reference
### Requirements/Notes
- V must be comparable by less (<)
- K must be comparable by equality (==)
- K and V do not have to be default constructible
#### constructor

```c++
  V v;
  interval_map<K, V> imap(v);
```

| Parameter | Type     | Description                |
| :-------- | :------- | :------------------------- |
| `m_valBegin` | `any` | Constructs an interval map with m_valBegin at [`numeric_limits<K>::min()`, `numeric_limits<K>::max()`]|

#### assign(K keyBegin, K keyEnd, V val);

Takes the start and end range for an interval and assigns val across it. 
**O(N)**

```c++
  V v1, v2;
  interval_map<K, V> imap(v1);
  K begin, end;
  imap.assign(begin, end, v2);
```

| Parameter | Type     | Description                        |
| :-------- | :------- | :--------------------------------  |
| `keyBegin`| `any` | Start of interval (inclusive)         |
| `keyEnd`  | `any` | End of interval (exclusive)           |
| `val`     | `any` | val inserted at [`keyBegin`, `keyEnd`)|


#### &operator[] (K key)

Returns the value of the interval that key is contained in. **O(logN)**

```c++
  V v1;
  interval_map<K, V> imap(v1);
  K k;
  cout << imap[k]; //returns v1
```

| Parameter | Type     | Description                |
| :-------- | :------- | :------------------------- |
| `key` | `any` | Returns the value of the interval that key in within.|

### Example Usage
```c++
interval_map<int, char> imap('A');
imap.assign(1, 3, 'B');
imap.assign(2, 4, 'C');
assert(imap[0], 'A');
assert(imap[1], 'B');
assert(imap[2], 'C');
assert(imap[3], 'C');
assert(imap[4], 'A');
```


