/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** utils ByteBufferUtils.hpp
*/

#pragma once

#include <string>
#include "indiestudio/common/ByteBuffer.hpp"

namespace IndieStudio {

    class ByteBufferUtils {
	public:
        static void writeString(ByteBuffer &buffer, const std::string &str)
        {
            std::size_t len = str.size();
            buffer << len;

            for (char c : str)
                buffer << c;
        }

        static std::string readString(ByteBuffer &buffer)
        {
            std::string str;

            std::size_t len = 0;
            buffer >> len;

            for (std::size_t i = 0; i < len; i += 1) {
                char c;
                buffer >> c;
                str += c;
            }

            return str;
        }

	protected:
	private:
    };

}