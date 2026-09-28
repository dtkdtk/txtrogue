#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

#include "gamecore.hpp"
#include "u_console.hpp"
#include "u_console_win32.hpp"

// ---------- глобальное состояние рендера ----------
std::mutex              g_mtx;
std::condition_variable g_cv;
bool                    g_dirty = true;    // первый кадр рисуем сразу
bool                    g_quit = false;

void mark_dirty() {
    { std::lock_guard lk{ g_mtx }; g_dirty = true; }
    g_cv.notify_one();
}

void request_quit() {
    { std::lock_guard lk{ g_mtx }; g_quit = true; }
    g_cv.notify_all();
}

// ---------- рендер-поток ----------
void render_worker(Console & cs, Win32ConsoleRenderer<wchar_t> & r) {
    std::unique_lock lk{ g_mtx };
    for (;;) {
        g_cv.wait(lk, [] { return g_dirty || g_quit; });
        if (g_quit) return;
        g_dirty = false;
        lk.unlock();
        r.present(cs);          // <- единственный WriteConsoleOutputW
        lk.lock();
    }
}

// ---------- main ----------
int main() {
    std::ios_base::sync_with_stdio(false);
    Win32ConsoleRenderer<wchar_t> Renderer{};
    if (!Renderer.init())
        throw std::runtime_error("Failed to initialize renderer");

    Console Cs{ Renderer.width(), Renderer.height() };
    //std::jthread renderer_thread{ render_worker, std::ref(Cs), std::ref(Renderer) };

    GameMap map{ 20, 10 };
    for (auto & t : map.tiles_flat()) t.cell.ch = L'.';
    map.tile_at(0, 0).cell.ch = L'@';

    auto redraw = [&] {
        Cs.clear();
        map.draw_into(Cs, 1, 1);
        Cs.put_string(0, 0, L"WASD/arrows: move, Ctrl+Q: quit, close button: quit");
        mark_dirty();
    };
    redraw();

    bool running = true;
    while (running) {
        InputEvent ev;
        if (!Renderer.wait_event(ev, 100)) continue;

        switch (ev.type) {
        case InputEvent::Type::Close:
            running = false;
            break;

        case InputEvent::Type::Resize:
            Cs.resize(Renderer.width(), Renderer.height());
            redraw();
            break;

        case InputEvent::Type::KeyDown:
            if (ev.is_key(KeyCode::Escape) ||
                (ev.ch == L'Q' && ev.with_ctrl())) {
                running = false;
            }
            else if (ev.is_char(L'w') || ev.is_key(KeyCode::Up)) {
                /* ... */
            }
            else if (ev.is_char(L'a') || ev.is_key(KeyCode::Left)) {
                /* ... */
            }
            else if (ev.is_char(L's') || ev.is_key(KeyCode::Down)) {
                /* ... */
            }
            else if (ev.is_char(L'd') || ev.is_key(KeyCode::Right)) {
                /* ... */
            }
            else if (ev.is_key(KeyCode::F5)) {
                redraw();
            }
            break;

        case InputEvent::Type::KeyUp:
        case InputEvent::Type::MouseDown:
        case InputEvent::Type::MouseUp:
        case InputEvent::Type::MouseMove:
        default:
            break;
        }
    }
    std::cout << "\nCLOSE\n\n";

    request_quit();     // разбудить рендер-поток
    // jthread сам сделает join в деструкторе
    Renderer.shutdown();
    return 0;
}
