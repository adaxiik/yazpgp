#pragma once

#include <functional>
#include <vector>
#include <variant>
#include <cstdint>

namespace yazpgp
{

    // https://stackoverflow.com/questions/9065081/how-do-i-get-the-argument-types-of-a-function-pointer-in-a-variadic-template-cla
    template<typename T> 
    struct function_traits;  

    template<typename R, typename ...Args> 
    struct function_traits<std::function<R(Args...)>>
    {
        static const size_t nargs = sizeof...(Args);

        typedef R result_type;

        template <size_t i>
        struct arg
        {
            typedef typename std::tuple_element<i, std::tuple<Args...>>::type type;
        };
    };


    enum class EventType
    {
        WindowResize,
        Scroll,
        Quit,

        COUNT
    };

    #define TYPE(X) \
        constexpr static EventType type = EventType::X;

    
    struct WindowResizeEvent
    {
        uint32_t width;
        uint32_t height;
        using Callback = std::function<void(const WindowResizeEvent&)>;

        TYPE(WindowResize)
    };


    struct ScrollEvent
    {
        int x_offset;
        int y_offset;
        using Callback = std::function<void(const ScrollEvent&)>;

        TYPE(Scroll)
    };

    struct QuitEvent
    {
        using Callback = std::function<void(const QuitEvent&)>;

        TYPE(Quit)
    };

    using EventCallback = std::variant<
        WindowResizeEvent::Callback,
        ScrollEvent::Callback,
        QuitEvent::Callback>;
        
    using Event = std::variant<WindowResizeEvent,
        ScrollEvent,
        QuitEvent>;

    static_assert(std::variant_size_v<EventCallback> == std::variant_size_v<Event>);
    static_assert(static_cast<size_t>(EventType::COUNT) == std::variant_size_v<Event>);


    struct EventInvoker
    {
        const std::map<EventType, std::vector<EventCallback>>& all_callbacks;

        EventInvoker(const std::map<EventType, std::vector<EventCallback>>& all_callbacks)
            : all_callbacks(all_callbacks) {}

        template<typename T>
        void operator()(const T& event) const
        {

            const auto& callbacks = all_callbacks.at(T::type);
            for (auto& callback : callbacks)
                std::get<typename T::Callback>(callback)(event);
        }
    };

    struct ListenerAdder
    {
        std::map<EventType, std::vector<EventCallback>>& callbacks;
        ListenerAdder(std::map<EventType, std::vector<EventCallback>>& callbacks)
            : callbacks(callbacks) {}

        template<typename T>
        void operator()(const T& callback) const
        {
            using EventRef = typename function_traits<T>::arg<0>::type;
            using Event = typename std::remove_reference<EventRef>::type;
            callbacks[Event::type].push_back(callback);
        }
    };

}