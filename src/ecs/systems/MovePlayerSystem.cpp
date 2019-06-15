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

                    Movement &mov = manager.template getComponent<Movement>(data);
                    auto &speed = manager.template getComponent<Speed>(data);
                    auto &pos = manager.template getComponent<Position>(data);
                    auto &node = manager.template getComponent<Node>(data);

                    bool did = false;
                    if (mov.up) {
                        std::cout << "up ; ";
                        world->move({0, 0, 1}, pos, speed, node);
                        did = true;
                    }
                    if (mov.down) {
                        std::cout << "down ; ";
                        world->move({0, 0, -1}, pos, speed, node);
                        did = true;
                    }
                    if (mov.left) {
                        std::cout << "left ; ";
                        world->move({-1, 0, 0}, pos, speed, node);
                        did = true;
                    }
                    if (mov.right) {
                        std::cout << "right ; ";
                        world->move({1, 0, 0}, pos, speed, node);
                        did = true;
                    }
                    std::cout << std::endl;

                    irr::scene::IAnimatedMeshSceneNode *animatedNode =
                        static_cast<irr::scene::IAnimatedMeshSceneNode *>(node.node);

                    if (!did) {
                        if (animatedNode->getFrameNr() == 76 || animatedNode->getFrameNr() <= 27)
                            animatedNode->setFrameLoop(27, 76);
                        return;
                    }
                    if (animatedNode->getFrameNr() > 27)
		                animatedNode->setFrameLoop(0, 27);
                }
            );
    }

    template class MovePlayerSystem<WorldECS>;

}