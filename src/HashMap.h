#pragma once

#include <stdexcept>
#include <cfloat>
#include <cmath>
#include <limits>
#include <set>
//https://github.com/openjdk-mirror/jdk7u-jdk/blob/master/src/share/classes/java/util/HashMap.java
template<typename K, typename V>
class HashMap;

template<typename K, typename V>
class Entry {
public:
    K *key;
    V *value;
    Entry<K, V> *next;
    const int hash;

public:
    /**
     * Creates new entry.
     */
    Entry(int h, K *k, V *v, Entry<K, V> *n) : hash(h) {
        value = v;
        next = n;
        key = k;
    }


    K *getKey() {
        return key;
    }

    V *getValue() {
        return value;
    }

    V *setValue(V *newValue) {
        V *oldValue = value;
        value = newValue;
        return oldValue;
    }

    bool equals(Entry *e) {
        K *k1 = getKey();
        K *k2 = e->getKey();
        if (k1 == k2 || (k1 != nullptr && k1->equals(k2))) {
            V *v1 = getValue();
            V *v2 = e->getValue();
            if (v1 == v2 || (v1 != nullptr && v1->equals(v2)))
                return true;
        }
        return false;
    }

    int hashCode() {
        return (key == nullptr ? 0 : key->hashCode()) ^
               (value == nullptr ? 0 : value->hashCode());
    }

    /**
     * This method is invoked whenever the value in an entry is
     * overwritten by an invocation of put(k,v) for a key k that's already
     * in the HashMap.
     */
    void recordAccess(HashMap<K, V> *m) {
    }

    /**
     * This method is invoked whenever the entry is
     * removed from the table.
     */
    void recordRemoval(HashMap<K, V> m) {
    }
};


class IllegalArgumentException final : public std::invalid_argument {
public:
    explicit IllegalArgumentException(const std::string &message) :
            std::invalid_argument(message) {}
};

template<typename K, typename V>
class HashMap final {

    /**
     * The default initial capacity - MUST be a power of two.
     */
    static constexpr int DEFAULT_INITIAL_CAPACITY = 16;

    /**
     * The maximum capacity, used if a higher value is implicitly specified
     * by either of the constructors with arguments.
     * MUST be a power of two <= 1<<30.
     */
    static constexpr int MAXIMUM_CAPACITY = 1 << 30;

    /**
     * The load factor used when none specified in constructor.
     */
    static constexpr float DEFAULT_LOAD_FACTOR = 0.75f;

    /**
     * The table, resized as necessary. Length MUST Always be a power of two.
     */
    Entry<K, V> ** table;
    std::size_t tableLength;

    /**
     * The number of key-value mappings contained in this map.
     */
    int size{};

    /**
     * The next size value at which to resize (capacity * load factor).
     * @serial
     */
    int threshold;

    /**
     * The load factor for the hash table.
     *
     * @serial
     */
    float loadFactor{}; // Todo const

    /**
     * The number of times this HashMap has been structurally modified
     * Structural modifications are those that change the number of mappings in
     * the HashMap or otherwise modify its internal structure (e.g.,
     * rehash).  This field is used to make iterators on Collection-views of
     * the HashMap fail-fast.  (See ConcurrentModificationException).
     */
    int modCount{};

public:

    static Entry<K, V>** makeTable(std::size_t size) {
        auto** table = new Entry<K, V>*[size];
        memset(table, 0, sizeof(Entry<K, V>**) * size);
        return table;
    }
    /**
     * Constructs an empty <tt>HashMap</tt> with the specified initial
     * capacity and load factor.
     *
     * @param  initialCapacity the initial capacity
     * @param  loadFactor      the load factor
     * @throws IllegalArgumentException if the initial capacity is negative
     *         or the load factor is nonpositive
     */
    HashMap(int initialCapacity, float loadFactor) {
        if (initialCapacity < 0)
            throw IllegalArgumentException(std::string("Illegal initial capacity: ") + std::to_string(initialCapacity));
        if (initialCapacity > MAXIMUM_CAPACITY)
            initialCapacity = MAXIMUM_CAPACITY;
        if (loadFactor <= 0 || std::isnan(loadFactor))
            throw IllegalArgumentException(std::string("Illegal load factor: ") +
                                           std::to_string(loadFactor));

        // Find a power of 2 >= initialCapacity
        int capacity = 1;
        while (capacity < initialCapacity)
            capacity <<= 1;

        this->loadFactor = loadFactor;
        threshold = (int) (capacity * loadFactor);
        table = makeTable(capacity);
        tableLength = capacity;
        init();
    }

