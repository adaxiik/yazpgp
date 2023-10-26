#pragma once
#include <functional>
namespace yazpgp
{
    template <typename T>
    class EventProducer
    {
        protected:
        std::function<void(const T&)> m_notify = nullptr;

        public:
        using Observer = std::function<void(const T&)>;
        T& set_notify_callback(Observer observer)
        {
            m_notify = observer;
            return static_cast<T&>(*this);
        }

        T& notify(const T& event)
        {
            if (m_notify)
                m_notify(event);
            return static_cast<T&>(*this);
        }
    };        
}