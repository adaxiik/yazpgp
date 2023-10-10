#pragma once
#include <vector>
#include <functional>
namespace yazpgp
{
    template <typename T>
    class EventSource
    {
    public:
        using Observer = std::function<void(const T&)>;
        void notify(const T& event)
        {
            for (const auto& observer : m_observers)
                observer(event);
        }

        void subscribe(Observer observer)
        {
            m_observers.push_back(observer);
        }
    private:
        std::vector<Observer> m_observers;
    };
}