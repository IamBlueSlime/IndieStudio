/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** world WorldSettings.hpp
*/

#pragma once

#include <irrlicht.h>
#include <string>
#include "indiestudio/common/ISerializable.hpp"

namespace IndieStudio {

    class WorldSettings : public ISerializable {
    public:
        struct Player {
            enum class ControlType {
                KEYBOARD,
                CONTROLLER,
                AI
            };

            ControlType controlType = ControlType::KEYBOARD;
            irr::EKEY_CODE keyboardUp = irr::KEY_KEY_Z;
            irr::EKEY_CODE keyboardLeft = irr::KEY_KEY_Q;
            irr::EKEY_CODE keyboardDown = irr::KEY_KEY_S;
            irr::EKEY_CODE keyboardRight = irr::KEY_KEY_D;
            irr::EKEY_CODE keyboardDrop = irr::KEY_KEY_C;
        };

        std::string name;
        unsigned short width = 17;
        unsigned short height = 11;
        std::string worldGenerator = "Basic";
        Player players[4];

        /* ISerializable implementation */
        void pack(ByteBuffer &buffer) const override;
        void unpack(ByteBuffer &buffer) override;

    protected:
    private:
    };

}
