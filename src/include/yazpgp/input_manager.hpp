#pragma once
#include <map>
#include <vector>
#include <SDL2/SDL.h>
#include "event.hpp"

template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;
 

namespace yazpgp
{
    class InputManager
    {
        std::map<EventType, std::vector<EventCallback>> m_callbacks;
        void dispatch(const Event& event) const;
    public:
        InputManager();
        void add_listener(const EventCallback& callback);
        void pool_events() const;
    };
}