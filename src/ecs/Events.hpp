/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Events
*/

#ifndef EVENTS_HPP_
#define EVENTS_HPP_

#include <cmath>
#include <unordered_map>
#include <mutex>
#include <iostream>
#include <cassert>
#include <thread>

#include <irrlicht/irrlicht.h>
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
                        std::cout << "operator== not implemented on this Ecs::Event::EventData : aborting" << std::endl;
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

namespace Ecs {
    namespace Event {

        class EventManager {
        public:
            EventManager();

            std::unordered_map<EventData, bool> &getEventQueue() {
                if (this->event_queue_switch) {
                    return this->event_queue2;
                } else {
                    return this->event_queue1;
                }
            }

            void clear_event_queue(void) {
                this->getEventQueue().clear();
                this->event_queue_switch = !this->event_queue_switch;
            }

            void push_event(const EventData &event) {
                std::unique_lock<std::mutex> lock;
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
            //TODO no need of mutex is the device is in the juel's singleton
            std::mutex mutex;
            irr::IrrlichtDevice *device = irr::createDevice(irr::video::EDT_OPENGL, irr::core::dimension2d<irr::u32>(1280, 960));;
            std::thread thread;
        };

        class IrrlichEventReceiver : public irr::IEventReceiver {
        public:
            IrrlichEventReceiver(EventManager &event_manager_)
            :
            event_manager(event_manager_) {}

            bool OnEvent(const irr::SEvent& event) override {
                EventData data;
                switch (event.EventType) {
                    case irr::EEVENT_TYPE::EET_KEY_INPUT_EVENT:
                        data.type = EventType::KEYBOARD_EVENT;
                        data.keyInput = event.KeyInput;
                        this->event_manager.push_event(data);
                        break;
                    default: break;
                }
                return true;
            }

        private:
            EventManager &event_manager;
        };

        inline EventManager::EventManager() {
            device->setEventReceiver(new IrrlichEventReceiver(*this));
            //TODO: utiliser le vrai device
            this->thread = std::thread([this]() {
                while (true) {
                    this->device->run();
                }
            });
        }
    }
}

#endif /* !EVENTS_HPP_ */