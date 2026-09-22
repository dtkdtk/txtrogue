#pragma once
#include <span>
#include <algorithm>
#include <cstddef>
#include <memory>
#include <utility>
#include <initializer_list>

template <typename T>
class DynSpan {

public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = T &;
    using const_reference = const T &;
    using pointer = T *;
    using const_pointer = const T *;
    using iterator = T *;
    using const_iterator = const T *;

    DynSpan() noexcept = default;

    explicit DynSpan(size_type n)
        : _data{ n ? std::make_unique<T[]>(n) : nullptr }
        , _size{ n } {
    }

    template <size_type N>
    DynSpan(T(&arr)[N])
        : _data{ N ? std::make_unique<T[]>(N) : nullptr }
        , _size{ N } {
        std::copy(arr, arr + N, _data.get());
    }

    DynSpan(std::initializer_list<T> init)
        : _data{ init.size() ? std::make_unique<T[]>(init.size()) : nullptr }
        , _size{ init.size() } {
        std::copy(init.begin(), init.end(), _data.get());
    }

    DynSpan(const DynSpan & other)
        : _data{ other._size ? std::make_unique<T[]>(other._size) : nullptr }
        , _size{ other._size } {
        std::copy(other._data.get(), other._data.get() + _size, _data.get());
    }

    DynSpan & operator=(const DynSpan & other) {
        if (this != &other) {
            DynSpan tmp{ other };
            swap(tmp);
        }
        return *this;
    }

    DynSpan(DynSpan &&) noexcept = default;
    DynSpan & operator=(DynSpan &&) noexcept = default;
    ~DynSpan() = default;

    void swap(DynSpan & other) noexcept {
        _data.swap(other._data);
        std::swap(_size, other._size);
    }

    T * data()  noexcept { return _data.get(); }
    const T * data()  const noexcept { return _data.get(); }
    size_type size()  const noexcept { return _size; }
    bool empty()     const noexcept { return _size == 0; }

    T * begin()  noexcept { return _data.get(); }
    T * end()    noexcept { return _data.get() + _size; }
    const T * begin()  const noexcept { return _data.get(); }
    const T * end()    const noexcept { return _data.get() + _size; }
    const T * cbegin() const noexcept { return _data.get(); }
    const T * cend()   const noexcept { return _data.get() + _size; }

    operator std::span<T>()       noexcept { return { _data.get(), _size }; }
    operator std::span<const T>() const noexcept { return { _data.get(), _size }; }

    T & operator[](size_type i)       noexcept { return _data[i]; }
    const T & operator[](size_type i) const noexcept { return _data[i]; }

    T & front()       noexcept { return _data[0]; }
    const T & front() const noexcept { return _data[0]; }
    T & back()        noexcept { return _data[_size - 1]; }
    const T & back()  const noexcept { return _data[_size - 1]; }

    bool has_value(const T & value) const {
        return std::ranges::find(*this, value) != end();
    }

    bool push_back(const T & value) { return push_back_impl(value); }
    bool push_back(T && value) { return push_back_impl(std::move(value)); }

    bool pop_back() {
        if (_size == 0) return false;
        --_size;
        return true;
    }

    void clear() noexcept { _size = 0; }

private:
    template <typename U>
    bool push_back_impl(U && value) {
        const size_type new_size = _size + 1;
        auto new_data = std::make_unique<T[]>(new_size);

        // перемещаем/копируем старые
        for (size_type i = 0; i < _size; ++i) {
            new_data[i] = std::move_if_noexcept(_data[i]);
        }
        new_data[_size] = std::forward<U>(value);

        _data = std::move(new_data);
        _size = new_size;
        return true;
    }

    std::unique_ptr<T[]> _data;
    size_type _size = 0;
};
