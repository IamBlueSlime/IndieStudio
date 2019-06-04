/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** ecs Events.hpp
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
		INDIE_MOUSE_EVENT
    };

    struct EventData {

        bool operator==(const EventData &other) const {

            if (other.type != this->type) {
                return false;
            }

            switch (this->type) {
                case EventType::INDIE_KEYBOARD_EVENT: return this->keyInput.Key == other.keyInput.Key;
                    break;
                case EventType::INDIE_MOUSE_EVENT: return this->mouseInput.Event == other.mouseInput.Event;
                    break;
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
                case IndieStudio::ECS::Event::EventType::INDIE_MOUSE_EVENT:
                    return (hash<int>()(static_cast<int>(event.type))) ^ (hash<int>()(static_cast<int>(event.mouseInput.Event) << 1)) >> 1;
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
            // std::cout << "get queue " << this->event_queue_switch <<  std::endl;
            if (this->event_queue_switch) {
                return this->event_queue2;
            } else {
                return this->event_queue1;
            }
        }

        void clear_event_queue(void) {
            // this->event_queue_switch = !this->event_queue_switch;
            // std::cout << "clear queue " << this->event_queue_switch <<  std::endl;
            this->getEventQueue().clear();
        }

        void push_event(const EventData event) {
            // std::cout << "push_event! on queue " << this->event_queue_switch << std::endl;
            if (this->event_queue_switch) {
                this->event_queue1[event] = true;
            } else {
                this->event_queue2[event] = true;
            }
        }

    private:
        std::unordered_map<EventData, bool> event_queue1;
        std::unordered_map<EventData, bool> event_queue2;
        bool event_queue_switch;
    };

    // class IrrlichEventReceiver : public irr::IEventReceiver {
    // public:
    //     IrrlichEventReceiver(EventManager &event_manager_)
    //     :
    //     event_manager(event_manager_) {}

    //     bool OnEvent(const irr::SEvent& event) override {
    //         EventData data;
    //         switch (event.EventType) {
    //             case irr::EEVENT_TYPE::EET_KEY_INPUT_EVENT:
    //                 data.type = EventType::KEYBOARD_EVENT;
    //                 data.keyInput = event.KeyInput;
    //                 this->event_manager.push_event(data);
    //                 break;
    //             default: break;
    //         }
    //         return true;
    //     }

    // private:
    //     EventManager &event_manager;
    // };

}
