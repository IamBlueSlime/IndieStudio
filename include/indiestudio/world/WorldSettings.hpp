/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** world WorldSettings.hpp
*/

#pragma once

#include "indiestudio/common/ISerializable.hpp"
#include "indiestudio/utils/ByteBufferUtils.hpp"

namespace IndieStudio {

    struct WorldSettings : public ISerializable {
        std::string name;

        /* ISerializable implementation */
        void pack(ByteBuffer &buffer) const override
        {
            ByteBufferUtils::writeString(buffer, this->name);
        }

        void unpack(ByteBuffer &buffer) override
        {
            this->name = ByteBufferUtils::readString(buffer);
        }
    };

}