    /**
     * Constructs an empty <tt>HashMap</tt> with the specified initial
     * capacity and the default load factor (0.75).
     *
     * @param  initialCapacity the initial capacity.
     * @throws IllegalArgumentException if the initial capacity is negative.
     */
    HashMap(int initialCapacity) : HashMap(initialCapacity, DEFAULT_LOAD_FACTOR) {}

    /**
     * Constructs an empty <tt>HashMap</tt> with the default initial capacity
     * (16) and the default load factor (0.75).
     */
    HashMap() {
        this->loadFactor = DEFAULT_LOAD_FACTOR;
        threshold = (int) (DEFAULT_INITIAL_CAPACITY * DEFAULT_LOAD_FACTOR);
        table = makeTable(DEFAULT_INITIAL_CAPACITY);
        tableLength = DEFAULT_INITIAL_CAPACITY;
        init();
    }

    /**
     * Constructs a new <tt>HashMap</tt> with the same mappings as the
     * specified <tt>Map</tt>.  The <tt>HashMap</tt> is created with
     * default load factor (0.75) and an initial capacity sufficient to
     * hold the mappings in the specified <tt>Map</tt>.
     *
     * @param   m the map whose mappings are to be placed in this map
     * @throws  NullPointerException if the specified map is null
     */
    HashMap(HashMap<K, V> &m) : HashMap(std::max((int) (m.size() / DEFAULT_LOAD_FACTOR) + 1,
                                                 DEFAULT_INITIAL_CAPACITY), DEFAULT_LOAD_FACTOR) {
        //putAllForCreate(m);
    }

    // internal utilities

    /**
     * Initialization hook for subclasses. This method is called
     * in all constructors and pseudo-constructors (clone, readObject)
     * after HashMap has been initialized but before any entries have
     * been inserted.  (In the absence of this method, readObject would
     * require explicit knowledge of subclasses.)
     */
    void init() {
    }

    /**
     * Applies a supplemental hash function to a given hashCode, which
     * defends against poor quality hash functions.  This is critical
     * because HashMap uses power-of-two length hash tables, that
     * otherwise encounter collisions for hashCodes that do not differ
     * in lower bits. Note: Null keys always map to hash 0, thus index 0.
     */
    static int hash(int h) {
        // This function ensures that hashCodes that differ only by
        // constant multiples at each bit position have a bounded
        // number of collisions (approximately 8 at default load factor).
        //Todo signed shift
        auto uh = (unsigned int) h;
        uh ^= (uh >> 20) ^ (uh >> 12);
        auto result = uh ^ (uh >> 7) ^ (uh >> 4);
        return (int)h;
    }

    /**
     * Returns index for hash code h.
     */
    static int indexFor(int h, int length) {
        return h & (length - 1);
    }

    /**
     * Returns the number of key-value mappings in this map.
     *
     * @return the number of key-value mappings in this map
     */
    int sizef() {    //Todo rename correctly.
        return size;
    }

    /**
     * Returns <tt>true</tt> if this map contains no key-value mappings.
     *
     * @return <tt>true</tt> if this map contains no key-value mappings
     */
    bool isEmpty() {
        return size == 0;
    }

