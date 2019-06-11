/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** world WorldManager.hpp
*/

#pragma once

#include <unordered_map>
#include <string>
#include "indiestudio/common/Logger.hpp"
#include "indiestudio/world/BasicWorldGenerator.hpp"
#include "indiestudio/world/IWorldManager.hpp"
#include "indiestudio/world/World.hpp"

namespace IndieStudio {

    class WorldManager : public IWorldManager {
    public:
        static const unsigned char FILE_MAGIC[sizeof(World::FileHeader::magic)];
        static const unsigned char FILE_FORMAT_VERSION;

        WorldManager();

        void init();

        World *create(WorldSettings &settings);
        World *load(const std::string &path);
        void save(const std::string &path, World *world);

        void registerGenerator(const std::string &name, IWorldGenerator *generator) override;

        const std::unordered_map<std::string, IWorldGenerator *> &getGenerators() const
        {
            return this->generators;
        }

        IWorld *getLoadedWorld() override { return this->loadedWorld.get(); }
        World *getLoadedWorldImpl() { return this->loadedWorld.get(); }

    protected:
    private:
        Logger logger;
        std::unordered_map<std::string, IWorldGenerator *> generators;
        BasicWorldGenerator basicWorldGenerator;
        std::unique_ptr<World> loadedWorld;

        static void assertValidWorld(ByteBuffer &buffer);
    };

}