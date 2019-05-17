/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** src LibraryWrapper.cpp
*/

#include <iostream>
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
        if (handle_ != nullptr)
            close();
        
        name_ = name;
        handle_ = dlopen(name.c_str(), RTLD_NOW);
        return handle_ != nullptr;
    }

    void LibraryWrapper::close()
    {
        if (handle_ == nullptr)
            return;
        
        dlclose(handle_);
        handle_ = nullptr;
    }

    const std::string &LibraryWrapper::getName() const
    {
        return name_;
    }
}