    /**
     * Returns the value to which the specified key is mapped,
     * or {@code null} if this map contains no mapping for the key.
     *
     * <p>More formally, if this map contains a mapping from a key
     * {@code k} to a value {@code v} such that {@code (key==null ? k==null :
     * key.equals(k))}, then this method returns {@code v}; otherwise
     * it returns {@code null}.  (There can be at most one such mapping.)
     *
     * <p>A return value of {@code null} does not <i>necessarily</i>
     * indicate that the map contains no mapping for the key; it's also
     * possible that the map explicitly maps the key to {@code null}.
     * The {@link #containsKey containsKey} operation may be used to
     * distinguish these two cases.
     *
     * @see #put(Object, Object)
     */
    V *get(K *key) {
        if (key == nullptr) {
            return getForNullKey();
        }
        int hash = this->hash(std::hash<K>{}(*key)); // Todo key->hashCode()
        for (Entry<K, V> *e = table[indexFor(hash, tableLength)]; e != nullptr; e = e->next) {
            K *k;
            if (e->hash == hash && ((k = e->key) == key || (*key == *k))) {
                return e->value;
            }
        }
        return nullptr;
    }

private:
    /**
     * Offloaded version of get() to look up null keys.  Null keys map
     * to index 0.  This null case is split out into separate methods
     * for the sake of performance in the two most commonly used
     * operations (get and put), but incorporated with conditionals in
     * others.
     */
    V *getForNullKey() {
        for (Entry<K, V> *e = table[0]; e != nullptr; e = e->next) {
            if (e->key == nullptr)
                return e->value;
        }
        return nullptr;
    }

public:
    /**
     * Returns <tt>true</tt> if this map contains a mapping for the
     * specified key.
     *
     * @param   key   The key whose presence in this map is to be tested
     * @return <tt>true</tt> if this map contains a mapping for the specified
     * key.
     */
    bool containsKey(K *key) {
        return getEntry(key) != nullptr;
    }

    /**
     * Returns the entry associated with the specified key in the
     * HashMap.  Returns null if the HashMap contains no mapping
     * for the key.
     */
    Entry<K, V> *getEntry(K *key) {
        int hash = (key == nullptr) ? 0 : hash(key->hashCode());
        for (Entry<K, V> *e = table[indexFor(hash, tableLength)]; e != nullptr; e = e->next) {
            K *k;
            if (e->hash == hash && ((k = e->key) == key || (key != nullptr && key->equals(k))))
                return e;
        }
        return nullptr;
    }

    /**
     * Associates the specified value with the specified key in this map.
     * If the map previously contained a mapping for the key, the old
     * value is replaced.
     *
     * @param key key with which the specified value is to be associated
     * @param value value to be associated with the specified key
     * @return the previous value associated with <tt>key</tt>, or
     *         <tt>null</tt> if there was no mapping for <tt>key</tt>.
     *         (A <tt>null</tt> return can also indicate that the map
     *         previously associated <tt>null</tt> with <tt>key</tt>.)
     */
public:
    V *put(K *key, V *value) {
        if (key == nullptr) {
            return putForNullKey(value);
        }
        int hash = this->hash(std::hash<K>{}(*key)); // Todo key->hashCode()
        int i = indexFor(hash, tableLength);
        for (Entry<K, V> *e = table[i]; e != nullptr; e = e->next) {
            K *k = e->key;
            if (e->hash == hash && (*key == *k)) {
                V *oldValue = e->value;
                e->value = value;
                e->recordAccess(this);
                return oldValue;
            }
        }

        modCount++;
        addEntry(hash, key, value, i);
        return nullptr;
    }


private:
    /**
     * Offloaded version of put for null keys
     */
    V *putForNullKey(V *value) {
        for (Entry<K, V> *e = table[0]; e != nullptr; e = e->next) {
            if (e->key == nullptr) {
                V* oldValue = e->value;
                e->value = value;
                e->recordAccess(this);
                return oldValue;
            }
        }
        modCount++;
        addEntry(0, nullptr, value, 0);
        return nullptr;
    }

private:

