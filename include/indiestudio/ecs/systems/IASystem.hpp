/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** IA
*/

#pragma once

#include <optional>

#include "indiestudio/ecs/Events.hpp"
#include "indiestudio/world/MapPattern.hpp"
#include "indiestudio/world/IWorld.hpp"

namespace IndieStudio {
struct Tile {
    std::size_t delta;
    MapPattern::TileType type;
};

using Hitmap = std::vector<std::vector<Tile>>;

class Poti {
public:

    enum class Direction {
        LEFT,
        RIGHT,
        TOP,
        BOT,
        NONE
    };

    struct Coord {
        std::size_t x;
        std::size_t y;
    };

    struct PathData {
        Direction dir;
        std::size_t pathLen;
    };


    std::optional<std::pair<Direction, std::size_t>> search_for(MapPattern::TileType target, MapPattern *map, std::size_t x, std::size_t y) {
        Hitmap hitmap = this->init_hitmap(map);
        this->fill_hitmap(hitmap, {x, y}, 0);
        std::optional<std::pair<Coord, std::size_t>> target_coord = this->find_nearest_target(target, hitmap);
        if (target_coord == std::nullopt) {
            return std::nullopt;
        }
        this->reset_hitmap(hitmap);
        this->fill_hitmap(hitmap, {target_coord.value().first.x, target_coord.value().first.y}, 0);
        return std::make_optional(std::make_pair(this->get_direction(hitmap, {x, y}), target_coord.value().second));
    }

private:

    Hitmap init_hitmap(MapPattern *map) {
        Hitmap hitmap;
        //std::cout << "Creating hitmap: " << std::endl;
        for (std::size_t i = 0 ; i < map->getHeight() ; i++) {
            hitmap.push_back(std::vector<Tile>());
            for (std::size_t j = 0 ; j < map->getWidth() ; j++) {
                //std::cout << static_cast<int>(map->get(j, 1, i)) << " ";
                hitmap[i].push_back({static_cast<std::size_t>(-1), map->get(j, 1, i)});
            }
            //std::cout << std::endl;
        }
        return hitmap;
    }

    void fill_hitmap(Hitmap &hitmap, Coord coord, std::size_t current) {
        hitmap[coord.y][coord.x].delta = current;

        if (!is_solid(hitmap[coord.y][coord.x - 1]) && hitmap[coord.y][coord.x - 1].delta > current + 1) {
            fill_hitmap(hitmap, {coord.x - 1, coord.y}, current + 1);
        }
        if (!is_solid(hitmap[coord.y][coord.x + 1]) && hitmap[coord.y][coord.x + 1].delta > current + 1) {
            fill_hitmap(hitmap, {coord.x + 1, coord.y}, current + 1);
        }
        if (!is_solid(hitmap[coord.y - 1][coord.x]) && hitmap[coord.y - 1][coord.x].delta > current + 1) {
            fill_hitmap(hitmap, {coord.x, coord.y - 1}, current + 1);
        }
        if (!is_solid(hitmap[coord.y + 1][coord.x]) && hitmap[coord.y + 1][coord.x].delta > current + 1) {
            fill_hitmap(hitmap, {coord.x, coord.y + 1}, current + 1);
        }
    }

    bool is_solid(const Tile &tile) {
        switch (tile.type) {
            case MapPattern::TileType::EMPTY: return false;
            case MapPattern::TileType::PLAYER: return false;
            case MapPattern::TileType::POWER_UP: return false;
            default: return true;
        }
    }

    void reset_hitmap(Hitmap &hitmap) {
        for (auto &tmp : hitmap) {
            for (auto &tile : tmp) {
                tile.delta = -1;
            }
        }
    }

    std::optional<std::pair<Coord, std::size_t>> find_nearest_target(MapPattern::TileType target, const Hitmap &hitmap) {
        std::size_t nearest = -1;
        Coord nearest_target = {0, 0};

        for (std::size_t y = 0 ; y < hitmap.size() ; y++) {
            for (std::size_t x = 0 ; x < hitmap[y].size() ; x++) {
                if (hitmap[y][x].type == target) {
                    is_nearest(hitmap, nearest, nearest_target, {x, y});
                }
            }
        }
        if (nearest == static_cast<std::size_t>(-1)) {
            return std::nullopt;
        }
        return std::make_optional(std::make_pair(nearest_target, nearest));
    }

