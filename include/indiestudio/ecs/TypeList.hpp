/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** ecs TypeList.hpp
*/

#pragma once

#include <cassert>

namespace IndieStudio::ECS {

    template<typename... Types>
    struct TypeList {

        /*  Size    */

        static constexpr auto size = sizeof...(Types);

        /*  getIndex    */

        template<typename ToSearch>
        static constexpr int getIndex() {
            return getIndex<ToSearch, 0, Types...>();
        }

        template<typename ToSearch, int idx, typename Head, typename... Tail>
        static constexpr int getIndex() {
            if (typeid(Head) == typeid(ToSearch)) {
                return idx;
            }
            return getIndex<ToSearch, idx + 1, Tail...>();
        }

        template<typename ToSearch, int idx>
        static constexpr int getIndex() {
            std::cerr << "The type you are searching for is not in the TypeList! Are you using systems without appropriate components?\n" << std::endl;
            assert(false);
            return -1;
        }

        /*  forEach */

        template<typename Callable>
        static constexpr void forEach(Callable func) {
            forEach<Callable, 0, Types...>(func);
        }

        template<typename Callable, int idx, typename Head, typename... Tail>
        static constexpr void forEach(Callable func) {
            func(Head(), idx);
            forEach<Callable, idx + 1, Tail...>(func);
        }

        template<typename Callable, int idx>
        static constexpr void forEach([[gnu::unused]] Callable func) {
        }

    };
}