    /**
     * This method is used instead of put by constructors and
     * pseudoconstructors (clone, readObject).  It does not resize the table,
     * check for comodification, etc.  It calls createEntry rather than
     * addEntry.
     */
    void putForCreate(K key, V value) {
        int hash = (key == nullptr) ? 0 : hash(key.hashCode());
        int i = indexFor(hash, tableLength);

        /**
         * Look for preexisting entry for key.  This will never happen for
         * clone or deserialize.  It will only happen for construction if the
         * input Map is a sorted map whose ordering is inconsistent w/ equals.
         */
        for (Entry<K, V> *e = table[i]; e != nullptr; e = e->next) {
            K *k;
            if (e->hash == hash &&
                ((k = e->key) == key || (key != nullptr && key.equals(k)))) {
                e->value = value;
                return;
            }
        }

        createEntry(hash, key, value, i);
    }

private:

    /**
     * Rehashes the contents of this map into a new array with a
     * larger capacity.  This method is called automatically when the
     * number of keys in this map reaches its threshold.
     *
     * If current capacity is MAXIMUM_CAPACITY, this method does not
     * resize the map, but sets threshold to Integer.MAX_VALUE.
     * This has the effect of preventing future calls.
     *
     * @param newCapacity the new capacity, MUST be a power of two;
     *        must be greater than current capacity unless current
     *        capacity is MAXIMUM_CAPACITY (in which case value
     *        is irrelevant).
     */
    void resize(int newCapacity) {
        int oldCapacity = tableLength;
        if (oldCapacity == MAXIMUM_CAPACITY) {
            threshold = std::numeric_limits<int>::max();
            return;
        }

        auto **newTable = makeTable(newCapacity);
        transfer(newTable, newCapacity);
        delete[] table;
        table = newTable;
        tableLength = newCapacity;
        threshold = (int) (newCapacity * loadFactor);
    }

    /**
     * Transfers all entries from current table to newTable.
     */
    void transfer(Entry<K, V> **newTable, std::size_t newCapacity) {
        Entry<K, V> **src = table;
        for (std::size_t j = 0; j < tableLength; j++) {
            Entry<K, V> *e = src[j];
            if (e != nullptr) {
                src[j] = nullptr;
                do {
                    Entry<K, V>* next = e->next;
                    int i = indexFor(e->hash, newCapacity);
                    e->next = newTable[i];
                    newTable[i] = e;
                    e = next;
                } while (e != nullptr);
            }
        }
    }

public:

    /**
     * Copies all of the mappings from the specified map to this map.
     * These mappings will replace any mappings that this map had for
     * any of the keys currently in the specified map.
     *
     * @param m mappings to be stored in this map
     * @throws NullPointerException if the specified map is null
     */
    void putAll(HashMap<K, V> *m) {
        int numKeysToBeAdded = m->size();
        if (numKeysToBeAdded == 0)
            return;

        /*
         * Expand the map if the map if the number of mappings to be added
         * is greater than or equal to threshold.  This is conservative; the
         * obvious condition is (m.size() + size) >= threshold, but this
         * condition could result in a map with twice the appropriate capacity,
         * if the keys to be added overlap with the keys already in this map.
         * By using the conservative calculation, we subject ourself
         * to at most one extra resize.
         */
        if (numKeysToBeAdded > threshold) {
            int targetCapacity = (int) (numKeysToBeAdded / loadFactor + 1);
            if (targetCapacity > MAXIMUM_CAPACITY)
                targetCapacity = MAXIMUM_CAPACITY;
            int newCapacity = tableLength;
            while (newCapacity < targetCapacity)
                newCapacity <<= 1;
            if (newCapacity > tableLength)
                resize(newCapacity);
        }
    }

public:
    /**
     * Removes the mapping for the specified key from this map if present.
     *
     * @param  key key whose mapping is to be removed from the map
     * @return the previous value associated with <tt>key</tt>, or
     *         <tt>null</tt> if there was no mapping for <tt>key</tt>.
     *         (A <tt>null</tt> return can also indicate that the map
     *         previously associated <tt>null</tt> with <tt>key</tt>.)
     */
    V *remove(K *key) {
        Entry<K, V> *e = removeEntryForKey(key);
        return (e == nullptr ? nullptr : e->value);
    }

