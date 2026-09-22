#pragma once
#include <vector>
#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <utility>
#include <algorithm>
#include <ranges>
#include <type_traits>
#include "u_basetypes.hpp"

template <typename T, typename size_type>
class Grid {

public:
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using reference = T &;
    using const_reference = const T &;
    using pointer = T *;
    using const_pointer = const T *;
    using coord = Coord<size_type>;

    // ==================== flat_iter ====================
    template <bool Const>
    class basic_flat_iter {
        using owner_ptr = std::conditional_t<Const, const Grid *, Grid *>;
        using ptr_type = std::conditional_t<Const, const T *, T *>;
    public:
        using iterator_category = std::random_access_iterator_tag;
        using iterator_concept = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = ptr_type;
        using reference = std::conditional_t<Const, const T &, T &>;

        basic_flat_iter() = default;
        basic_flat_iter(owner_ptr grid, size_type index) noexcept
            : grid_{ grid }, index_{ index } {
        }

        reference operator*()  const noexcept { return grid_->_data[index_]; }
        pointer   operator->() const noexcept { return &grid_->_data[index_]; }
        reference operator[](difference_type n) const noexcept { return grid_->_data[index_ + n]; }

        basic_flat_iter & operator++()    noexcept { ++index_; return *this; }
        basic_flat_iter  operator++(int) noexcept { auto t = *this; ++index_; return t; }
        basic_flat_iter & operator--()    noexcept { --index_; return *this; }
        basic_flat_iter  operator--(int) noexcept { auto t = *this; --index_; return t; }

        basic_flat_iter & operator+=(difference_type n) noexcept { index_ += n; return *this; }
        basic_flat_iter & operator-=(difference_type n) noexcept { index_ -= n; return *this; }

        friend basic_flat_iter operator+(basic_flat_iter it, difference_type n) noexcept { return it += n; }
        friend basic_flat_iter operator+(difference_type n, basic_flat_iter it) noexcept { return it += n; }
        friend basic_flat_iter operator-(basic_flat_iter it, difference_type n) noexcept { return it -= n; }

        friend difference_type operator-(const basic_flat_iter & a, const basic_flat_iter & b) noexcept {
            return static_cast<difference_type>(a.index_) - static_cast<difference_type>(b.index_);
        }

        friend bool operator==(const basic_flat_iter & a, const basic_flat_iter & b) noexcept {
            return a.index_ == b.index_;
        }
        friend auto operator<=>(const basic_flat_iter & a, const basic_flat_iter & b) noexcept {
            return a.index_ <=> b.index_;
        }

        coord coord() const noexcept {
            return { index_ % grid_->_width, index_ / grid_->_width };
        }

        size_type index() const noexcept { return index_; }

    private:
        owner_ptr grid_ = nullptr;
        size_type index_ = 0;
    };

    // ==================== row_iter ====================
    template <bool Const>
    class basic_row_iter {
        using owner_ptr = std::conditional_t<Const, const Grid *, Grid *>;
        using ptr_type = std::conditional_t<Const, const T *, T *>;
    public:
        using iterator_category = std::random_access_iterator_tag;
        using iterator_concept = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = ptr_type;
        using reference = std::conditional_t<Const, const T &, T &>;

        basic_row_iter() = default;
        basic_row_iter(owner_ptr grid, size_type y, size_type x) noexcept
            : grid_{ grid }, y_{ y }, x_{ x } {
        }

        reference operator*()  const noexcept { return grid_->_data[y_ * grid_->_width + x_]; }
        pointer   operator->() const noexcept { return &grid_->_data[y_ * grid_->_width + x_]; }
        reference operator[](difference_type n) const noexcept {
            return grid_->_data[y_ * grid_->_width + x_ + n];
        }

        basic_row_iter & operator++()    noexcept { ++x_; return *this; }
        basic_row_iter  operator++(int) noexcept { auto t = *this; ++x_; return t; }
        basic_row_iter & operator--()    noexcept { --x_; return *this; }
        basic_row_iter  operator--(int) noexcept { auto t = *this; --x_; return t; }

        basic_row_iter & operator+=(difference_type n) noexcept { x_ += n; return *this; }
        basic_row_iter & operator-=(difference_type n) noexcept { x_ -= n; return *this; }

        friend basic_row_iter operator+(basic_row_iter it, difference_type n) noexcept { return it += n; }
        friend basic_row_iter operator+(difference_type n, basic_row_iter it) noexcept { return it += n; }
        friend basic_row_iter operator-(basic_row_iter it, difference_type n) noexcept { return it -= n; }

        friend difference_type operator-(const basic_row_iter & a, const basic_row_iter & b) noexcept {
            return static_cast<difference_type>(a.x_) - static_cast<difference_type>(b.x_);
        }

