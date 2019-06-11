/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** ecs systems SolidBombSystem.hpp
*/

#include "indiestudio/ecs/Components.hpp"
#include "indiestudio/ecs/systems/SolidBombSystem.hpp"
#include "indiestudio/world/World.hpp"

namespace IndieStudio::ECS::System {

    using namespace ECS::Component;

    template<typename ManagerType>
    void SolidBombSystem<ManagerType>::process(ManagerType &manager, World *world)
    {
        auto pattern = world->getPattern();

        manager.template forEntitiesWith<IsBomb, Node, NotSolid, Position, PosedBy>(
        [&](auto &data, [[gnu::unused]] auto id) {
            const auto &position = manager.template getComponent<Position>(data);
            const auto BombInTile = pattern->positionToTile(position.x, position.z);

            const auto &PosedByID = manager.template getComponent<PosedBy>(data);
            const auto &player_pos = manager.template getComponent<Position>(PosedByID.id);
            const auto playerInTile = pattern->positionToTile(player_pos.x, player_pos.z);

            if (BombInTile.first - playerInTile.first <= 1 &&
                BombInTile.second - playerInTile.second <= 1) {
                return;
            }
            auto node = manager.template getComponent<Node>(data).node;
            auto selector = world->getScene().scene->createTriangleSelectorFromBoundingBox(node);
            node->setTriangleSelector(selector);
            world->getMeta()->addTriangleSelector(selector);
            manager.template unsetComponent<NotSolid>(data);
        });
    }

    template class SolidBombSystem<WorldECS>;

}