    /**
     * Removes and returns the entry associated with the specified key
     * in the HashMap.  Returns null if the HashMap contains no mapping
     * for this key.
     */
    Entry<K, V> *removeEntryForKey(K *key) {
        int hash = (key == nullptr) ? 0 : hash(key->hashCode());
        int i = indexFor(hash, tableLength);
        Entry<K, V> *prev = table[i];
        Entry<K, V> *e = prev;

        while (e != nullptr) {
            Entry<K, V> *next = e->next;
            K *k;
            if (e->hash == hash &&
                ((k = e->key) == key || (key != nullptr && key->equals(k)))) {
                modCount++;
                size--;
                if (prev == e)
                    table[i] = next;
                else
                    prev->next = next;
                e->recordRemoval(this);
                return e;
            }
            prev = e;
            e = next;
        }

        return e;
    }

    /**
     * Special version of remove for EntrySet.
     */
    Entry<K, V> *removeMapping(Entry<K, V> *entry) {

        K *key = entry->getKey();
        int hash = (key == nullptr) ? 0 : hash(key->hashCode());
        int i = indexFor(hash, tableLength);
        Entry<K, V> *prev = table[i];
        Entry<K, V> *e = prev;

        while (e != nullptr) {
            Entry<K, V> *next = e->next;
            if (e->hash == hash && e->equals(entry)) {
                modCount++;
                size--;
                if (prev == e)
                    table[i] = next;
                else
                    prev->next = next;
                e->recordRemoval(this);
                return e;
            }
            prev = e;
            e = next;
        }

        return e;
    }

public:

    /**
     * Removes all of the mappings from this map.
     * The map will be empty after this call returns.
     */
    void clear() {
        modCount++;
        Entry<K, V> *tab = table;
        for (int i = 0; i < tableLength; i++)
            tab[i] = nullptr;
        size = 0;
    }

public:

    /**
     * Returns <tt>true</tt> if this map maps one or more keys to the
     * specified value.
     *
     * @param value value whose presence in this map is to be tested
     * @return <tt>true</tt> if this map maps one or more keys to the
     *         specified value
     */
    bool containsValue(V *value) {
        if (value == nullptr)
            return containsNullValue();

        auto *tab = table;
        for (int i = 0; i < tableLength; i++)
            for (auto *e = tab[i]; e != nullptr; e = e->next)
                if (value->equals(e->value))
                    return true;
        return false;
    }

private:

    /**
     * Special-case code for containsValue with null argument
     */
    bool containsNullValue() {
        auto *tab = table;
        for (int i = 0; i < tableLength; i++)
            for (auto *e = tab[i]; e != nullptr; e = e->next)
                if (e->value == nullptr)
                    return true;
        return false;
    }

public:
    /**
     * Adds a new entry with the specified key, value and hash code to
     * the specified bucket.  It is the responsibility of this
     * method to resize the table if appropriate.
     *
     * Subclass overrides this to alter the behavior of put method.
     */
    void addEntry(int hash, K* key, V* value, int bucketIndex) {
        Entry<K, V> *e = table[bucketIndex];
        table[bucketIndex] = new Entry<K, V>(hash, key, value, e);
        size += 1;
        if (size >= threshold) {
            resize(2 * tableLength);
        }
    }

    /**
     * Like addEntry except that this version is used when creating entries
     * as part of Map construction or "pseudo-construction" (cloning,
     * deserialization).  This version needn't worry about resizing the table.
     *
     * Subclass overrides this to alter the behavior of HashMap(Map),
     * clone, and readObject.
     */
    void createEntry(int hash, K key, V value, int bucketIndex) {
        Entry<K, V> e = table[bucketIndex];
        table[bucketIndex] = new Entry<K, V>(hash, key, value, e);
        size++;
    }


    // These methods are used when serializing HashSets
    int capacity() { return tableLength; }

    //Todo
    float loadFactorf() { return loadFactor; }
};
