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
    : m_callbacks()
    , m_keys()
    , m_keys_previous()
    , m_mouse_x(0)
    , m_mouse_y(0)
    , m_mouse_delta_x(0)
    , m_mouse_delta_y(0)
    {
        // static_assert(static_cast<size_t>(EventType::COUNT) == 3);

        std::fill(m_keys.begin(), m_keys.end(), false);
        std::fill(m_keys_previous.begin(), m_keys_previous.end(), false);

        for (size_t i = 0; i < static_cast<size_t>(EventType::COUNT); ++i)
            m_callbacks.insert({static_cast<EventType>(i), {}});

    }

    void InputManager::add_listener(const EventCallback& callback)
    {
        std::visit(ListenerAdder{m_callbacks}, callback);
    }

    void InputManager::add_listener(const WhileKeyIsDownEvent::Callback& callback, Key key)
    {
        m_while_key_down_callbacks[key].push_back(callback);
    }

    void InputManager::dispatch(const Event& event) const
    {
        std::visit(EventInvoker{m_callbacks}, event);
    }

    void InputManager::dispatch(const WhileKeyIsDownEvent& event) const
    {
        if (m_while_key_down_callbacks.count(event.key))
            for (const auto& callback : m_while_key_down_callbacks.at(event.key))
                callback(event);
    }

    void InputManager::pool_events()
    {
        std::copy(m_keys.begin(), m_keys.end(), m_keys_previous.begin());
        m_mouse_delta_x = 0;
        m_mouse_delta_y = 0;
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);

            switch (event.type)
            {
                case SDL_QUIT:
                {
                    dispatch(QuitEvent{});
                    break;
                }
                case SDL_WINDOWEVENT:
                {
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                        dispatch(WindowResizeEvent{
                            .width = static_cast<uint32_t>(event.window.data1),
                            .height = static_cast<uint32_t>(event.window.data2)
                        });

                    break;
                }
                case SDL_MOUSEWHEEL:
                {
                    dispatch(ScrollEvent{
                        .x_offset = event.wheel.x,
                        .y_offset = event.wheel.y,
                    });

                    break;
                }
                case SDL_KEYDOWN:
                {
                    auto scancode = event.key.keysym.scancode;
                    m_keys[static_cast<size_t>(Key{scancode})] = true;
                    dispatch(KeyDownEvent{
                        .key = Key{scancode}
                    });
                    break;
                }
                case SDL_KEYUP:
                {
                    auto scancode = event.key.keysym.scancode;
                    m_keys[static_cast<size_t>(Key{scancode})] = false;
                    dispatch(KeyUpEvent{
                        .key = Key{scancode}
                    });
                    break;
                }
                case SDL_MOUSEMOTION:
                {
                    m_mouse_x = event.motion.x;
                    m_mouse_y = event.motion.y;
                    m_mouse_delta_x = event.motion.xrel;
                    m_mouse_delta_y = event.motion.yrel;
                    break;
                }
                default:
                    break;
            }
        }

        for (int i = 0; i < static_cast<int>(Key::YAZPGP_NUM_SCANCODES); i++)
        {
            if (m_keys[i])
                dispatch(WhileKeyIsDownEvent{.key = Key{i}});
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

    int InputManager::mouse_x() const
    {
        return m_mouse_x;
    }

    int InputManager::mouse_y() const
    {
        return m_mouse_y;
    }

    int InputManager::mouse_delta_x() const
    {
        return m_mouse_delta_x;
    }

    int InputManager::mouse_delta_y() const
    {
        return m_mouse_delta_y;
    }

}