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
        static void writeString(ByteBuffer &buffer, const std::string &str);
        static std::string readString(ByteBuffer &buffer);

	protected:
	private:
    };

}