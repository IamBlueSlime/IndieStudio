/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Entity
*/

#ifndef ENTITY_HPP_
#define ENTITY_HPP_

#include <cstdint>
#include <optional>
#include <bitset>
#include <tuple>
#include <variant>

namespace Ecs {

    struct EmptyComponent {};
    static inline constexpr EmptyComponent empty_component = EmptyComponent();

    template<typename... ComponentsTypes>
    struct Entity {
        std::size_t id;
        std::optional<std::size_t> parent;
        std::bitset<sizeof...(ComponentsTypes)> component_signature;
        std::array<std::variant<EmptyComponent, ComponentsTypes...>, sizeof...(ComponentsTypes)> components;

        static size_t id_seed;
    };
}

template<typename... ComponentsTypes>
size_t Ecs::Entity<ComponentsTypes...>::id_seed = 0;

#endif /* !ENTITY_HPP_ */