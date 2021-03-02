#include <iostream>
#include <vector>
#include <list>
#include <stdexcept>

using namespace std;

template<class KeyType, class ValueType, class Hash = std::hash<KeyType>>
class HashMap {
public:
    using element = pair<const KeyType, ValueType>;
    using iterator = typename vector<element>::iterator;
    using const_iterator = typename vector<element>::const_iterator;

private:
    int CountOfElements = 0;
    Hash hasher;
    vector<element> elements;
    vector<list<int>> buckets;

    void sizeTwice() {
        CountOfElements = 0;
        auto curr = elements;
        elements.clear();
        int size = buckets.size();
        if (size == 0) size = 1;
        else size *= 2;
        buckets.clear();
        buckets.resize(size);
        for (auto i : curr) {
            insert(i);
        }
    }

    void checkSize() {
        if (buckets.size() == elements.size()) sizeTwice();
    }

public:
    HashMap(const Hash &custom = Hash()) : hasher(custom) {
        CountOfElements = 0;
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

    HashMap(initializer_list<element> initList, const Hash &custom = Hash())
            : HashMap(custom) {
        for (auto elem : initList) {
            insert(elem);
        }
    }

    HashMap(const HashMap &other) {
        *this = other;
    }

    HashMap &operator=(const HashMap &other) {
        hasher = other.hasher;
        CountOfElements = other.CountOfElements;
        vector<element> x;
        for (auto i : other.elements) x.push_back(i);
        elements.clear();
        for (auto i : x) elements.push_back(i);
        buckets = other.buckets;
        return *this;
    }


    int size() const {
        return CountOfElements;
    }

    bool empty() const {
        return (CountOfElements == 0);
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

    int getIndex(KeyType key) const {
        return hasher(key) % buckets.size();
    }

    void insert(std::pair<KeyType, ValueType> newElem) {
        checkSize();
        if (find(newElem.first) == end()) {
            buckets[getIndex(newElem.first)].push_back(elements.size());
            elements.push_back(newElem);
            ++CountOfElements;
        }
    }

    void changeNumber(const KeyType &key, int newNumber) {
        auto &bucket = buckets[getIndex(key)];
        for (auto i = bucket.begin(); i != bucket.end(); ++i) {
            if (elements[*i].first == key) {
                *i = newNumber;
                return;
            }
        }
    }

    void erase(const KeyType &key) {
        if (buckets.empty()) {
            return;
        }
        auto &bucket = buckets[getIndex(key)];
        for (auto i = bucket.begin(); i != bucket.end(); ++i) {
            if (elements[*i].first == key) {
                if ((*i) != CountOfElements - 1) {
                    changeNumber(elements.back().first, *i);
                    swap(elements[*i].second, elements.back().second);
                    swap(const_cast<KeyType &>(elements[*i].first), const_cast<KeyType &>(elements.back().first));
                }
                elements.pop_back();
                --CountOfElements;
                bucket.erase(i);
                return;
            }
        }
    }

    const_iterator find(const KeyType &key) const {
        if (elements.size() == 0) return end();
        auto bucket = buckets[getIndex(key)];
        for (auto i = bucket.begin(); i != bucket.end(); ++i) {
            if (elements[*i].first == key) {
                return elements.begin() + (*i);
            }
        }
        return end();
    }

    iterator find(const KeyType &key) {
        if (elements.size() == 0) return end();
        auto bucket = buckets[getIndex(key)];
        for (auto i = bucket.begin(); i != bucket.end(); ++i) {
            if (elements[*i].first == key) {
                return elements.begin() + (*i);
            }
        }
        return end();
    }

    ValueType &operator[](const KeyType &key) {
        iterator iter = find(key);
        if (iter != end()) {
            return iter->second;
        }
        insert(make_pair(key, ValueType()));
        return find(key)->second;
    }


    const ValueType &at(const KeyType &key) const {
        const_iterator iter = find(key);
        if (iter != end()) {
            return iter->second;
        }
        throw out_of_range("Not found");
    }

    void clear() {
        while (CountOfElements) {
            erase(elements.back().first);
        }
    }
};