    void is_nearest(const Hitmap &hitmap, std::size_t &nearest, Coord &nearest_target, Coord current_coord) {
        if (current_coord.x > 0 && hitmap[current_coord.y][current_coord.x - 1].delta < nearest) {
            nearest = hitmap[current_coord.y][current_coord.x - 1].delta;
            nearest_target = current_coord;
        }
        if (current_coord.x < hitmap[current_coord.y].size() && hitmap[current_coord.y][current_coord.x + 1].delta < nearest) {
            nearest = hitmap[current_coord.y][current_coord.x + 1].delta;
            nearest_target = current_coord;
        }
        if (current_coord.y > 0 && hitmap[current_coord.y - 1][current_coord.x].delta < nearest) {
            nearest = hitmap[current_coord.y - 1][current_coord.x].delta;
            nearest_target = current_coord;
        }
        if (current_coord.y < hitmap.size() && hitmap[current_coord.y + 1][current_coord.x].delta < nearest) {
            nearest = hitmap[current_coord.y + 1][current_coord.x].delta;
            nearest_target = current_coord;
        }
    }

    Direction get_direction(const Hitmap &hitmap, Coord coord) {
        std::size_t tmp = -1;
        Direction direction = Direction::LEFT;

        if (coord.x > 0 && hitmap[coord.y][coord.x - 1].delta < tmp) {
            tmp = hitmap[coord.y][coord.x - 1].delta;
            direction = Direction::LEFT;
        }
        if (coord.x < hitmap[coord.y].size() && hitmap[coord.y][coord.x + 1].delta < tmp) {
            tmp = hitmap[coord.y][coord.x + 1].delta;
            direction = Direction::RIGHT;
        }
        if (coord.y > 0 && hitmap[coord.y - 1][coord.x].delta < tmp) {
            tmp = hitmap[coord.y - 1][coord.x].delta;
            direction = Direction::TOP;
        }
        if (coord.y < hitmap.size() && hitmap[coord.y + 1][coord.x].delta < tmp) {
            tmp = hitmap[coord.y + 1][coord.x].delta;
            direction = Direction::BOT;
        }
        return direction;
    }

};
}

namespace IndieStudio::ECS::System {

    using namespace ECS::Event;

    template<typename ManagerType>
    class IASystem : public BaseSystem<ManagerType> {
    public:
        void process(ManagerType &manager, World *world) override {
            (void) world;

            manager.template forEntitiesWith<IA>(
                [&manager, world, this](auto &data, [[gnu::unused]] auto id) {
                    auto &ia = manager.template getComponent<IA>(data);
                    auto &movement = manager.template getComponent<Movement>(data);
                    auto &position = manager.template getComponent<Position>(data);

                    std::optional<Poti::Direction> decision = this->execute_action(ia.current_action, position, world);
                    while (!decision) {
                            ia.current_action = this->select_action();
                            decision = this->execute_action(ia.current_action, position, world);
                    }

                    if (this->emergency_move(decision, position)) {
                        ia.current_action = IA::Action::NOTHING;
                    }

                    // TODO: move dans la bonne direction
                    this->move(decision.value(), movement);
                }
            );
        }

        void move(Poti::Direction direction, Movement &movement) {
                movement.up = false;
                movement.down = false;
                movement.left = false;
                movement.right = false;
                //std::cout << "Moving at ";
            if (direction == Poti::Direction::BOT) {
                //std::cout << "bot" << std::endl;
                movement.down = true;
            } else if (direction == Poti::Direction::TOP) {
                //std::cout << "top" << std::endl;
                movement.up = true;
            } else if (direction == Poti::Direction::LEFT) {
                //std::cout << "left" << std::endl;
                movement.left = true;
            } else if (direction == Poti::Direction::RIGHT) {
                //std::cout << "right" << std::endl;
                movement.right = true;
            } else {
                //std::cout << "nothing" << std::endl;
            }
        }

