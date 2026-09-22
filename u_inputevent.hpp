#pragma once

enum class KeyCode {
    Unknown,
    Char,                                            // смотри .ch
    Enter, Escape, Backspace, Tab, Space,
    Left, Right, Up, Down,
    Home, End, PageUp, PageDown, Insert, Delete,
    F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
};

struct KeyModifiers {
    bool shift = false;
    bool ctrl = false;
    bool alt = false;
};

struct InputEvent {
    enum class Type {
        None,
        KeyDown, KeyUp,
        Resize, Close,
        MouseDown, MouseUp, MouseMove,
    };

    Type       type = Type::None;
    KeyCode    key = KeyCode::Unknown;
    wchar_t    ch = 0;
    KeyModifiers mods;

    size_t x = 0, y = 0;                        // мышь
    size_t width = 0, height = 0;               // resize (из события)

    bool is_key(KeyCode k) const noexcept {
        return type == Type::KeyDown && key == k;
    }
    bool is_char(wchar_t c) const noexcept {
        return type == Type::KeyDown && key == KeyCode::Char && ch == c;
    }
    bool with_ctrl()  const noexcept { return mods.ctrl; }
    bool with_alt()   const noexcept { return mods.alt; }
    bool with_shift() const noexcept { return mods.shift; }
};
