/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** IA
*/

#pragma once

#include <optional>

//#include "indiestudio/gameplay/BombFactory.hpp"

#include "indiestudio/world/MapPattern.hpp"
#include "indiestudio/world/IWorld.hpp"
#include "indiestudio/ecs/Components.hpp"
#include "indiestudio/ecs/BaseSystem.hpp"

namespace IndieStudio {
struct Tile {
    std::size_t delta;
    MapPattern::TileType type;
};

using Hitmap = std::vector<std::vector<Tile>>;

template <typename ManagerType>
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

    std::optional<std::pair<Direction, std::size_t>> search_for(ManagerType &manager, MapPattern::TileType target, MapPattern *map, std::size_t x, std::size_t y, bool should_go_through_explosions, std::size_t player_id) {
        Hitmap hitmap = this->init_hitmap(manager, map, should_go_through_explosions);
        manager.template forEntitiesWith<ECS::Component::IsPlayer, ECS::Component::Alive>([&manager, &hitmap, player_id](auto &, std::size_t id) {
            auto &position = manager.template getComponent<ECS::Component::Position>(id);
            std::pair<short, short> player_coord = MapPattern::positionToTile(position.x, position.z);
            Tile &tile = hitmap[player_coord.second][player_coord.first];
            if (player_id == id) return;
            if (tile.type != MapPattern::TileType::BOMB && tile.type != MapPattern::TileType::BOMB_EXPLOSION && tile.type != MapPattern::TileType::SOLID_POTENTIAL_EXPLOSION && tile.type != MapPattern::TileType::POTENTIAL_EXPLOSION) {
                tile.type = MapPattern::TileType::PLAYER;
            }
        });
        this->fill_hitmap(hitmap, {x, y}, 0);
        std::optional<std::pair<Coord, std::size_t>> target_coord = this->find_nearest_target(target, hitmap);
        if (target_coord == std::nullopt) {
            return std::nullopt;
        }
        this->reset_hitmap(hitmap);
        this->fill_hitmap(hitmap, {target_coord.value().first.x, target_coord.value().first.y}, 0);

        return std::make_optional(std::make_pair(this->get_direction(hitmap, {x, y}), target_coord.value().second));
    }

