/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** mod LibraryWrapper.hpp
*/

#pragma once

#include <string>
#include <dlfcn.h>

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
            if (handle_ == nullptr)
                return nullptr;

            return static_cast<T>(dlsym(handle_, symbolName.c_str()));
        }

        const std::string &getName() const;

    protected:
    private:
        std::string name_;
        void *handle_ = nullptr;
    };

}
