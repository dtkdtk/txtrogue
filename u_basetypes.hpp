#pragma once
#include <cstddef>
#include <cstring>
#include <cstdint>
#include <array>
#include <compare>
#include <string_view>
using std::size_t;



template <typename size_type>
struct Coord {
    size_type x = 0;
    size_type y = 0;

    friend bool operator==(const Coord &, const Coord &) = default;
};



struct Key8 {
    using T = char[8];
    using value_type = char;
    static constexpr size_t size = 8;

    T data;

    Key8(const T && initial) noexcept {
        std::memcpy(data, initial, size);
    }

    Key8(const T & initial) noexcept {
        std::memcpy(data, initial, size);
    }

    Key8(const char * ptr) noexcept {
        std::memcpy(data, ptr, size);
    }

    Key8(std::string_view sv) noexcept {
        std::memcpy(data, sv.data(), sv.size() < size ? sv.size() : size);
    }

    Key8(const std::array<char, 8> & arr) noexcept {
        std::memcpy(data, arr.data(), size);
    }

    Key8() noexcept = default;
    Key8(const Key8 &) noexcept = default;
    Key8(Key8 &&) noexcept = default;
    Key8 & operator=(const Key8 &) noexcept = default;
    Key8 & operator=(Key8 &&) noexcept = default;
    ~Key8() = default;

    operator const char * ()        const   noexcept { return data; }
    operator char * ()                      noexcept { return data; }
    operator std::string_view()     const   noexcept { return { data, size }; }
    operator std::array<char, 8>()  const   noexcept {
        std::array<char, 8> arr;
        std::memcpy(arr.data(), data, size);
        return arr;
    }

    char & operator[](size_t i)             noexcept { return data[i]; }
    const char & operator[](size_t i) const noexcept { return data[i]; }

    friend bool operator==(const Key8 &, const Key8 &) noexcept = default;

    friend std::strong_ordering
        operator<=>(const Key8 & a, const Key8 & b) noexcept {
        const int cmp = std::memcmp(a.data, b.data, size);
        if (cmp < 0) return std::strong_ordering::less;
        if (cmp > 0) return std::strong_ordering::greater;
        return std::strong_ordering::equal;
    }
};
namespace std {
    template<>
    struct hash<Key8> {
        size_t operator()(const Key8 & k) const noexcept {
            size_t h = 1469598103934665603ull;
            for (size_t i = 0; i < Key8::size; i++) {
                h ^= static_cast<unsigned char>(k[i]);
                h *= 1099511628211ull;
            }
            return h;
        }
    };
}



struct Color {
    uint8_t r = 255;
    uint8_t g = 255;
    uint8_t b = 255;
    uint8_t a = 255;

    friend bool operator==(const Color &, const Color &) = default;

    static constexpr Color rgb(uint8_t r,
        uint8_t g,
        uint8_t b,
        uint8_t a = 255) noexcept {
        return { r, g, b, a };
    }

    // 0xRRGGBB
    static constexpr Color from_hex(uint32_t hex) noexcept {
        return {
            static_cast<uint8_t>((hex >> 16) & 0xFFu),
            static_cast<uint8_t>((hex >> 8) & 0xFFu),
            static_cast<uint8_t>(hex & 0xFFu),
            255
        };
    }

    // 0xRRGGBBAA
    static constexpr Color from_hex_rgba(uint32_t hex) noexcept {
        return {
            static_cast<uint8_t>((hex >> 24) & 0xFFu),
            static_cast<uint8_t>((hex >> 16) & 0xFFu),
            static_cast<uint8_t>((hex >> 8) & 0xFFu),
            static_cast<uint8_t>(hex & 0xFFu)
        };
    }

    constexpr Color with_alpha(uint8_t alpha) const noexcept {
        return { r, g, b, alpha };
    }

    constexpr Color lerp(const Color & o, float t) const noexcept {
        return {
            static_cast<uint8_t>(r + (o.r - r) * t),
            static_cast<uint8_t>(g + (o.g - g) * t),
            static_cast<uint8_t>(b + (o.b - b) * t),
            static_cast<uint8_t>(a + (o.a - a) * t)
        };
    }

    // Взвешенное расстояние (глаз чувствительнее к зелёному).
    // Веса (2, 4, 3) — удачный компромисс для квантования.
    constexpr uint32_t distance_squared(const Color & o) const noexcept {
        const int dr = static_cast<int>(r) - static_cast<int>(o.r);
        const int dg = static_cast<int>(g) - static_cast<int>(o.g);
        const int db = static_cast<int>(b) - static_cast<int>(o.b);
        return static_cast<uint32_t>(dr * dr * 2 + dg * dg * 4 + db * db * 3);
    }

    constexpr int luminance() const noexcept {
        // ITU-R BT.601
        return (299 * r + 587 * g + 114 * b) / 1000;
    }

