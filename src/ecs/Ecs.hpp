/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Ecs
*/

#ifndef MANAGER_HPP_
#define MANAGER_HPP_

#include <cstdint>
#include <optional>
#include <bitset>
#include <tuple>
#include <map>
#include <functional>
#include <type_traits>
#include <iostream>
#include <variant>
#include <memory>
#include <cassert>

//#include "./Systems.hpp"
#include "./TypeList.hpp"
#include "./Entity.hpp"
#include "./Components.hpp"

namespace Ecs {

    namespace System {
        template<typename ...ComponentTypes>
        struct BaseSystem;
    }

    using namespace Ecs::System;
    using namespace Ecs::Component;

    template<typename... ComponentTypes>
    class Manager {

        using EntityType = Entity<ComponentTypes...>;
        using Components = TypeList<ComponentTypes...>;

    public:
        Manager() {}
        ~Manager() {}
        // Iterate only on entities owning components given as template types, and apply the given lambda
        //
        // Lambda parameters are a reference on the currently inspected entity and its ID
        // This custom forEach is used by the systems to apply logic on matching entities
        template<typename... components_searched>
        void forEntitiesWith(const std::function<void (EntityType&, std::size_t id)> func) {
            const auto mask = generateMask<components_searched...>();
            for (auto entity : this->entities) {
                if ((entity.second.component_signature & mask) == mask)
                    func(entity.second, entity.first);
            }
        }

        // Add an empty entity to the manager, with a unique ID, and return a reference on it
        EntityType &addEntity() {
            EntityType &new_entity = this->entities[EntityType::id_seed];
            EntityType::id_seed += 1;
            return new_entity;
        }

        // Delete the given entity from the manager
        void delEntity(EntityType entity) {
            this->entities.erase(entity.id);
        }

        // Delete the entity of the given ID from the manager
        void delEntity(size_t id) {
            this->entities.erase(id);
        }

        // TODO: définir une manière d'ajouter des systems
        // template<typename Types>
        // void addSystem() {
        //     Types::forEach
        // }

        // TODO: définir une manière de delete un système avec le groupe
        void delSystem() {

        }

        // Return true is the given entity own the component, false otherwise
        template <typename T>
        bool hasComponent(EntityType &entity) {
            try {
                std::get<T>(entity.components[Components::template getIndex<T>()]);
                return true;
            } catch (...) {}
            return false;
        }

        // Return true is the entity matching the given ID own the component, false otherwise
        template <typename T>
        bool hasComponent(std::size_t id) {
            return hasComponent<T>(this->entities[id]);
        }

        // Return the component of the templated type from the given entity
        //
        // Will raise error if the component is missing (meaning this function should only be called from systems)
        template <typename T>
        T &getComponent(EntityType &entity) {
            return std::get<T>(entity.components[Components::template getIndex<T>()]);
        }

        // Return the component of the templated type from entity matching the given ID
        //
        // Will raise error if the component is missing (meaning this function should only be called from systems)
        template <typename T>
        T &getComponent(size_t id) {
            return getComponent<T>(this->entities[id]);
        }

        // Set the given component in the entity
        //
        // Type is automatically deduced from the parameter type
        template <typename T>
        void setComponent(EntityType &entity, T value) {
            entity.components[Components::template getIndex<T>()] = value;
            recomputeBitset(entity);
        }

        // Set the given component in the entity matching the given ID
        //
        // Type is automatically deduced from the parameter type
        template <typename T>
        void setComponent(size_t id, T value) {
            setComponent<T>(this->entities[id], value);
        }

        // Remove the component of the templated type from the given entity
        template <typename T>
        void unsetComponent(EntityType &entity) {
            entity.components[Components::template getIndex<T>()] = empty_component;
            recomputeBitset(entity);
        }

        // Remove the component of the templated type from the entity matching the given ID
        template <typename T>
        void unsetComponent(size_t id) {
            unsetComponent<T>(this->entities[id]);
        }


    protected:
    private:

        template<typename... components_searched>
        constexpr std::bitset<Components::size> generateMask() {
            std::bitset<Components::size> mask;
            TypeList<components_searched...>::forEach([&mask](auto component, [[gnu::unused]] int idx) {
                mask[Components::template getIndex<typeof(component)>()] = true;
            });
            std::cout << "Mask of forEntityWith: " << mask << std::endl;
            return mask;
        }

        void recomputeBitset(EntityType &entity) {
            Components::forEach([this, &entity](auto component, int idx) {
                if (this->hasComponent<typeof(component)>(entity)) {
                    entity.component_signature[idx] = true;
                } else {
                    entity.component_signature[idx] = false;
                }
            });
            std::cout << "Changes detected in entity ; recomputed bitmap : " << entity.component_signature << std::endl;
        }

        std::map<std::size_t, EntityType> entities; //TODO entity en unique ptr! Possibilité d'en ajouter / retirer oklm
//        std::vector<BaseSystem> systems;    //TODO systems en unique ptr!
    };
}

#endif /* !MANAGER_HPP_ */