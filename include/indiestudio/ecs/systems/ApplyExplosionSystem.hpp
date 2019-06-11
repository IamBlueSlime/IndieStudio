/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** ecs systems ApplyExplosionSystem.hpp
*/

#pragma once

#include "indiestudio/ecs/BaseSystem.hpp"
#include "indiestudio/world/MapPattern.hpp"

namespace IndieStudio::ECS::System {

    template<typename ManagerType>
    class ApplyExplosionSystem : public BaseSystem<ManagerType> {
    public:
        void process(ManagerType &manager, World *world) override;
        void chainBomb(ManagerType &manager, int entityID, int bombID);
        int findBombByID(ManagerType &manager, MapPattern *pattern, std::pair<short, short> posInTile);
        void destroyBlock(ManagerType &manager, World *world, std::pair<short, short> posInTile);
    protected:
    private:
    };
}