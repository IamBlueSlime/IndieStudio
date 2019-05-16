/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** utils CRCUtils.hpp
*/

#pragma once

#include <utility>

namespace IndieStudio {

	class CRCUtils {
	public:
		static unsigned int crc32(const void *buffer, std::size_t len);

	protected:
	private:
	};

}