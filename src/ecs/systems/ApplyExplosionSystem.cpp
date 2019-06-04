/*
** EPITECH PROJECT, 2019
** IndieStudio
** File description:
** Apply explosion
*/

#include "indiestudio/ecs/systems/ApplyExplosionSystem.hpp"
#include "indiestudio/world/World.hpp"

// namespace IndieStudio::ECS::System {
//     template <typename ManagerType>
//     void ApplyExplosion<ManagerType>::process(ManagerType &manager, World *world) {
//         manager.template forEntitiesWith<IsBomb, Position, IsExploding, ExplosionRange>(
//             [&manager](auto &data, [[gnu::unused]] auto id) {
//                 auto &Position = manager.template getComponent<Position>(data);
//                 auto &Range = manager.template getComponent<ExplosionRange>(data);

//                 for (int i = 0; i < Range.explosionRangeUp; i++) {
//                     if (world->getPattern()->get(Position.x, 1, Position.z + i) == IndieStudio::MapPattern::TileType::BREAKABLE_BLOCK) {
//                         world->getPattern()->set(Position.x, 1, Position.z + i, IndieStudio::MapPattern::TileType::BOMB_EXPLOSION);
//                         Range.explosionRangeUp = i;
//                         break;
//                     }
//                 }
//                 for (int i = 0; i < Range.explosionRangeLeft; i++) {
//                     if (world->getPattern()->get(Position.x - i, 1, Position.z) == IndieStudio::MapPattern::TileType::BREAKABLE_BLOCK) {
//                         world->getPattern()->set(Position.x - i, 1, Position.z, IndieStudio::MapPattern::TileType::BOMB_EXPLOSION);
//                         Range.explosionRangeLeft = i;
//                         break;
//                     }
//                 }
//                 for (int i = 0; i < Range.explosionRangeDown; i++) {
//                     if (world->getPattern()->get(Position.x, 1, Position.z - i) == IndieStudio::MapPattern::TileType::BREAKABLE_BLOCK) {
//                         world->getPattern()->set(Position.x, 1, Position.z - i, IndieStudio::MapPattern::TileType::BOMB_EXPLOSION);
//                         Range.explosionRangeDown = i;
//                         break;
//                     }
//                 }
//                 for (int i = 0; i < Range.explosionRangeRight; i++) {
//                     if (world->getPattern()->get(Position.x + i, 1, Position.z) == IndieStudio::MapPattern::TileType::BREAKABLE_BLOCK) {
//                         world->getPattern()->set(Position.x + i, 1, Position.z, IndieStudio::MapPattern::TileType::BOMB_EXPLOSION);
//                         Range.explosionRangeRight = i;
//                         break;
//                     }
//                 }
//             });
//     }
// }