        friend bool operator==(const basic_row_iter & a, const basic_row_iter & b) noexcept {
            return a.x_ == b.x_;
        }
        friend auto operator<=>(const basic_row_iter & a, const basic_row_iter & b) noexcept {
            return a.x_ <=> b.x_;
        }

        coord coord() const noexcept { return { x_, y_ }; }
        size_type x() const noexcept { return x_; }
        size_type y() const noexcept { return y_; }

    private:
        owner_ptr grid_ = nullptr;
        size_type y_ = 0;
        size_type x_ = 0;
    };

    // ==================== column_iter ====================
    template <bool Const>
    class basic_column_iter {
        using owner_ptr = std::conditional_t<Const, const Grid *, Grid *>;
        using ptr_type = std::conditional_t<Const, const T *, T *>;
    public:
        using iterator_category = std::random_access_iterator_tag;
        using iterator_concept = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = ptr_type;
        using reference = std::conditional_t<Const, const T &, T &>;

        basic_column_iter() = default;
        basic_column_iter(owner_ptr grid, size_type x, size_type y) noexcept
            : grid_{ grid }, x_{ x }, y_{ y } {
        }

        reference operator*()  const noexcept { return grid_->_data[y_ * grid_->_width + x_]; }
        pointer   operator->() const noexcept { return &grid_->_data[y_ * grid_->_width + x_]; }
        reference operator[](difference_type n) const noexcept {
            return grid_->_data[(y_ + n) * grid_->_width + x_];
        }

        basic_column_iter & operator++()    noexcept { ++y_; return *this; }
        basic_column_iter  operator++(int) noexcept { auto t = *this; ++y_; return t; }
        basic_column_iter & operator--()    noexcept { --y_; return *this; }
        basic_column_iter  operator--(int) noexcept { auto t = *this; --y_; return t; }

        basic_column_iter & operator+=(difference_type n) noexcept { y_ += n; return *this; }
        basic_column_iter & operator-=(difference_type n) noexcept { y_ -= n; return *this; }

        friend basic_column_iter operator+(basic_column_iter it, difference_type n) noexcept { return it += n; }
        friend basic_column_iter operator+(difference_type n, basic_column_iter it) noexcept { return it += n; }
        friend basic_column_iter operator-(basic_column_iter it, difference_type n) noexcept { return it -= n; }

        friend difference_type operator-(const basic_column_iter & a, const basic_column_iter & b) noexcept {
            return static_cast<difference_type>(a.y_) - static_cast<difference_type>(b.y_);
        }

        friend bool operator==(const basic_column_iter & a, const basic_column_iter & b) noexcept {
            return a.y_ == b.y_;
        }
        friend auto operator<=>(const basic_column_iter & a, const basic_column_iter & b) noexcept {
            return a.y_ <=> b.y_;
        }

        coord coord() const noexcept { return { x_, y_ }; }
        size_type x() const noexcept { return x_; }
        size_type y() const noexcept { return y_; }

    private:
        owner_ptr grid_ = nullptr;
        size_type x_ = 0;
        size_type y_ = 0;
    };

    // ==================== ranges (flat/row/column) ====================
    template <bool Const>
    struct flat_range {
        using grid_ptr = std::conditional_t<Const, const Grid *, Grid *>;
        using iterator = basic_flat_iter<Const>;

        grid_ptr grid = nullptr;

        iterator begin() const noexcept { return iterator{ grid, 0 }; }
        iterator end()   const noexcept { return iterator{ grid, static_cast<size_type>(grid->_data.size()) }; }
        size_type size() const noexcept { return static_cast<size_type>(grid->_data.size()); }
        bool empty()     const noexcept { return grid->_data.empty(); }
    };

    template <bool Const>
    struct row_range {
        using grid_ptr = std::conditional_t<Const, const Grid *, Grid *>;
        using iterator = basic_row_iter<Const>;

        grid_ptr  grid = nullptr;
        size_type y = 0;

        iterator begin() const noexcept { return iterator{ grid, y, 0 }; }
        iterator end()   const noexcept { return iterator{ grid, y, grid->_width }; }
        size_type size() const noexcept { return grid->_width; }
        bool empty()     const noexcept { return grid->_width == 0; }
    };

    template <bool Const>
    struct column_range {
        using grid_ptr = std::conditional_t<Const, const Grid *, Grid *>;
        using iterator = basic_column_iter<Const>;

        grid_ptr  grid = nullptr;
        size_type x = 0;

        iterator begin() const noexcept { return iterator{ grid, x, 0 }; }
        iterator end()   const noexcept { return iterator{ grid, x, grid->_height }; }
        size_type size() const noexcept { return grid->_height; }
        bool empty()     const noexcept { return grid->_height == 0; }
    };

