/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Main.cpp
*/

#include "ecs/Ecs.hpp"
#include "indiestudio/Bootstraper.hpp"

#include <thread>


using namespace Ecs::Component;

// Components registers in the manager
using EcsManager = Ecs::Manager<
Position,
Speed,
Movable,
Alive,
Indestructible,
LifeTime,
ExplosionLifeTime,
IsExploding,
ExplosionRange,
BombType,
IsPlayer,
IsAI,
IsBomb,
SoundID,
Direction,
Texture,
ID
>;

using namespace Ecs::System;

// Systems used to run the manager
using EcsSystems = Systems<
applyExplosion<EcsManager>,
explosionDuration<EcsManager>,
setupExplosion<EcsManager>,
drawEntities<EcsManager>
>;

int main(int ac, char **av)
{
    IndieStudio::Bootstraper::start(ac, av);
    // Creating a new manager
    auto manager = EcsManager();

    // Creating entity
    // auto &player = manager.addEntity();
    // auto &bomb = manager.addEntity();
    // auto &player2 = manager.addEntity();
    // Donc forget to get the REFERENCE, so you can modify the entity with it

    // Create component
    // Position position1 = {11, 12, 9};
    // Position position2 = {10, 12, 9};
    // Position posBomb = {9, 2, 1};
    // Position posBomb2 = {8, 2, 1};

    // // Set this component on the entity
    // manager.setComponent(player, position2);
    // manager.setComponent(player, Alive());

    // manager.setComponent(player2, position1);
    // manager.setComponent(player2, Alive());

    // manager.setComponent(bomb, posBomb);
    // manager.setComponent(bomb, IsExploding());
    // manager.setComponent(bomb, IsBomb());
    // manager.setComponent(bomb, ExplosionRange());
    // manager.setComponent(bomb, LifeTime());
//    manager.setComponent(bomb, ExplosionLifeTime());

//     auto &test_entity = manager.addEntity();
//     EventCallbacks<EcsManager> event_listener;

//     EventData event_left;
//     event_left.type = EventType::KEYBOARD_EVENT;
//     event_left.keyInput.Key = irr::KEY_LEFT;

//     EventData event_right;
//     event_right.type = EventType::KEYBOARD_EVENT;
//     event_right.keyInput.Key = irr::KEY_RIGHT;

//     event_listener.addCallback(event_left, [](const EventData &event, std::size_t entity_id, auto &manager) {
//         std::cout << "left pressed in entity" << entity_id << std::endl;
// //        auto &component = manager.getComponent<LeComponentCherché>(entity_id);
//     });

//     event_listener.addCallback(event_right, [](const EventData &event, std::size_t entity_id, auto &manager) {
//         std::cout << "right pressed in entity" << entity_id << std::endl;
//     });

//     manager.setComponent(test_entity, event_listener);

    // event_listener.addCallback(event_left, [](const EventData &event, std::size_t entity_id, auto &manager) {
    //     (void)event;
    //     (void)manager;
    //     std::cout << "left pressed to entity" << entity_id << std::endl;
//        auto &component = manager.getComponent<LeComponentCherché>(entity_id);
    // });

    // event_listener.addCallback(event_right, [](const EventData &event, std::size_t entity_id, auto &manager) {
    //     (void)event;
    //     (void)manager;
    //     std::cout << "right pressed to entity" << entity_id << std::endl;
    // });

//    Run le manager avec les systems spécifiés
    manager.run(EcsSystems());
}