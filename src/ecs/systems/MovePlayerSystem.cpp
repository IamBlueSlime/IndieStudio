/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** ecs systems MovePlayerSystem.hpp
*/

#include "indiestudio/ecs/systems/MovePlayerSystem.hpp"
#include "indiestudio/world/World.hpp"

namespace IndieStudio::ECS::System {

    using namespace ECS::Component;

    template<typename ManagerType>
    void MovePlayerSystem<ManagerType>::process(ManagerType &manager, World *world)
    {
        manager.template forEntitiesWith<Movement, Alive>(
                [&](auto &data, [[gnu::unused]] std::size_t id)
                {
                    if (world->getSettings().elapsedSeconds == 0)
                        return;

                    auto &node = manager.template getComponent<Node>(data);
                    auto &pos = manager.template getComponent<Position>(data);
                    auto absPos = node.node->getAbsolutePosition();

                    pos.x = absPos.X;
                    pos.y = absPos.Y;
                    pos.z = absPos.Z;

                    Movement &mov = manager.template getComponent<Movement>(data);
                    auto &speed = manager.template getComponent<Speed>(data);
                    irr::core::vector3df vect(0, 0, 0);

                    if (mov.up) {
                        vect.Z += 1;
                    } if (mov.down) {
                        vect.Z -= 1;
                    } if (mov.left) {
                        vect.X -= 1;
                    } if (mov.right) {
                        vect.X += 1;
                    }

                    irr::scene::IAnimatedMeshSceneNode *animatedNode =
                        static_cast<irr::scene::IAnimatedMeshSceneNode *>(node.node);

                    if (vect.X == 0 && vect.Z == 0) {
                        if (animatedNode->getFrameNr() == 76 || animatedNode->getFrameNr() <= 27)
                            animatedNode->setFrameLoop(27, 76);
                        return;
                    } else {
                        world->move(vect, pos, speed, node);
                    }
                    if (animatedNode->getFrameNr() > 27)
		                animatedNode->setFrameLoop(0, 27);
                }
            );
    }

    template class MovePlayerSystem<WorldECS>;

}