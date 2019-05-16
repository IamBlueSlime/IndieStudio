#include <cstdint>
#include <optional>
#include <bitset>
#include <tuple>
#include <map>
#include <functional>
#include <type_traits>
#include <iostream>

namespace Ecs
{

    template<typename... Types>
    struct TypeList {

        static constexpr auto size = sizeof...(Types);

        template<typename ToSearch>
        static int getIndex() {
            return getIndex<ToSearch, 0, Types...>();
        }

        template<typename ToSearch, int idx, ToSearch, typename... Tail>
        static int getIndex() {
            return idx;
        }

        template<typename ToSearch, int idx, typename Head, typename... Tail>
        static int getIndex() {
            return getIndex<ToSearch, idx + 1, Tail...>();
        }

        template<typename ToSearch, int idx>
        static int getIndex() {
            return -1;
        }

        template<typename Callable>
        static void forEach(Callable func) {
            forEach<Callable, 0, Types...>(func);
        }

        template<typename Callable, int idx, typename Head, typename... Tail>
        static void forEach(Callable func) {
            func(Head(), idx);
            forEach<Callable, idx + 1, Tail...>(func);
        }

        template<typename Callable, int idx>
        static void forEach(Callable func) {
        }
    };

    template<typename... components_types>
    struct Entity {
        std::size_t id;
        std::optional<std::size_t> parent;
        std::bitset<sizeof...(components_types)> component_signature;
        std::tuple<components_types...> components;     //TODO: faire un tuple d optionnal (HARDCORE)
    };

    template<typename... components_types>
    class Manager {
        using Components = TypeList<components_types...>;
        using EntityType = Entity<components_types...>;

    public:
        Manager() {
            std::cout << "Length of typelist : " << Components::size << std::endl;
            std::cout << "Index of string : " << Components::template getIndex<std::string>() << std::endl;
            std::cout << "Index of bool : " << Components::template getIndex<bool>() << std::endl;
            std::cout << "Index of int : " << Components::template getIndex<int>() << std::endl;
        }
        ~Manager() {}

        template<typename... components_searched>
        void forEntitiesWith(const std::function<void (EntityType&, std::size_t id)>) {
            std::bitset<Components::size> mask;
            std::cout << mask << std::endl;
            TypeList<components_searched...>::forEach([&mask](auto component, int idx) {
                int tmp = Components::template getIndex<typeof(component)>();
                if (tmp != -1) {
                    mask[tmp] = true;
                }
            });
            std::cout << mask << std::endl;
            // TODO: bitmask generated! But should be modified using a constexpr, this mask can be generated at compile time
            for (auto entity : this->entities) {
                std::cout << typeid(entity).name() << std::endl;
                // TODO: check si la signature match et lance la lambda
            }
        }

        // TODO: Pour recalculer le bitset lors de l'ajout / deletion d'un component à une entitée
        void recomputeBitset(EntityType entity) {
            // recalculate the component signature of an entity
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
        std::map<std::size_t, EntityType> entities;
    };
}