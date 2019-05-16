/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** world WorldManager.cpp
*/

#include <exception>
#include <fstream>
#include "indiestudio/world/WorldManager.hpp"
#include "indiestudio/utils/CRCUtils.hpp"

namespace IndieStudio {

    const unsigned char WorldManager::FILE_MAGIC[sizeof(World::FileHeader::magic)] =
        { '\\', 'B', 'B', 'M' };
    const unsigned char WorldManager::FILE_FORMAT_VERSION = 1;

    WorldManager::WorldManager() : logger("WorldManager")
    {}

    World WorldManager::load(const std::string &path)
    {
        std::ifstream file(path, std::ios::binary);

        if (!file.good())
            throw std::runtime_error("Failed to open the file");

        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        ByteBuffer buffer(size);
        
        if (!file.read(*buffer, size))
            throw std::runtime_error("Failed to read the file");
        
        file.close();
        this->assertValidWorld(buffer);

        World world;
        world.unpack(buffer);
        return world;
    }

    void WorldManager::save(const World &world)
    {
        std::ofstream file(world.getSettings().name + ".bmm", std::ios::trunc);

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
    }

    void WorldManager::assertValidWorld(ByteBuffer &buffer)
    {
        if (buffer.getSize() < sizeof(World::FileHeader))
            throw new std::logic_error("The provided file is not a Bomberman map");
        
        World::FileHeader header;
        buffer >> header;

        if (std::memcmp(header.magic, FILE_MAGIC, sizeof(header.magic)) != 0)
            throw new std::logic_error("The provided file is not a Bomberman map");
        
        if (header.formatVersion != FILE_FORMAT_VERSION)
            throw new std::logic_error("The map was created with a different version and therefore is not compatible");
        
        if (buffer.getSize() - buffer.getReadCursor() != header.size)
            throw new std::logic_error("The map size dosn't match the saved one");
        
        void *payload = *buffer + sizeof(World::FileHeader);

        unsigned int computedChecksum =
            CRCUtils::crc32(payload, header.size);
        
        if (computedChecksum != header.checksum)
            throw new std::logic_error("The map checksum doesn't match");
    }

}