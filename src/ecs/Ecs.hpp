#include <cstdint>
#include <optional>
#include <bitset>
#include <tuple>
#include <map>
#include <functional>
#include <type_traits>
#include <iostream>
#include <variant>
#include <cassert>

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
            std::cerr << "The type you are searching for is not in the TypeList! Are you using systems without appropriate components?\n" << std::endl;
            assert(false);
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
    class Tuple {
        using Components = TypeList<components_types...>;

    public:
        std::tuple<std::optional<components_types>...> components;

        template <typename T>
        std::optional<T> get() {
            int idx = Components::template getIndex<T>();
            return std::get<std::optional<T>>(Tuple::visit_at(components, idx));
        }

        template <typename T>
        void set(T value) {
            int idx = Components::template getIndex<T>();
            return Tuple::replace_at(components, idx, std::make_optional(value));
        }

        template <typename T>
        void unset() {
            int idx = Components::template getIndex<T>();
            return Tuple::replace_at(components, idx, std::nullopt);
        }

        template <int I>
        struct visit_impl
        {
            template <typename T>
            static std::variant<std::nullopt_t, std::optional<components_types>...> visit(T& tup, int idx)
            {
                if (idx == I - 1) {
                    auto tmp = std::get<I - 1>(tup);
                    return tmp;
                } else {
                    return visit_impl<I - 1>::visit(tup, idx);
                }
            }
        };

        template <>
        struct visit_impl<0>
        {
            template <typename T>
            static std::variant<std::nullopt_t, std::optional<components_types>...> visit(T& tup, int idx) {
                std::cout << "The type you are searching is not in the tuple :/ how is it even possible?" << std::endl;
                assert(false);
                return std::nullopt;
            }
        };

        template <typename... Ts>
        std::variant<std::nullopt_t, std::optional<components_types>...> visit_at(std::tuple<Ts...> const& tup, int idx)
        {
            return visit_impl<sizeof...(Ts)>::visit(tup, idx);
        }

        template <typename... Ts>
        std::variant<std::nullopt_t, std::optional<components_types>...> visit_at(std::tuple<Ts...>& tup, int idx)
        {
            return visit_impl<sizeof...(Ts)>::visit(tup, idx);
        }

        template <size_t I>
        struct replace_impl
        {
            template <typename T, typename F>
            static void visit(T& tup, int idx, F value)
            {
                if (idx == I - 1){
                    auto tmp = reinterpret_cast<F&>(std::get<I - 1>(tup));
                    tmp = reinterpret_cast<F>(value);
                } else {
                    replace_impl<I - 1>::visit(tup, idx, value);
                }
            }
        };

        template <>
        struct replace_impl<0>
        {
            template <typename T, typename F>
            static void visit(T& tup, int idx, F value) { assert(false); }
        };

        template <typename F, typename... Ts>
        void replace_at(std::tuple<Ts...> const& tup, int idx, F value)
        {
            replace_impl<sizeof...(Ts)>::visit(tup, idx, value);
        }

        template <typename F, typename... Ts>
        void replace_at(std::tuple<Ts...>& tup, int idx, F value)
        {
            replace_impl<sizeof...(Ts)>::visit(tup, idx, value);
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

            Component1 first;
            Component2 second;
            Component3 third;
            std::get<0>(test1.components) = std::make_optional(first);

            Tuple<components_types...> tuple;
            if (tuple.template get<Component1>() == std::nullopt) {
                std::cout << "Wesh ia R! les std optional marchent!" << std::endl;
            } else {
                std::cout << "Ia un problème! il devrait pas y avoir qqch!" << std::endl;
            }
            if (tuple.template get<Component2>() == std::nullopt) {
                std::cout << "Wesh ia R! les std optional marchent!" << std::endl;
            } else {
                std::cout << "Ia un problème! il devrait pas y avoir qqch!" << std::endl;
            }
            if (tuple.template get<Component2>() == std::nullopt) {
                std::cout << "Wesh ia R! les std optional marchent!" << std::endl;
            } else {
                std::cout << "Ia un problème! il devrait pas y avoir qqch!" << std::endl;
            }

//            tuple.template set<Component2>(second);

//            std::get<1>(tuple.components) = std::make_optional(second);

            if (tuple.template get<Component1>() == std::nullopt) {
                std::cout << "Wesh ia R! les std optional marchent!" << std::endl;
            } else {
                std::cout << "Ia un problème! il devrait pas y avoir qqch!" << std::endl;
            }
            if (tuple.template get<Component2>() == std::nullopt) {
                std::cout << "Wesh ia R! les std optional marchent!" << std::endl;
            } else {
                std::cout << "Ia un problème! il devrait pas y avoir qqch!" << std::endl;
            }
            if (tuple.template get<Component3>() == std::nullopt) {
                std::cout << "Wesh ia R! les std optional marchent!" << std::endl;
            } else {
                std::cout << "Ia un problème! il devrait pas y avoir qqch!" << std::endl;
            }

//            Components::template getElemAt<0>(test1.components) = std::make_optional(first);

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
//            return std::get<Components::template getIndex<T>()>(entity.components);
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