    // Базовые
    static constexpr Color transparent() noexcept { return { 0,   0,   0,   0 }; }
    static constexpr Color black()       noexcept { return { 0,   0,   0, 255 }; }
    static constexpr Color white()       noexcept { return { 255, 255, 255, 255 }; }
    static constexpr Color red()         noexcept { return { 255,   0,   0, 255 }; }
    static constexpr Color green()       noexcept { return { 0, 128,   0, 255 }; }
    static constexpr Color blue()        noexcept { return { 0,   0, 255, 255 }; }
    static constexpr Color yellow()      noexcept { return { 255, 255,   0, 255 }; }
    static constexpr Color cyan()        noexcept { return { 0, 255, 255, 255 }; }
    static constexpr Color magenta()     noexcept { return { 255,   0, 255, 255 }; }

    // Оттенки серого
    static constexpr Color gray()        noexcept { return { 128, 128, 128, 255 }; }
    static constexpr Color grey()        noexcept { return gray(); }
    static constexpr Color dim_gray()    noexcept { return { 105, 105, 105, 255 }; }
    static constexpr Color dark_gray()   noexcept { return { 64,  64,  64, 255 }; }
    static constexpr Color light_gray()  noexcept { return { 211, 211, 211, 255 }; }
    static constexpr Color silver()      noexcept { return { 192, 192, 192, 255 }; }
    static constexpr Color gainsboro()   noexcept { return { 220, 220, 220, 255 }; }
    static constexpr Color slate_gray()  noexcept { return { 112, 128, 144, 255 }; }
    static constexpr Color charcoal()    noexcept { return { 54,  69,  79, 255 }; }

    // Тёмные
    static constexpr Color dark_red()     noexcept { return { 139,   0,   0, 255 }; }
    static constexpr Color dark_green()   noexcept { return { 0, 100,   0, 255 }; }
    static constexpr Color dark_blue()    noexcept { return { 0,   0, 139, 255 }; }
    static constexpr Color dark_cyan()    noexcept { return { 0, 139, 139, 255 }; }
    static constexpr Color dark_magenta() noexcept { return { 139,   0, 139, 255 }; }
    static constexpr Color dark_yellow()  noexcept { return { 139, 139,   0, 255 }; }
    static constexpr Color dark_orange()  noexcept { return { 255, 140,   0, 255 }; }
    static constexpr Color dark_violet()  noexcept { return { 148,   0, 211, 255 }; }

    // Светлые
    static constexpr Color light_red()     noexcept { return { 255, 102, 102, 255 }; }
    static constexpr Color light_green()   noexcept { return { 144, 238, 144, 255 }; }
    static constexpr Color light_blue()    noexcept { return { 173, 216, 230, 255 }; }
    static constexpr Color light_cyan()    noexcept { return { 224, 255, 255, 255 }; }
    static constexpr Color light_magenta() noexcept { return { 255, 182, 255, 255 }; }
    static constexpr Color light_yellow()  noexcept { return { 255, 255, 224, 255 }; }
    static constexpr Color light_pink()    noexcept { return { 255, 182, 193, 255 }; }
    static constexpr Color light_salmon()  noexcept { return { 255, 160, 122, 255 }; }

    // Насыщенные / яркие
    static constexpr Color bright_red()     noexcept { return { 255,  69,   0, 255 }; }
    static constexpr Color bright_green()   noexcept { return { 0, 255,   0, 255 }; }
    static constexpr Color bright_blue()    noexcept { return { 30, 144, 255, 255 }; }
    static constexpr Color bright_cyan()    noexcept { return { 0, 255, 255, 255 }; }
    static constexpr Color bright_magenta() noexcept { return { 255,   0, 255, 255 }; }
    static constexpr Color bright_yellow()  noexcept { return { 255, 255,   0, 255 }; }

