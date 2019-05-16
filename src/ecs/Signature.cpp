/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** ecs Signature.cpp
*/

#include "indiestudio/ecs/Signature.hpp"
#include "indiestudio/utils/ByteBufferUtils.hpp"

namespace IndieStudio {

    template<>
    void Signature::packType<std::string>(const std::string &type, ByteBuffer &buffer)
    {
        ByteBufferUtils::writeString(buffer, type);
    }

    template<>
    void Signature::unpackType<std::string>(std::string &type, ByteBuffer &buffer)
    {
        type = ByteBufferUtils::readString(buffer);
    }

}