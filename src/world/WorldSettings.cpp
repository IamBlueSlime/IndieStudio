/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** world WorldSettings.cpp
*/

#include "indiestudio/world/WorldSettings.hpp"
#include "indiestudio/utils/ByteBufferUtils.hpp"

namespace IndieStudio {

    void WorldSettings::pack(ByteBuffer &buffer)
    {
        ByteBufferUtils::writeString(buffer, this->name);
        buffer << this->width;
        buffer << this->height;
        ByteBufferUtils::writeString(buffer, this->worldGenerator);
        buffer << this->elapsedSeconds;
    }

    void WorldSettings::unpack(ByteBuffer &buffer)
    {
        this->name = ByteBufferUtils::readString(buffer);
        buffer >> this->width;
        buffer >> this->height;
        this->worldGenerator = ByteBufferUtils::readString(buffer);
        buffer >> this->elapsedSeconds;
    }

}