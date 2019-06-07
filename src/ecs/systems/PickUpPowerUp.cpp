/*
** EPITECH PROJECT, 2019
** indiStudio
** File description:
** Pickup powerup system
*/

// #include "indiestudio/ecs/systems/PickUpPowerUp.hpp"

// namespace IndieStudio::ECS::System {

//     bool isOnPowerUp(IndieStudio::ECS::System::PickUpPowerUp::Coord player, Coord powerup, MapPattern *map) {
//             if (map->get(player.x, 1, player.y) == map->get(powerup.x, 1, powerup.y))
//                 return true;
//             return false;
//     }

//     Coord PickUpPowerUp::convertPosition(Position p) {
//         std::pair<short, short> tileScalePos = MapPattern::positionToTile(p.x, p.z);
//         PickUpPowerUp::Coord coord = {static_cast<std::size_t>(tileScalePos.first), static_cast<std::size_t>(tileScalePos.second)};
//         return coord;
//     }
// }