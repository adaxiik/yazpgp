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
        std::map<Key, std::vector<WhileKeyIsDownEvent::Callback>> m_while_key_down_callbacks;

        void dispatch(const Event& event) const;
        void dispatch(const WhileKeyIsDownEvent& event) const;

        std::array<bool, static_cast<uint32_t>(Key::YAZPGP_NUM_SCANCODES)> m_keys;
        std::array<bool, static_cast<uint32_t>(Key::YAZPGP_NUM_SCANCODES)> m_keys_previous;

        int m_mouse_x;
        int m_mouse_y;
        int m_mouse_delta_x;
        int m_mouse_delta_y;
    public:
        InputManager();
        void add_listener(const EventCallback& callback);
        void add_listener(const WhileKeyIsDownEvent::Callback& callback, Key key);

        void pool_events();
        bool get_key(Key key) const;
        bool get_key_down(Key key) const;
        bool get_key_up(Key key) const;
        int mouse_x() const;
        int mouse_y() const;
        int mouse_delta_x() const;
        int mouse_delta_y() const;
    };
}