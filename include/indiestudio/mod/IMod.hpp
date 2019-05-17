/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** mod IMod.hpp
*/

#pragma once

#include <functional>
#include <string>

namespace IndieStudio {

    class IMod {
    public:
        struct Descriptor
        {
            std::string id;
            std::string name;
            std::string author;
            std::string version;
            std::function<IMod *()> entryPoint;
        };

        virtual ~IMod() {}
        virtual void onEnable() = 0;
        virtual void onDisable() = 0;
    };

}

#define EXPORT_MOD(id, name, author, version, clazz)                                               \
    extern "C" {                                                                                   \
        static IndieStudio::IMod *createMod()                                                      \
        {                                                                                          \
            return new clazz(emulator);                                                            \
        }                                                                                          \
                                                                                                   \
        IndieStudio::IMod::Descriptor Mod_DESCRIPTOR = { id, name, author, version, &createMod };  \
    }
