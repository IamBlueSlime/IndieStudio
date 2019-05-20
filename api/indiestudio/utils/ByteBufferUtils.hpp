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
        template<typename L = std::size_t>
        static void writeString(ByteBuffer &buffer, const std::string &str)
        {
            L len = str.size();
            buffer << len;

            for (char c : str)
                buffer << c;
        }

        template<typename L = std::size_t>
        static std::string readString(ByteBuffer &buffer)
        {
            std::string str;

            L len; 
            buffer >> len;

            for (L i = 0; i < len; i += 1) {
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