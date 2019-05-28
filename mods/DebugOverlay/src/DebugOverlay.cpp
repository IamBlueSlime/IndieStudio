/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** DebugOverlay DebugOverlay.cpp
*/

#include "indiestudio/debugoverlay/DebugOverlay.hpp"

namespace IndieStudio {

    namespace DebugOverlay {

        DebugOverlay::DebugOverlay()
        {}

        void DebugOverlay::onEnable(IGame &game)
        {
            this->logger->info("Hello!");
        }

        void DebugOverlay::onDisable(IGame &game)
        {
            this->logger->info("Bye!");
        }

    }

}

EXPORT_MOD(
    "debugoverlay",
    "DebugOverlay",
    "Jérémy Levilain <jeremy.levilain@epitech.eu>",
    "1.0.0",
    IndieStudio::DebugOverlay::DebugOverlay
)
