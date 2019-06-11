/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** common Registry.hpp
*/

#pragma once

#include <functional>
#include <sstream>
#include <unordered_map>
#include "indiestudio/common/Error.hpp"

namespace IndieStudio {

    template<typename K, typename V>
    class Registry {
    public:
        void add(K key, V value, bool overrideExisting = false)
        {
            if (this->has(key)) {
                if (!overrideExisting) {
                    std::stringstream ss;
                    ss << key;

                    throw RegistryError("Tried to register an object with an existing key (" + ss.str() + ")");
                }

                this->removeKey(key);
            }

            this->container.insert(std::make_pair(key, value));
        }

        bool removeKey(K key)
        {
            if (this->has(key)) {
                this->container.erase(key);
                return true;
            }

            return false;
        }

        bool removeValue(V &value)
        {
            for (auto it = this->container.begin(); it != this->container.end(); ++it) {
                if (&it.second == &value) {
                    this->removeKey(it.first);
                    return true;
                }
            }

            return false;
        }

        V &get(K key)
        {
            if (!this->has(key)) {
                std::stringstream ss;
                ss << key;

                throw RegistryError("Tried to get an unknown key (" + ss.str() + ")");
            }

            return this->container[key];
        }

        void forEach(std::function<void(K key, V &value)> &&callback)
        {
            for (auto it = this->container.begin(); it != this->container.end(); ++it)
                callback(it->first, it->second);
        }

        bool has(K key) const
        {
            return this->container.find(key) != this->container.end();
        }

    protected:
    private:
        std::unordered_map<K, V> container;
    };

    template<typename V>
    class IncrementalRegistry : public Registry<std::size_t, V> {
    public:
        IncrementalRegistry() = default;
        ~IncrementalRegistry() = default;

        void add(V value, bool overrideExisting = false) override
        {
            this->add(this->counter++, value, overrideExisting);
        }

    protected:
    private:
        std::size_t counter = 0;
    };

}