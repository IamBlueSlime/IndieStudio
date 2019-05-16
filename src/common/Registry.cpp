/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** common Registry.cpp
*/

#include <sstream>
#include "indiestudio/common/Registry.hpp"

namespace IndieStudio {

    template<typename K, typename V>
    void Registry<K, V>::add(K key, V value, bool overrideExisting)
    {
        if (this->has(key)) {
            if (!overrideExisting) {
                std::stringstream ss;
                ss << key;

                throw new std::runtime_error("Tried to register an object with an existing key (" + ss.str() + ")");
            }

            this->remove(key);
        }

        this->internal.insert(std::make_pair(key, value));
    }

    template<typename K, typename V>
    bool Registry<K, V>::removeKey(K key)
    {
        if (this->has(key)) {
            this->internal.erase(key);
            return true;
        }

        return false;
    }

    template<typename K, typename V>
    bool Registry<K, V>::removeValue(V &value)
    {
        for (auto it = this->internal.begin(); it != this->internal.end(); ++it) {
            if (&it.second == &value) {
                this->removeKey(it.first);
                return true;
            }
        }

        return false;
    }

    template<typename K, typename V>
    const V &Registry<K, V>::get(K key) const
    {
        if (!this->has(key)) {
            std::stringstream ss;
            ss << key;

            throw new std::runtime_error("Tried to get an unknown key (" + ss.str() + ")");
        }

        return this->internal[key];
    }

    template<typename K, typename V>
    bool Registry<K, V>::has(K key) const
    {
        return this->internal.find(key) != this->internal.end();
    }

}
