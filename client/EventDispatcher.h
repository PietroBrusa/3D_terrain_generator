#pragma once

#include <vector>
#include <map>

#include <string>
#include <functional>
#include <algorithm>

template<typename... Args>

class EventDispatcher {
public:
    static EventDispatcher<Args...>& getInstance()
    {
        static EventDispatcher<Args...> instance;
        return instance;
    }

    size_t subscribe(const std::string& eventId, std::function<void(Args...)> callback) 
    {
        size_t id = nextId++;
        listeners[eventId].push_back({ id, callback });
        return id;
    }

    void unsubscribe(const std::string& eventId, size_t id)
    {
        auto it = listeners.find(eventId);
        if (it != listeners.end())
        {
            auto& list = it->second;
            auto originalSize = list.size();

            list.erase(std::remove_if(list.begin(), list.end(),
                [id](const Handler& h) { return h.id == id; }), list.end());

            if (list.empty()) {
                listeners.erase(it);
            }
        }
    }

    void dispatch(const std::string& eventId, Args... args)
    {
        if (listeners.count(eventId)) {
            for (auto& handler : listeners[eventId]) {
                handler.cb(args...);
            }
        }
    }

private:
    struct Handler {
        size_t id;
        std::function<void(Args...)> cb;
    };

    EventDispatcher() : nextId(0) {}
    std::map<std::string, std::vector<Handler>> listeners;
    size_t nextId;
};
