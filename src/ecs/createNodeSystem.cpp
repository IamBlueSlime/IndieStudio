/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** createNodeSystem
*/

#include "../Components.hpp"
#include "../BaseSystem.hpp"
#include "indiestudio/Game.hpp"

template<typename ManagerType>
void createAllNode(ManagerType &manager)
{
    manager.template forEntitiesWith<NodeCreate, MeshPath>(
    [&manager](auto &data, [[gnu::unused]] auto id) {
        auto &meshPath = manager.getComponent<MeshPath>(data);
        auto node = scenemg->addAnimatedMeshSceneNode(scenemg->getMesh(meshPath))

	    manager.template setComponent(data, Node(node);
    });
}

template <typename ManagerType>
void setupAllNode(ManagerType &manager)
{
    manager.template forEntitiesWith<Node, Position, Scale, MaterialTexture, MaterialFlag>(
    [&manager](auto &data, [[gnu::unused]] auto id) {
        // TODO: set attributes of the node
    });
}

template <typename ManagerType>
void initAllEntities(ManagerType &manager)
{
    createAllNode(manager);
    setupAllNode(manager);
}