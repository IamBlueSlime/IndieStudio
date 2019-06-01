/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** world WorldSettings.cpp
*/

#include "indiestudio/world/WorldSettings.hpp"
#include "indiestudio/utils/ByteBufferUtils.hpp"

namespace IndieStudio {

    void WorldSettings::pack(ByteBuffer &buffer) const
    {
        ByteBufferUtils::writeString<unsigned short>(buffer, this->name);
        buffer << this->width;
        buffer << this->height;
        buffer << this->players;
    }

    void WorldSettings::unpack(ByteBuffer &buffer)
    {
        this->name = ByteBufferUtils::readString<unsigned short>(buffer);
        buffer >> this->width;
        buffer >> this->height;
        buffer >> this->players;
    }

}