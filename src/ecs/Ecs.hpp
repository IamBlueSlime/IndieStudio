#include <cstdint>
#include <optional>
#include <bitset>
#include <tuple>
#include <map>
#include <functional>
#include <type_traits>
#include <iostream>

struct Component1 {
    int x;
    int y;
};

struct Component2 {
    std::string str;
};

struct Component3 {
};

namespace Ecs
{

    template<typename... Types>
    struct TypeList {

        static constexpr auto size = sizeof...(Types);

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
//            static_assert(idx != idx, "The type you are searching for is not in the TypeList! Are you using systems without appropriate components?\n")
            return -1;
        }

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
        static constexpr void forEach(Callable func) {
        }
    };

    template<typename... components_types>
    struct Entity {
        std::size_t id;
        std::optional<std::size_t> parent;
        std::bitset<sizeof...(components_types)> component_signature;
        std::tuple<std::optional<components_types>...> components;     //TODO: faire un tuple d optionnal (HARDCORE)
    };

    template<typename... components_types>
    class Manager {
        using Components = TypeList<components_types...>;
        using EntityType = Entity<components_types...>;

    public:
        Manager() {
            EntityType test1;
            if (std::get<0>(test1.components) == std::nullopt) {
                std::cout << "Wesh ia R! les std optional marchent!" << std::endl;
            } else {
                std::cout << "Ia un problème! il devrait pas y avoir qqch!" << std::endl;
            }

            Component1 first;
            Component2 second;
            Component3 third;
            std::get<0>(test1.components) = std::make_optional(first);

            // Testing possibilty to pass variable to std::get at compile time using constexpr...
            // Seems to be fucking difficult... That was not expected. Currently working on this
            //
            // std::cout << "Idx du comp 1: " << Components::template getIndex<Component1>() << std::endl;
            // constexpr int idx = Components::template getIndex<Component1>();
            // std::get<idx>(test1.components) = std::make_optional(first);
            recomputeBitset(test1);
            //test1.component_signature[0] = true;

            EntityType test2;
            std::get<0>(test2.components) = std::make_optional(first);
            std::get<1>(test2.components) = std::make_optional(second);
            recomputeBitset(test2);
            //test2.component_signature[0] = true;
            //test2.component_signature[1] = true;

            EntityType test3;
            std::get<0>(test3.components) = std::make_optional(first);
            std::get<1>(test3.components) = std::make_optional(second);
            std::get<2>(test3.components) = std::make_optional(third);
            recomputeBitset(test3);
            //test3.component_signature[0] = true;
            //test3.component_signature[1] = true;
            //test3.component_signature[2] = true;

            this->entities[0] = test1;
            this->entities[1] = test2;
            this->entities[2] = test3;
        }
        ~Manager() {}

        template<typename... components_searched>
        constexpr std::bitset<Components::size> generateMask() {
            std::bitset<Components::size> mask;
            TypeList<components_searched...>::forEach([&mask](auto component, int idx) {
                int tmp = Components::template getIndex<typeof(component)>();
                std::cout << "Found " << tmp << std::endl;
                if (tmp != -1) {
                    mask[tmp] = true;
                }
            });
            std::cout << "Mask: " << mask << std::endl;
            return mask;
        }

        template<typename... components_searched>
        void forEntitiesWith(const std::function<void (EntityType&, std::size_t id)> func) {
            const auto mask = generateMask<components_searched...>();
            for (auto entity : this->entities) {
                if ((entity.second.component_signature & mask) == mask)
                    func(entity.second, entity.first);
            }
        }

        // TODO: Pour recalculer le bitset lors de l'ajout / deletion d'un component à une entitée
        void recomputeBitset(EntityType &entity) {
            // Components::forEach([this, &entity](auto component, int idx) {
            //     if (this->getComponent<typeof(component)>(entity) == std::nullopt) {
            //         entity.component_signature[idx] = false;
            //     } else {
            //         entity.component_signature[idx] = true;
            //     }
            // });
            // std::cout << "recomputed bitmap : " << entity.component_signature << std::endl;
        }

        // TODO: patch this function; actually, std::get can't take runtime variables :()
        template<typename T>
        std::optional<T> getComponent(const EntityType &entity) {
            return std::get<Components::template getIndex<T>()>(entity.components);
        }

        // TODO: définir une manière d'ajouter des entitées
        void addEntity() {

        }

        // TODO: définir une manière de delete une entititée avec le groupe
        void delEntity() {

        }

        // TODO: définir une manière d'ajouter des systems
        void addSystem() {

        }

        // TODO: définir une manière de delete un système avec le groupe
        void delSystem() {

        }

    protected:
    private:
        std::map<std::size_t, EntityType> entities; //TODO entity en unique ptr! Possibilité d'en ajouter / retirer oklm
    };
}