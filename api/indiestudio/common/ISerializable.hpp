/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** common ISerializable.hpp
*/

#pragma once

#include "indiestudio/common/ByteBuffer.hpp"

namespace IndieStudio {

    class ISerializable {
    public:
        virtual ~ISerializable() = default;
        virtual void pack(ByteBuffer &buffer) const = 0;
        virtual void unpack(ByteBuffer &buffer) = 0;
   };

}