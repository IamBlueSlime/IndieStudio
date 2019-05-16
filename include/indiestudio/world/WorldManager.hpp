/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** world WorldManager.hpp
*/

#pragma once

#include <string>
#include "indiestudio/common/Logger.hpp"
#include "indiestudio/world/World.hpp"

namespace IndieStudio {

    class WorldManager {
    public:
        static const unsigned char FILE_MAGIC[sizeof(World::FileHeader::magic)];
        static const unsigned char FILE_FORMAT_VERSION;

        WorldManager();

        World load(const std::string &path);
        void save(const World &world);

    protected:
    private:
        Logger logger;

        static void assertValidWorld(ByteBuffer &buffer);
    };

}