#pragma once
#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include "u_dynspan.hpp"

template <typename Key, typename Value>
class MapSpan {

public:
    struct Entry {
        Key   key;
        Value value;

        friend bool operator==(const Entry &, const Entry &) = default;
    };

    using key_type = Key;
    using mapped_type = Value;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using storage_type = DynSpan<Entry>;

    using iterator = typename storage_type::iterator;
    using const_iterator = typename storage_type::const_iterator;
    using pointer = typename storage_type::pointer;
    using const_pointer = typename storage_type::const_pointer;
    using reference = typename storage_type::reference;
    using const_reference = typename storage_type::const_reference;

    MapSpan() = default;

    MapSpan(std::initializer_list<Entry> init)
        : _data{ init } {}

    explicit MapSpan(size_type n)
        : _data{ n } {}

    // ==================== size ====================
    size_type size()  const noexcept { return _data.size(); }
    bool      empty() const noexcept { return _data.empty(); }
    void      clear()       noexcept { _data.clear(); }

    // ==================== iteration ====================
    iterator       begin()  noexcept { return _data.begin(); }
    iterator       end()    noexcept { return _data.end(); }
    const_iterator begin()  const noexcept { return _data.begin(); }
    const_iterator end()    const noexcept { return _data.end(); }
    const_iterator cbegin() const noexcept { return _data.cbegin(); }
    const_iterator cend()   const noexcept { return _data.cend(); }

    // ==================== find / contains ====================
    iterator find(const Key & key) noexcept {
        for (iterator it = _data.begin(); it != _data.end(); ++it) {
            if (it->key == key) return it;
        }
        return _data.end();
    }
    const_iterator find(const Key & key) const noexcept {
        for (const_iterator it = _data.begin(); it != _data.end(); ++it) {
            if (it->key == key) return it;
        }
        return _data.end();
    }

    bool contains(const Key & key) const noexcept {
        return find(key) != _data.end();
    }

    size_type index_of(const Key & key) const noexcept {
        const const_iterator it = find(key);
        if (it == _data.end()) return static_cast<size_type>(-1);
        return static_cast<size_type>(it - _data.begin());
    }

    // ==================== get by key ====================
    Value & at(const Key & key) {
        auto it = find(key);
        if (it == _data.end())
            throw std::out_of_range{ "MapSpan::at: key not found" };
        return it->value;
    }
    const Value & at(const Key & key) const {
        auto it = find(key);
        if (it == _data.end())
            throw std::out_of_range{ "MapSpan::at: key not found" };
        return it->value;
    }

    Value * try_get(const Key & key) noexcept {
        auto it = find(key);
        return it == _data.end() ? nullptr : &it->value;
    }
    const Value * try_get(const Key & key) const noexcept {
        auto it = find(key);
        return it == _data.end() ? nullptr : &it->value;
    }

    // ==================== set / insert / remove by key ====================
    Value & set(const Key & key, Value value) {
        auto it = find(key);
        if (it == _data.end()) {
            _data.push_back(Entry{ key, std::move(value) });
            return _data.back().value;
        }
        it->value = std::move(value);
        return it->value;
    }

    std::pair<iterator, bool> try_insert(const Key & key, Value value) {
        auto it = find(key);
        if (it != _data.end()) return { it, false };
        _data.push_back(Entry{ key, std::move(value) });
        return { _data.end() - 1, true };
    }


    bool remove(const Key & key) {
        auto it = find(key);
        if (it == _data.end()) return false;
        const size_type idx = static_cast<size_type>(it - _data.begin());
        for (size_type i = idx; i + 1 < _data.size(); ++i) {
            _data[i] = std::move(_data[i + 1]);
        }
        _data.pop_back();
        return true;
    }

    // ==================== access by index ====================
    Entry & entry_at(size_type i)             noexcept { return _data[i]; }
    const Entry & entry_at(size_type i) const noexcept { return _data[i]; }

    Key & key_at(size_type i)             noexcept { return _data[i].key; }
    const Key & key_at(size_type i) const noexcept { return _data[i].key; }

    Value & value_at(size_type i)             noexcept { return _data[i].value; }
    const Value & value_at(size_type i) const noexcept { return _data[i].value; }

    // ==================== raw access ====================
    storage_type & data()       noexcept { return _data; }
    const storage_type & data() const noexcept { return _data; }

private:
    storage_type _data;
};
