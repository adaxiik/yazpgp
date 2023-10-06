#include "input_manager.hpp"
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_sdl2.h>
#include <imgui/backends/imgui_impl_opengl3.h>


template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;
 

namespace yazpgp
{
    InputManager::InputManager() 
    : m_callbacks( {
        {EventType::WindowResize, {}},
        {EventType::Scroll, {}},
        {EventType::Quit, {}},
    })
    , m_keys()
    , m_keys_previous()
    , m_mouse_x(0)
    , m_mouse_y(0)
    , m_mouse_x_previous(0)
    , m_mouse_y_previous(0)
    {
        static_assert(static_cast<size_t>(EventType::COUNT) == 3);

        std::fill(m_keys.begin(), m_keys.end(), false);
        std::fill(m_keys_previous.begin(), m_keys_previous.end(), false);

    }

    void InputManager::add_listener(const EventCallback& callback)
    {
        std::visit(ListenerAdder{m_callbacks}, callback);
    }

    void InputManager::dispatch(const Event& event) const
    {
        std::visit(EventInvoker{m_callbacks}, event);
    }

    void InputManager::pool_events()
    {
        std::copy(m_keys.begin(), m_keys.end(), m_keys_previous.begin());

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
            {
                dispatch(QuitEvent{});
            }
            else if (event.type == SDL_WINDOWEVENT)
            {
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    dispatch(WindowResizeEvent{
                        .width = static_cast<uint32_t>(event.window.data1),
                        .height = static_cast<uint32_t>(event.window.data2)
                    });
                }
            }
            else if (event.type == SDL_MOUSEWHEEL)
            {
                dispatch(ScrollEvent{
                    .x_offset = event.wheel.x,
                    .y_offset = event.wheel.y
                });
            }
            else if (event.type == SDL_KEYDOWN)
            {
                m_keys[static_cast<size_t>(Key{event.key.keysym.scancode})] = true;
            }
            else if (event.type == SDL_KEYUP)
            {
                m_keys[static_cast<size_t>(Key{event.key.keysym.scancode})] = false;
            }
            // TODOO: mouse
        }
    }

    bool InputManager::get_key(Key key) const
    {
        return m_keys[static_cast<size_t>(key)];
    }

    bool InputManager::get_key_down(Key key) const
    {
        return m_keys[static_cast<size_t>(key)] && !m_keys_previous[static_cast<size_t>(key)];
    }

    bool InputManager::get_key_up(Key key) const
    {
        return !m_keys[static_cast<size_t>(key)] && m_keys_previous[static_cast<size_t>(key)];
    }

}