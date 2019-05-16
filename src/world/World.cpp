/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** world World.cpp
*/

#include "indiestudio/world/World.hpp"

namespace IndieStudio {

    World::World(WorldSettings settings) : settings(settings)
    {}

    World::World() : World(WorldSettings())
    {}

    void World::pack(ByteBuffer &buffer) const
    {
        this->settings.pack(buffer);
    }

    void World::unpack(ByteBuffer &buffer)
    {
        this->settings.unpack(buffer);
    }

}