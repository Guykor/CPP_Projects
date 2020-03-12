/**
 * @file HashMap.hpp
 * @author  Guy Kornblit
 *
 * @brief HashMap generic container.
 *
 */
// ------------------------------ includes ------------------------------

#include "iostream"
#include <array>
#include <vector>
#include <stdexcept>

// -------------------------- const definitions -------------------------
#define LOWER_LOAD_FACTOR 1.0/4
#define UPPER_LOAD_FACTOR 3.0/4
#define INIT_CAPACITY 16
#define MIN_CAPACITY 1
#define TABLE_SIZE_FACTOR 2

#define KEY_NOT_FOUND_MSG "The Key supplied isn't in map"
#define NO_ELEMS 0
#define MISMATCH_INPUT_LEN_MSG "HashMap: the input vectors should be in the same size."
#define FIRST_BUCKET 0


// ------------------------------ HashMap Definition -----------------------------

using namespace std;

/**
 * HashMap class template, that stores const pairs of keys and values.
 * the Map is implemented with simple hash function and open addressing method.
 * includes const forward iterator.
 * @tparam KeyT typename that supports comparing, std::hash operation and copy constructible.
 * @tparam ValueT KeyT typename that supports comparing, std::hash operation and copy constructible.
 */
template <typename KeyT, typename ValueT>
class HashMap
{
private:

    // -------------- Const Definitions -------------

    /**
     * element in map, implementation guarantees that the value won't be changed after
     * inserted to map (one exception with the vectors input constructor).
     */
    typedef std::pair<KeyT, ValueT> _pair;
    /**
     * buckets as part of the open address implementation.
     */
    typedef vector<_pair> Bucket;
    /**
     * an Object that stores a map element meta data, i.e reference to the pair and it's index
     * inside it's bucket.
     */
    typedef struct ElemMeta
    {
        _pair &elem;
        int idxInBucket;
    } ElemMeta;

    // -------------- members -------------
    /**
     * number of elements (pairs) stored in the instance.
     */
    int _nElements;
    /**
     * map capacity, i.e number of buckets.
     */
    int _capacity;
    /**
     * pointer to an array of buckets in heap.
     */
    Bucket *_hashTable;

    // -------------- Methods -------------
    /**
     * access to a pair element in the map based on it's key, if the key is in map, a reference
     * to pair will be returned, and the idx of the bucket will be updated in the arg supplied.
     * otherwise, if the key not in map, an exception will be thrown.
     * @param key - lookup value.
     * @throws out of range exception if the key isn't in the Map.
     * @return ElemMeta object by value, containing a reference to the pair and it's bucket idx.
     */
    ElemMeta _getPairByKey(const KeyT &key) const
    {
        Bucket &b = _hashTable[_hash(key)];
        for (int i = 0; i < (int)b.size(); i++)
        {
            if (b[i].first == key)
            {
                return ElemMeta{b[i], i};
            }
        }
        throw std::out_of_range(KEY_NOT_FOUND_MSG);
    }

    /**
     * hash a key and bind the value to a valid bucket idx.
     * @param key key to hash
     * @return int by value of the relevant idx in map.
     */
    int _hash(const KeyT& key) const noexcept
    {
        return std::hash<KeyT>()(key) & (capacity() - 1);
    }

    /**
     * this function resizes the array of buckets to a given capacity,
     * it do so by building a new HashMap with the new capacity, re-hashing all elements
     * accordingly and than setting the current instance with the temp instance fields.
     * @param newSize - given new size of the element, the function will check that this number
     * is valid (according to current status of the map).
     */
    void _resizeTable(int newSize)
    {
        if (size() <= newSize && newSize >= MIN_CAPACITY)
        {
            HashMap resized = HashMap(newSize);
            for (auto it = this->cbegin(); it != cend(); ++it)
            {
                resized.insert(it->first, it->second);
            }
            *this = std::move(resized);
        }
    }

    /**
     * constructor that build a map with a given size.
     * @param capacity - number
     */
    HashMap(const int capacity)
    : _nElements(NO_ELEMS), _capacity(capacity), _hashTable(new Bucket[_capacity]) {}

