/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** mod LibraryWrapper.hpp
*/

#pragma once

#include <string>

#ifdef WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace IndieStudio {

    class LibraryWrapper {
    public:
        LibraryWrapper();
        LibraryWrapper(const std::string &name);

        bool open(const std::string &name);
        void close();

        template<typename T>
        T getSymbol(const std::string &symbolName)
        {
            if (handle == nullptr)
                return nullptr;

#ifdef WIN32
            return static_cast<T>(GetProcAddress(handle, symbolName.c_str()));
#else
            return static_cast<T>(dlsym(handle, symbolName.c_str()));
#endif
        }

        const std::string &getName() const { return this->name; }

    protected:
    private:
        std::string name;
        void *handle = nullptr;
    };

}