    std::optional<std::pair<Direction, std::size_t>> search_for_with_bomb(ManagerType &manager, MapPattern::TileType target, MapPattern *map, std::size_t x, std::size_t y, bool should_go_through_explosions, Coord bomb, std::size_t player_id) {
        Hitmap hitmap = this->init_hitmap(manager, map, should_go_through_explosions);
        hitmap[bomb.y][bomb.x] = {static_cast<std::size_t>(-1), MapPattern::TileType::BOMB};

            auto &stats = manager.template getComponent<ECS::Component::Stat>(player_id);
            std::size_t bomb_range = stats.range;
            std::pair<short, short> bomb_coord = {bomb.x, bomb.y};

            for (std::size_t i = 0 ; i < bomb_range ; i++) {
                auto &tile = hitmap[bomb_coord.second - i][bomb_coord.first];
                if (tile.type == MapPattern::TileType::BREAKABLE_BLOCK ||
                tile.type == MapPattern::TileType::BORDER_WALL_BLOCK ||
                tile.type == MapPattern::TileType::INNER_WALL_BLOCK ||
                tile.type == MapPattern::TileType::FLOOR_FIRST ||
                tile.type == MapPattern::TileType::FLOOR_SECOND) {
                    break;
                }
                if (tile.type == MapPattern::TileType::BOMB || tile.type == MapPattern::TileType::SOLID_POTENTIAL_EXPLOSION || tile.type == MapPattern::TileType::BOMB_EXPLOSION) {
                    continue;
                }
                tile.type = MapPattern::TileType::POTENTIAL_EXPLOSION;
            }

            for (std::size_t i = 0 ; i < bomb_range ; i++) {
                auto &tile = hitmap[bomb_coord.second + i][bomb_coord.first];
                if (tile.type == MapPattern::TileType::BREAKABLE_BLOCK ||
                tile.type == MapPattern::TileType::BORDER_WALL_BLOCK ||
                tile.type == MapPattern::TileType::INNER_WALL_BLOCK ||
                tile.type == MapPattern::TileType::FLOOR_FIRST ||
                tile.type == MapPattern::TileType::FLOOR_SECOND) {
                    break;
                }
                if (tile.type == MapPattern::TileType::BOMB || tile.type == MapPattern::TileType::SOLID_POTENTIAL_EXPLOSION || tile.type == MapPattern::TileType::BOMB_EXPLOSION) {
                    continue;
                }
                tile.type = MapPattern::TileType::POTENTIAL_EXPLOSION;
            }

            for (std::size_t i = 0 ; i < bomb_range ; i++) {
                auto &tile = hitmap[bomb_coord.second][bomb_coord.first - i];
                if (tile.type == MapPattern::TileType::BREAKABLE_BLOCK ||
                tile.type == MapPattern::TileType::BORDER_WALL_BLOCK ||
                tile.type == MapPattern::TileType::INNER_WALL_BLOCK ||
                tile.type == MapPattern::TileType::FLOOR_FIRST ||
                tile.type == MapPattern::TileType::FLOOR_SECOND) {
                    break;
                }
                if (tile.type == MapPattern::TileType::BOMB || tile.type == MapPattern::TileType::SOLID_POTENTIAL_EXPLOSION || tile.type == MapPattern::TileType::BOMB_EXPLOSION) {
                    continue;
                }
                tile.type = MapPattern::TileType::POTENTIAL_EXPLOSION;
            }

            for (std::size_t i = 0 ; i < bomb_range ; i++) {
                auto &tile = hitmap[bomb_coord.second][bomb_coord.first + i];
                if (tile.type == MapPattern::TileType::BREAKABLE_BLOCK ||
                tile.type == MapPattern::TileType::BORDER_WALL_BLOCK ||
                tile.type == MapPattern::TileType::INNER_WALL_BLOCK ||
                tile.type == MapPattern::TileType::FLOOR_FIRST ||
                tile.type == MapPattern::TileType::FLOOR_SECOND) {
                    break;
                }
                if (tile.type == MapPattern::TileType::BOMB || tile.type == MapPattern::TileType::SOLID_POTENTIAL_EXPLOSION || tile.type == MapPattern::TileType::BOMB_EXPLOSION) {
                    continue;
                }
                tile.type = MapPattern::TileType::POTENTIAL_EXPLOSION;
            }

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

    Hitmap init_hitmap(ManagerType &manager, MapPattern *map, bool should_go_through_explosion) {
        Hitmap hitmap;
        for (std::size_t i = 0 ; i < map->getHeight() ; i++) {
            hitmap.push_back(std::vector<Tile>());
            for (std::size_t j = 0 ; j < map->getWidth() ; j++) {
                hitmap[i].push_back({static_cast<std::size_t>(-1), map->get(j, 1, i)});
            }
        }
        this->add_futur_explosion(manager, hitmap, should_go_through_explosion);
        return hitmap;
    }

    void add_futur_explosion(ManagerType &manager, Hitmap &hitmap, bool should_go_through_explosion) {
        manager.template forEntitiesWith<ECS::Component::IsBomb>([&manager, should_go_through_explosion, &hitmap](auto &bomb, [[gnu::unused]] std::size_t id) {
            auto &bomb_pos = manager.template getComponent<ECS::Component::Position>(bomb);
            auto &bomb_range = manager.template getComponent<ECS::Component::ExplosionRange>(bomb);
            std::pair<short, short> bomb_coord = MapPattern::positionToTile(bomb_pos.x, bomb_pos.z);

            for (std::size_t i = 0 ; i < bomb_range.explosionRangeUp ; i++) {
                auto &tile = hitmap[bomb_coord.second - i][bomb_coord.first];
                if (tile.type == MapPattern::TileType::BREAKABLE_BLOCK ||
                tile.type == MapPattern::TileType::BORDER_WALL_BLOCK ||
                tile.type == MapPattern::TileType::INNER_WALL_BLOCK ||
                tile.type == MapPattern::TileType::FLOOR_FIRST ||
                tile.type == MapPattern::TileType::FLOOR_SECOND) {
                    break;
                }
                if (tile.type == MapPattern::TileType::BOMB || tile.type == MapPattern::TileType::BOMB_EXPLOSION) {
                    continue;
                }
                if (should_go_through_explosion) {
                    tile.type = MapPattern::TileType::POTENTIAL_EXPLOSION;
                } else {
                    tile.type = MapPattern::TileType::SOLID_POTENTIAL_EXPLOSION;
                }
            }

            for (std::size_t i = 0 ; i < bomb_range.explosionRangeDown ; i++) {
                auto &tile = hitmap[bomb_coord.second + i][bomb_coord.first];
                if (tile.type == MapPattern::TileType::BREAKABLE_BLOCK ||
                tile.type == MapPattern::TileType::BORDER_WALL_BLOCK ||
                tile.type == MapPattern::TileType::INNER_WALL_BLOCK ||
                tile.type == MapPattern::TileType::FLOOR_FIRST ||
                tile.type == MapPattern::TileType::FLOOR_SECOND) {
                    break;
                }
                if (tile.type == MapPattern::TileType::BOMB || tile.type == MapPattern::TileType::BOMB_EXPLOSION) {
                    continue;
                }
                if (should_go_through_explosion) {
                    tile.type = MapPattern::TileType::POTENTIAL_EXPLOSION;
                } else {
                    tile.type = MapPattern::TileType::SOLID_POTENTIAL_EXPLOSION;
                }
            }

            for (std::size_t i = 0 ; i < bomb_range.explosionRangeLeft ; i++) {
                auto &tile = hitmap[bomb_coord.second][bomb_coord.first - i];
                if (tile.type == MapPattern::TileType::BREAKABLE_BLOCK ||
                tile.type == MapPattern::TileType::BORDER_WALL_BLOCK ||
                tile.type == MapPattern::TileType::INNER_WALL_BLOCK ||
                tile.type == MapPattern::TileType::FLOOR_FIRST ||
                tile.type == MapPattern::TileType::FLOOR_SECOND) {
                    break;
                }
                if (tile.type == MapPattern::TileType::BOMB || tile.type == MapPattern::TileType::BOMB_EXPLOSION) {
                    continue;
                }
                if (should_go_through_explosion) {
                    tile.type = MapPattern::TileType::POTENTIAL_EXPLOSION;
                } else {
                    tile.type = MapPattern::TileType::SOLID_POTENTIAL_EXPLOSION;
                }
            }

            for (std::size_t i = 0 ; i < bomb_range.explosionRangeRight ; i++) {
                auto &tile = hitmap[bomb_coord.second][bomb_coord.first + i];
                if (tile.type == MapPattern::TileType::BREAKABLE_BLOCK ||
                tile.type == MapPattern::TileType::BORDER_WALL_BLOCK ||
                tile.type == MapPattern::TileType::INNER_WALL_BLOCK ||
                tile.type == MapPattern::TileType::FLOOR_FIRST ||
                tile.type == MapPattern::TileType::FLOOR_SECOND) {
                    break;
                }
                if (tile.type == MapPattern::TileType::BOMB || tile.type == MapPattern::TileType::BOMB_EXPLOSION) {
                    continue;
                }
                if (should_go_through_explosion) {
                    tile.type = MapPattern::TileType::POTENTIAL_EXPLOSION;
                } else {
                    tile.type = MapPattern::TileType::SOLID_POTENTIAL_EXPLOSION;
                }
            }

        });
    }

    void fill_hitmap(Hitmap &hitmap, Coord coord, std::size_t current, bool first_it) {
        hitmap[coord.y][coord.x].delta = current;

        if (!first_it && is_solid(hitmap[coord.y][coord.x])) {
            return;
        }

        if (coord.x > 0 && hitmap[coord.y][coord.x - 1].delta > current + 1) {
            fill_hitmap(hitmap, {coord.x - 1, coord.y}, current + 1, false);
        }
        if (coord.x < hitmap[coord.y].size() && hitmap[coord.y][coord.x + 1].delta > current + 1) {
            fill_hitmap(hitmap, {coord.x + 1, coord.y}, current + 1, false);
        }
        if (coord.y > 0 && hitmap[coord.y - 1][coord.x].delta > current + 1) {
            fill_hitmap(hitmap, {coord.x, coord.y - 1}, current + 1, false);
        }
        if (coord.y < hitmap.size() && hitmap[coord.y + 1][coord.x].delta > current + 1) {
            fill_hitmap(hitmap, {coord.x, coord.y + 1}, current + 1, false);
        }
    }

    void fill_hitmap(Hitmap &hitmap, Coord coord, std::size_t current) {
        this->fill_hitmap(hitmap, coord, current, true);
    }

    bool is_solid(const Tile &tile) {
        switch (tile.type) {
            case MapPattern::TileType::EMPTY: return false;
            case MapPattern::TileType::PLAYER: return false;
            case MapPattern::TileType::POWER_UP: return false;
            case MapPattern::TileType::POTENTIAL_EXPLOSION: return false;
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
        if (hitmap[current_coord.y][current_coord.x].delta < nearest) {
            nearest = hitmap[current_coord.y][current_coord.x].delta;
            nearest_target = current_coord;
        }
    }

    Direction get_direction(const Hitmap &hitmap, Coord coord) {
        std::size_t tmp = -1;
        Direction direction = Direction::NONE;

        if (coord.x > 0 && hitmap[coord.y][coord.x - 1].delta <= tmp && !is_solid(hitmap[coord.y][coord.x - 1])) {
            tmp = hitmap[coord.y][coord.x - 1].delta;
            direction = Direction::LEFT;
        }
        if (coord.x < hitmap[coord.y].size() && hitmap[coord.y][coord.x + 1].delta <= tmp && !is_solid(hitmap[coord.y][coord.x + 1])) {
            tmp = hitmap[coord.y][coord.x + 1].delta;
            direction = Direction::RIGHT;
        }
        if (coord.y > 0 && hitmap[coord.y - 1][coord.x].delta <= tmp && !is_solid(hitmap[coord.y - 1][coord.x])) {
            tmp = hitmap[coord.y - 1][coord.x].delta;
            direction = Direction::TOP;
        }
        if (coord.y < hitmap.size() && hitmap[coord.y + 1][coord.x].delta <= tmp && !is_solid(hitmap[coord.y + 1][coord.x])) {
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

            manager.template forEntitiesWith<IA, Alive>(
                [&manager, world, this](auto &data, [[gnu::unused]] auto id) {
                    auto &ia = manager.template getComponent<IA>(data);
                    auto &movement = manager.template getComponent<Movement>(data);
                    auto &position = manager.template getComponent<Position>(data);

                    std::optional<typename Poti<ManagerType>::Direction> decision = this->execute_action(manager, ia.current_action, position, world, data.id);
                    if (!decision) {
                            ia.current_action = this->select_action();
                            decision = this->execute_action(manager, ia.current_action, position, world, data.id);
                    }

                    if (this->emergency_move(manager, decision, position, world, id)) {
                        ia.current_action = IA::Action::NOTHING;
                    }

                    this->move(decision, movement);
                }
            );
        }

        void move(std::optional<typename Poti<ManagerType>::Direction> direction, Movement &movement) {
            movement.up = false;
            movement.down = false;
            movement.left = false;
            movement.right = false;

            if (!direction.has_value()) {
            } else if (direction.value() == Poti<ManagerType>::Direction::BOT) {
                movement.up = true;
            } else if (direction.value() == Poti<ManagerType>::Direction::TOP) {
                movement.down = true;
            } else if (direction.value() == Poti<ManagerType>::Direction::LEFT) {
                movement.left = true;
            } else if (direction.value() == Poti<ManagerType>::Direction::RIGHT) {
                movement.right = true;
            }
        }

        bool emergency_move(ManagerType &manager, std::optional<typename Poti<ManagerType>::Direction> &decision, Position position, IWorld *world, std::size_t player_id) {

            MapPattern *tilemap = world->getPattern();

            std::pair<short, short> coord = MapPattern::positionToTile(position.x, position.z);

            std::optional<std::pair<typename Poti<ManagerType>::Direction, std::size_t>> empty = this->marron.search_for(manager, MapPattern::TileType::EMPTY, tilemap, coord.first, coord.second, true, player_id);
            std::optional<std::pair<typename Poti<ManagerType>::Direction, std::size_t>> player = this->marron.search_for(manager, MapPattern::TileType::PLAYER, tilemap, coord.first, coord.second, true, player_id);
            std::optional<std::pair<typename Poti<ManagerType>::Direction, std::size_t>> powerup = this->marron.search_for(manager, MapPattern::TileType::POWER_UP, tilemap, coord.first, coord.second, true, player_id);
            if (empty.has_value() && empty.value().second == 0) {
                return false;
            }
            if (player.has_value() && player.value().second == 0) {
                return false;
            }
            if (powerup.has_value() && powerup.value().second == 0) {
                return false;
            }
            if (!empty.has_value() && !player.has_value() && !powerup.has_value()) {
                decision = std::make_optional(Poti<ManagerType>::Direction::NONE);
                return true;
            }

            std::optional<std::pair<typename Poti<ManagerType>::Direction, std::size_t>> nearest;
            if (empty.has_value() && (!player.has_value() || (player.has_value() && empty.value().second <= player.value().second)) && (!powerup.has_value() || (powerup.has_value() && empty.value().second <= powerup.value().second))) {
                nearest = empty;
            }
            if (player.has_value() && (!empty.has_value() || (empty.has_value() && player.value().second <= empty.value().second)) && (!powerup.has_value() || (powerup.has_value() && player.value().second <= powerup.value().second))) {
                nearest = player;
            }
            if (powerup.has_value() && (!empty.has_value() || (empty.has_value() && powerup.value().second <= empty.value().second)) && (!player.has_value() || (player.has_value() && powerup.value().second <= player.value().second))) {
                nearest = powerup;
            }

            decision = std::make_optional(nearest.value().first);
            return true;
        }

        bool emergency_move_with_bomb(ManagerType &manager, std::optional<typename Poti<ManagerType>::Direction> &decision, Position position, IWorld *world, typename Poti<ManagerType>::Coord bomb, std::size_t player_id) {

            MapPattern *tilemap = world->getPattern();

            std::pair<short, short> coord = MapPattern::positionToTile(position.x, position.z);

            std::optional<std::pair<typename Poti<ManagerType>::Direction, std::size_t>> empty = this->marron.search_for_with_bomb(manager, MapPattern::TileType::EMPTY, tilemap, coord.first, coord.second, false, bomb, player_id);
            std::optional<std::pair<typename Poti<ManagerType>::Direction, std::size_t>> player = this->marron.search_for_with_bomb(manager, MapPattern::TileType::PLAYER, tilemap, coord.first, coord.second, false, bomb, player_id);
            std::optional<std::pair<typename Poti<ManagerType>::Direction, std::size_t>> powerup = this->marron.search_for_with_bomb(manager, MapPattern::TileType::POWER_UP, tilemap, coord.first, coord.second, false, bomb, player_id);
            if (empty.has_value() && empty.value().second == 0) {
                return false;
            }
            if (player.has_value() && player.value().second == 0) {
                return false;
            }
            if (powerup.has_value() && powerup.value().second == 0) {
                return false;
            }
            if (!empty.has_value() && !player.has_value() && !powerup.has_value()) {
                decision = std::make_optional(Poti<ManagerType>::Direction::NONE);
                return true;
            }

            std::optional<std::pair<typename Poti<ManagerType>::Direction, std::size_t>> nearest;
            if (empty.has_value() && (!player.has_value() || (player.has_value() && empty.value().second <= player.value().second)) && (!powerup.has_value() || (powerup.has_value() && empty.value().second <= powerup.value().second))) {
                nearest = empty;
            }
            if (player.has_value() && (!empty.has_value() || (empty.has_value() && player.value().second <= empty.value().second)) && (!powerup.has_value() || (powerup.has_value() && player.value().second <= powerup.value().second))) {
                nearest = player;
            }
            if (powerup.has_value() && (!empty.has_value() || (empty.has_value() && powerup.value().second <= empty.value().second)) && (!player.has_value() || (player.has_value() && powerup.value().second <= player.value().second))) {
                nearest = powerup;
            }

            decision = std::make_optional(nearest.value().first);
            return true;
        }

        bool simulate_bomb(typename Poti<ManagerType>::Coord bomb, ManagerType &manager, Position position, IWorld *world, std::size_t player_id) {
            MapPattern *tilemap = world->getPattern();


            std::pair<short, short> coord = MapPattern::positionToTile(position.x, position.z);

            this->marron.search_for_with_bomb(manager, MapPattern::TileType::PLAYER, tilemap, coord.first, coord.second, false, bomb, player_id);

            std::optional<typename Poti<ManagerType>::Direction> tmp = std::make_optional(Poti<ManagerType>::Direction::NONE);
            bool test = this->emergency_move_with_bomb(manager, tmp, position, world, bomb, player_id);
            if (test == false || (tmp.has_value() && tmp.value() != Poti<ManagerType>::Direction::NONE)) {
                return true;
            }
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

        std::optional<typename Poti<ManagerType>::Direction> execute_action(ManagerType &manager, IA::Action action, Position position, IWorld *world, std::size_t id) {
            switch (action) {
                case IA::Action::ATK: return atk_player(manager, position, world, id);
                case IA::Action::WALL: return destroy_wall(manager, position, world, id);
                case IA::Action::PICKUP: return pick_powerup(manager, position, world, id);
                default: return std::nullopt;
            }
        }

        std::optional<typename Poti<ManagerType>::Direction> atk_player(ManagerType &manager, Position position, IWorld *world, std::size_t id) {
            (void) id;

            MapPattern *tilemap = world->getPattern();

            std::pair<short, short> coord = MapPattern::positionToTile(position.x, position.z);

            std::optional<std::pair<typename Poti<ManagerType>::Direction, std::size_t>> decision = this->marron.search_for(manager, MapPattern::TileType::PLAYER, tilemap, coord.first, coord.second, false, id);

            if (decision == std::nullopt) {
                return std::nullopt;
            }

            if (decision.value().second <= 3) {
                std::pair<short, short> player_coord = MapPattern::positionToTile(position.x, position.z);

                if (this->simulate_bomb({static_cast<std::size_t>(player_coord.first), static_cast<std::size_t>(player_coord.second)}, manager, position, world, id)) {
                    auto &stat = manager.template getComponent<Stat>(id);
                    world->dropBomb(player_coord.first * 20 + 0.5, player_coord.second * 20 + 0.5, id, stat.range);
                    return std::nullopt;
                }
            }

            return std::make_optional(decision.value().first);
        }

        std::optional<typename Poti<ManagerType>::Direction> destroy_wall(ManagerType &manager, Position position, IWorld *world, [[gnu::unused]] std::size_t id) {
            MapPattern *tilemap = world->getPattern();

            std::pair<short, short> coord = MapPattern::positionToTile(position.x, position.z);

            std::optional<std::pair<typename Poti<ManagerType>::Direction, std::size_t>> decision = this->marron.search_for(manager, MapPattern::TileType::BREAKABLE_BLOCK, tilemap, coord.first, coord.second, false, id);

            if (decision == std::nullopt) {
                return std::nullopt;
            }

            if (decision.value().second == 1) {
                std::pair<short, short> player_coord = MapPattern::positionToTile(position.x, position.z);

                if (this->simulate_bomb({static_cast<std::size_t>(player_coord.first), static_cast<std::size_t>(player_coord.second)}, manager, position, world, id)) {
                    auto &stat = manager.template getComponent<Stat>(id);
                    world->dropBomb(player_coord.first * 20 + 0.5, player_coord.second * 20 + 0.5, id, stat.range);
                    return std::nullopt;
                } else {
                    return std::nullopt;
                }

            }
            return std::make_optional(decision.value().first);
        }

        std::optional<typename Poti<ManagerType>::Direction> pick_powerup(ManagerType &manager, Position position, IWorld *world, [[gnu::unused]] std::size_t id) {
            MapPattern *tilemap = world->getPattern();

            std::pair<short, short> coord = MapPattern::positionToTile(position.x, position.z);

            if (tilemap->get(coord.second, 1, coord.first) == MapPattern::TileType::POWER_UP) {
                return std::nullopt;
            }

            std::optional<std::pair<typename Poti<ManagerType>::Direction, std::size_t>> decision = this->marron.search_for(manager, MapPattern::TileType::POWER_UP, tilemap, coord.first, coord.second, false, id);
            if (decision == std::nullopt) {
                return std::nullopt;
            }

            return std::make_optional(decision.value().first);
        }

    protected:
    private:
        Poti<ManagerType> marron;
    };
}