    /**
     * assignment operator helper function, swaps between two HashMaps their members.
     * @param first hash map to move the second into
     * @param second hash map it's members we want to take.
     */
    friend void _swap(HashMap &first, HashMap &second) noexcept
    {
        std::swap(first._nElements, second._nElements);
        std::swap(first._capacity, second._capacity);
        std::swap(first._hashTable, second._hashTable);
    }

public:
    /**
     * Default constructor, construct an empty HashMap.
     */
    HashMap()
    : HashMap(INIT_CAPACITY){}

    /**
     * ctor that receives vector of keys and vector of values and insert them to HashMap
     * respectively.
     * if a key appears more than one time in the keys vector, the value linked to this key will
     * be override with the latter value.
     * @param keys vector of type KeyT
     * @param values vector of type ValueT
     */
    HashMap(const vector<KeyT>& keys, const vector<ValueT> &values)
    : HashMap(INIT_CAPACITY)
    {
        if (keys.size() != values.size())
        {
            throw std::invalid_argument(MISMATCH_INPUT_LEN_MSG);
        }
        for (int i = 0; i < (int)keys.size(); ++i)
        {
            (*this)[keys[i]] = values[i];
        }
    }

    /**
     * copy ctor.
     * assumes that there is only one key per value (every pair is unique).
     */
    HashMap(const HashMap &other)
    : _nElements(other.size()), _capacity(other.capacity()),
        _hashTable(new Bucket[other.capacity()])
    {
        for (int i = 0; i < other.capacity(); ++i)
        {
            _hashTable[i] = other._hashTable[i];
        }
    }

    /**
     * move copy ctor.
     */
     HashMap(HashMap && other) noexcept
    {
        _hashTable = other._hashTable;
        other._hashTable = nullptr;
        _nElements = other.size();
        _capacity = other.capacity();
    }

    /**
     * HashMap destructor.
     */
    ~HashMap()
    {
         delete[] _hashTable;
    }

    /**
     * assignment operator implementation using copy-swap idiom
     * @param other object to copy from.
     * @return reference of the Map instance after assignment.
     */
    HashMap& operator=(HashMap other)
    {
        _swap(*this, other);
        return *this;
    }

    /**
     * @return the number of key,value pairs in the HashMap.
     */
    int size() const noexcept {return _nElements; }
    /**
     * @return capacity of the HashMap.
     */
    int capacity() const noexcept {return _capacity; }
    /**
     * @return bool value stating if the HashMap is empty.
     */
    bool empty() const noexcept {return _nElements == 0; }

    /**
     * Gets a key and a value and inserts them in the HashMap
     * @param key of type KeyT
     * @param value of type ValueT
     * @return true if insertion was successful, false otherwise (if the key already in map).
     */
    bool insert(const KeyT &key, const ValueT &value) noexcept
    {
        if (containsKey(key))
        {
            return false;
        }

        _nElements++;
        _hashTable[_hash(key)].push_back({key, value}); //copy or move key and val to buck.
        if (getLoadFactor() > UPPER_LOAD_FACTOR)
        {
            _resizeTable(capacity() * TABLE_SIZE_FACTOR);
        }
        return true;
    }

    /**
     * gets a key and checks if it exist in the HashMap.
     * @param key KeyT type
     * @return bool stating if the Hashmap contains the key.
     */
    bool containsKey(const KeyT &key) const noexcept
    {
        try
        {
            _getPairByKey(key);
            return true;
        }
        catch (std::out_of_range& e)
        {
            return false;
        }
    }

    /**
     * Gets a Key and returns the value linked to it in the map, if the key exists in Map.
     * @param key KeyT
     * @throws if the Key isn't in the map.
     * @return reference to the value linked to the key.
     */
    ValueT& at(const KeyT &key)
    {
        return _getPairByKey(key).elem.second;
    }