        bool emergency_move(std::optional<Poti::Direction> &decision, Position position) {
            (void) decision;
            (void) position;
            // dodge les cases en explosions (a priori already OK)

            // MapPattern *tilemap = world->getPattern();

            // Poti::Coord coord = convert_position(position);

            // if (decision.value() == Poti::Direction::RIGHT) {
            //     if (tilemap[coord.y][coord.x] )
            // } else if (decision.value() == Poti::Direction::LEFT) {

            // } else if (decision.value() == Poti::Direction::TOP) {

            // } else if (decision.value() == Poti::Direction::BOT) {

            // }

            // dodge le rayon d action des bombes "endormies"
            // renvoie true si il y a quelque chose à eviter
            // renvoie false sinon

            // This removes a warning
            return false;
        }

        IA::Action select_action() {
            int rand = std::rand() % 3;

            switch (rand) {
                case 0: return IA::Action::ATK;
                case 1: return IA::Action::WALL;
                case 2: return IA::Action::PICKUP;
                default: return IA::Action::NOTHING;
            }
        }

        std::optional<Poti::Direction> execute_action(IA::Action action, Position position, IWorld *world) {
            switch (action) {
                case IA::Action::ATK: return atk_player(position, world);
                case IA::Action::WALL: return destroy_wall(position, world);
                case IA::Action::PICKUP: return pick_powerup(position, world);
                default: return std::nullopt;
            }
        }

        std::optional<Poti::Direction> atk_player(Position position, IWorld *world) {
            MapPattern *tilemap = world->getPattern();

            //std::cout << "Attacking!" << std::endl;
//            Poti::Coord coord = convert_position(position);
            std::pair<short, short> coord = MapPattern::positionToTile(position.x, position.y);

            std::optional<std::pair<Poti::Direction, std::size_t>> decision = this->marron.search_for(MapPattern::TileType::PLAYER, tilemap, coord.first, coord.second);

            if (decision == std::nullopt) {
                //std::cout << "No player in range :(!" << std::endl;
                return std::nullopt;
            }

            if (decision.value().second <= 3) {
                //std::cout << "Attacked! :D i finished my move!" << std::endl;
                // TODO: poser bombe
                // TODO: ajouter du bruit aléatoire, et prendre en compte les stats du player
                return std::nullopt;
            }

            return std::make_optional(decision.value().first);
        }

        std::optional<Poti::Direction> destroy_wall(Position position, IWorld *world) {
            MapPattern *tilemap = world->getPattern();

            //std::cout << "Breacking wall!" << std::endl;
            std::pair<short, short> coord = MapPattern::positionToTile(position.x, position.y);

            std::optional<std::pair<Poti::Direction, std::size_t>> decision = this->marron.search_for(MapPattern::TileType::BREAKABLE_BLOCK, tilemap, coord.first, coord.second);

            if (decision == std::nullopt) {
                //std::cout << "No wall in range :(!" << std::endl;
                return std::nullopt;
            }

            if (decision.value().second == 1) {
                //std::cout << "Breacked wall!" << std::endl;
                // TODO: poser bombe
                // l'action sera reroll, mais cancel immédiatement à l iteration suivante pour échapper à sa propre bombe
                return std::nullopt;
            }

            return std::make_optional(decision.value().first);
        }

        std::optional<Poti::Direction> pick_powerup(Position position, IWorld *world) {
            MapPattern *tilemap = world->getPattern();

            //std::cout << "Picking powerup!" << std::endl;
            std::pair<short, short> coord = MapPattern::positionToTile(position.x, position.y);

            if (tilemap->get(coord.second, 1, coord.first) == MapPattern::TileType::POWER_UP) {
                //std::cout << "Powerup picked!" << std::endl;
                // TODO: apply powerup effect
                // TODO: delete le powerup
                // TODO: update la tilemap
                return std::nullopt;
            }

            std::optional<std::pair<Poti::Direction, std::size_t>> decision = this->marron.search_for(MapPattern::TileType::POWER_UP, tilemap, coord.first, coord.second);
            if (decision == std::nullopt) {
                //std::cout << "No powerup in range :(!" << std::endl;
                return std::nullopt;
            }

            return std::make_optional(decision.value().first);
        }

    protected:
    private:
        Poti marron;
    };
}