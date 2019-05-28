/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Components
*/

#ifndef COMPONENTS_HPP_
#define COMPONENTS_HPP_

#include <string>
#include <unordered_map>
#include <map>
#include <vector>
#include <functional>
#include <ctime>

#include "./Events.hpp"

using namespace Ecs::Event;

namespace Ecs::Component {

    struct Position {
        float x;
        float y;
        float z;
    };

    struct Speed {
        float x;
        float y;
        float z;
    };

    struct Alive {
    };

    struct Movable {
    };

    struct Indestructible {
    };

    struct LifeTime {
        LifeTime(int _lifeTime = std::time(nullptr))
        :
        lifeTime(_lifeTime) {}

        int lifeTime;
    };

    struct ExplosionLifeTime {
        ExplosionLifeTime(int _explosionLifeTime = std::time(nullptr))
        :
        explosionLifeTime(_explosionLifeTime) {}

        int explosionLifeTime;
    };

    struct IsExploding {
    };

    struct ExplosionRange {
        ExplosionRange(float _explosionRange = 3.0f)
        :
        explosionRangeUp(_explosionRange),
        explosionRangeLeft(_explosionRange),
        explosionRangeDown(_explosionRange),
        explosionRangeRight(_explosionRange) {}

        float explosionRangeUp;
        float explosionRangeLeft;
        float explosionRangeDown;
        float explosionRangeRight;
    };

    struct BombType {
        enum {
            DEFAULT = 1,
            STICKY = 2,
            MEGA = 4
        };

        BombType(int _bombType = BombType::DEFAULT)
        :
        bombType(_bombType) {}

        int bombType;
    };

    struct IsPlayer {
    };

    struct IsAI {
    };

    struct IsBomb {
    };

    struct SoundID {
        SoundID(int _soundID = 0)
        :
        soundID(_soundID) {}

        int soundID;
    };

    struct Direction {
        enum {
            STAY = 1,
            UP = 2,
            LEFT = 4,
            DOWN = 8,
            RIGHT = 16
        };

        Direction(int _direction = Direction::STAY)
        :
        direction(_direction) {}

        int direction;
    };

    struct Texture {
        Texture(std::string _texture = "")
        :
        texture(_texture) {}

        std::string texture;
    };

    struct ID {
        ID(int _id)
        :
        id(_id) {}

        int id;
    };

    template<typename ManagerType>
    struct EventCallbacks {

        using CallbackType = std::function<void(const EventData&, std::size_t, ManagerType &)>;

    public:
        const auto &getCallbacks() { return callbacks; }

        void addCallback(const EventData &event, CallbackType &&callback) {
            this->callbacks[event].push_back(std::forward<CallbackType>(callback));
        }

    private:
        std::unordered_map<EventData, std::vector<CallbackType>> callbacks;
    };

}

#endif /* !COMPONENTS_HPP_ */