    /**
     * Gets a Key and returns the value linked to it in the map, if the key exists in Map.
     * @param key KeyT
     * @throws if the Key isn't in the map.
     * @return const reference to the value linked to the key.
     */
    const ValueT& at(const KeyT &key) const
    {
        return _getPairByKey(key).elem.second;
    }

    /**
     * gets a key and erases it and value linked to it.
     * @param key KeyT
     * @return true if removal was successful, false otherwise.
     */
    bool erase(const KeyT key) noexcept
    {
        try
        {
            int bucketIdx = _hash(key);
            auto it = _hashTable[bucketIdx].begin();
            _hashTable[bucketIdx].erase(it + _getPairByKey(key).idxInBucket);
            --_nElements;
            if (getLoadFactor() < LOWER_LOAD_FACTOR)
            {
                _resizeTable(capacity() / TABLE_SIZE_FACTOR);
            }
            return true;
        }
        catch (std::out_of_range &e)
        {
            return false;
        }
    }

    /**
     * returns the current LoadFactor status
     * @return load factor of type double.
     */
    double getLoadFactor() const noexcept
    {
        return (size() / (double) capacity());
    }

    /**
     * gets a key and returns the size (num of elems) in the bucket containing the key.
     * if the key isn't in the Map, an exceptions will be raised.
     * @param key KeyT
     * @return size of bucket of type int.
     */
    int bucketSize(const KeyT &key) const
    {
        return _hashTable[bucketIndex(key)].size();
    }

    /**
     * gets a key and returns the idx of the bucket the the key is in.
     * @throws an expection if the key is not in the Map.
     * @param key
     * @return idx of the bucket the the key is in.
     */
    int bucketIndex(const KeyT &key) const
    {
        if (!containsKey(key))
        {
            throw std::out_of_range(KEY_NOT_FOUND_MSG);
        }
        return _hash(key);
    }

    /**
     * removes all the elements in the map, capacity of the map will remain unchanged.
     */
    void clear() noexcept
    {
        for (int i = 0; i < capacity(); ++i)
        {
            _hashTable[i].clear();
        }
        _nElements = 0;
    }

    /**
     * access operator, if a given key isn't in map, a new pair containing the key with a default
     * value will be created and a reference to the value will be return (and will allow changing
     * it).
     * @param key - search value in map
     * @return reference to the value linked to the map, if the key doesn't exist in map, the new
     * pair default value ref will be returned.
     */
    ValueT& operator[](const KeyT& key) noexcept
    {
        try
        {
            return _getPairByKey(key).elem.second;
        }
        catch (std::out_of_range &e)
        {
            ValueT valueDefault = ValueT();
            insert(key, valueDefault);
            return _getPairByKey(key).elem.second;
        }
    }
    /**
     * const access operator, if a given key isn't in map, a random value reference will be
     * returned.
     * @param key - search value in map
     * @return reference to the value linked to the map
     */
    const ValueT& operator[](const KeyT& key) const noexcept
    {
        try
        {
            return _getPairByKey(key).elem.second;
        }
        catch (std::out_of_range &e)
        {
            return cbegin()->second;
        }
    }


    /**
     * checks if two HashMap instances have identical sets of values (key-value relation and same
     * number of elements)
     * @param other another hash map to compare with
     * @return true if the same, false otherwise.
     */
    bool operator==(const HashMap& other) const
    {
        if (size() == other.size())
        {
            try
            {
                for (auto it = other.cbegin(); it != other.cend(); ++it)
                {
                    if (_getPairByKey(it->first).elem.second != it->second)
                    {
                        return false;
                    }
                }
                return true;
            }
            catch (std::out_of_range &e)
            {
                return false;
            }
        }
        return false;
    }

    /**
     * returns true if there's a difference between the two maps in one of the pairs, of in the
     * number of elements stored in them.
     */
    bool operator!=(const HashMap& other) const
    {
        return !((*this) == other);
    }

    /**
     * Const Forward Iterator class to the hashMap.
     */
    class const_iterator
    {
    public:
        //iterator traits//
        typedef const_iterator self_type;
        typedef const _pair* pointer;
        typedef const _pair value_type;
        typedef const _pair& reference;
        typedef std::forward_iterator_tag iterator_category;
        typedef std::ptrdiff_t difference_type;

