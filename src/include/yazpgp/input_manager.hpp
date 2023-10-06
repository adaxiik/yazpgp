#pragma once
#include <map>
#include <vector>
#include <SDL2/SDL.h>
#include <array>
#include "event.hpp"
#include "keys.hpp"

namespace yazpgp
{
    class InputManager
    {
        std::map<EventType, std::vector<EventCallback>> m_callbacks;
        void dispatch(const Event& event) const;

        std::array<bool, static_cast<uint32_t>(Key::YAZPGP_NUM_SCANCODES)> m_keys;
        std::array<bool, static_cast<uint32_t>(Key::YAZPGP_NUM_SCANCODES)> m_keys_previous;
        // TODOO:
        int m_mouse_x;
        int m_mouse_y;
        int m_mouse_x_previous;
        int m_mouse_y_previous;
    public:
        InputManager();
        void add_listener(const EventCallback& callback);
        void pool_events();
        bool get_key(Key key) const;
        bool get_key_down(Key key) const;
        bool get_key_up(Key key) const;
    };
}