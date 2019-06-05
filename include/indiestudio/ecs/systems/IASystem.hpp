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

namespace IndieStudio::ECS::System {

    using namespace ECS::Event;

    template<typename ManagerType>
    class IASystem : public BaseSystem<ManagerType> {
    public:
        void process(ManagerType &manager, World *world) override {
            (void) world;

            manager.template forEntitiesWith<IA>(
                [&manager, this](auto &data, [[gnu::unused]] auto id) {
                    auto &ia = manager.template getComponent<IA>(data);
                    auto &speed = manager.template getComponent<Speed>(data);
                    auto &position = manager.template getComponent<Position>(data);

                    if (this->emergency_move()) {
                        // cancel current action
                        ia.current_action = IA::Action::NOTHING;
                    } else {
                        if (ia.current_action == IA::Action::NOTHING) {
                            ia.current_action = this->select_action();
                        }
                        // executer la current action
                        while (!this->execute_action(ia.current_action)) {
                            ia.current_action = this->select_action();
                        }
                    }
                }
            );
        }

        bool emergency_move() {
            // dodge les cases en explosions
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
            }
        }

        bool execute_action(IA::Action action) {
            switch (action) {
                case IA::Action::ATK: return atk_player();
                case IA::Action::WALL: return destroy_wall();
                case IA::Action::PICKUP: return pick_powerup();
                default: return false;
            }
        }

        bool atk_player() {
            return true;
        }

        bool destroy_wall() {
            Poti poti();
            std::optional<Poti::PathData> path;

            path = poti.search_for(MapPattern:TileType::BREAKABLE_BLOCK, world->getPattern(), position.x, position.y);
            if (path == std::nullopt)
                return false;
            if (path.pathLen == 1) {
                //pose la bombe
                AI.current_action = IA::Action::NOTHING;
            } else
                moveAI(path.dir);
            return true;
            return false;
        }

        bool pick_powerup() {
            Poti poti();
            std::optional<Poti::PathData> path;

            path = poti.search_for(MapPattern:TileType::POWER_UP, world->getPattern(), position.x, position.y);
            if (path == std::nullopt)
                return false;
            UpdatePosition(path.dir);
            if (world->getPattern()->get(position.x, 1, position.y) == MapPattern::TileType::POWER_UP) {
                pickupPowerUp();
                AI.current_action = IA::Action::NOTHING;
            }
            moveAI(path.dir);
            world->getPattern()->set(position.x, 1, position.y, MapPattern::TileType::PLAYER);
            return true;
        }

        void pickupPowerUp() {
            //applique l'effet du PowerUp
        }

        void UpdatePosition(Poti::Direction dir) {
            switch (dir) {
                case Poti::Direction::LEFT: position.x -= 1;
                case Poti::Direction::RIGHT: position.x += 1;
                case Poti::Direction::TOP: position.y += 1;
                case Poti::Direction::BOT: position.y -= 1;
            }
        }

    protected:
    private:
    };
}

namespace IndieStudio {
struct Tile {
    std::size_t delta;
    MapPattern::TileType type;
};

using Hitmap = std::vector<std::vector<Tile>>;

class Poti {

    enum class Direction {
        LEFT,
        RIGHT,
        TOP,
        BOT
    };

    struct Coord {
        std::size_t x;
        std::size_t y;
    };

    struct PathData {
        Direction dir;
        std::size_t pathLen;
    };

    std::optional<PathData> search_for(MapPattern::TileType target, MapPattern *map, std::size_t x, std::size_t y) {
        PathData pathData;
        Hitmap hitmap = this->init_hitmap(map);
        this->fill_hitmap(hitmap, {x, y}, 0);
        std::optional<std::pair<Direction, std::size_t>> target_coord = this->find_nearest_target(target, hitmap);
        if (target_coord == std::nullopt) {
            return std::nullopt;
        }
        this->reset_hitmap(hitmap);
        this->fill_hitmap(hitmap, {target_coord.value().first.x, target_coord.value().first.y}, 0);
        pathData.dir = this->get_direction(hitmap, {x, y});
        pathData.pathLen = target_coord.value().second;
        return std::make_optional(pathData);
    }

private:

    Hitmap init_hitmap(MapPattern *map) {
        Hitmap hitmap;
        for (std::size_t i = 0 ; i < map->getHeight() ; i++) {
            hitmap.push_back(std::vector<Tile>());
            for (std::size_t j = 0 ; j < map->getWidth() ; j++) {
                hitmap[i].push_back({static_cast<std::size_t>(-1), map->get(j, 1, i)});
            }
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

    std::optional<std::pair<Direction, std::size_t>> find_nearest_target(MapPattern::TileType target, const Hitmap &hitmap) {
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