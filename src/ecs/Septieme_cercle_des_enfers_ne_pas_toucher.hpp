/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Septieme_cercle_des_enfers_ne_pas_toucher
*/

#ifndef SEPTIEME_CERCLE_DES_ENFERS_NE_PAS_TOUCHER_HPP_
#define SEPTIEME_CERCLE_DES_ENFERS_NE_PAS_TOUCHER_HPP_

namespace Ecs {

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
                  // POINT CRITIQUE
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
}

#endif /* !SEPTIEME_CERCLE_DES_ENFERS_NE_PAS_TOUCHER_HPP_ */
