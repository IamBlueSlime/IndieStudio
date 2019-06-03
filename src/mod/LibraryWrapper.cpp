/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** src LibraryWrapper.cpp
*/

#include "indiestudio/mod/LibraryWrapper.hpp"

namespace IndieStudio {

    LibraryWrapper::LibraryWrapper()
    {}

    LibraryWrapper::LibraryWrapper(const std::string &name)
    {
        open(name);
    }

    bool LibraryWrapper::open(const std::string &name)
    {
        if (this->handle != nullptr)
            close();

        this->name = name;
#ifdef WIN32
        this->handle = LoadLibrary(TEXT(name.c_str()));
#else
        this->handle = dlopen(name.c_str(), RTLD_NOW);
#endif
        return this->handle != nullptr;
    }

    void LibraryWrapper::close()
    {
        if (this->handle == nullptr)
            return;

#ifdef WIN32
        FreeLibrary(static_cast<HMODULE>(this->handle));
#else
        dlclose(this->handle);
#endif
        this->handle = nullptr;
    }
}