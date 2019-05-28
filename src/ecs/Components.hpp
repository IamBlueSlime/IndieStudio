/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Components
*/

#ifndef COMPONENTS_HPP_
#define COMPONENTS_HPP_

#include <string>
#include <unordered_map>
#include <map>
#include <vector>
#include <functional>

#include "./Events.hpp"

namespace Ecs::Component {

    using namespace Ecs::Event;

    struct Component1 {
        int x;
        int y;
    };

    struct Component2 {
        Component2(const std::string &str_ = "Default")
        :
        str(str_) {}
        std::string str;
    };

    struct Component3 {
    };

    struct Drawable {

    };

    // struct AnimatedMesh : irr::scene::IAnimatedMeshSceneNode {
    // };

    template<typename ManagerType>
    struct EventCallbacks {

        using CallbackType = std::function<void(const EventData&, std::size_t, ManagerType &)>;

    public:
        const auto &getCallbacks() { return callbacks; }

        void addCallback(const EventData &event, CallbackType &&callback) {
            this->callbacks[event].push_back(std::forward<CallbackType>(callback));
        }

    private:
        std::unordered_map<EventData, std::vector<CallbackType>> callbacks;
    };

}

#endif /* !COMPONENTS_HPP_ */