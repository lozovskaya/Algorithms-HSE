#include <iostream>
#include <vector>
#include <list>
#include <stdexcept>


template<class KeyType, class ValueType, class Hash = std::hash<KeyType>>
class HashMap {
public:
    using element = std::pair<const KeyType, ValueType>;
    using iterator = typename std::vector<element>::iterator;
    using const_iterator = typename std::vector<element>::const_iterator;

private:
    int32_t countOfElements = 0;
    Hash hasher;
    std::vector<element> elements;
    std::vector<std::list<int32_t>> buckets;

    void doubleSize() {
        countOfElements = 0;
        auto curr = elements;
        elements.clear();
        int32_t size = buckets.size();
        if (size == 0) {
            size = 1;
        }
        else size *= 2;
        buckets.clear();
        buckets.resize(size);
        for (auto i : curr) {
            insert(i);
        }
    }

    void checkSize() {
        if (buckets.size() == elements.size()) {
            doubleSize();
        }
    }

    void changeNumber(const KeyType &key, int32_t newNumber) {
        auto& bucket = buckets[getIndex(key)];
        for (auto i = bucket.begin(); i != bucket.end(); ++i) {
            if (elements[*i].first == key) {
                *i = newNumber;
                return;
            }
        }
    }
    
    int32_t find_inside(const KeyType &key) const {
        if (elements.size() == 0) {
            return -1;
        }
        auto bucket = buckets[getIndex(key)];
        for (auto i = bucket.begin(); i != bucket.end(); ++i) {
            if (elements[*i].first == key) {
                return (*i);
            }
        }
        return -1;
    }


public:
    HashMap(const Hash &custom = Hash()) : hasher(custom) {
        countOfElements = 0;
        elements.clear();
        buckets.clear();
    };

    template<typename Iterator>
    HashMap(Iterator begin, Iterator end, const Hash &custom = Hash()) : HashMap(custom) {
        while (begin != end) {
            insert(*begin);
            ++begin;
        }
    }

    HashMap(const std::initializer_list<element> &initList, const Hash &custom = Hash())
            : HashMap(initList.begin(), initList.end(), custom) {}

    HashMap(const HashMap& other) : HashMap(other.begin(), other.end(), other.hasher) {}

    HashMap& operator=(const HashMap& other) {
        hasher = other.hasher;
        countOfElements = other.countOfElements;
        std::vector<element> x;
        for (auto i : other.elements) {
            x.push_back(i);
        }
        elements.clear();
        for (auto i : x) {
            elements.push_back(i);
        }
        buckets = other.buckets;
        return *this;
    }


    int32_t size() const {
        return countOfElements;
    }

    bool empty() const {
        return countOfElements == 0;
    }

    Hash hash_function() const {
        return hasher;
    }

    iterator begin() {
        return elements.begin();
    }

    const_iterator begin() const {
        return elements.begin();
    }

    iterator end() {
        return elements.end();
    }

    const_iterator end() const {
        return elements.end();
    }

    int32_t getIndex(KeyType key) const {
        return hasher(key) % buckets.size();
    }

    void insert(std::pair<KeyType, ValueType> newElem) {
        checkSize();
        if (find(newElem.first) == end()) {
            buckets[getIndex(newElem.first)].push_back(elements.size());
            elements.push_back(newElem);
            ++countOfElements;
        }
    }


    void erase(const KeyType &key) {
        if (buckets.empty()) {
            return;
        }
        auto& bucket = buckets[getIndex(key)];
        for (auto i = bucket.begin(); i != bucket.end(); ++i) {
            if (elements[*i].first == key) {
                if ((*i) != countOfElements - 1) {
                    changeNumber(elements.back().first, *i);
                    std::swap(elements[*i].second, elements.back().second);
                    std::swap(const_cast<KeyType&>(elements[*i].first), const_cast<KeyType&>(elements.back().first));
                }
                elements.pop_back();
                --countOfElements;
                bucket.erase(i);
                return;
            }
        }
    }

    const_iterator find(const KeyType &key) const {
        int32_t index = find_inside(key);
        if (index == -1) {
            return end();
        }
        return elements.begin() + index;
    }

    iterator find(const KeyType &key) {
        int32_t index = find_inside(key);
        if (index == -1) {
            return end();
        }
        return elements.begin() + index;
    }

    ValueType &operator[] (const KeyType &key) {
        iterator iter = find(key);
        if (iter != end()) {
            return iter->second;
        }
        insert(std::make_pair(key, ValueType()));
        return find(key)->second;
    }


    const ValueType &at(const KeyType &key) const {
        const_iterator iter = find(key);
        if (iter != end()) {
            return iter->second;
        }
        throw std::out_of_range("Not found");
    }

    void clear() {
        while (countOfElements) {
            erase(elements.back().first);
        }
    }
};
