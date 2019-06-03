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
        ByteBufferUtils::writeString<unsigned short>(buffer, this->worldGenerator);
        
        for (int i = 0; i < 4; i += 1) {
            buffer << this->players[i].controlType;

            if (this->players[i].controlType == Player::ControlType::KEYBOARD) {
                buffer << this->players[i].keyboardUp;
                buffer << this->players[i].keyboardLeft;
                buffer << this->players[i].keyboardDown;
                buffer << this->players[i].keyboardRight;
                buffer << this->players[i].keyboardDrop;
            }
        }
    }

    void WorldSettings::unpack(ByteBuffer &buffer)
    {
        this->name = ByteBufferUtils::readString(buffer);
        buffer >> this->width;
        buffer >> this->height;
        this->worldGenerator = ByteBufferUtils::readString(buffer);

        for (int i = 0; i < 4; i += 1) {
            buffer >> this->players[i].controlType;

            if (this->players[i].controlType == Player::ControlType::KEYBOARD) {
                buffer >> this->players[i].keyboardUp;
                buffer >> this->players[i].keyboardLeft;
                buffer >> this->players[i].keyboardDown;
                buffer >> this->players[i].keyboardRight;
                buffer >> this->players[i].keyboardDrop;
            }
        }
    }

}