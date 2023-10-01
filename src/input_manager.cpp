#include "input_manager.hpp"
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_sdl2.h>
#include <imgui/backends/imgui_impl_opengl3.h>
namespace yazpgp
{
    InputManager::InputManager() : m_callbacks( {
        {EventType::WindowResize, {}},
        {EventType::Scroll, {}},
        {EventType::Quit, {}}
    })
    {
        static_assert(static_cast<size_t>(EventType::COUNT) == 3);
    }


    void InputManager::add_listener(const EventCallback& callback)
    {
        std::visit(ListenerAdder{m_callbacks}, callback);
    }

    void InputManager::dispatch(const Event& event) const
    {
        std::visit(EventInvoker{m_callbacks}, event);
    }

    void InputManager::pool_events() const
    {
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
        }
    }
}