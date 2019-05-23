/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Events
*/

#ifndef EVENTS_HPP_
#define EVENTS_HPP_

#include <cmath>
#include <vector>
#include <mutex>
#include <iostream>
#include <cassert>

#include <irrlicht/IEventReceiver.h>

namespace Ecs {
    namespace Event {

        enum class EventType {
            GUI_EVENT,
            JOYSTICK_EVENT,
            KEYBOARD_EVENT,
            MOUSE_EVENT,
            CUSTOM_EVENT_1
        };

        struct EventData {

            bool operator==(const EventData &other) const {

                if (other.type != this->type) {
                    return false;
                }

                switch (this->type) {
                    case EventType::GUI_EVENT: return this->guiEvent.EventType == other.guiEvent.EventType;
                        break;
                    case EventType::KEYBOARD_EVENT: return this->keyInput.Key == other.keyInput.Key;
                        break;
                    case EventType::MOUSE_EVENT: return this->mouseInput.Event == other.mouseInput.Event;
                        break;
                    case EventType::CUSTOM_EVENT_1: return this->custom_event_1 == other.custom_event_1;
                    default:
                        std::cout << "operator== not implemented on Ecs::Event::EventData : aborting" << std::endl;
                        assert(false);
                        return false;
                }
            }

            EventType type;

            union {
                struct irr::SEvent::SGUIEvent guiEvent;
                struct irr::SEvent::SJoystickEvent joystickEvent;
                struct irr::SEvent::SKeyInput keyInput;
                struct irr::SEvent::SMouseInput mouseInput;
                bool custom_event_1;
            };
        };

        class EventManager {
        public:
            std::vector<EventData> &getEventQueue() { return this->event_queue; }

            void push_event(const EventData &event) {
                std::unique_lock<std::mutex> lock;
                event_queue.push_back(event);
            }

        private:
            std::vector<EventData> event_queue;
            std::mutex mutex;
        };
    }
}

namespace std {

    template<>
    struct hash<Ecs::Event::EventData> {

        std::size_t operator()([[gnu::unused]] const Ecs::Event::EventData &event) const
        {
            return static_cast<std::size_t>(std::exp(0) - 1.0);
        }
    };
}

#endif /* !EVENTS_HPP_ */