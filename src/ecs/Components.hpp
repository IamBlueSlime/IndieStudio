/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Components
*/

#ifndef COMPONENTS_HPP_
#define COMPONENTS_HPP_

#include <string>

namespace Ecs::Component {

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

}

#endif /* !COMPONENTS_HPP_ */
