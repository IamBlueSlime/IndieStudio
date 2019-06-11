/*
** EPITECH PROJECT, 2019
** indie studio
** File description:
** pickup powerup
*/

#pragma once

#include <utility>
#include "indiestudio/ecs/Components.hpp"
#include "indiestudio/ecs/BaseSystem.hpp"
#include "indiestudio/world/MapPattern.hpp"

namespace IndieStudio::ECS::System {

    using namespace ECS::Component;

    template<typename ManagerType>
    class PickUpPowerUpSystem : public BaseSystem<ManagerType> {
        public:
            void process(ManagerType &manager, World *world) override;

        protected:
        private:
            bool isOnPowerUp(
                const std::pair<std::size_t, std::size_t> &player, 
                const std::pair<std::size_t, std::size_t> &powerUp, MapPattern *map);
            void applyPowerUp(Speed &speed, Stat &stat, IsPowerUp &powerUp);
    };
}