    private:
        typedef typename Bucket::const_iterator vectorIterator;


        /**
         * reference to the HashMap instance linked to this Iterator.
         */
        const HashMap &_map;
        /**
         * current Bucket that the iterator is in.
         */
        int _currBucket;
        /**
         * pointer to the current bucket in the map, kept as a member to save a lot of accesses
         * in map.
         */
        Bucket *_bucketPtr = nullptr;
        /**
         * pointer to an pair<KeyT, ValueT> element in the map.
         */
        vectorIterator _pairPtr;

        /**
         * sets the _bucketPtr to the first bucket in HashMap that is not empty,
         * and updates the index of that bucket to _currBucket;
         */
        void _nextBucket()
        {
            for (int i = ++_currBucket; i < _map.capacity(); ++i)
            {
                if (!(_map._hashTable[i]).empty())
                {
                    _bucketPtr = &(_map._hashTable[i]);
                    _pairPtr = _bucketPtr->cbegin();
                    _currBucket = i;
                    return;
                }
            }
            _setEnd();
        }
        /**
         * this function progresses the member of the iterator to the next pair.
         * this function makes sure that the iterator won't jump over to the next bucket unless
         * we've reached all pairs stored in the current bucket.
         */
        void _nextPair()
        {
            if (++_pairPtr == _bucketPtr->cend())
            {
                _nextBucket();
            }
        }

        /**
         * Sets the end point of the iterator by default to the last bucket end() iterator.
         * @return ref to the iterator with updated members.
         */
        self_type& _setEnd()
        {
            _currBucket = _map.capacity() - 1;
            _bucketPtr = &_map._hashTable[_currBucket];
            _pairPtr = _bucketPtr->cend();
            return *this;
        }


    public:

        /**
         * finds the first occupied bucket in the HashMap and sets the iterator to point
         * at it's first element (pair).
         * if the Map is empty than the iterator will point to the cend().
         */
        const_iterator(const HashMap& map, int startFromBucket) noexcept
        : _map(map), _currBucket(startFromBucket - 1)
        {
            if (!map.empty())
            {
                _nextBucket();
            }
            else
            {
                _setEnd();
            }
        }

        /**
         * returns a reference to a pair
         */
        reference operator*() const { return *_pairPtr; }

        /**
         * returns a pointer to a pair.
         */
        pointer operator->() const { return _pairPtr.operator->(); }

        /**
         * pre increment, i.e move iterator so it will point to the following element in map.
         */
        self_type& operator++()
        {
            _nextPair();
            return *this;
        }

        /**
         * post increment to the following pair.
         */
        self_type operator++(int)
        {
            const_iterator tmp = *this; // based on std::vector::iterator "operator=" overload.
            _nextPair();
            return tmp;
        }

        /**
         * comparison between two iterators.
         * @return true iff the iterators points on the same element in map.
         */
        bool operator==(const const_iterator &other) const
        {
            return _pairPtr == other._pairPtr;
        }
        /**
         * @return true iff the iterators points to different elements.
         */
        bool operator!=(const const_iterator &other) const
        {
            return _pairPtr != other._pairPtr;
        }
    };

    /**
     * returns const_iterator points to the first Bucket-chronological-order element in map.
     */
    const_iterator begin() const noexcept
    {
        return const_iterator(*this, 0);
    }

    /**
    * returns const_iterator points to the last Bucket-chronological-order element in map.
    */
    const_iterator end() const noexcept
    {
        return const_iterator(*this, capacity());
    }

    /**
    * returns const_iterator points to the first Bucket-chronological-order element in map.
    */
    const_iterator cbegin() const noexcept
    {
        return const_iterator(*this, 0);
    }
    /**
    * returns const_iterator points to the last Bucket-chronological-order element in map.
    */
    const_iterator cend() const noexcept
    {
        return const_iterator(*this, capacity());
    }
};


