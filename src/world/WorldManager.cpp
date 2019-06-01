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
    const unsigned char WorldManager::FILE_FORMAT_VERSION = 1;

    WorldManager::WorldManager() : logger("worldmanager")
    {}

    void WorldManager::init()
    {
        this->registerGenerator("Basic", &this->basicWorldGenerator);
    }

    void WorldManager::create(WorldSettings &settings)
    {
        logger.info("Creating world...");
        this->loadedWorld = std::make_unique<World>(settings);
    }

    void WorldManager::load(const std::string &path)
    {
        logger.info("Loading world file '" + path + "'...");
        std::ifstream file(path, std::ios::binary | std::ios::ate);

        if (!file.good())
            throw std::runtime_error("Failed to open the file");

        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        ByteBuffer buffer(size);
        
        if (!file.read(*buffer, size))
            throw std::runtime_error("Failed to read the file");
        
        file.close();
        this->assertValidWorld(buffer);

        std::unique_ptr<World> world = std::make_unique<World>();
        world->unpack(buffer);
        logger.info("Loaded world '" + world->getSettings().name + "'.");

        this->loadedWorld = std::move(world);
    }

    void WorldManager::save(const std::string &path, const World &world)
    {
        logger.info("Saving world '" + world.getSettings().name + "' at '" + path + "'...");
        std::ofstream file(path, std::ios::trunc);

        ByteBuffer buffer;
        world.pack(buffer);
        
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
        const IWorldGenerator *generator)
    {
        this->generators.insert(std::make_pair(name, generator));
        this->logger.info("Registered world generator '" + name + "'.");
    }

    void WorldManager::assertValidWorld(ByteBuffer &buffer)
    {
        World::FileHeader header;

        std::cout << buffer.getSize() << std::endl;

        if (buffer.getSize() < sizeof(header))
            throw std::logic_error("The provided file is not a Bomberman map");
        
        buffer >> header;

        if (std::memcmp(header.magic, FILE_MAGIC, sizeof(header.magic)) != 0)
            throw std::logic_error("The provided file is not a Bomberman map");
        
        if (header.formatVersion != FILE_FORMAT_VERSION)
            throw std::logic_error("The map was created with a different version and therefore is not compatible");
        
        if (buffer.getSize() - buffer.getReadCursor() != header.size)
            throw std::logic_error("The map size dosn't match the saved one");
        
        void *payload = *buffer + sizeof(World::FileHeader);

        unsigned int computedChecksum =
            CRCUtils::crc32(payload, header.size);
        
        if (computedChecksum != header.checksum)
            throw std::logic_error("The map checksum doesn't match");
    }

}