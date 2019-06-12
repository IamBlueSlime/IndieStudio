/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** common Events.hpp
*/

#pragma once

#include <cmath>
#include <unordered_map>
#include <iostream>
#include <cassert>
#include <irrlicht.h>

namespace IndieStudio::ECS::Event {

    enum class EventType {
		INDIE_KEYBOARD_EVENT,
		INDIE_JOYSTICK_EVENT,
		INDIE_MOUSE_EVENT,
        INDIE_UNIQUE_TAG_EVENT
    };

    struct EventData {

        bool operator==(const EventData &other) const {

            if (other.type != this->type) {
                return false;
            }

            switch (this->type) {
                case EventType::INDIE_KEYBOARD_EVENT: return this->keyInput.Key == other.keyInput.Key;
                case EventType::INDIE_JOYSTICK_EVENT:
                    return this->joystickEvent.Axis == other.joystickEvent.Axis
                    && this->joystickEvent.ButtonStates == other.joystickEvent.ButtonStates
                    && this->joystickEvent.Joystick == other.joystickEvent.Joystick;
                case EventType::INDIE_MOUSE_EVENT: return this->mouseInput.Event == other.mouseInput.Event;
                case EventType::INDIE_UNIQUE_TAG_EVENT: return this->uniqueTag == other.uniqueTag;
                default:
                    std::cout << "operator== not implemented on this ECS::Event::EventData : aborting" << std::endl;
                    assert(false);
                    return false;
            }
        }

        EventType type;

        union {
            struct irr::SEvent::SKeyInput keyInput;
            struct irr::SEvent::SJoystickEvent joystickEvent;
            struct irr::SEvent::SMouseInput mouseInput;
            long uniqueTag;
        };
    };

}

namespace std {

    template<>
    struct hash<IndieStudio::ECS::Event::EventData> {

        std::size_t operator()([[gnu::unused]] const IndieStudio::ECS::Event::EventData &event) const
        {
            switch (event.type) {
                case IndieStudio::ECS::Event::EventType::INDIE_KEYBOARD_EVENT:
                    return (hash<int>()(static_cast<int>(event.type))) ^ (hash<int>()(static_cast<int>(event.keyInput.Key) << 1)) >> 1;
                case IndieStudio::ECS::Event::EventType::INDIE_JOYSTICK_EVENT: return 0;
                case IndieStudio::ECS::Event::EventType::INDIE_MOUSE_EVENT:
                    return (hash<int>()(static_cast<int>(event.type))) ^ (hash<int>()(static_cast<int>(event.mouseInput.Event) << 1)) >> 1;
                case IndieStudio::ECS::Event::EventType::INDIE_UNIQUE_TAG_EVENT:
                    return (hash<int>()(static_cast<int>(event.type))) ^ (hash<long>()(event.uniqueTag));
                default:
                    std::cout << "hash not implemented on this ECS::Event::EventData <<: aborting" << std::endl;
                    assert(false);
                    return 0;
            }
        }
    };
}

namespace IndieStudio::ECS::Event {

    class EventManager {
    public:
        std::unordered_map<EventData, bool> &getEventQueue() {
            // if (this->event_queue_switch) {
            //     return this->event_queue2;
            // } else {
                return this->event_queue1;
            // }
        }

        void clear_event_queue() {
            this->getEventQueue().clear();
        }

        void switch_event_queue() {
            this->event_queue_switch = !this->event_queue_switch;
        }

        void push_event(const EventData event) {
            // if (this->event_queue_switch) {
                this->event_queue1[event] = true;
            // } else {
            //     this->event_queue2[event] = true;
            // }
        }

    private:
        std::unordered_map<EventData, bool> event_queue1;
        std::unordered_map<EventData, bool> event_queue2;
        bool event_queue_switch = false;
    };

}
