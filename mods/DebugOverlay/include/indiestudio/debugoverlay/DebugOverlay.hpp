/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** DebugOverlay DebugOverlay.hpp
*/

#pragma once

#include "indiestudio/mod/AMod.hpp"

namespace IndieStudio {

    namespace DebugOverlay {

        class DebugOverlay : public AMod {
        public:
            DebugOverlay();

            void onEnable(IGame &game) override;
            void onDisable(IGame &game) override;

        protected:
        private:
        };

    }

}
