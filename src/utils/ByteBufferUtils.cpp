/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** utils ByteBufferUtils.hpp
*/

#include "indiestudio/utils/ByteBufferUtils.hpp"

namespace IndieStudio {

    void ByteBufferUtils::writeString(ByteBuffer &buffer, const std::string &str)
    {
        buffer << str.size();

        for (char c : str)
            buffer << c;
    }

    std::string ByteBufferUtils::readString(ByteBuffer &buffer)
    {
        std::string str;

        std::size_t len;
        buffer >> len;

        for (std::size_t i = 0; i < len; i += 1) {
            char c;
            buffer >> c;
            str += c;
        }
        return str;
    }

}