    // ==================== rows_iter (итератор по строкам) ====================
    // operator* возвращает row_range, а не row_iter!
    template <bool Const>
    class basic_rows_iter {
        using owner_ptr = std::conditional_t<Const, const Grid *, Grid *>;
    public:
        using iterator_category = std::random_access_iterator_tag;
        using iterator_concept = std::random_access_iterator_tag;
        using value_type = row_range<Const>;
        using difference_type = std::ptrdiff_t;
        using pointer = void;
        using reference = row_range<Const>;

        basic_rows_iter() = default;
        basic_rows_iter(owner_ptr grid, size_type y) noexcept
            : grid_{ grid }, y_{ y } {
        }

        reference operator*() const noexcept { return row_range<Const>{grid_, y_}; }
        reference operator[](difference_type n) const noexcept {
            return row_range<Const>{grid_, static_cast<size_type>(static_cast<difference_type>(y_) + n)};
        }

        basic_rows_iter & operator++()    noexcept { ++y_; return *this; }
        basic_rows_iter  operator++(int) noexcept { auto t = *this; ++y_; return t; }
        basic_rows_iter & operator--()    noexcept { --y_; return *this; }
        basic_rows_iter  operator--(int) noexcept { auto t = *this; --y_; return t; }

        basic_rows_iter & operator+=(difference_type n) noexcept { y_ += n; return *this; }
        basic_rows_iter & operator-=(difference_type n) noexcept { y_ -= n; return *this; }

        friend basic_rows_iter operator+(basic_rows_iter it, difference_type n) noexcept { return it += n; }
        friend basic_rows_iter operator+(difference_type n, basic_rows_iter it) noexcept { return it += n; }
        friend basic_rows_iter operator-(basic_rows_iter it, difference_type n) noexcept { return it -= n; }

        friend difference_type operator-(const basic_rows_iter & a, const basic_rows_iter & b) noexcept {
            return static_cast<difference_type>(a.y_) - static_cast<difference_type>(b.y_);
        }

        friend bool operator==(const basic_rows_iter & a, const basic_rows_iter & b) noexcept {
            return a.y_ == b.y_;
        }
        friend auto operator<=>(const basic_rows_iter & a, const basic_rows_iter & b) noexcept {
            return a.y_ <=> b.y_;
        }

        size_type y() const noexcept { return y_; }

    private:
        owner_ptr grid_ = nullptr;
        size_type y_ = 0;
    };

    // ==================== columns_iter (итератор по столбцам) ====================
    template <bool Const>
    class basic_columns_iter {
        using owner_ptr = std::conditional_t<Const, const Grid *, Grid *>;
    public:
        using iterator_category = std::random_access_iterator_tag;
        using iterator_concept = std::random_access_iterator_tag;
        using value_type = column_range<Const>;
        using difference_type = std::ptrdiff_t;
        using pointer = void;
        using reference = column_range<Const>;

        basic_columns_iter() = default;
        basic_columns_iter(owner_ptr grid, size_type x) noexcept
            : grid_{ grid }, x_{ x } {
        }

        reference operator*() const noexcept { return column_range<Const>{grid_, x_}; }
        reference operator[](difference_type n) const noexcept {
            return column_range<Const>{grid_, static_cast<size_type>(static_cast<difference_type>(x_) + n)};
        }

        basic_columns_iter & operator++()    noexcept { ++x_; return *this; }
        basic_columns_iter  operator++(int) noexcept { auto t = *this; ++x_; return t; }
        basic_columns_iter & operator--()    noexcept { --x_; return *this; }
        basic_columns_iter  operator--(int) noexcept { auto t = *this; --x_; return t; }

        basic_columns_iter & operator+=(difference_type n) noexcept { x_ += n; return *this; }
        basic_columns_iter & operator-=(difference_type n) noexcept { x_ -= n; return *this; }

        friend basic_columns_iter operator+(basic_columns_iter it, difference_type n) noexcept { return it += n; }
        friend basic_columns_iter operator+(difference_type n, basic_columns_iter it) noexcept { return it += n; }
        friend basic_columns_iter operator-(basic_columns_iter it, difference_type n) noexcept { return it -= n; }

        friend difference_type operator-(const basic_columns_iter & a, const basic_columns_iter & b) noexcept {
            return static_cast<difference_type>(a.x_) - static_cast<difference_type>(b.x_);
        }

        friend bool operator==(const basic_columns_iter & a, const basic_columns_iter & b) noexcept {
            return a.x_ == b.x_;
        }
        friend auto operator<=>(const basic_columns_iter & a, const basic_columns_iter & b) noexcept {
            return a.x_ <=> b.x_;
        }

