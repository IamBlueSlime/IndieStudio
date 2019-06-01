/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** ecs Entity.hpp
*/

#pragma once

#include <cstdint>
#include <optional>
#include <bitset>
#include <tuple>
#include <variant>

namespace IndieStudio::ECS {

    struct EmptyComponent {};
    static inline constexpr EmptyComponent empty_component = EmptyComponent();

    template<typename... ComponentsTypes>
    struct Entity {
        std::size_t id;
        std::optional<std::size_t> parent;
        std::bitset<sizeof...(ComponentsTypes)> component_signature;
        std::tuple<std::optional<ComponentsTypes>...> components;
    };
}
