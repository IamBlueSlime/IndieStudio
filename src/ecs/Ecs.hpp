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
        std::tuple<components_types...> components;     //TODO: faire un tuple d optionnal (HARDCORE)
    };

    template<typename... components_types>
    class Manager {
        using Components = TypeList<components_types...>;
        using EntityType = Entity<components_types...>;

    public:
        Manager() {
            EntityType test1;
            test1.component_signature[0] = true;

            EntityType test2;
            test2.component_signature[2] = true;

            EntityType test3;
            test3.component_signature[0] = true;
            test3.component_signature[1] = true;
            test3.component_signature[2] = true;

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