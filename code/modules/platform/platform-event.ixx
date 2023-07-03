module;

export module platform:event;

import cstd;
import core;

export enum class EventCode : u8
{
    app_quit = 0x01,
    app_close,
    app_destroy,

    key_pressed,
    key_released,

    button_pressed,
    button_released,

    mouse_moved,
    mouse_wheel,

    window_resized,

    event_builtin_max = 0xFF
};

export union Event
{ // A 128-bit union for builtin and custom events
	s16 key;
    s16 button;

    s16 mouse_position[2];
    s8  mouse_delta;

    s32 window_size[2];

    s64 custom_data[2];
};

export using event_callback_pfn = bool (*)(u16 code, Event event, void* listener_inst, void* sender);

export class EventSystem 
{
public:
    EventSystem()
    { //can leave things value-initialized
    }

    //std::array should handle cleanup
    ~EventSystem() {}

    bool register_event(u16 code, void* listener, event_callback_pfn on_event_fire)
    {
        if(code >= c_max_event_codes)
        {
            return false;
        }

        EventCodeEntry& entry = m_registered_events[code];
        for (const auto& event : entry)
        {
            if (listener == event.listener && on_event_fire == event.callback)
            { // attempting to add duplicate event
                return false;
            }
        }

        entry.push_back({
            .listener = listener,
            .callback = on_event_fire,
        });

        return true;
    }

    bool unregister_event(u16 code, void* listener, event_callback_pfn on_event_fire)
    {
        if(code >= c_max_event_codes)
        {
            return false;
        }

        EventCodeEntry& entry = m_registered_events[code];
        if (entry.size() == 0) 
            return false; // no events to unregister

        for (s64 index = 0; const auto& event : entry)
        {
            if (listener == event.listener && on_event_fire == event.callback)
            {
                entry[index] = entry[entry.size() - 1];
                entry.pop_back();
                return true;
            }
            ++index;
        }

        return false;
    }

    bool fire_event(u16 code, Event event, void* sender = nullptr)
    {
        if(code >= c_max_event_codes)
        {
            return false;
        }

        EventCodeEntry& entry = m_registered_events[code];
        if (entry.size() == 0) 
            return false; // no events to fire

        for (const auto& registered_event : entry)
        {
            if (registered_event.callback(code, event, registered_event.listener, sender))
            {
                return true;
            }
        }

        return false; // event not found
    }

private:
    static constexpr u64 c_max_event_codes = 16384;

    struct RegisteredEvent
    {
        void*              listener = nullptr;
        event_callback_pfn callback = nullptr;
    };

    using EventCodeEntry = std::vector<RegisteredEvent>;

    std::array<EventCodeEntry, c_max_event_codes> m_registered_events{};
};