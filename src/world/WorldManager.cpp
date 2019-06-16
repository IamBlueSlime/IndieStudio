/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** world WorldManager.cpp
*/

#include <exception>
#include <iostream>
#include <fstream>
#include "indiestudio/world/WorldManager.hpp"
#include "indiestudio/utils/CRCUtils.hpp"

namespace IndieStudio {

    const unsigned char WorldManager::FILE_MAGIC[sizeof(World::FileHeader::magic)] =
        { '\\', 'B', 'M', 'M' };
    const unsigned char WorldManager::FILE_FORMAT_VERSION = 2;

    const std::string WorldManager::BASIC_WORLD_GENERATOR = "Basic";

    WorldManager::WorldManager() : logger("worldmanager")
    {}

    void WorldManager::init()
    {
        this->registerGenerator(BASIC_WORLD_GENERATOR, &this->basicWorldGenerator);
        this->registerGenerator("Vertical", &this->verticalWorldGenerator);
        this->registerGenerator("Empty", &this->emptyWorldGenerator);
    }

    World *WorldManager::create(WorldSettings &settings)
    {
        logger.info("Creating world...");
        this->loadedWorld = std::make_unique<World>(settings);
        this->loadedWorld->create(*this);
        return this->loadedWorld.get();
    }

    World *WorldManager::load(const std::string &path)
    {
        logger.info("Loading world file '" + path + "'...");
        std::ifstream file(path, std::ios::binary | std::ios::ate);

        if (!file.good())
            throw SaveError("Failed to open the file");

        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        ByteBuffer buffer(size);

        if (!file.read(*buffer, size))
            throw SaveError("Failed to read the file");

        file.close();
        this->assertValidWorld(buffer);
        logger.debug("The world file is valid.");

        std::unique_ptr<World> world = std::make_unique<World>();
        logger.debug("Unpacking world...");
        world->unpack(buffer);
        logger.info("Loaded world '" + world->getSettings().name + "'.");

        this->loadedWorld = std::move(world);
        return this->loadedWorld.get();
    }

    void WorldManager::save(const std::string &path, World *world)
    {
        logger.info("Saving world '" + world->getSettings().name + "' at '" + path + "'...");
        std::ofstream file(path, std::ios::trunc);

        ByteBuffer buffer;
        world->pack(buffer);

        World::FileHeader header;
        std::memcpy(header.magic, FILE_MAGIC, sizeof(header.magic));
        header.formatVersion = FILE_FORMAT_VERSION;
        header.size = buffer.getSize();
        header.checksum = CRCUtils::crc32(*buffer, header.size);

        file.write((char *) &header, sizeof(header));
        file.write(*buffer, header.size);
        file.close();

        logger.info("Saved world.");
    }

    void WorldManager::registerGenerator(const std::string &name,
        IWorldGenerator *generator)
    {
        this->generators.insert(std::make_pair(name, generator));
        this->logger.info("Registered world generator '" + name + "'.");
    }

    void WorldManager::assertValidWorld(ByteBuffer &buffer)
    {
        World::FileHeader header;

        if (buffer.getSize() < sizeof(header))
            throw SaveError("The provided file is not a Bomberman map");

        buffer >> header;

        if (std::memcmp(header.magic, FILE_MAGIC, sizeof(header.magic)) != 0)
            throw SaveError("The provided file is not a Bomberman map");

        if (header.formatVersion != FILE_FORMAT_VERSION)
            throw SaveError("The map was created with a different version and therefore is not compatible");

        if (buffer.getSize() - buffer.getReadCursor() != header.size)
            throw SaveError("The map size dosn't match the saved one");
        void *payload = *buffer + sizeof(World::FileHeader);

        unsigned int computedChecksum =
            CRCUtils::crc32(payload, header.size);

        if (computedChecksum != header.checksum)
            throw SaveError("The map checksum doesn't match");
    }

}