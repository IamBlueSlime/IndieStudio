/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** ecs Signature.cpp
*/

#include "indiestudio/ecs/Signature.hpp"

namespace IndieStudio {

    template<>
    void Signature::packType<std::string>(const std::string &type, ByteBuffer &buffer)
    {
        buffer << type.size();

        for (const char &c : type)
            buffer << c;
    }

    template<>
    void Signature::unpackType<std::string>(std::string &type, ByteBuffer &buffer)
    {
        std::size_t len;
        buffer >> len;

        for (std::size_t i = 0; i < len; i += 1) {
            char c;
            buffer >> c;
            type += c;
        }
    }

}