/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** UUID
*/

#include "indiestudio/ecs/UUID.hpp"

std::ostream &operator<<(std::ostream &stream, const IndieStudio::UUID &id)
{
    stream << id.getUUID();
    return stream;
}