    // Названные (CSS-подобные)
    static constexpr Color orange()       noexcept { return { 255, 165,   0, 255 }; }
    static constexpr Color purple()       noexcept { return { 128,   0, 128, 255 }; }
    static constexpr Color pink()         noexcept { return { 255, 192, 203, 255 }; }
    static constexpr Color brown()        noexcept { return { 165,  42,  42, 255 }; }
    static constexpr Color gold()         noexcept { return { 255, 215,   0, 255 }; }
    static constexpr Color lime()         noexcept { return { 0, 255,   0, 255 }; }
    static constexpr Color teal()         noexcept { return { 0, 128, 128, 255 }; }
    static constexpr Color navy()         noexcept { return { 0,   0, 128, 255 }; }
    static constexpr Color olive()        noexcept { return { 128, 128,   0, 255 }; }
    static constexpr Color maroon()       noexcept { return { 128,   0,   0, 255 }; }
    static constexpr Color aqua()         noexcept { return { 0, 255, 255, 255 }; }
    static constexpr Color fuchsia()      noexcept { return { 255,   0, 255, 255 }; }
    static constexpr Color violet()       noexcept { return { 238, 130, 238, 255 }; }
    static constexpr Color indigo()       noexcept { return { 75,   0, 130, 255 }; }
    static constexpr Color turquoise()    noexcept { return { 64, 224, 208, 255 }; }
    static constexpr Color crimson()      noexcept { return { 220,  20,  60, 255 }; }
    static constexpr Color salmon()       noexcept { return { 250, 128, 114, 255 }; }
    static constexpr Color coral()        noexcept { return { 255, 127,  80, 255 }; }
    static constexpr Color tomato()       noexcept { return { 255,  99,  71, 255 }; }
    static constexpr Color khaki()        noexcept { return { 240, 230, 140, 255 }; }
    static constexpr Color beige()        noexcept { return { 245, 245, 220, 255 }; }
    static constexpr Color ivory()        noexcept { return { 255, 255, 240, 255 }; }
    static constexpr Color lavender()     noexcept { return { 230, 230, 250, 255 }; }
    static constexpr Color plum()         noexcept { return { 221, 160, 221, 255 }; }
    static constexpr Color orchid()       noexcept { return { 218, 112, 214, 255 }; }
    static constexpr Color sky_blue()     noexcept { return { 135, 206, 235, 255 }; }
    static constexpr Color steel_blue()   noexcept { return { 70, 130, 180, 255 }; }
    static constexpr Color royal_blue()   noexcept { return { 65, 105, 225, 255 }; }
    static constexpr Color dodger_blue()  noexcept { return { 30, 144, 255, 255 }; }
    static constexpr Color midnight_blue()noexcept { return { 25,  25, 112, 255 }; }
    static constexpr Color forest_green() noexcept { return { 34, 139,  34, 255 }; }
    static constexpr Color sea_green()    noexcept { return { 46, 139,  87, 255 }; }
    static constexpr Color spring_green() noexcept { return { 0, 255, 127, 255 }; }
    static constexpr Color olive_drab()   noexcept { return { 107, 142,  35, 255 }; }
    static constexpr Color chartreuse()   noexcept { return { 127, 255,   0, 255 }; }
    static constexpr Color chocolate()    noexcept { return { 210, 105,  30, 255 }; }
    static constexpr Color tan()          noexcept { return { 210, 180, 140, 255 }; }
    static constexpr Color wheat()        noexcept { return { 245, 222, 179, 255 }; }
    static constexpr Color coral_pink()   noexcept { return { 255, 117, 152, 255 }; }
    static constexpr Color hot_pink()     noexcept { return { 255, 105, 180, 255 }; }
    static constexpr Color deep_pink()    noexcept { return { 255,  20, 147, 255 }; }
    static constexpr Color peach_puff()   noexcept { return { 255, 218, 185, 255 }; }
    static constexpr Color misty_rose()   noexcept { return { 255, 228, 225, 255 }; }
    static constexpr Color powder_blue()  noexcept { return { 176, 224, 230, 255 }; }
    static constexpr Color honeydew()     noexcept { return { 240, 255, 240, 255 }; }
    static constexpr Color mint_cream()   noexcept { return { 245, 255, 250, 255 }; }

    // Xterm/ANSI-16 (для явной привязки к терминалам)
    static constexpr Color ansi_black()          noexcept { return { 0,   0,   0, 255 }; }
    static constexpr Color ansi_red()            noexcept { return { 205,   0,   0, 255 }; }
    static constexpr Color ansi_green()          noexcept { return { 0, 205,   0, 255 }; }
    static constexpr Color ansi_yellow()         noexcept { return { 205, 205,   0, 255 }; }
    static constexpr Color ansi_blue()           noexcept { return { 0,   0, 238, 255 }; }
    static constexpr Color ansi_magenta()        noexcept { return { 205,   0, 205, 255 }; }
    static constexpr Color ansi_cyan()           noexcept { return { 0, 205, 205, 255 }; }
    static constexpr Color ansi_white()          noexcept { return { 229, 229, 229, 255 }; }
    static constexpr Color ansi_bright_black()   noexcept { return { 127, 127, 127, 255 }; }
    static constexpr Color ansi_bright_red()     noexcept { return { 255,   0,   0, 255 }; }
    static constexpr Color ansi_bright_green()   noexcept { return { 0, 255,   0, 255 }; }
    static constexpr Color ansi_bright_yellow()  noexcept { return { 255, 255,   0, 255 }; }
    static constexpr Color ansi_bright_blue()    noexcept { return { 92,  92, 255, 255 }; }
    static constexpr Color ansi_bright_magenta() noexcept { return { 255,   0, 255, 255 }; }
    static constexpr Color ansi_bright_cyan()    noexcept { return { 0, 255, 255, 255 }; }
    static constexpr Color ansi_bright_white()   noexcept { return { 255, 255, 255, 255 }; }
};




enum class Alignment {
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight,
    Center,
};

enum class BorderStyle {
    None,
    Single,
    Double,
};



template <typename CharT>
struct BasicCell {
    CharT ch = CharT{ ' ' };
    Color fg = Color::white();
    Color bg = Color::black();

    friend bool operator==(const BasicCell &, const BasicCell &) = default;
};

using Cell = BasicCell<wchar_t>;
