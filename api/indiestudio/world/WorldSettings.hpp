/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** world WorldSettings.hpp
*/

#pragma once

#include "indiestudio/common/ISerializable.hpp"

namespace IndieStudio {

    class WorldSettings : public ISerializable {
    public:
        std::string name;
        unsigned int width = 17;
        unsigned int height = 11;

        /* ISerializable implementation */
        void pack(ByteBuffer &buffer) const override;
        void unpack(ByteBuffer &buffer) override;
    
    protected:
    private:
    };

}