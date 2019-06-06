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
                KEYBOARD = 0,
                CONTROLLER,
                AI
            };

            ControlType controlType = ControlType::AI;
            irr::EKEY_CODE keyboardUp = irr::KEY_KEY_Z;
            irr::EKEY_CODE keyboardLeft;
            irr::EKEY_CODE keyboardDown;
            irr::EKEY_CODE keyboardRight;
            irr::EKEY_CODE keyboardDrop;

            unsigned short kills = 0;
        };

        WorldSettings()
        {
            irr::EKEY_CODE keys[4][5] = {
                { irr::KEY_KEY_Z, irr::KEY_KEY_Q, irr::KEY_KEY_S, irr::KEY_KEY_D, irr::KEY_KEY_E },
                { irr::KEY_KEY_T, irr::KEY_KEY_F, irr::KEY_KEY_G, irr::KEY_KEY_H, irr::KEY_KEY_Y },
                { irr::KEY_KEY_I, irr::KEY_KEY_J, irr::KEY_KEY_K, irr::KEY_KEY_L, irr::KEY_KEY_O },
                { irr::KEY_UP, irr::KEY_LEFT, irr::KEY_DOWN, irr::KEY_RIGHT, irr::KEY_RSHIFT }
            };

            for (int i = 0; i < 4; i += 1) {
                this->players[i].keyboardUp = keys[i][0];
                this->players[i].keyboardLeft = keys[i][1];
                this->players[i].keyboardDown = keys[i][2];
                this->players[i].keyboardRight = keys[i][3];
                this->players[i].keyboardDrop = keys[i][4];
            }

            this->players[0].controlType = Player::ControlType::KEYBOARD;
        }

        std::string name;
        unsigned short width = 17;
        unsigned short height = 11;
        std::string worldGenerator = "Basic";
        Player players[4];
        unsigned long elapsedSeconds = 0;

        /* ISerializable implementation */
        void pack(ByteBuffer &buffer) const override;
        void unpack(ByteBuffer &buffer) override;

    protected:
    private:
    };

}
