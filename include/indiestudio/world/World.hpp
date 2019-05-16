/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** world World.hpp
*/

#pragma once

#include "indiestudio/common/ISerializable.hpp"
#include "indiestudio/world/WorldSettings.hpp"

namespace IndieStudio {

    class World : public ISerializable {
    public:
        struct FileHeader {
            unsigned char magic[4];
            unsigned char formatVersion;
            unsigned long size;
            unsigned int checksum;
        } __attribute__((packed));

        World(WorldSettings settings);
        World();

        /* ISerializable implementation */
        void pack(ByteBuffer &buffer) const override;
        void unpack(ByteBuffer &buffer) override;

        WorldSettings &getSettings() { return this->settings; };
        const WorldSettings &getSettings() const { return this->settings; }

    protected:
    private:
        WorldSettings settings;
    };

}