        size_type x() const noexcept { return x_; }

    private:
        owner_ptr grid_ = nullptr;
        size_type x_ = 0;
    };

    // ==================== rows_range / columns_range ====================
    template <bool Const>
    struct rows_range {
        using grid_ptr = std::conditional_t<Const, const Grid *, Grid *>;
        using iterator = basic_rows_iter<Const>;

        grid_ptr grid = nullptr;

        iterator begin() const noexcept { return iterator{ grid, 0 }; }
        iterator end()   const noexcept { return iterator{ grid, grid->_height }; }
        size_type size() const noexcept { return grid->_height; }
        bool empty()     const noexcept { return grid->_height == 0; }
    };

    template <bool Const>
    struct columns_range {
        using grid_ptr = std::conditional_t<Const, const Grid *, Grid *>;
        using iterator = basic_columns_iter<Const>;

        grid_ptr grid = nullptr;

        iterator begin() const noexcept { return iterator{ grid, 0 }; }
        iterator end()   const noexcept { return iterator{ grid, grid->_width }; }
        size_type size() const noexcept { return grid->_width; }
        bool empty()     const noexcept { return grid->_width == 0; }
    };

    // ==================== aliases ====================
    using flat_iter = basic_flat_iter<false>;
    using const_flat_iter = basic_flat_iter<true>;
    using row_iter = basic_row_iter<false>;
    using const_row_iter = basic_row_iter<true>;
    using column_iter = basic_column_iter<false>;
    using const_column_iter = basic_column_iter<true>;
    using rows_iter = basic_rows_iter<false>;
    using const_rows_iter = basic_rows_iter<true>;
    using columns_iter = basic_columns_iter<false>;
    using const_columns_iter = basic_columns_iter<true>;

    // ==================== constructors ====================
    Grid() = default;

    Grid(size_type width, size_type height)
        : _width{ width }, _height{ height }, _data(width * height) {
    }

    Grid(size_type width, size_type height, const T & value)
        : _width{ width }, _height{ height }, _data(width * height, value) {
    }

    Grid(size_type width, size_type height, std::initializer_list<T> init)
        : _width{ width }, _height{ height }, _data(init) {
        if (_data.size() != width * height) {
            throw std::invalid_argument{ "Grid: init size mismatch" };
        }
    }

    // ==================== size ====================
    size_type width()  const noexcept { return _width; }
    size_type height() const noexcept { return _height; }
    size_type size()   const noexcept { return static_cast<size_type>(_data.size()); }
    bool empty()       const noexcept { return _data.empty(); }

    bool in_bounds(size_type x, size_type y) const noexcept {
        return x < _width && y < _height;
    }

    // ==================== access ====================
    reference at(size_type x, size_type y) {
        if (!in_bounds(x, y)) throw std::out_of_range{ "Grid::at" };
        return _data[y * _width + x];
    }
    const_reference at(size_type x, size_type y) const {
        if (!in_bounds(x, y)) throw std::out_of_range{ "Grid::at" };
        return _data[y * _width + x];
    }

    reference at(coord c) { return at(c.x, c.y); }
    const_reference at(coord c) const { return at(c.x, c.y); }

    reference operator[](coord c) noexcept { return _data[c.y * _width + c.x]; }
    const_reference operator[](coord c) const noexcept { return _data[c.y * _width + c.x]; }

    pointer data() noexcept { return _data.data(); }
    const_pointer data() const noexcept { return _data.data(); }

    // ==================== ranges access ====================
    auto flat() noexcept { return flat_range<false>{this}; }
    auto flat() const noexcept { return flat_range<true>{this}; }

    auto rows() noexcept { return rows_range<false>{this}; }
    auto rows() const noexcept { return rows_range<true>{this}; }

    auto columns() noexcept { return columns_range<false>{this}; }
    auto columns() const noexcept { return columns_range<true>{this}; }

    auto row_at(size_type y) noexcept { return row_range<false>{this, y}; }
    auto row_at(size_type y) const noexcept { return row_range<true>{this, y}; }

    auto column_at(size_type x) noexcept { return column_range<false>{this, x}; }
    auto column_at(size_type x) const noexcept { return column_range<true>{this, x}; }

    // ==================== modifiers ====================
    void fill(const T & value) { std::fill(_data.begin(), _data.end(), value); }

    void resize(size_type new_width, size_type new_height) {
        _width = new_width;
        _height = new_height;
        _data.resize(new_width * new_height);
    }

    void clear() noexcept {
        _data.clear();
        _width = _height = 0;
    }

private:
    size_type _width = 0;
    size_type _height = 0;
    std::vector<T> _data;
};
