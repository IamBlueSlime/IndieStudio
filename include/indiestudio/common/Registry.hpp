/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** common Registry.hpp
*/

#pragma once

#include <unordered_map>

namespace IndieStudio {

    template<typename K, typename V>
    class Registry {
    public:
        Registry() = default;
        ~Registry() = default;

        void add(K key, V value, bool overrideExisting = false);
        bool removeKey(K key);
        bool removeValue(V &value);
        const V &get(K key) const;

        bool has(K key) const;

    protected:
    private:
        std::unordered_map<K, V> internal;
    };

    template<typename V>
    class IncrementalRegistry : public Registry<std::size_t, V> {
    public:
        IncrementalRegistry() = default;
        ~IncrementalRegistry() = default;

        void add(V value, bool overrideExisting = false)
        {
            this->add(this->counter++, value, overrideExisting);
        }

    protected:
    private:
        std::size_t counter = 0;
    };

}