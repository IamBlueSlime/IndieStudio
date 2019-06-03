/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** ecs ECSManager.hpp
*/

#pragma once

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

#include "indiestudio/ecs/TypeList.hpp"
#include "indiestudio/ecs/Entity.hpp"
#include "indiestudio/ecs/Components.hpp"
#include "indiestudio/ecs/Systems.hpp"
#include "indiestudio/ecs/Events.hpp"

namespace IndieStudio::ECS {

    using namespace IndieStudio::ECS::Component;
    using namespace IndieStudio::ECS::Event;
    using namespace IndieStudio::ECS::System;

    template<typename... ComponentTypes>
    class ECSManager {

        using EntityType = Entity<EventCallbacks<ECSManager<ComponentTypes...>>, ComponentTypes...>;
        using Components = TypeList<EventCallbacks<ECSManager<ComponentTypes...>>, ComponentTypes...>;

    public:
        ECSManager() {}
        ~ECSManager() {}

        // Iterate only on entities owning components given as template types, and apply the given lambda
        //
        // Lambda parameters are a reference on the currently inspected entity and its ID
        // This custom forEach is used by the systems to apply logic on matching entities
        template<typename... components_searched>
        void forEntitiesWith(const std::function<void (EntityType&, std::size_t id)> func) {
            const auto mask = generateMask<components_searched...>();
            for (auto &entity : this->entities) {
                if ((entity.second.component_signature & mask) == mask) {
                    func(entity.second, entity.first);
                }
            }
        }

        template<typename... SystemTypes>
        void run([[gnu::unused]] Systems<SystemTypes...> tmp) {
            auto systems = SystemsImpl<typeof(*this), EventSystem<typeof(*this)>, SystemTypes...>();

            while (true) {
                systems.process();
                this->event_manager.clear_event_queue();
            }
        }

        EventManager &getEventManager() {
            return this->event_manager;
        }

        // Add an empty entity to the manager, with a unique ID, and return a reference on it
        EntityType &addEntity() {
            EntityType &new_entity = this->entities[this->id_seed];
            new_entity.id = this->id_seed;
            this->id_seed += 1;
            return new_entity;
        }

        // Delete the given entity from the manager
        void delEntity(EntityType &entity) {
            this->entities.erase(entity.id);
        }

        // Delete the entity of the given ID from the manager
        void delEntity(std::size_t id) {
            this->entities.erase(id);
        }

        // Flush all the entities from the manager
        void flushEntities() {
            this->entities.clear();
        }

        // Return true is the given entity own the component, false otherwise
        template <typename T>
        bool hasComponent(EntityType &entity) {
            return std::get<std::optional<T>>(entity.components).has_value();
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
            return std::get<std::optional<T>>(entity.components).value();
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
            std::get<std::optional<T>>(entity.components) = value;
            entity.component_signature[Components::template getIndex<T>()] = true;
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
            std::get<std::optional<T>>(entity.components) = std::nullopt;
            entity.component_signature[Components::template getIndex<T>()] = false;
        }

        // Remove the component of the templated type from the entity matching the given ID
        template <typename T>
        void unsetComponent(size_t id) {
            unsetComponent<T>(this->entities[id]);
        }

        void recomputeEntityBitset(EntityType &entity) {
            Components::forEach([this, &entity](auto component, int idx) {
                if (this->hasComponent<typeof(component)>(entity)) {
                    entity.component_signature[idx] = true;
                } else {
                    entity.component_signature[idx] = false;
                }
            });
        }

    protected:
    private:
        template<typename... components_searched>
        constexpr std::bitset<Components::size> generateMask() {
            std::bitset<Components::size> mask;
            TypeList<components_searched...>::forEach([&mask](auto component, [[gnu::unused]] int idx) {
                mask[Components::template getIndex<typeof(component)>()] = true;
            });
            return mask;
        }

        std::map<std::size_t, EntityType> entities;
        EventManager event_manager;
        size_t id_seed = 0;
    };
}
