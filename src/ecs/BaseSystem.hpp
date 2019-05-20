/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** BaseSystem
*/

#ifndef BASESYSTEM_HPP_
#define BASESYSTEM_HPP_

#include "./Ecs.hpp"

namespace Ecs {
    namespace System {

        template<typename ...ComponentTypes>
        struct BaseSystem {
            BaseSystem(Manager<ComponentTypes...> &manager_)
            :
            manager(manager_) {}

            virtual void process();

            Manager<ComponentTypes...> &manager;
        };


}

#endif /* !BASESYSTEM_HPP_ */
